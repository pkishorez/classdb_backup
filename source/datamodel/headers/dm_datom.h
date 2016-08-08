/**
 * @file dm_datom.h
 * @author Kishore
 * @date August 2, 2016, 11:27 AM 
 */

datom_t dm_datom_createint32(char *name);
datom_t dm_datom_createint64(char *name);
datom_t dm_datom_createstring(char *name, int size);
datom_t dm_datom_createboolean(char *name);
int dm_datom_size(datom_t datom);
int dm_datom_align(datom_t datom);

char* dm_datom_validate(char *name);
void dm_datom_json(json_create_t *json, datom_t datom);