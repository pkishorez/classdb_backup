/**
 * @file common_utility.h
 * @author Kishore
 * @date April 16, 2016, 11:27 AM 
 */

#include "common/headers/init.h"

void print_trace(void);
void* malloc_addspace(void* src, int unit_size, int n_units, int add_units);

void* malloc_delUnit(void* src, int unit_size, int n_units, int del_unit_index);