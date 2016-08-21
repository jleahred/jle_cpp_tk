#
# Output...
OUT = bin/idl_test
#
#
# Directories with source code
DIR_SRC_CXX = src
#
#
# library paths
LIBS = -L./jle/lib -ljlecore  -lrt
#
#
# include directories
INCLUDES = -I./src -I./jle/src



.PHONY : idl.json
idl.json:
	find . -name '*.type' | xargs ./jle/tools/bin/idl/hpt  ./src/type.gram  ./src/type.gram.templ
