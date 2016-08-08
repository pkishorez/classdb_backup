/**
 * @file dm_collection.h
 * @author Kishore
 * @date August 5, 2016, 2:29 PM 
 */

dm_collection_t dm_collection_create(const char *name);
char* dm_collection_validate(const char *name);
void dm_collection_json(json_create_t *json, dm_collection_t collection);