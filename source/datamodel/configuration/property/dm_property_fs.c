/**
 * @file dm_property_fs.c
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
#include "datamodel/headers/dm_property_fs.h"
#include "datamodel/headers/dm_utilities.h"

#include "datamodel/headers/dm_unit.h"

dm_property_t dm_property_fs_create(char *name)
{
	dm_property_t property = dm_property_create(name);
	
	dm_property_fs_t *prop_fs = (dm_property_fs_t *)malloc(sizeof(dm_property_fs_t));
	prop_fs->size = -1;
	prop_fs->n_units = 0;
	prop_fs->units = NULL;
	
	property.property = (void*)prop_fs;
	property.type = DM_PROPERTY_FS;
	return property;
}

char* dm_property_fs_add(dm_property_t property, dm_unit_t unit)
{
	assert(property.type==DM_PROPERTY_FS);
	dm_property_fs_t *prop = (dm_property_fs_t *)property.property;
	assert(prop->n_units>=0);
	
	dm_unit_t *lunit;
	
	prop->units = (dm_unit_t *)malloc_addspace(prop->units, sizeof(unit), prop->n_units, 1);
	lunit = prop->units + prop->n_units++;
	
	*lunit = unit;
	
	return "Successfully added unit to property.";
}

char* dm_property_fs_delete(dm_property_t property, char *name)
{
	assert(property.type==DM_PROPERTY_FS);
	dm_property_fs_t *prop = (dm_property_fs_t *)property.property;
	assert(prop->n_units>=0);
	
	if (prop->units==NULL)
	{
		return "No Units present to delete.";
	}
	
	int index=0;
	dm_unit_t *unit;
	for (index=0; index<prop->n_units; index++)
	{
		unit = prop->units+index;
		if (!strcmp(name, unit->name))
			break;
	}
	if (index==prop->n_units)
		return "Unit with name specified is not present.";
	
	prop->units = (dm_unit_t *)malloc_delUnit(prop->units, sizeof(dm_unit_t), prop->n_units--, index);
	return "Successfully deleted.";
}


void dm_property_fs_json(json_create_t *json, dm_property_fs_t *property)
{
	json_add_number(json, "n_units", property->n_units);
	json_add_number(json, "size", property->size);
	
	int i;
	json_add_array(json, "units");
	for (i=0; i<property->n_units; i++)
	{
		json_add_json(json, NULL);
		dm_unit_json(json, property->units[i]);
		json_end_json(json);
	}
	json_end_array(json);
}