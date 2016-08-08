/**
 * @file core_json.c
 * @author Kishore
 * @date May 17, 2016, 1:17 AM
 * 
 * @brief 
 * 
 * 
 */

#include "headers/core_memory.h"
#include "headers/core_json.h"
#include "headers/core_regex.h"


// Generate JSON code goes here.

#ifndef json_write
	#define json_write(ptr, str) ptr = memory_writestr(ptr, str);
#endif

void _json_beautify(json_create_t *json);
void _json_key(json_create_t *json, char *key);
void _json_add_nest_custom(json_create_t *json, char *key, char *custom);
void _json_end_nest_custom(json_create_t *json, char *custom);

void _json_beautify(json_create_t *json)
{
	if (json->multiple_propeties)
		json_write(json->ptr, ",");
	if (json->beautify)
	{
		json_write(json->ptr, "\n");
		int i = 0;
		for (i=0; i<(json->nest_count+1); i++)
			json_write(json->ptr, "   ");
	}
}

void _json_key(json_create_t *json, char *key)
{
	if (key==NULL)
		return;
	json_write(json->ptr, "\"");
	json_write(json->ptr, key);
	json_write(json->ptr, "\" : ");
}

void _json_add_nest_custom(json_create_t *json, char *key, char *custom)
{
	_json_beautify(json);
	_json_key(json, key);
	
	json_write(json->ptr, custom);
	json->nest_count++;
	json->multiple_propeties = FALSE;
}

void _json_end_nest_custom(json_create_t *json, char *custom)
{
	assert(json->nest_count>0);
	json->nest_count--;
	
	json->multiple_propeties = FALSE;
	
	_json_beautify(json);
	json_write(json->ptr, custom);
	json->multiple_propeties = TRUE;
}


json_create_t json_start(memory_pointer_t ptr, int beautify)
{
	json_create_t json;
	json.beautify = beautify;
	json.nest_count = 0;
	json.multiple_propeties = FALSE;
	
	json_write(ptr, "{");
	json.ptr = ptr;
	return json;
}

void json_add_number(json_create_t *json, char *key, long number)
{
	char output[50];
	snprintf(output, 50, "%ld", number);
	
	_json_beautify(json);
	_json_key(json, key);
	json_write(json->ptr, output);
	json->multiple_propeties = TRUE;
}

void json_add_string(json_create_t *json, char *key, const char *str)
{
	_json_beautify(json);
	_json_key(json, key);

	json_write(json->ptr, "\"");
	json_write(json->ptr, str);
	json_write(json->ptr, "\"");
	json->multiple_propeties = TRUE;
}

void json_add_boolean(json_create_t *json, char *key, int bool)
{
	_json_beautify(json);
	_json_key(json, key);

	if (bool){
		json_write(json->ptr, "true");
	}
	else{
		json_write(json->ptr, "false");
	}
	json->multiple_propeties = TRUE;
}

void json_add_null(json_create_t *json, char *key)
{
	_json_beautify(json);
	_json_key(json, key);

	json_write(json->ptr, "null");
	json->multiple_propeties = TRUE;
}

void json_add_json(json_create_t *json, char *key)
{
	_json_add_nest_custom(json, key, "{");
}

void json_add_array(json_create_t *json, char *key)
{
	_json_add_nest_custom(json, key, "[");
}

void json_end_json(json_create_t *json)
{
	_json_end_nest_custom(json, "}");
}

void json_end_array(json_create_t *json)
{
	_json_end_nest_custom(json, "]");
}

void json_end(json_create_t *json)
{
	if (json->beautify)
		json_write(json->ptr, "\n");
	
	json_write(json->ptr, "}");
}


json_t _json_get_property(memory_pointer_t ptr);
int _json_p_jsonarray(memory_pointer_t ptr, char customopen, char customclose);


json_t json_parse_init(memory_pointer_t ptr)
{
	json_t json = _json_get_property(ptr);
	if (json.type==JSON_DATATYPE_INVALID){
		ansi_error("Error trying to init json\n");
	}
	return json;
}


json_t json_p(json_t targetjson, char *targetkey)
{
	regex_status_t status;
	regex_cap_t caps[2];
	memory_pointer_t ptr = targetjson.pointer;
	json_t json;
	json.type = JSON_DATATYPE_INVALID;
	int ch = memory_pointer_get(ptr, 0);
	if (ch==-1)
		return json;
	
	if (ch!='{')
		return json;
	ptr = memory_pointer_index(ptr, 1);

	// Parse each property here.
	while (1)
	{
		char ch = memory_pointer_get(ptr, 0);
		if (ch=='}'){
			json.type = JSON_DATATYPE_INVALID;
			json.len = 0;
			return json;
		}
		
		// GET KEY.
		char *regex_key = "*<wspace>(<string>)*<wspace>:*<wspace>";
		status = regex_parse_mem(regex_key, ptr, caps, 2);
		if (status.status==REGEX_PARSE_ERROR){
			return json;
		}
		char key[caps[0].len - caps[0].index +1];
		memory_copy0(memory_pointer_index(ptr, caps[0].index+1), key, caps[0].len-2);
		ptr = memory_pointer_index(ptr, status.str_parsed);
		
		json = _json_get_property(ptr);
		
		if (json.type==JSON_DATATYPE_INVALID)
			return json;
		if (strstr(key, targetkey)==key){
			return json;
		}
		
		ptr = memory_pointer_index(ptr, json.len);
		status = regex_parse_mem("*<wspace>,*<wspace>", ptr, NULL, 0);
		if (status.status==REGEX_PARSE_SUCCESS)
			ptr = memory_pointer_index(ptr, status.str_parsed);
	}
}

json_t json_array_p(json_t targetjson, int index)
{
	regex_status_t status;
	regex_cap_t caps[2];
	memory_pointer_t ptr = targetjson.pointer;
	json_t json;
	json.type = JSON_DATATYPE_INVALID;
	int ch = memory_pointer_get(ptr, 0);
	if (ch==-1)
		return json;
	
	if (ch!='[')
		return json;
	ptr = memory_pointer_index(ptr, 1);
	
	// Parse each property here.
	int lindex = 0;
	while (1)
	{
		char ch = memory_pointer_get(ptr, 0);
		if (ch=='}')
			return json;
		
		// GET KEY.
		char *regex_key = "*<wspace>";
		status = regex_parse_mem(regex_key, ptr, caps, 2);
		if (status.status==REGEX_PARSE_ERROR){
			return json;
		}
		ptr = memory_pointer_index(ptr, status.str_parsed);
		
		json = _json_get_property(ptr);
		if (json.type==JSON_DATATYPE_INVALID)
			return json;
		if (lindex++==index){
			return json;
		}
		
		ptr = memory_pointer_index(ptr, json.len);
		status = regex_parse_mem("*<wspace>,*<wspace>", ptr, NULL, 0);
		if (status.status==REGEX_PARSE_SUCCESS)
			ptr = memory_pointer_index(ptr, status.str_parsed);
	}
}

int json_p_num(json_t json, char *key)
{
	json = json_p(json, key);
	
	if (json.type==JSON_DATATYPE_NUMBER){
		char num[json.len+1];
		memory_copy0(json.pointer, num, json.len);
		return atoi(num);
	}
	ansi_error("Error trying to read Number prop : %s from json\n", key);
	exit(1);
}
json_t json_p_str(json_t json, char *key)
{
	json = json_p(json, key);
	if (json.type==JSON_DATATYPE_STRING){
		json.len = json.len-2;
		json.pointer = memory_pointer_index(json.pointer, 1);
		return json;
	}
	ansi_error("Error reading prop : %s from json. Not String.\n", key);
	exit(1);
}
json_t json_p_json(json_t json, char *key)
{
	json = json_p(json, key);
	if(json.type==JSON_DATATYPE_JSON)
		return json;
	
	ansi_error("Error trying to read prop : %s from json. Not JSON.\n", key);
	exit(1);
}
json_t _json_get_property(memory_pointer_t ptr)
{
	json_t json;
	json.pointer = ptr;
	json.type = JSON_DATATYPE_INVALID;
	// Check if value is string.
	int len;
	if ((len = _json_p_str(ptr))>0){
		json.len = len;
		json.type = JSON_DATATYPE_STRING;
	}

	// Check if value is number
	else if ((len = _json_p_number(ptr))>0){
		json.len = len;
		json.type = JSON_DATATYPE_NUMBER;
	}

	// Check if nested JSON.
	else if ((len = _json_p_json(ptr))>0){
		json.len = len;
		json.type = JSON_DATATYPE_JSON;
	}

	// Check if nested ARRAY.
	else if ((len = _json_p_array(ptr))>0){
		json.len = len;
		json.type = JSON_DATATYPE_ARRAY;
	}

	return json;
}
typedef enum {
	JSON_PARSE_ERROR = -1,
	JSON_PARSE_INVALID = 0
}json_parse_code;


int _json_p_str(memory_pointer_t ptr)
{
	char *regex = "<string>";
	regex_status_t status = regex_parse_mem(regex, ptr, NULL, 0);
	if (status.status==REGEX_PARSE_SUCCESS){
		return status.str_parsed;
	}
	if (status.str_parsed==0)
		return 0;
	return -1;
}
int _json_p_number(memory_pointer_t ptr)
{
	char *number = "+<digit>";
	char *number2 = "-+<digit>";
	regex_status_t status = regex_parse_mem(number, ptr, NULL, 0);
	if (status.status==REGEX_PARSE_ERROR){
		status = regex_parse_mem(number2, ptr, NULL, 0);
		if (status.status==REGEX_PARSE_ERROR){
			return 0;
		}
	}
	return status.str_parsed;
}

int _json_p_json(memory_pointer_t ptr)
{
	return _json_p_jsonarray(ptr, '{', '}');
}

int _json_p_array(memory_pointer_t ptr)
{
	return _json_p_jsonarray(ptr, '[', ']');
}


int _json_p_jsonarray(memory_pointer_t ptr, char customopen, char customclose)
{
	int ch = memory_pointer_get(ptr, 0), len = 1;
	char end;
	if (ch!=customopen)
		return 0;
	
	ptr = memory_pointer_index(ptr, 1);
	int i=0, nest = 0;
	
	for (ch=memory_get_move(&ptr); ch!=-1; ch=memory_get_move(&ptr), len++)
	{
		if (ch=='"' || ch=='\''){
			int str_len = _json_p_str(ptr);
			if (str_len==-1 || str_len==0){
				return -1;
			}
			
			ptr = memory_pointer_index(ptr, str_len);
			len += str_len;
			
			ch = memory_get_move(&ptr);
			if (ch==-1)
				return -1;
		}
		if (ch==customopen){
			nest++;
		}
		else if (ch==customclose)
		{
			if (nest==0)
				return len;
			nest--;
		}
	}
	return -1;
}
