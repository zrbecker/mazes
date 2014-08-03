
maze: main.cpp Image.cpp Image.hpp
	clang++ -o maze -std=c++1y -stdlib=libc++ -Wall -Wextra -pedantic main.cpp Image.cpp

.PHONY: clean
clean:
	rm maze

