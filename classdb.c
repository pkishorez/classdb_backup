/**
 * @file classdb.c
 * @author Kishore
 * @date June 2, 2016, 3:32 PM
 * 
 * @brief 
 * 
 * 
 */

#include "core/headers/core_file.h"
#include "core/headers/core_memory.h"
#include "core/headers/core_regex.h"
#include "core/headers/core_request.h"
#include "core/headers/core_socket.h"
#include "core/headers/core_epoll.h"
#include "core/headers/core_json.h"
#include "datamodel/headers/dm_collection_f.h"
#include "datamodel/headers/dm_property_fs.h"
#include "datamodel/headers/dm_unit.h"
#include "datamodel/headers/dm_datom.h"
#include "datamodel/headers/dm_collection.h"

main()
{
	memory_init();
	request_init();
	
	int mem_index = memory_create();
	
	json_create_t _json = json_start(memory_pointer_start(mem_index), TRUE), *json = &_json;
	
	dm_collection_t collection = dm_collection_fixed_create("User");
	dm_property_t property = dm_property_fs_create("Profile");
	dm_unit_t personal = dm_unit_create("Personal");
	dm_unit_t others = dm_unit_create("Others");
	
	dm_collection_fixed_add(collection, property);
	
	dm_unit_add(&personal, dm_datom_createstring("name", 100));
	dm_unit_add(&personal, dm_datom_createstring("father", 100));
	dm_unit_add(&personal, dm_datom_createstring("mother", 100));
	dm_unit_add(&personal, dm_datom_createboolean("ismale"));
	dm_unit_add(&personal, dm_datom_createint32("age"));
	dm_unit_add(&personal, dm_datom_createint64("Seconds"));
	
	dm_unit_add(&others, dm_datom_createstring("college", 100));
	dm_unit_add(&others, dm_datom_createstring("branch", 100));
	dm_unit_add(&others, dm_datom_createstring("bestfriend", 100));
	dm_unit_add(&others, dm_datom_createboolean("graduate"));
	dm_unit_add(&others, dm_datom_createint32("friends"));
	dm_unit_add(&others, dm_datom_createint64("marks"));
	
	dm_property_fs_add(property, personal);
	dm_property_fs_add(property, others);
	
	dm_collection_json(json, collection);
	json_end(json);
	
	memory_printAll(mem_index);
	
	return;
	
	int req_store = request_store();
	int server = socket_create_server("8000");
	socket_make_nonblocking(server);
	socket_listen(server);
	
	request_add(req_store, server, "r", FALSE, -1);
	
	while (1)
	{
		request_get_t reqs[10];
		int n = request_wait(req_store, reqs, 10, 2000);
		
		int i;
		for (i=0; i<n; i++)
		{
			if (reqs[i].fd==server)
			{
				if (epoll_can_read(reqs[i].events))
				{
					while (1)
					{
						int client = socket_accept_nonblocking(server);
						if (client==-1)
							break;
						
						request_get_t req = request_add(req_store, client, "r", TRUE, -1);
						//ansi_success("Accept connection : %d", req.req_id);
						if (req.req_id==-1){
							close(client);
						}
					}
				}
			}
			else if (epoll_can_read(reqs[i].events))
			{
				char buffer[512];
				while (1)
				{
					int r = read(reqs[i].fd, buffer, 512);
					if (r==0)
					{
						// End of file.
						request_delete(req_store, reqs[i].req_id);
						close(reqs[i].fd);
						break;
					}
					if (r==-1){
						http_status_t status = http_parse(reqs[i].http);
						if (status==HTTP_CONTINUE){
							printf("CONTINUE\n");
							break;
						}
						else{
							if (status==HTTP_ERROR){
								ansi_error("HTTP_ERROR\n");
							}
							else if (status==HTTP_SUCCESS){
								//ansi_success("HTTP_SUCCES\n");
							}
							request_modify(req_store, reqs[i].req_id, "w", 0);
							break;
						}
					}
					memory_appendmem(reqs[i].mem_index, buffer, r);
				}
			}
			else if (epoll_can_write(reqs[i].events))
			{
				while (1)
				{
					request_delete(req_store, reqs[i].req_id);
					close(reqs[i].fd);
					break;
				}
			}
			else{
				request_delete(req_store, reqs[i].req_id);
				close(reqs[i].fd);
			}
		}
		//request_debug();
	}
}