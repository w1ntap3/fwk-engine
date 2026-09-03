TARGET := raylib
SRC := main.c
FLAGS := -lraylib -lm -Wall -Wextra
PARTICLE_COUNT := 1000
run:
	gcc -o ${TARGET} ${SRC} ${FLAGS}
	./raylib ${PARTICLE_COUNT}
