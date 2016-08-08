C_PARAMS = -rdynamic

###############################################################################
##                              UTILITY FUNCTIONS                            ##
###############################################################################

define temp_object
$(foreach fil, $(notdir $(1)), $(addprefix bin/temp/, $(fil:.c=.o)))
endef

define compile
$(strip $(call temp_object, $(1))): $(1) $(HEADERS)
	gcc $(C_PARAMS) -c $(1) -o $(call temp_object, $(1)) $(INCLUDE)
endef



#######################################################################@#######
##                                VARIABLES                                  ##
###########################################################################@###

HEADERS = $(wildcard source/common/headers/*.h) $(wildcard source/core/headers/*.h) $(wildcard source/datamodel/headers/*.h) $(wildcard source/libraries/headers/*.h)
INC=./source/
INCLUDE = $(foreach file, $(INC), $(addprefix -I,$(file)))

SLICES := source/core/core_*.c source/libraries/lib_*.c source/common/common_*.c source/datamodel/configuration/property/dm_*.c source/datamodel/configuration/collection/dm_*.c source/datamodel/configuration/dm_*.c
SLICE_FILES := $(foreach slice, $(SLICES), $(wildcard $(slice)))
SLICE_FILES_TEMP := $(foreach file, $(SLICE_FILES), $(call temp_object, $(file)))






###############################################################################
##                            MAIN DEFINITIONS                               ##
###############################################################################

bin/main: main.c bin/ClassDB.a Makefile
	gcc $(C_PARAMS) main.c bin/ClassDB.a -o $@ $(INCLUDE)
	@echo Done Compiling.

bin/classdb: classdb.c bin/ClassDB.a Makefile
	gcc $(C_PARAMS) classdb.c bin/ClassDB.a -o $@ $(INCLUDE)
	@echo Done Compiling.

bin/ClassDB.a: $(SLICE_FILES_TEMP)
	ar r $@ $^

$(foreach file, $(SLICE_FILES), $(eval $(call compile, $(file))))


.PHONY: clean run class
	
class: bin/classdb
	
run: bin/main
	clear
	@bin/main

clean:
	-rm bin/temp/*.o
	-rm bin/main
	-rm bin/classdb
	-rm bin/ClassDB.a
