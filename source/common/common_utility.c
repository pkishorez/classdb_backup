/**
 * @file common_utility.c
 * @author Kishore
 * @date April 16, 2016, 11:27 AM
 * 
 * @brief 
 * 
 * 
 */

#include "headers/common_utility.h"
void print_trace(void)
{
	void *array[100];
	size_t size;
	char **strings;
	size_t i;

	size = backtrace (array, 100);
	strings = backtrace_symbols (array, size);

	printf ("Obtained %zd stack frames.\n", size);

	for (i = 0; i < size; i++)
	   printf ("%s\n", strings[i]);

	free (strings);
}

void* malloc_addspace(void* src, int unit_size, int n_units, int add_units)
{
	if (n_units==0){
		assert(src==NULL);
		return malloc(unit_size*add_units);
	}

	void *space = (void*)malloc(unit_size*(n_units+add_units));
	memcpy(space, src, unit_size*n_units);
	free(src);
	return space;
}


void* malloc_delUnit(void* src, int unit_size, int n_units, int del_unit_index)
{
	assert(del_unit_index>=0);
	assert(del_unit_index<n_units);
	assert(n_units>=1);
	
	char* dest = (char *)malloc(unit_size*(n_units-1));
	memcpy(dest, src, unit_size*del_unit_index);
	memcpy(dest+unit_size*del_unit_index, src+(del_unit_index+1)*unit_size, unit_size*(n_units-del_unit_index-1));
	
	free(src);
	return dest;
}