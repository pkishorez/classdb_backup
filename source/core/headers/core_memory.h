/**
 * @file core_memory.h
 * @author Kishore
 * @date March 26, 2016, 9:56 PM 
 */

#include "common/headers/init.h"

void memory_init();

int memory_create();
void memory_delete(int instance_index);
void memory_free(int instance_index);

memory_pointer_t memory_write(memory_pointer_t pointer, const char *data, int len);
memory_pointer_t memory_writestr(memory_pointer_t pointer, const char *data);
int memory_pointer_get(memory_pointer_t ptr, int index);

memory_pointer_t memory_appendmem(int mem_index, const char *data, int len);
memory_pointer_t memory_appendstr(int mem_index, const char *data);
memory_pointer_t memory_appendstr0(int mem_index, const char *data);

int memory_copy(memory_pointer_t pointer, char *mem, int len);
int memory_copy0(memory_pointer_t pointer, char *mem, int len);
int memory_copystr(memory_pointer_t pointer, char *mem);

int memory_strlen(memory_pointer_t pointer);

memory_pointer_t memory_pointer_index(memory_pointer_t pointer, int index);

memory_pointer_t memory_pointer_start(int mem_index);
memory_pointer_t memory_pointer_next(memory_pointer_t pointer);
memory_pointer_t memory_pointer_end(int mem_index);

void memory_printAll(int mem_index);
void memory_print(memory_pointer_t pointer, int len);