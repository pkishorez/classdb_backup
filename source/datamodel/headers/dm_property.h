/**
 * @file dm_property.h
 * @author Kishore
 * @date August 5, 2016, 2:33 PM 
 */

dm_property_t dm_property_create(const char *name);

char* dm_property_validate(const char *name);
void dm_property_json(json_create_t *json, dm_property_t property);