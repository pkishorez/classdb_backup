/**
 * @file headers.h
 * @author Kishore
 * @date August 2, 2016, 11:12 AM 
 */

#ifndef UNIQUE_DATAMODEL_DEFINITIONS_IDENTIFIER
	#define UNIQUE_DATAMODEL_DEFINITIONS_IDENTIFIER

	#define DM_NAME_SIZE 100
	typedef enum{
		DM_INVALID,
		DM_INT32,
		DM_INT64,
		DM_STRING,
		DM_BOOLEAN
	}dm_datom_type_t;

	typedef struct{
		char name[DM_NAME_SIZE+1];
		dm_datom_type_t type;
		int offset;
		int _size;
	}datom_t;
	
	#define DM_UNIT_ALIGN 16
	typedef struct{
		char name[DM_NAME_SIZE+1];
		int size;
		int offset;
		int n_datoms;
		datom_t *datoms;
	}dm_unit_t;

	/**
	 * PROPERTY STRUCTS GOES HERE.
	 */
	
	#define DM_PROPERTY_ALIGN 32
	typedef enum{
		DM_PROPERTY_INVALID,
		DM_PROPERTY_FS,
		DM_PROPERTY_FM
	}dm_property_type_t;
	
	typedef struct{
		char name[DM_NAME_SIZE];
		
		dm_property_type_t type;
		void* property;
	}dm_property_t;
	
	typedef struct{
		int size;

		int n_units;
		dm_unit_t *units;
	}dm_property_fs_t;
	
	typedef struct{
		int size;

		dm_unit_t *unit;
	}dm_property_fm_t;

	
	/**
	 * COLLECTION STRUCTS GOES HERE.
	 */
	#define DM_COLLECTION_ALIGN 64
	typedef enum{
		DM_COLLECTION_INVALID,
		DM_COLLECTION_FIXED,
		DM_COLLECTION_VARIABLE,
		DM_COLLECTION_FLEXIBLE
	}dm_collection_type_t;

	
	typedef struct{
		char name[DM_NAME_SIZE];
		dm_collection_type_t type;
		void* collection;
	}dm_collection_t;
	
	typedef struct{
		int size;
		int n_properties;
		dm_property_t *properties;
	}dm_collection_fixed_t;
	/**
	 * INDEPENDENT STRUCTS USED INTERNALLY.
	 */
	typedef struct{
		int offset;
		int freesize;
	}dm_block_t;
	
	
#endif