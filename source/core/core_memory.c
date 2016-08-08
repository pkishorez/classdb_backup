/**
 * @file core_memory.c
 * @author Kishore
 * @date March 26, 2016, 9:56 PM
 * 
 * @brief 
 * 
 * 
 */

#include "headers/core_memory.h"


static struct {
	memory_instance_t *instances;
	memory_unit_t *units;
	memory_unit_t *unit_start;
	int instance_start;
}_;

static memory_unit_t* instance_addUnit(memory_instance_t *instance);
static memory_unit_t* unit_getNew(void);
static void instance_units_freeAll(memory_instance_t *instance);


/**
 * Initializes memory  
 */
void memory_init()
{
	ansi_sinfo("Initializing Memory Module...");
	
	int i=0;
	_.units = (memory_unit_t *)malloc(sizeof(memory_unit_t)*MEMORY_N_UNITS);
	_.unit_start = _.units;
	for (i=0; i<MEMORY_N_UNITS; i++)
	{
		_.units[i].filled = 0;
		_.units[i].sindex = 0;
		_.units[i].next = _.units+i+1;
	}
	_.units[MEMORY_N_UNITS-1].next = NULL;
	
	// Initialize Instance blocks
	_.instances = (memory_instance_t *)malloc(sizeof(memory_instance_t)*MEMORY_N_INSTANCES);
	for (i=0; i<MEMORY_N_INSTANCES; i++)
	{
		_.instances[i].start = NULL;
		_.instances[i].end = NULL;
		_.instances[i].free = TRUE;
		_.instances[i].next = i+1;
		_.instances[i].error = FALSE;
	}
	_.instances[MEMORY_N_INSTANCES-1].next = -1;
	ansi_info("\rOK >> Memory Module.\n");
}

/**
 * Creates an memory instance in memory module.
 * @returns created memory instance identifier.
 */
int memory_create()
{
	int freeinstance = _.instance_start;
	if (freeinstance==-1){
		ansi_error("Maximum number of memory instances reached...");
		return -1;
	}
	assert(_.instances[freeinstance].free);

	memory_unit_t *freeunit = unit_getNew();
	if (freeunit==NULL){
		ansi_error("No free units found. Couldn't create memory instance.");
		return -1;
	}
	
	_.instances[freeinstance].free  = FALSE;
	_.instances[freeinstance].start = freeunit;
	_.instances[freeinstance].end   = freeunit;
	_.instances[freeinstance].error = FALSE;
	
	_.instance_start = _.instances[_.instance_start].next;

	return freeinstance;
}

/**
 * Deletes memory instance with id `instance_index`
 * @param instance_index : Id of instance that needs to be deleted.
 */
void memory_delete(int instance_index)
{
	assert(instance_index>=0 && instance_index<MEMORY_N_INSTANCES);
	assert(!_.instances[instance_index].free);

	instance_units_freeAll(_.instances + instance_index);
	_.instances[instance_index].next = _.instance_start;
	_.instances[instance_index].free = TRUE;
	_.instance_start = instance_index;
}

void memory_free(int instance_index)
{
	assert(instance_index>=0 && instance_index<MEMORY_N_INSTANCES);
	assert(!_.instances[instance_index].free);

	instance_units_freeAll(_.instances + instance_index);
	memory_unit_t *freeunit = unit_getNew();
	_.instances[instance_index].start = freeunit;
	_.instances[instance_index].end   = freeunit;
	
}


/**
 * Overwrites data stored in memory index at memory pointer `pointer`.
 * @param pointer : Pointer from where data should be overwritten.
 * @param data : Data with which the memory should be overwritten.
 * @param len : Amount of data that needs to be overwritten.
 * @returns length of data overwritten.
 * 
 * If Memory pointed is less than length of data, this function tries to
 * expand the memory space and append the data.
 */
memory_pointer_t memory_write(memory_pointer_t pointer, const char *data, int len)
{	
	memory_unit_t *unit = pointer.mem_unit;

	int copied=0;
	assert(len>=0);
	
	if (unit==NULL)
		return pointer;
	
	while (copied!=len)
	{
		if (pointer.index>=MEMORY_UNIT_MAXSIZE){
			assert(pointer.index==MEMORY_UNIT_MAXSIZE);
			
			pointer = memory_pointer_next(pointer);
			if (pointer.mem_unit==NULL){
				unit = instance_addUnit(_.instances + pointer.mem_index);
				pointer.mem_unit = unit;
				if (unit==NULL)
				{
					return pointer;
				}
				pointer.index = unit->sindex;
			}
		}
		int size = MEMORY_UNIT_MAXSIZE - pointer.index;
		int copy = (size>=(len-copied)) ? len-copied : size;
		memcpy(unit->data+pointer.index, data+copied, copy);
		copied+=copy;
		pointer.index += copy;
		unit->filled += copy;
	}
	return pointer;
}


memory_pointer_t memory_writestr(memory_pointer_t pointer, const char *data)
{
	return memory_write(pointer, data, strlen(data));
}

int memory_pointer_get(memory_pointer_t ptr, int index)
{
	if (ptr.mem_unit==NULL)
		return -1;
	ptr = memory_pointer_index(ptr, index);
	if (ptr.mem_unit==NULL)
		return -1;
	return ptr.mem_unit->data[ptr.index];
}

/**
 * Appends data of length len to memory instance.
 * @param mem_index: Memory instance id.
 * @param data: Character array that needs to be appended.
 * @param len: Length of string that needs to be appended.
 * @returns Amount of data appended.
 */
memory_pointer_t memory_appendmem(int mem_index, const char *data, int len)
{
	return memory_write(memory_pointer_end(mem_index), data, len);
}

/**
 * Append string to memory instance.
 * @param mem_index: Memory instance identifier.
 * @param data: String that needs to be appended.
 * @returns amount of length appended.
 */
memory_pointer_t memory_appendstr(int mem_index, const char *data)
{
	return memory_write(memory_pointer_end(mem_index), data, strlen(data));
}

memory_pointer_t memory_appendstr0(int mem_index, const char *data)
{
	return memory_write(memory_pointer_end(mem_index), data, strlen(data)+1);
}


int memory_get_move(memory_pointer_t *ptr)
{
	assert(ptr->mem_index>=0 && ptr->mem_index<MEMORY_N_INSTANCES);
	assert(!_.instances[ptr->mem_index].free);
	
	if (ptr->index==ptr->mem_unit->filled){
		*ptr = memory_pointer_next(*ptr);
		if (ptr->mem_unit==NULL)
			return -1;
	}
	
	if (ptr->mem_unit->filled==0)
		return -1;
	
	return ptr->mem_unit->data[ptr->mem_index++];
}

memory_pointer_t memory_pointer_start(int mem_index)
{
	assert(mem_index>=0 && mem_index<MEMORY_N_INSTANCES);
	assert(!_.instances[mem_index].free);

	memory_pointer_t pointer;
	pointer.mem_index = mem_index;
	pointer.mem_unit = _.instances[mem_index].start;
	pointer.index = pointer.mem_unit->sindex;
	return pointer;
}

memory_pointer_t memory_pointer_next(memory_pointer_t pointer)
{
	assert((pointer.mem_index>=0) && (pointer.mem_index<MEMORY_N_INSTANCES));
	assert(!_.instances[pointer.mem_index].free);

	assert(pointer.mem_unit!=NULL);
	if (pointer.mem_unit->next==NULL)
	{
		pointer.mem_unit = NULL;
		pointer.index = -1;
		return pointer;
	}
	
	pointer.mem_unit = pointer.mem_unit->next;
	pointer.index = pointer.mem_unit->sindex;
	return pointer;
}

memory_pointer_t memory_pointer_end(int mem_index)
{
	assert(mem_index>=0 && mem_index<MEMORY_N_INSTANCES);
	assert(!_.instances[mem_index].free);

	memory_unit_t *mem = _.instances[mem_index].end;
	assert(mem!=NULL);
	
	memory_pointer_t pointer = {
		.mem_index = mem_index,
		.index = mem->filled,
		.mem_unit = mem
	};
	return pointer;
}

/**
 * Get memory reference to memory instance at index `index`.
 * @param memory_index: Memory instance identifier.
 * @param index: Index of memory instance where the reference is needed.
 * @return memory instance reference.
 */
memory_pointer_t memory_pointer_index(memory_pointer_t pointer, int index)
{
	if (pointer.mem_unit==NULL)
		return pointer;
	if (index==0)
	{
		assert(pointer.index<=pointer.mem_unit->filled);
		if (pointer.mem_unit->filled==pointer.index)
		{
			return memory_pointer_next(pointer);
		}
		return pointer;
	}

	assert(pointer.mem_unit->filled>=pointer.index);
	if (pointer.index==pointer.mem_unit->filled){
		pointer=memory_pointer_next(pointer);
		if (pointer.mem_unit==NULL)
			return pointer;
	}

	int moved = 0;

	while (moved!=index){
		assert(pointer.mem_unit->filled>=pointer.index);
		int space = pointer.mem_unit->filled - pointer.index;
		space = (space>(index-moved))?(index-moved):space;
	
		pointer.index += space;
		moved += space;
		assert(pointer.index<=pointer.mem_unit->filled);
		if (pointer.index==pointer.mem_unit->filled){
			pointer = memory_pointer_next(pointer);
			if (pointer.mem_unit==NULL)
				return pointer;
		}
	}
	return pointer;
}

/**
 * Copies data in memory instance to memory buffer `mem`.
 * @param mem_index: Memory instance identifier.
 * @param index: Index from which the copy should occur.
 * @param len: Length of data to be copied.
 * @param mem: Memory buffer to which data needs to be copied.
 * @returns Amount of data copied.
 */
int memory_copy(memory_pointer_t pointer, char *mem, int len)
{
	int i=0;
	if (pointer.mem_unit==NULL)
		return 0;
	
	pointer = memory_pointer_index(pointer, 0);
	if (pointer.mem_unit==NULL){
		ansi_error("Tried copying from unassigned memory.\n");
		return 0;
	}
	int copied = 0;
	while (copied!=len){
		if (pointer.mem_unit==NULL)
			return copied;
		
		int space = pointer.mem_unit->filled - pointer.index;
		space = (space>=(len-copied))?(len-copied):space;
		memcpy(mem+copied, pointer.mem_unit->data+pointer.index, space);
		
		copied += space;
		pointer = memory_pointer_next(pointer);
	}
	return copied;
}

int memory_copy0(memory_pointer_t pointer, char *mem, int len)
{
	int copied = memory_copy(pointer, mem, len);
	*(mem+copied) = '\0';
	return copied;
}

int memory_strlen(memory_pointer_t pointer)
{
	int i=0;
	if (pointer.mem_unit==NULL){
		return -1;
	}
	pointer = memory_pointer_index(pointer, 0);
	if (pointer.mem_unit==NULL){
		ansi_error("Tried to access unallocated memory.\n");
		return -1;
	}
	
	for(i=0; *(pointer.mem_unit->data+pointer.index)!='\0'; i++)
	{
		pointer.index++;
		if (pointer.index==pointer.mem_unit->filled){
			pointer = memory_pointer_next(pointer);
			if (pointer.mem_unit==NULL)
				return -1;
			if (pointer.index==pointer.mem_unit->filled)
				return -1;
		}
	}
	return i;
}

/**
 * Prints memory stored in instance.
 * @param mem_index: Memory instance identifier.
 */
void memory_printAll(int mem_index)
{
	memory_unit_t *start = _.instances[mem_index].start;
	while (1)
	{
		int i=0;
		for (i=start->sindex; i<start->filled; i++)
		{
			if (isprint(start->data[i]) || start->data[i]=='\n')
				printf("%c", start->data[i]);
			else
				printf("%c", start->data[i]);
		}
		start = start->next;
		if (start==NULL)
			break;
	}
	printf("\n");
	//memory_print(memory_pointer_start(mem_index), -1);
}


/**
 * Prints memory stored in instance of length len starting from `index`.
 * @param mem_index: Memory instance identifier.
 * @param index: Index from which printing should start.
 * @param len: Length of data that should be printed. -1 if upto end should
 * be printed.
 */
void memory_print(memory_pointer_t pointer, int len)
{
	pointer = memory_pointer_index(pointer, 0);
	if (pointer.mem_unit==NULL){
		ansi_error("NO CONTENT.\n");
		return;
	}
	int i;
	for (i=0; (i<len) || (len==-1); i++)
	{
		if (pointer.index>=pointer.mem_unit->filled)
		{
			pointer = memory_pointer_next(pointer);
			if (pointer.mem_unit==NULL)
				break;
		}
		char ch = pointer.mem_unit->data[pointer.index];
		if (isprint(ch))
			printf("%c", ch);
		else
			printf("%c", ch);
		pointer.index++;
	}
	printf("\n");
}


static memory_unit_t* instance_addUnit(memory_instance_t *instance)
{
	memory_unit_t *unit = instance->end;

	assert(unit->next==NULL);
	
	unit->next = unit_getNew();
	if (unit->next==NULL)
		instance->error = TRUE;
	return instance->end = unit->next;
}

static memory_unit_t* unit_getNew(void)
{
	memory_unit_t *freeunit = _.unit_start;
	if (freeunit==NULL){
		ansi_error("Maximum number of memory units reached...");
		return NULL;
	}
	
	freeunit->filled = 0;
	freeunit->sindex = 0;
	
	_.unit_start = freeunit->next;
	
	freeunit->next = NULL;
	return freeunit;
}

static void instance_units_freeAll(memory_instance_t *instance)
{
	assert(instance->end->next==NULL);
	
	instance->end->next = _.unit_start;
	_.unit_start = instance->start;
}