/**
 * @file lib_http.h
 * @author Kishore
 * @date July 28, 2016, 11:32 PM 
 */

#include "common/headers/init.h"

http_request_t http_init(memory_pointer_t ptr);
http_status_t http_parse(http_request_t *http);
