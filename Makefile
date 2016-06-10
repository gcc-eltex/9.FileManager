# Исходники
SRC=$(wildcard src/*.c)
#Обьектные файлы
OBJ=$(patsubst src/%.c,%.o,$(SRC))

all: libs
	gcc file_manager.c -o file_manager -L./libs/ -lfmanager -lncursesw -lm
	rm -rf *.o
libs: $(OBJ)
	ar -cr ./libs/libfmanager.a $(OBJ)
$(OBJ):
	gcc $(SRC) -c
clean:
	rm -rf *.o file_manager ./libs/libfmanager.a	
run:
	./file_manager
