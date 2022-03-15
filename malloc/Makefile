CC := clang
CXX := clang++
CFLAGS := -g -Wall -Werror -fPIC

all: myallocator.so test/malloc-test

clean:
	rm -rf obj myallocator.so test/malloc-test

obj/allocator.o: allocator.c
	mkdir -p obj
	$(CC) $(CFLAGS) -c -o obj/allocator.o allocator.c

myallocator.so: heaplayers/gnuwrapper.cpp heaplayers/wrapper.h obj/allocator.o
	$(CXX) -shared $(CFLAGS) -o myallocator.so heaplayers/gnuwrapper.cpp obj/allocator.o

test/malloc-test: test/malloc-test.c
	clang -fno-omit-frame-pointer -o test/malloc-test test/malloc-test.c -D_GNU_SOURCE
