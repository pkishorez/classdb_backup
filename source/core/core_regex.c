/**
 * @file core_regex.c
 * @author Kishore
 * @date March 26, 2016, 9:21 PM
 * 
 * @brief 
 * 
 * 
 */

#include "headers/core_memory.h"
#include "headers/core_regex.h"
#include "common/headers/common_utility.h"

static inline void _char_update();
static enum parse_status parse_atom(int ch, char *regex);
static enum parse_status parse_terminal(int ch, char *regex);
static inline void char_move();
static int regex_move_step(char *regex);

static struct{
	int str_parsed;
	int reg_parsed;
	char *regex;
	char *string;
	int caps;
	memory_pointer_t mg;
	enum regex_error status;
	int ch;
}_;

enum parse_status{
	ATOM_PARSE_ERROR = 0,
	ATOM_PARSE_SUCCESS = 1
};

enum regex_error regex_validate_mem(char *regex, memory_pointer_t mg)
{
	return regex_parse_mem(regex, mg, NULL, 0).status;
}


regex_status_t _regex_parse(char *regex, regex_cap_t *caps, int n_caps);

inline regex_status_t regex_parse_mem(
	char *regex, memory_pointer_t mg, regex_cap_t *caps, int n_caps)
{
	_.string = NULL;
	_.mg = mg;
	return _regex_parse(regex, caps, n_caps);
}

/**
 * 
 * @param regex
 * @param str
 * @param caps
 * @param n_caps
 * @return 
 */
inline regex_status_t regex_parse_str(
	char *regex, char *str, regex_cap_t *caps, int n_caps)
{
	_.string = str;
	return _regex_parse(regex, caps, n_caps);
}

regex_status_t _regex_parse(
	char *regex, regex_cap_t *caps, int n_caps)
{
	_.str_parsed = 0;
	_.reg_parsed = 0;
	_.regex = regex;
	_.caps = 0;
	int cap_index = 0;
	
	_char_update();
	
	enum parse_status atom_status;
	_.status = REGEX_PARSE_SUCCESS;
	while (regex[_.reg_parsed]!='\0')
	{
		switch(regex[_.reg_parsed])
		{
			case '\0': {
				_.status = REGEX_PARSE_SUCCESS;
				goto end;
			}
			case '*': {
				_.reg_parsed++;
				int i=0;
				while ((atom_status=parse_atom(_.ch, regex+_.reg_parsed))==ATOM_PARSE_SUCCESS){
					char_move();
				}
				_.reg_parsed += regex_move_step(regex+_.reg_parsed);
				break;
			}
			case '+': {;
				_.reg_parsed++;
				int i=0;
				while ((atom_status=parse_atom(_.ch, regex+_.reg_parsed))==ATOM_PARSE_SUCCESS){
					char_move(), i++;
				}
				if (i==0){
					_.status = REGEX_PARSE_ERROR;
					goto end;
				}
				_.reg_parsed += regex_move_step(regex+_.reg_parsed);
				break;
			}
			case '(': {
				if ((n_caps>0) && (cap_index<n_caps))
				{
					caps[cap_index].index = _.str_parsed;
				}
				_.reg_parsed++;
				break;
			}
			case ')': {
				if ((n_caps>0) && (cap_index<n_caps)){
					caps[cap_index].len = _.str_parsed - caps[cap_index].index;
					cap_index++;
					_.caps++;
				}
				_.reg_parsed++;
				break;
			}
			case '<': {
				if (strstr(_.regex+_.reg_parsed, "<string>")==(_.regex+_.reg_parsed))
				{
					char end;
					if (_.ch=='\'')
						end = '\'';
					else if (_.ch=='"')
						end = '"';
					else{
						_.status = REGEX_PARSE_ERROR;
						goto end;
					}

					int i=0, isescape = FALSE;
					while (TRUE){
						char_move();
						if (isescape){
							isescape = FALSE;
							continue;
						}
						if (_.ch=='\\'){
							isescape=TRUE;
							continue;
						}
						else if (_.ch==end){
							char_move();
							break;
						}
					}
					_.reg_parsed += regex_move_step(_.regex+_.reg_parsed);
					break;
				}
			}
			default: {
				
				if (parse_atom(_.ch, _.regex+_.reg_parsed)==ATOM_PARSE_ERROR)
				{
					_.status = REGEX_PARSE_ERROR;
					goto end;
				}
				_.reg_parsed += regex_move_step(_.regex+_.reg_parsed);
				char_move();
				break;
			}
		}
	}
	end:{
		regex_status_t status;
		status.n_caps = _.caps;
		status.regex_parsed = _.reg_parsed;
		status.str_parsed = _.str_parsed;
		status.status = _.status;
		status.cont = memory_pointer_index(_.mg, status.str_parsed);
		return status;
	}
}


static enum parse_status parse_atom(int ch, char *regex)
{
	if (ch=='\0')
	{
		return ATOM_PARSE_ERROR;
	}
	switch(*(regex))
	{
		case '{': {
			int i = 0;
			regex++;
			for (; (regex[0]!='\0') && (regex[0]!='}'); regex=regex_move_step(regex)+regex)
			{
				if (parse_atom(ch, regex)==ATOM_PARSE_SUCCESS){
					return ATOM_PARSE_SUCCESS;
				}
			}
			return ATOM_PARSE_ERROR;
		}
		case '!': {
			regex++;
			return (parse_atom(ch, regex)==ATOM_PARSE_SUCCESS)?ATOM_PARSE_ERROR:ATOM_PARSE_SUCCESS;
		}
		case '<': {
			return parse_terminal(ch, regex);
		}
		default: {
			if (ch==*regex)
				return ATOM_PARSE_SUCCESS;
			return ATOM_PARSE_ERROR;
		}
	}
}


static struct {
	char *class;
	int (*func)(int);
}classes[]={
	{
		.class = "<digit>",.func = &isdigit
	},{
		.class = "<alpha>",.func = &isalpha
	},{
		.class = "<upper>",.func = &isupper
	},{
		.class = "<lower>",.func = &islower
	},{
		.class = "<alnum>",.func = &isalnum
	},{
		.class = "<blank>",.func = &isblank
	},{
		.class = NULL,.func = NULL
	}
};
static enum parse_status parse_terminal(int ch, char *regex)
{
	int i=0;
	for (i=0; classes[i].class!=NULL; i++)
	{
		if (strstr(regex, classes[i].class)==regex)
		{
			if (classes[i].func(ch))
			{
				return ATOM_PARSE_SUCCESS;
			}
			return ATOM_PARSE_ERROR;
		}
	}
	
	if (strstr(regex, "<wspace>")==regex){
		if ((ch=='\n') || isblank(ch))
			return ATOM_PARSE_SUCCESS;
		return ATOM_PARSE_ERROR;
	}
	ansi_error("Terminal %s not found.\n", regex);
	exit(1);
}


static int regex_move_step(char *regex)
{
	int i=0;
	switch(*regex)
	{
		case '<': {
			for (i=0; (regex[i]!='\0') && (regex[i]!='>'); i++);
			if (regex[i]=='\0'){
				ansi_error("< should be followed with > :: %s\n", _.regex);
				return -1;
			}
			return i+1;
		}
		case '{': {
			for (i=0; (regex[i]!='\0') && (regex[i]!='}'); i++);
			if (regex[i]=='\0'){
				ansi_error("{ should be followed with } :: %s\n", _.regex);
				return -1;
			}
			return i+1;
			
		}
		case '!': {
			return regex_move_step(regex+1)+1;
		}
		default: {
			return 1;
		}
	}
}


static inline void _char_update()
{
	if (_.string!=NULL){
		_.ch = _.string[_.str_parsed];
		return;
	}
	
	int ch = memory_get_move(&_.mg);
	_.ch = (ch==-1)?'\0':ch;
}

static inline void char_move()
{
	_.str_parsed++;
	if (_.string==NULL)
		_.mg.index++;
	_char_update();
}
