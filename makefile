.PHONY : default
default:
	@echo  choose a target   libs doc clean


.PHONY : doc
doc:
	doxygen Doxyfile


.PHONY : github
github:
	git push github master --force


