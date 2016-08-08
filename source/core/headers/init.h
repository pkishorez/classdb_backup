/**
 * @file core_init.h
 * @author Kishore
 * @date March 20, 2016, 12:59 PM 
 */

#ifndef UNIQUE_CORE_DEFINITIONS_IDENTIFIER
	#define UNIQUE_CORE_DEFINITIONS_IDENTIFIER

	/**
	 * ==================================
	 * ANSI RELATED DEFINITIONS GOES HERE
	 * ==================================
	 */
	enum colors {
		C_BLACK,
		C_RED,
		C_GREEN,
		C_YELLOW,
		C_BLUE,
		C_MAGENTA,
		C_CYAN,
		C_WHITE,
		C_DEFAULT = 9
	};

	enum params {
		A_BOLD = 1u << 0
	};

	
	#define ansi_bold_text(...)\
		{\
			ansi_bold();\
			printf(__VA_ARGS__);\
			ansi_default();\
		}

	#define ansi_uline_text(...)\
		{\
			ansi_uline();\
			printf(__VA_ARGS__);\
			ansi_default();\
		}

	#define ansi_uline_bold_text(...)\
		{\
			ansi_bold();\
			ansi_uline();\
			printf(__VA_ARGS__);\
			ansi_default();\
		}

	#define ansi_fg_bg_text(fg, bg, ...)\
		{\
			ansi_fg_bg(fg,bg);\
			printf( __VA_ARGS__ );\
			ansi_default();\
			fflush(stdout);\
		}


	#define ansi_fg_bg_textln(a, b, ...)\
		{\
			ansi_fg_bg(a,b);\
			printf( __VA_ARGS__ );\
			ansi_default();\
			printf("\n");\
			fflush(stdout);\
		}

	#define ansi_success(...)\
		{\
			ansi_bold();\
			ansi_fg_bg_text(C_GREEN, C_DEFAULT, "\n%s:%d: ", __FILE__, __LINE__);\
			ansi_fg_bg_text(C_GREEN, C_DEFAULT, "Success : " __VA_ARGS__);\
			fflush(stdout);\
		}

	#define ansi_error(...)\
		{\
			ansi_fg_bg_text(C_RED, C_DEFAULT, "%s:%d: ", __FILE__, __LINE__);\
			ansi_fg_bg_text(C_RED, C_DEFAULT, __VA_ARGS__);\
			fflush(stdout);\
		}

	#define ansi_error_abort(...)\
		{\
			ansi_fg_bg_text(C_RED, C_DEFAULT, "%s:%d: ", __FILE__, __LINE__);\
			ansi_fg_bg_text(C_RED, C_DEFAULT, __VA_ARGS__);\
			fflush(stdout);\
			print_trace();\
		}

	#define ansi_sinfo(...)\
		{\
			ansi_clear_line();\
			ansi_fg_bg_text(C_CYAN, C_DEFAULT, __VA_ARGS__);\
		}

	#define ansi_info(...)\
		{\
			ansi_clear_line();\
			ansi_fg_bg_text(C_GREEN, C_DEFAULT, __VA_ARGS__);\
		}
	
	/**
	 * ======================================
	 * SOCKET RELATED Definitions GOES HERE
	 * ======================================
	 */
	
	
	
	/**
	 * =========================
	 * REQUEST CONTROLLER MODULE 
	 * =========================
	 */
	
	#define REQUEST_MAXREQUESTS 100000

	/**
	 * ========================
	 *      MEMORY MODULE
	 * ========================
	 */
	#define MEMORY_UNIT_MAXSIZE 1
	#define MEMORY_N_UNITS REQUEST_MAXREQUESTS*10
	#define MEMORY_N_INSTANCES REQUEST_MAXREQUESTS

	typedef struct memory_struct{
		int sindex;
		int filled;
		int free;
		char data[MEMORY_UNIT_MAXSIZE];
		struct memory_struct *next;
	}memory_unit_t;
	
	
	typedef struct{
		memory_unit_t *start;
		memory_unit_t *end;
		int free;
		int t_len;
		int next;
		int error;
	}memory_instance_t;
	
	
	typedef struct{
		int index;
		int mem_index;
		memory_unit_t *mem_unit;
	}memory_pointer_t;
	
	
	/**
	 * ======================
	 *      REGEX MODULE
	 * ======================
	 */
	
	enum regex_error{
		REGEX_PARSE_ERROR = 0,
		REGEX_PARSE_SUCCESS = 1
	};
	
	typedef struct{
		int str_parsed;
		int regex_parsed;
		enum regex_error status;
		int n_caps;
		memory_pointer_t cont;
	}regex_status_t;
	
	typedef struct{
		int index;
		int len;
	}regex_cap_t;
	
	
	
	/**
	 * ======================
	 *      BSON MODULE
	 * ======================
	 */
	#define BSON_MAX_NESTED 10
	#define BSON_MAX_PROPERTY_NAME_SIZE 100
	enum bson_types{
		BSON_TYPE_UNKNOWN = '\x00',
		BSON_TYPE_DOCUMENT = '\x03',
		BSON_TYPE_FLOAT = '\x01',
		BSON_TYPE_STRING = '\x02',
		BSON_TYPE_INT32 = '\x10',
		BSON_TYPE_INT64 = '\x12'
	};
	typedef struct{
		int mem_index;
		memory_pointer_t _docpointers[BSON_MAX_NESTED];
		int _docsizes[BSON_MAX_NESTED];
		int _docno;
		int error;
	}bson_t;
	
	typedef struct{
		memory_pointer_t pointer;
		memory_pointer_t name;
		int size;
	}bson_document_t;
	
	typedef struct{
		enum bson_types type;
		memory_pointer_t name;
		memory_pointer_t value;
	}bson_property_t;
	
	/**
	 * ======================
	 *      JSON MODULE
	 * ======================
	 */
	
	typedef enum json_datatypes{
		JSON_DATATYPE_INVALID,
		JSON_DATATYPE_NUMBER,
		JSON_DATATYPE_STRING,
		JSON_DATATYPE_JSON,
		JSON_DATATYPE_ARRAY,
		JSON_DATATYPE_BOOLEAN,
		JSON_DATATYPE_NULL
	}json_datatype_t;
	typedef struct{
		memory_pointer_t ptr;
		int multiple_propeties;
		int nest_count;
		int beautify;
	}json_create_t;

	typedef struct{
		memory_pointer_t pointer;
		json_datatype_t type;
		int len;
	}json_t;
#endif