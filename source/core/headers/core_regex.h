/**
 * @file core_regex.h
 * @author Kishore
 * @date March 27, 2016, 9:19 PM 
 */

#include "common/headers/init.h"

enum regex_error regex_validate_mem(char *regex, memory_pointer_t mg);

regex_status_t regex_parse_mem
	(char *regex, memory_pointer_t mg, regex_cap_t *caps, int n_caps);
regex_status_t regex_parse_str
	(char *regex, char *str, regex_cap_t *caps, int n_caps);