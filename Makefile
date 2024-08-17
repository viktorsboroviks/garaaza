.PHONY: \
	format \
	clang-format \
	lint \
	lint-cpp \
	clean \
	distclean

rododendrs:
	git clone git@github.com:viktorsboroviks/rododendrs.git
	cd rododendrs; git checkout v1.4

format: clang-format

clang-format: include/garaza.hpp
	clang-format -i $^

lint: lint-cpp

lint-cpp: include/garaza.hpp
	cppcheck \
		--enable=warning,portability,performance \
		--enable=style,information \
		--enable=missingInclude \
		--inconclusive \
		--library=std,posix,gnu \
		--platform=unix64 \
		--language=c++ \
		--std=c++20 \
		--inline-suppr \
		--check-level=exhaustive \
		--suppress=missingIncludeSystem \
		--suppress=checkersReport \
		--checkers-report=cppcheck_report.txt \
		-I./include \
		-I./rododendrs/include \
		$^

clean:
	rm -rfv *.txt

distclean: clean
	rm -rfv rododendrs
