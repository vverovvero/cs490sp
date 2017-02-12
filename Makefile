CC      = clang++
CFLAGS  = -std=c++11 -stdlib=libc++
RM      = rm -f

default: all

all: main

main: main.o
	${CC} ${CFLAGS} -o $@ $^

main.o: main.cpp

valgrind: main
	valgrind -q —-tool=memcheck —leak-check=yes ./main

clean:
	$(RM) main *.o
	