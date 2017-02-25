IDIR 	= /usr/local/include/cairo/
CC      = clang++
CFLAGS  = -std=c++11 -stdlib=libc++
RM      = rm -f

LIBS = -L/usr/local/lib

default: all

all: main

main: main.o SCEparser.cpp vec3.o
	${CC} ${CFLAGS} -I${IDIR} ${LIBS} -o $@  $^ -lcairo   

vec3.o: vec3.cpp

main.o: main.cpp vec3.cpp

SCEparser.o: SCEparser.cpp vec3.cpp

valgrind: main
	valgrind -q —-tool=memcheck —leak-check=yes ./main

clean:
	$(RM) main *.o



	
