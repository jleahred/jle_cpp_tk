# include directories
INCLUDES = -I../../src


# library paths
#LIBS = -L../../../lib  -ljlesupport  -lrt



# compiler options
include ../../compopt_make  #si es de depuración, las opciones de compilación serán -O0 -g y definirá la macro VALGRIND



SRCS_CXX = $(wildcard *.cpp)
EXECS = $(SRCS_CXX:%.cpp=%.exec)





default: run


.PHONY : prepare
prepare:
	rm *.exec  ||  true

run : prepare  $(EXECS)
	@echo "FIN ----------------------------------------------------------------"


.PHONY : debug
debug:
	echo $(EXECS)
	echo $(SRCS_CXX)



%.exec: %.cpp
	@echo $@ '----------------------------------------------------------------'
	@$(CXX) $(INCLUDES) $(CXXFLAGS) $<  $(LIBS) -o $@
	$(VALGRIND) ./$@
	rm -f ./$@



.PHONY : clean
clean:
	rm -f *.exec


#onefile.exec : onefile.cpp
#	$(CXX) $(INCLUDES) $(CXXFLAGS) $<  $(LIBS) -o $@
