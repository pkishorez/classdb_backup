/**
 * @file dm_unit.c
 * @author Kishore
 * @date August 1, 2016, 10:48 PM
 * 
 * @brief 
 * 
 * 
 */

#include "common/headers/init.h"
#include "common/headers/common_utility.h"
#include "core/headers/core_json.h"
#include "datamodel/headers/dm_unit.h"
#include "datamodel/headers/dm_utilities.h"

dm_unit_t dm_unit_create(const char *name)
{
	assert(dm_unit_validate(name)==NULL);

	dm_unit_t unit;
	strcpy(unit.name, name);
	unit.size = -1;
	unit.offset = -1;
	unit.datoms = NULL;
	unit.n_datoms = 0;
	
	return unit;
}

char* dm_unit_add(dm_unit_t *unit, datom_t datom)
{
	unit->datoms = (datom_t *)malloc_addspace(unit->datoms, sizeof(datom_t), unit->n_datoms++, 1);
	unit->datoms[unit->n_datoms-1] = datom;
	return NULL;
}

char* dm_unit_delete(dm_unit_t *unit, char *name)
{
	if (unit->datoms==NULL)
		return "No datoms present to delete.";
	int index=0;
	datom_t *datom;
	for (index=0; index<unit->n_datoms; index++)
	{
		datom = unit->datoms+index;
		if (!strcmp(name, datom->name))
			break;
	}
	if (index==unit->n_datoms)
		return "Datom with name specified is not present.";
	
	unit->datoms = (datom_t *)malloc_delUnit(unit->datoms, sizeof(datom_t), unit->n_datoms--, index);
	return NULL;
}

dm_block_t dm_unit_freeblock(const dm_unit_t *const unit)
{
	dm_block_t block;
	
	if (unit->n_datoms==0){
		block.offset = 0;
		block.freesize = unit->size;
	}
	else{
		datom_t ldatom = unit->datoms[unit->n_datoms-1];
		block.offset = ldatom.offset+dm_datom_size(ldatom);
		block.freesize = unit->size - block.offset;
		return block;
	}
}

char* dm_unit_validate(const char *name)
{
	return dm_validate_name(name);
}


void dm_unit_json(json_create_t *json, dm_unit_t unit)
{
	json_add_string(json, "name", unit.name);
	json_add_number(json, "offset", unit.offset);
	json_add_number(json, "size", unit.size);
	json_add_number(json, "n_datoms", unit.n_datoms);
	
	int i=0;
	json_add_array(json, "datoms");
	for (i=0; i<unit.n_datoms; i++)
	{
		json_add_json(json, NULL);
		dm_datom_json(json, unit.datoms[i]);
		json_end_json(json);
	}
	json_end_array(json);
}