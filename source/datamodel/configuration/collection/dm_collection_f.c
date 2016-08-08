/**
 * @file dm_collection_f.c
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
#include "datamodel/headers/dm_collection_f.h"
#include "datamodel/headers/dm_utilities.h"

dm_collection_t dm_collection_fixed_create(char *name)
{
	dm_collection_t collection = dm_collection_create(name);
	
	dm_collection_fixed_t *collection_f = (dm_collection_fixed_t *)malloc(sizeof(dm_collection_fixed_t));
	collection_f->size = -1;
	collection_f->n_properties = 0;
	collection_f->properties = NULL;
	
	collection.collection = (void*)collection_f;
	collection.type = DM_COLLECTION_FIXED;
	return collection;
}

char* dm_collection_fixed_add(dm_collection_t collection, dm_property_t property)
{
	assert(collection.type==DM_COLLECTION_FIXED);
	dm_collection_fixed_t *fixed = (dm_collection_fixed_t *)collection.collection;
	assert(fixed->n_properties>=0);
	
	dm_property_t *lprop;
	
	fixed->properties = (dm_property_t *)malloc_addspace(
			fixed->properties,
			sizeof(dm_property_t), 
			fixed->n_properties,
			1
		);
	
	lprop = fixed->properties + fixed->n_properties++;
	
	*lprop = property;
	
	return "Successfully Added Property.";
}

char* dm_collection_fixed_delete(dm_collection_t collection, char *name)
{
	dm_collection_fixed_t *collection_fixed = (dm_collection_fixed_t *)collection.collection;
	
	if (collection_fixed->properties==NULL)
	{
		return "No Units present to delete.";
	}
	
	int index=0;
	dm_property_t *property;
	for (index=0; index<collection_fixed->n_properties; index++)
	{
		property = collection_fixed->properties+index;
		if (!strcmp(name, property->name))
			break;
	}
	if (index==collection_fixed->n_properties)
		return "Unit with name specified is not present.";
	
	collection_fixed->properties = 
			(dm_property_t *)malloc_delUnit(
				collection_fixed->properties,
				sizeof(dm_property_t),
				collection_fixed->n_properties--,
				index
			);
	return NULL;
}


void dm_collection_fixed_json(json_create_t *json, dm_collection_fixed_t *collection_f)
{
	int i;
	json_add_number(json, "size", collection_f->size);
	json_add_number(json, "n_properties", collection_f->n_properties);
	
	json_add_array(json, "properties");
	for (i=0; i<collection_f->n_properties; i++)
	{
		json_add_json(json, NULL);
		dm_property_json(json, collection_f->properties[i]);
		json_end_json(json);
	}
	json_end_array(json);
}