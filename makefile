.PHONY : default
default:
	@echo  choose a target   libs doc clean  examples


.PHONY : examples
examples:
	make -C examples  run



.PHONY : doc
doc:
	doxygen Doxyfile


.PHONY : show_doc
show_doc:
	firefox doc/html/index.html


.PHONY : github
github:
	git push github master --force


.PHONY : github_doc
github_doc:  #doc
	rm -rf /home/maiquel/inet.prj/web/jleahred.github.io/jle_cpp_tk.doc
	mkdir /home/maiquel/inet.prj/web/jleahred.github.io/jle_cpp_tk.doc
	cp -r doc/html/* /home/maiquel/inet.prj/web/jleahred.github.io/jle_cpp_tk.doc
	cd /home/maiquel/inet.prj/web/jleahred.github.io/; git gui; git push github master


