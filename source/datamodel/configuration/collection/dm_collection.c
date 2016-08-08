/**
 * @file dm_collection.c
 * @author Kishore
 * @date August 4, 2016, 1:07 AM
 * 
 * @brief 
 * 
 * 
 */


#include "common/headers/init.h"
#include "common/headers/common_utility.h"
#include "core/headers/core_json.h"
#include "datamodel/headers/dm_collection.h"
#include "datamodel/headers/dm_utilities.h"


dm_collection_t dm_collection_create(const char *name)
{
	assert(dm_collection_validate(name)==NULL);

	dm_collection_t collection;
	strcpy(collection.name, name);
	
	return collection;
}


char* dm_collection_validate(const char *name)
{
	return dm_validate_name(name);
}

void dm_collection_json(json_create_t *json, dm_collection_t collection)
{
	json_add_string(json, "name", collection.name);
	switch(collection.type)
	{
		case DM_COLLECTION_FIXED:
			json_add_string(json, "type", "COLLECTION_FIXED");
			dm_collection_fixed_json(json, (dm_collection_fixed_t *)collection.collection);
			break;
		case DM_COLLECTION_VARIABLE:
			ansi_error("Specified collection type is not yet implemented");
			break;
		default:{
			ansi_error_abort("INVALID COLLECTION TYPE SPECIFIED.\n");
		}
	}
}