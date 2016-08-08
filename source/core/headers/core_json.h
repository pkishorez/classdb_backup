/**
 * @file core_json.h
 * @author Kishore
 * @date May 17, 2016, 1:18 AM 
 */

#include "common/headers/init.h"

json_create_t json_start(memory_pointer_t ptr, int beautify);

	void json_add_number(json_create_t *json, char *key, long number);
	void json_add_string(json_create_t *json, char *key, const char *str);
	void json_add_boolean(json_create_t *json, char *key, int bool);
	void json_add_null(json_create_t *json, char *key);
	
	void json_add_json(json_create_t *json, char *key);
	void json_end_json(json_create_t *json);
	
	void json_add_array(json_create_t *json, char *key);
	void json_end_array(json_create_t *json);

void json_end(json_create_t *json);


json_t json_parse_init(memory_pointer_t ptr);

json_t json_p(json_t targetjson, char *targetkey);
json_t json_array_p(json_t targetjson, int index);

int json_p_num(json_t json, char *key);
json_t json_p_str(json_t json, char *key);
json_t json_p_json(json_t json, char *key);