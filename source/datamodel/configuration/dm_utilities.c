/**
 * @file dm_utilities.c
 * @author Kishore
 * @date August 3, 2016, 11:24 PM
 * 
 * @brief 
 * 
 * 
 */

#include "common/headers/init.h"
#include "datamodel/headers/dm_utilities.h"
char* dm_validate_name(const char *name)
{
	if (name==NULL)
		return "Name not provided for creating property.";

	if (strlen(name)>=DM_NAME_SIZE || strlen(name)<1)
		return "Name of the property exceeded.";
	
	return NULL;
}