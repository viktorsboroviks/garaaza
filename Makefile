.PHONY: \
	format \
	format-cpp \
	lint \
	lint-cpp \
	clean \
	distclean

rododendrs:
	git clone git@github.com:viktorsboroviks/rododendrs.git
	cd rododendrs; git checkout v1.4

format: format-cpp

format-cpp: include/garaaza.hpp
	clang-format -i $^

lint: lint-cpp

lint-cpp: include/garaaza.hpp
	cppcheck \
		--enable=warning,portability,performance \
		--enable=style,information \
		--inconclusive \
		--library=std,posix,gnu \
		--platform=unix64 \
		--language=c++ \
		--std=c++20 \
		--inline-suppr \
		--check-level=exhaustive \
		--suppress=checkersReport \
		--checkers-report=cppcheck_report.txt \
		-I./include \
		-I./rododendrs/include \
		$^

clean:
	rm -rfv *.txt

distclean: clean
	rm -rfv rododendrs
