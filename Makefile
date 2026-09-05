TARGET := raylib
SRC := ${wildcard src/*.c}
FLAGS := -lraylib -lm -Wall -Wextra
PARTICLE_COUNT := 100
run:
	gcc -o ${TARGET} ${SRC} ${FLAGS}
	./raylib ${PARTICLE_COUNT}
