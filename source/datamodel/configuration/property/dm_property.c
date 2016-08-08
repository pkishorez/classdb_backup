/**
 * @file dm_property.c
 * @author Kishore
 * @date August 2, 2016, 2:47 PM
 * 
 * @brief 
 * 
 * 
 */


#include "common/headers/init.h"
#include "common/headers/common_utility.h"
#include "core/headers/core_json.h"
#include "datamodel/headers/dm_property.h"
#include "datamodel/headers/dm_utilities.h"


dm_property_t dm_property_create(const char *name)
{
	assert(dm_property_validate(name)==NULL);

	dm_property_t prop;
	strcpy(prop.name, name);
	
	return prop;
}


char* dm_property_validate(const char *name)
{
	return dm_validate_name(name);
}

void dm_property_json(json_create_t *json, dm_property_t property)
{
	json_add_string(json, "name", property.name);
	switch(property.type)
	{
		case DM_PROPERTY_FS:{
			json_add_string(json, "type", "PROPERTY_FIXED_SINGLE");
			dm_property_fs_json(json, (dm_property_fs_t *)property.property);
			break;
		}
	}
}