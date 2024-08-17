.PHONY: \
	format \
	clang-format \
	lint \
	cppcheck-lint \
	clean \
	distclean

rododendrs:
	git clone git@github.com:viktorsboroviks/rododendrs.git
	cd rododendrs; git checkout v1.4

format: clang-format

clang-format: include/garaza.hpp
	clang-format -i $^

lint: cppcheck-lint

cppcheck-lint: include/garaza.hpp
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
