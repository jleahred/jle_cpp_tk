.PHONY : default
default:
	@echo  choose a target   libs doc clean


.PHONY : doc
doc:
	cd data/doxygen/; doxygen Doxyfile


