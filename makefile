VALGRIND_FLAGS=--leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all --trace-children=yes
CFLAGS =-std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g
CC = gcc

all: clean formatear valgrind-ejemplo valgrind-tp1 valgrind-alumno

valgrind-alumno: pruebas_alumno
	valgrind $(VALGRIND_FLAGS) ./pruebas_alumno

pruebas_alumno: src/*.c pruebas_alumno.c
	$(CC) $(CFLAGS) src/*.c pruebas_alumno.c -o pruebas_alumno

valgrind-ejemplo: ejemplo
	valgrind $(VALGRIND_FLAGS) ./ejemplo

valgrind-tp1: tp1
	valgrind $(VALGRIND_FLAGS) ./tp1 "ejemplos/pokedex.csv"

ejemplo: src/*.c ejemplo.c
	$(CC) $(CFLAGS) src/*.c ejemplo.c -o ejemplo

tp1: src/*.c tp1.c
	$(CC) $(CFLAGS) src/*.c tp1.c -o tp1
formatear:
	clang-format --style=file -i ejemplo.c tp1.c pruebas_alumno.c src/pokedex.c src/csv.c 
clean:
	rm -f pruebas_alumno ejemplo tp1
