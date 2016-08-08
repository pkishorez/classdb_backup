/**
 * @file dm_block.c
 * @author Kishore
 * @date August 2, 2016, 6:08 PM
 * 
 * @brief 
 * 
 * 
 */

#include "common/headers/init.h"


char* dm_block_add_datom(dm_block_t block, datom_t *datom)
{
	assert(dm_datom_size(*datom)>0);
	assert(dm_datom_align(*datom)>=1);

	int pad = block.offset%dm_datom_align(*datom);
	
	if (dm_datom_size(*datom) < (block.freesize - pad)){
		datom->offset = block.offset + pad;
		return NULL;
	}
	return "Couldn't add datom. Not enough space.";
}