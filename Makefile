.PHONY: format clang-format

format: clang-format

clang-format: include/garaza.hpp
	clang-format -i $^
