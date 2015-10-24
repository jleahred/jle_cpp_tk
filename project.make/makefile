# VERSION 1.0   2015-10-24



include project.mak


include jle/compopt_make.debug
incremental: CXXFLAGS=$(CXXFLAGS_DEBUG)
incremental: CXXFLAGS=$(CXXFLAGS_DEBUG)
incremental: CFLAGS=$(CFLAGS_DEBUG)
incremental: LDFLAGS=$(LDFLAGS_DEBUG)
incremental: CXX=$(CXX_DEBUG)
incremental: CC=$(CC_DEBUG)

include jle/compopt_make.release
release: CXXFLAGS=$(CXXFLAGS_RELEASE)
release: CXXFLAGS=$(CXXFLAGS_RELEASE)
release: CFLAGS=$(CFLAGS_RELEASE)
release: LDFLAGS=$(LDFLAGS_RELEASE)
release: CXX=$(CXX_RELEASE)
release: CC=$(CC_RELEASE)


custom: CXXFLAGS=$(CXXFLAGS_CUSTOM)
custom: CXXFLAGS=$(CXXFLAGS_CUSTOM)
custom: CFLAGS=$(CFLAGS_CUSTOM)
custom: LDFLAGS=$(LDFLAGS_CUSTOM)
custom: CXX=$(CXX_CUSTOM)
custom: CC=$(CC_CUSTOM)








## ---------------------------------------------------------


# getting the source files
SRCS_CXX = $(foreach dir, $(DIR_SRC_CXX), $(wildcard $(dir)/*.cpp))


# source to objects
OBJS_CXX = $(foreach file,$(SRCS_CXX:%.cpp=%.o), temp/$(notdir $(file)))



# preparing VPATH
VPATH =  $(foreach dir, $(DIR_SRC_CXX), $(dir))



DELETE_SPACES = for f in `find . -type f -regex '.*\.h$$\|.*\.cpp$$\|.*\.impl$$\|.*\.fsm$$\|.*\.type$$\|.*\makefile$$\|.*\.mak$$'` ; do cat $$f | sed 's/[ \t]*$$//' > temp.rms; mv -f temp.rms $$f;  done




## ---------------------------------------------------------


.PHONY : default
default:
	@echo
	@echo
	@echo
	@echo "make release"
	@echo "--------------"
	@echo "it will compile the program on release version"
	@echo
	@echo
	@echo
	@echo
	@echo "make clean"
	@echo "--------------"
	@echo "it will delete de temp files (objects .o)"
	@echo
	@echo
	@echo
	@echo
	@echo "make idl"
	@echo "--------------"
	@echo "it will find .type files and will generate .cpp and .h"
	@echo
	@echo
	@echo
	@echo
	@echo





release:  clean  prepare $(OUT)

incremental: $(OUT)


custom: $(OUT)








.PHONY : prepare
prepare:  delete_spaces
	mkdir -p bin
	cp $(OUT) $(OUT).old  ||  true
	mkdir -p temp


.PHONY : clean
clean:
	rm -f $(OBJS_CXX)



.PHONY : idl
idl:
	find . -name '*.type' | xargs ./jle/tools/bin/idl/hpt ./jle/tools/bin/idl/type.gram  ./jle/tools/bin/idl/type.gram.templ




.PHONY : delete_spaces
delete_spaces:
	$(DELETE_SPACES)



temp/%.o: %.cpp
	$(CXX) -c $(INCLUDES) $(CXXFLAGS) -o $@ $<


$(OUT): $(OBJS_CXX)
	$(CXX)  $(OBJS_CXX) $(LIBS)  -o $(OUT)

