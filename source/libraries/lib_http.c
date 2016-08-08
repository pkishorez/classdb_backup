/**
 * @file lib_http.c
 * @author Kishore
 * @date July 28, 2016, 9:51 PM
 * 
 * @brief 
 * 
 * 
 */

#include "headers/lib_http.h"
#include "core/headers/core_memory.h"
#include "core/headers/core_regex.h"
#include "core/headers/core_json.h"


http_status_t http_parse_headers(http_request_t *http);

http_request_t http_init(memory_pointer_t ptr)
{
	http_request_t http;

	http.headers.length = 0;
	http.is_parsed = FALSE;
	http.ptr = ptr;

	http.parse._end = 0;
	http.parse.cont = ptr;
	return http;
}

http_status_t http_parse(http_request_t *http)
{
	char *headers_end = "\r\n\r\n";
	memory_pointer_t *cont = (memory_pointer_t *)&(http->parse.cont);
	//memory_printAll(http->ptr.mem_index);
	while (1)
	{
		assert(http->parse.cont.index<=http->parse.cont.mem_unit->filled);
		
		if (http->parse.cont.index==http->parse.cont.mem_unit->filled){
			if (http->parse.cont.mem_unit->next==NULL){
				return HTTP_CONTINUE;
			}
			http->parse.cont = memory_pointer_next(http->parse.cont);
			assert(http->parse.cont.mem_unit!=NULL);
		}
		int i=0;
		for (; http->parse.cont.index < http->parse.cont.mem_unit->filled; http->parse.cont.index++)
		{
			if (headers_end[http->parse._end]==http->parse.cont.mem_unit->data[i]){
				http->parse._end++;
				if (http->parse._end==4){
					http->is_parsed = TRUE;
					return http_parse_headers(http);
				}
			}
			else{
				http->parse._end = 0;
			}
		}
	}
}

http_status_t http_parse_headers(http_request_t *http)
{
	regex_cap_t caps[10];
	regex_status_t status;
	memory_pointer_t ptr = memory_pointer_start(http->ptr.mem_index);
	char *status1 = "GET (*! ) HTTP/<digit>.<digit>\r\n";
	char *status2 = "POST (*! ) HTTP/<digit>.<digit>\r\n";
	
	status = regex_parse_mem(status1, ptr, caps, 10);
	if (status.status==REGEX_PARSE_ERROR)
		status = regex_parse_mem(status2, ptr, caps, 10);
	
	if (status.status==REGEX_PARSE_ERROR){
		ansi_error("HTTP ERROR\n");
		memory_print(ptr, -1);
		return HTTP_ERROR;
	}
	http->headers.uri.ptr = memory_pointer_index(ptr, caps[0].index);
	http->headers.uri.len = caps[0].len;
	
	
	ptr = memory_pointer_index(ptr, status.str_parsed);
	
	// Headers parsing goes here.
	while (1)
	{
		char *header = "*<blank>(+!:):*<blank>(+!\r)\r\n";
		status = regex_parse_mem(header, ptr, caps, 10);
		if (status.status==REGEX_PARSE_ERROR)
			break;
		
		if ((caps[0].len>HTTP_HEADER_NAME_LENGTH) || (caps[1].len>HTTP_HEADER_VALUE_LENGTH)){
			ansi_error("Http header fields values exceeded limits.\n");
			return HTTP_ERROR;
		}
		char name[HTTP_HEADER_NAME_LENGTH+1];
		char value[HTTP_HEADER_VALUE_LENGTH+1];
		
		memory_copy0(memory_pointer_index(ptr, caps[0].index), name, caps[0].len);
		memory_copy0(memory_pointer_index(ptr, caps[1].index), value, caps[1].len);
		
		if (strstr(name, "Content-Length")==name){
			http->headers.content_length = atoi(value);
		}
		ptr = memory_pointer_index(ptr, status.str_parsed);
		
		char *trail = "\r\n";
		status = regex_parse_mem(trail, ptr, caps, 10);

		if (status.status==REGEX_PARSE_SUCCESS){
			// End of parsing Headers
			ansi_success("HTTP_SUCCESS\n");
			http->body.ptr = memory_pointer_index(ptr, 2);
			
			char *json = "json=";
			status=regex_parse_mem(json, http->body.ptr, NULL, 0);
			if (status.status==REGEX_PARSE_SUCCESS){
				http->body.json = json_parse_init(memory_pointer_index(http->body.ptr, status.str_parsed));
				printf("BODY JSON : ");
				memory_print(http->body.ptr, -1);
			}

			memory_free(http->ptr.mem_index);
			http->write = memory_pointer_start(http->ptr.mem_index);
			printf("DONE\n");
			return HTTP_SUCCESS;
		}
	}

	ansi_error("ERROR HERE\n");
	return HTTP_ERROR;
}