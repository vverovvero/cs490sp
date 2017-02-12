IDIR 	= /usr/local/include/cairo/
CC      = clang++
CFLAGS  = -std=c++11 -stdlib=libc++
DEPS 	= main.h
RM      = rm -f

LIBS = -L/usr/local/lib

default: all

all: main

main: main.o
	${CC} ${CFLAGS} -I${IDIR} ${LIBS} -o $@  $^ -lcairo   

main.o: main.cpp ${DEPS} 

valgrind: main
	valgrind -q —-tool=memcheck —leak-check=yes ./main

clean:
	$(RM) main *.o



	
