/**
 * @file lib_init.h
 * @author Kishore
 * @date March 19, 2016, 11:35 PM 
 */

#ifndef UNIQUE_LIB_DEFINITIONS_IDENTIFIER
	#define UNIQUE_LIB_DEFINITIONS_IDENTIFIER
	
	/**
	 * ==================================
	 * ANSI RELATED DEFINITIONS GOES HERE
	 * ==================================
	 */

	typedef enum {
		HTTP_POST,
		HTTP_GET
	}http_request_type_t;
	typedef struct {
		memory_pointer_t ptr;
		int is_parsed;
		struct {
			memory_pointer_t cont;
			int _end;
		}parse;
		struct {
			int length;
			int content_length;
			http_request_type_t type;
			struct{
				memory_pointer_t ptr;
				int len;
			}uri;
		}headers;
		struct{
			memory_pointer_t ptr;
			json_t json;
		}body;
		memory_pointer_t write;
	}http_request_t;
	
	typedef enum http_status{
		HTTP_ERROR,
		HTTP_CONTINUE,
		HTTP_SUCCESS
	}http_status_t;
	
	#define HTTP_HEADER_NAME_LENGTH 100
	#define HTTP_HEADER_VALUE_LENGTH 200

	/**
	 * ===================================
	 *   WEBSOCKET DEFINITIONS GOES HERE
	 * ===================================
	 */

	typedef struct{
		memory_pointer_t in;
		memory_pointer_t out;
	}websocket_t;
	
#endif