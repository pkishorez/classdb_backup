/**
 * @file main.c
 * @author Kishore
 * @date March 19, 2016, 12:36 PM
 * 
 * @brief 
 * 
 * 
 */

#include "core/headers/core_ansi.h"
#include "core/headers/core_bson.h"
#include "core/headers/core_request.h"
#include "core/headers/core_memory.h"
#include "core/headers/core_socket.h"
#include "core/headers/core_epoll.h"
#include "core/headers/core_regex.h"
#include "libraries/headers/lib_http.h"
#include "core/headers/core_json.h"
#include "core/headers/core_file.h"
#include "datamodel/headers/dm_property_fixed_single.h"
#include "datamodel/headers/dm_collection_fixed.h"
#include "datamodel/headers/dm_collection.h"
#include "datamodel/headers/dm_datom.h"


void read_request(request_get_t request);
static int req_fd;
int main()
{
	memory_init();
	

	collection_init("config.json");
	collection_save("config1.json");
	
	return 0;
}