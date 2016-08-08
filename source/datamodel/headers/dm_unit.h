/**
 * @file dm_unit.h
 * @author Kishore
 * @date August 2, 2016, 11:27 AM 
 */

dm_unit_t dm_unit_create(const char *name);
char* dm_unit_add(dm_unit_t *unit, datom_t datom);
char* dm_unit_delete(dm_unit_t *unit, char *name);
dm_block_t dm_unit_freeblock(const dm_unit_t *const unit);

char* dm_unit_validate(const char *name);
void dm_unit_json(json_create_t *json, dm_unit_t unit);