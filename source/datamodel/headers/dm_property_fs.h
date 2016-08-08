/**
 * @file dm_property_fs.h
 * @author Kishore
 * @date August 5, 2016, 2:34 PM 
 */

dm_property_t dm_property_fs_create(char *name);
char* dm_property_fs_add(dm_property_t property, dm_unit_t unit);
char* dm_property_fs_delete(dm_property_t property, char *name);
void dm_property_fs_json(json_create_t *json, dm_property_fs_t *property);