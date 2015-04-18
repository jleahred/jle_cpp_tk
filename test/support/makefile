# include directories
INCLUDES = -I../../src


# library paths
LIBS = -L../../lib  -ljlesupport



# compiler options
include ../../compopt_make  #si es de depuración, las opciones de compilación serán -O0 -g y definirá la macro VALGRIND



SRCS_CXX = $(wildcard *.cpp)
EXECS = $(SRCS_CXX:%.cpp=%.exec)





default: test


.PHONY : prepare
prepare:
	@rm *.exec  ||  true  2> /dev/null

test : prepare  $(EXECS)
	@echo
	@echo
	@echo #"TEST ENDED <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"
	@echo
	@echo


.PHONY : debug
debug:
	echo $(EXECS)
	echo $(SRCS_CXX)



%.exec: %.cpp
	@echo
	@echo
	@echo
	@echo #$@
	@echo '----------------------------------------------------------------------------'
	@$(CXX) $(INCLUDES) $(CXXFLAGS) $<  $(LIBS) -o $@
	$(VALGRIND) ./$@
	@rm -f ./$@



.PHONY : clean
clean:
	rm -f *.exec


#onefile.exec : onefile.cpp
#	$(CXX) $(INCLUDES) $(CXXFLAGS) $<  $(LIBS) -o $@
