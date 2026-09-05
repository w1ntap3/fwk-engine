TARGET := raylib
SRC := ${wildcard src/*.c}
FLAGS := -lraylib -lm -Wall -Wextra

run:
	gcc -o ${TARGET} ${SRC} ${FLAGS}
	./raylib
