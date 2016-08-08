/**
 * @file dm_datom.c
 * @author Kishore
 * @date August 2, 2016, 11:27 AM
 * 
 * @brief 
 * 
 * 
 */

#include "common/headers/init.h"
#include "core/headers/core_json.h"
#include "datamodel/headers/dm_datom.h"
#include "datamodel/headers/dm_utilities.h"

char* datom_type(dm_datom_type_t type);
static datom_t _dm_datom_create(char *name);

static datom_t _dm_datom_create(char *name)
{
	assert(dm_datom_validate(name)==NULL);

	datom_t datom;
	strcpy(datom.name, name);
	datom.offset = -1;
	datom._size = -1;
	return datom;
}


datom_t dm_datom_createint32(char *name)
{
	datom_t datom = _dm_datom_create(name);
	datom.type = DM_INT32;
	return datom;
}

datom_t dm_datom_createint64(char *name)
{
	datom_t datom = _dm_datom_create(name);
	datom.type = DM_INT64;
	return datom;
}

datom_t dm_datom_createstring(char *name, int size)
{
	assert(size>0);
	datom_t datom = _dm_datom_create(name);
	datom.type = DM_STRING;
	datom._size = size;
	return datom;
}

datom_t dm_datom_createboolean(char *name)
{
	datom_t datom = _dm_datom_create(name);
	datom.type = DM_BOOLEAN;
	return datom;
}

char* datom_type(dm_datom_type_t type)
{
	switch(type)
	{
		case DM_BOOLEAN:
			return "boolean";
		case DM_STRING:
			return "string";
		case DM_INT32:
			return "int32";
		case DM_INT64:
			return "int64";
		default:
			return "INVALID";
	}
}
int dm_datom_size(datom_t datom)
{
	switch(datom.type)
	{
		case DM_BOOLEAN:
			return 1;
		case DM_INT32:
			return 4;
		case DM_INT64:
			return 8;
		case DM_STRING:
			return datom._size;
		default:{
			ansi_error_abort("Invalid Datom : %s. Doesn't belong to any datatype.\n", datom.name);
		}
	}
	ansi_error_abort("This situation never happens :)\n");
}

int dm_datom_align(datom_t datom)
{
	switch(datom.type)
	{
		case DM_BOOLEAN:
			return 1;
		case DM_INT32:
			return 4;
		case DM_INT64:
			return 8;
		case DM_STRING:
			return 1;
		default:{
			ansi_error_abort("Invalid Datom : %s. Doesn't belong to any datatype.\n", datom.name);
		}
	}
	ansi_error_abort("This situation never happens :)\n");
}

char* dm_datom_validate(char *name)
{
	return dm_validate_name(name);
}

void dm_datom_json(json_create_t *json, datom_t datom)
{
	json_add_string(json, "name", datom.name);
	json_add_number(json, "offset", datom.offset);
	json_add_number(json, "size", datom._size);
	json_add_string(json, "type", datom_type(datom.type));
}