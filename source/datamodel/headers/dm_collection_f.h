/**
 * @file dm_collection_f.h
 * @author Kishore
 * @date August 5, 2016, 2:30 PM 
 */

dm_collection_t dm_collection_fixed_create(char *name);
char* dm_collection_fixed_add(dm_collection_t collection, dm_property_t property);
char* dm_collection_fixed_delete(dm_collection_t collection, char *name);
void dm_collection_fixed_json(json_create_t *json, dm_collection_fixed_t *collection_f);