CXX = c++
CC = c++
CPPFLAGS = -std=c++11 -Wno-implicit-exception-spec-mismatch

example: example.o tmalloc.o

clean:
	rm *.o example

example.o: example.cpp

tmalloc.o: tmalloc.cpp datastruct.h
