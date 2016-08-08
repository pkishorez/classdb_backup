/**
 * @file lib_mmap.c
 * @author Kishore
 * @date October 4, 2015, 2:59 PM
 * 
 * @brief 
 * 
 * 
 */

#include "headers/core_ansi.h"
#include "headers/core_file.h"
#include "headers/core_memory.h"
/**
 * Maps the file to memory and returns the pointer to the memory.
 * @param file : valid path of file.
 * @returns pointer to the data.
 * 
 * No need of error checking.
 */
void* file_map(char *file)
{
	int fd = open(file, O_RDWR,  S_IRUSR | S_IWUSR);
	assert(fd!=-1);

	struct stat sb;
	assert(fstat(fd, &sb)!=-1);

	void *addr = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	assert(addr!=MAP_FAILED);

	return addr;
}


/**
 * Creates file specified in `file` with `prealloc_memory` size pre allocated.
 * @param file : valid path where the file should be created.
 * @param prealloc_memory : The size of file that should be preallocated.
 * @returns file descriptor of the created file.
 * 
 * This function throws error if file already exist, or do not have specified
 * permissions to create file there.
 */
int file_create(char *file, int prealloc_memory)
{
	int fd;
	
	fd = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	assert(fd!=-1);

	int i=prealloc_memory;
	char prealloc[1024];
	for (i=0; i<1024; i++)
		prealloc[i] = 0;
	
	i=prealloc_memory;
	
	while (i!=0){
		int w = (i>1024)?1024:i;
		int written = write(fd, prealloc, w);
		assert(written==(w));
		i -= written;
	}
	return fd;
}

/**
 * Returns the file size of the file specified in `file`.
 * @param file : Valid file path.
 */
int file_size(char *file)
{
	int fd = file_open(file);
	struct stat sb;
	assert(fstat(fd, &sb)!=-1);
	
	return sb.st_size;
}

/**
 * Checks if file specified in `file` exists or not.
 * @param file : valid file path.
 * @returns true if exists and false if do not exist.
 */
int file_exist(char *file)
{
	int fd = open(file, O_RDWR);
	close(fd);
	if (fd==-1)
	{
		return 0;
	}
	return 1;
}


/**
 * Opens 
 * @param file
 * @return 
 */
int file_open(char *file)
{
	int fd = open(file, O_RDWR | O_CREAT);
	if (fd!=-1)
		return fd;
	file_error(file);
	return -1;
}


/**
 * Prints the error occurred during opening the file.
 * @param file : Just for referencing.
 */
void file_error(char *file)
{
	if (errno==EACCES)
		ansi_error("File %s has no permission to open in read or write mode.\n", file);
	if (errno==ENOENT)
		ansi_error("File %s do not exist.\n", file);
}


void file_write_memory(int fd, int mem_index)
{
	if (fd==-1){
		ansi_error("Couldn't write memory to file.\n");
		exit(1);
	}
	
	memory_pointer_t pointer = memory_pointer_start(mem_index);
	for (; pointer.mem_unit!=NULL; pointer = memory_pointer_next(pointer))
	{
		int written = 0;
		
		while (written!=pointer.mem_unit->filled)
		{
			int w = write(fd, pointer.mem_unit->data+written, (pointer.mem_unit->filled)-written);
			if (w==-1 || w==0){
				ansi_error("Error while trying to write.\n");
				exit(1);
			}
			written += w;
		}
	}
}

int file_read_to_memory(char *file)
{
	int fd = file_open(file);
	assert(fd!=-1);
	
	int mem_index = memory_create();
	assert(mem_index!=-1);
	
	while (1)
	{
		char buffer[512];
		int r = read(fd, buffer, 512);
		if (r==-1 || r==0)
			return mem_index;
		memory_appendmem(mem_index, buffer, r);
	}
	return mem_index;
}