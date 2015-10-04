.PHONY : default
default:
	@echo  choose a target   libs doc clean  examples


.PHONY : all
all:  space_trailing  libs





.PHONY : examples
examples:
	make -C examples  run



.PHONY : doc
doc: generate_commit_sha1_file  stats
	export JLE_CPP_TK="`cat VERSION`  sh1:`cat COMMIT`"; doxygen Doxyfile
	echo "\"\"" >  COMMIT

.PHONY : doc_show
doc_show:
	firefox doc/html/index.html

.PHONY : test
test:
	make -C test test



.PHONY : github
github:
	git push github master --force


.PHONY : github_doc
github_doc:  #doc
	rm -rf /home/maiquel/inet.prj/web/jleahred.github.io/jle_cpp_tk.doc
	mkdir /home/maiquel/inet.prj/web/jleahred.github.io/jle_cpp_tk.doc
	cp -r doc/html/* /home/maiquel/inet.prj/web/jleahred.github.io/jle_cpp_tk.doc
	cd /home/maiquel/inet.prj/web/jleahred.github.io/; git gui; git push github master




.PHONY : space_trailing
space_trailing:
	for f in `find . -type f -regex '.*\.hpp$$\|.*\.h$$\|.*\.cpp$$\|.*\.impl$$\|.*\.fsm$$\|.*\.msg$$\|.*\makefile$$\|.*\.mak$$\|.*\_make\.release$$\|.*\_make\.debug$$'` ; do  cat $$f | sed 's/[ \t]*$$//' > temp.rms; mv -f temp.rms $$f;  done



.PHONY : generate_commit_sha1_file
generate_commit_sha1_file:
	git log -1 --format="\"%H\"" > COMMIT

.PHONY : delete_commit_sha1_file
delete_commit_sha1_file:
	echo "\"\"" >  COMMIT




.PHONY : prepare
prepare:
	mkdir -p lib
	mkdir -p temp


.PHONY : clean
clean: cleanlibs


.PHONY : cleanlibs
cleanlibs:
	make -C src/core clean
	make -C src/net clean
	find . -name temp | xargs rm -rf




.PHONY : libs
libs: prepare clean   lib_jle_core lib_jle_net


.PHONY : lib_jle_core
lib_jle_core: prepare
	make -C src/core lib


.PHONY : lib_jle_net
lib_jle_net: prepare
	make -C src/net lib



.PHONY : stats
stats:
	@echo "# STATS" > STATS.md
	@echo  >> STATS.md
	@echo  "_ | lines | files" >> STATS.md
	@echo  "------------- | -------------: | -------------:" >> STATS.md
	@echo "/ | "  $$(find . -name '*.h' -o -name '*.h'pp -o -name '*.cpp' | grep -v cov-int | xargs cat | wc -l)  " | " $$(find . -name '*.h' -o -name '*.h'pp -o -name '*.cpp' | grep -v cov-int | wc -l)  >> STATS.md
	@echo "src/ | "  $$(cd src; find . -name '*.h' -o -name '*.h'pp -o -name '*.cpp' | grep -v cov-int | xargs cat | wc -l)  " | " $$(cd src; find . -name '*.h' -o -name '*.h'pp -o -name '*.cpp' | grep -v cov-int | wc -l)  >> STATS.md
	@echo "src/core/ | "  $$(cd src/core; find . -name '*.h' -o -name '*.h'pp -o -name '*.cpp' | grep -v cov-int | xargs cat | wc -l)  " | " $$(cd src/core; find . -name '*.h' -o -name '*.h'pp -o -name '*.cpp' | grep -v cov-int | wc -l)  >> STATS.md
	@echo "src/net/ | "  $$(cd src/net; find . -name '*.h' -o -name '*.h'pp -o -name '*.cpp' | grep -v fossa | xargs cat | wc -l)  " | " $$(cd src/net; find . -name '*.h' -o -name '*.h'pp -o -name '*.cpp' | grep -v fossa | wc -l)  >> STATS.md
	@echo "examples/ | "  $$(cd examples; find . -name '*.h' -o -name '*.h'pp -o -name '*.cpp' | grep -v cov-int | xargs cat | wc -l)  " | " $$(cd examples; find . -name '*.h' -o -name '*.h'pp -o -name '*.cpp' | grep -v cov-int | wc -l)  >> STATS.md
	@echo "test/ | "  $$(cd test; find . -name '*.h' -o -name '*.h'pp -o -name '*.cpp' | grep -v cov-int | xargs cat | wc -l)  " | " $$(cd test; find . -name '*.h' -o -name '*.h'pp -o -name '*.cpp' | grep -v cov-int | wc -l)  >> STATS.md
	@echo "tools/ | "  $$(cd tools; find . -name '*.h' -o -name '*.h'pp -o -name '*.cpp' | grep -v cov-int | xargs cat | wc -l)  " | " $$(cd tools; find . -name '*.h' -o -name '*.h'pp -o -name '*.cpp' | grep -v cov-int | wc -l)  >> STATS.md

	@echo "" >> STATS.md
	@echo "" >> STATS.md
	@echo  "extern | lines | files" >> STATS.md
	@echo  "------------- | -------------: | -------------:" >> STATS.md
	@echo "src/net/ | "  $$(cd src/net; find . -name '*.h' -o -name '*.h'pp -o -name '*.cpp' -o -name '*.c' | grep  fossa | xargs cat | wc -l)  " | " $$(cd src/net; find . -name '*.h' -o -name '*.h'pp -o -name '*.cpp' -o -name '*.c' | grep  fossa | wc -l)  >> STATS.md


coverity_path := /home/maiquel/inet.prj/cov-analysis-linux64-7.7.0/bin
.PHONY : coverity
coverity:
	$(coverity_path)/cov-build --dir cov-int make libs
	tar czvf jle_cpp_tk.tgz cov-int
	curl --form token=Umtbp4HSV3PFODRaQHSKyw \
		--form email=jleahred@gmail.com \
		--form file=@jle_cpp_tk.tgz \
		--form version="0.0" \
		--form description="Developing" \
		https://scan.coverity.com/builds?project=jleahred%2Fjle_cpp_tk
