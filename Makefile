TARGET := raylib
SRC := main.c
FLAGS := -lraylib -lm -Wall -Wextra
PARTICLE_COUNT := 10
run:
	gcc -o ${TARGET} ${SRC} ${FLAGS}
	./raylib ${PARTICLE_COUNT}
