# Исходники
SRC=$(wildcard src/*.c)
#Обьектные файлы
OBJ=$(patsubst src/%.c,%.o,$(SRC))

all: libs
	gcc main.c -o main -L./libs/ -lfmanager -lncursesw -lm
	rm -rf *.o
libs: $(OBJ)
	ar -cr ./libs/libfmanager.a $(OBJ)
$(OBJ):
	gcc $(SRC) -c
clear:
	rm -rf *.o
run:
	./main