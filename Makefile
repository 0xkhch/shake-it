CC = gcc 
flags = -Wall -Wextra -Werror -ggdb
output = build/main
inc = raylib/include
lib = raylib/lib 

all: main.c build
	${CC} main.c ${flags} -I${inc} -L${lib} -l:libraylib.a -lm -o ${output}

build:
	mkdir build/

help:
	@echo "all: compiles the whole project"
	@echo "build: makes the build directory"
	@echo "clean: removes the build directory"
	@echo "help: prints this menu"

clean:
	rm -rf build/
