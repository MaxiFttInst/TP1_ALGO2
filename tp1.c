#include "src/csv.h"
#include "src/pokedex.h"
#include <stdio.h>
#include <string.h>

struct cantidad_tipos {
	int agua;
	int fuego;
	int planta;
	int roca;
	int electrico;
	int normal;
	int lucha;
};

bool leer_int(const char *str, void *ctx)
{
	return sscanf(str, "%d", (int *)ctx) == 1;
}

bool crear_string_nuevo(const char *str, void *ctx)
{
	char *nuevo = malloc(strlen(str) + 1);
	if (nuevo == NULL)
		return false;
	strcpy(nuevo, str);
	*(char **)ctx = nuevo;
	return true;
}

bool leer_caracter(const char *str, void *ctx)
{
	*(char *)ctx = *(char *)str;
	return true;
}
bool contar_por_tipo(struct pokemon *poke, void *ctx){
	// printf("Nombre: %s \n", poke->nombre);
	switch (poke->tipo) {
		case 'A':
			(((struct cantidad_tipos*)ctx)->agua)++;
			break;
		case 'F':
			(((struct cantidad_tipos*)ctx)->fuego)++;
			break;
		case 'P':
			(((struct cantidad_tipos*)ctx)->planta)++;
			break;
		case 'R':
			(((struct cantidad_tipos*)ctx)->roca)++;
			break;
		case 'E':
			(((struct cantidad_tipos*)ctx)->electrico)++;
			break;
		case 'N':
			(((struct cantidad_tipos*)ctx)->normal)++;
			break;
		case 'L':
			(((struct cantidad_tipos*)ctx)->lucha)++;
			break;
	
	}
	return true;
}
int main(int argc, char const *argv[])
{
	if (argc < 2){
		printf("Ingrese algún argumento válido");
		return -1;
	}
	struct archivo_csv *archivo =
		abrir_archivo_csv(argv[1], ';');

	bool (*funciones[5])(const char *, void *) = { crear_string_nuevo,
						       leer_caracter,
						       leer_int, leer_int, leer_int };
	char caracter;
	char *nombre = NULL;
	int fuerza;
	int destreza;
	int resistencia;
	void *punteros[5] = { &nombre, &caracter, &fuerza, &destreza, &resistencia};

	struct pokedex *pdex = pokedex_crear();
	struct pokemon a_agregar = {
		.nombre = NULL,
		.tipo = 0,
		.fuerza = 0,
		.destreza = 0,
		.resistencia = 0
	};
	struct cantidad_tipos cant_tipos = {
		.agua = 0,
		.fuego = 0,
		.planta = 0,
		.roca = 0,
		.electrico = 0,
		.normal = 0,
		.lucha = 0
	};
	while (leer_linea_csv(archivo, 5, funciones, punteros) == 5) {
		printf("Nombre: %s, Tipo: %c, Fuerza: %d, Destreza: %d, Resistencia: %d \n",
		 nombre, caracter, fuerza, destreza, resistencia);
		a_agregar.nombre = nombre;
		a_agregar.tipo = caracter;
		a_agregar.fuerza = fuerza;
		a_agregar.destreza = destreza;
		a_agregar.resistencia = resistencia;
		pokedex_agregar_pokemon(pdex, a_agregar);
	}
	pokedex_iterar_pokemones(pdex, contar_por_tipo, &cant_tipos);
	printf("Cantidad de pokemones de cada tipo \n");
	printf("Agua: %d \n", cant_tipos.agua);
	printf("Fuego: %d \n", cant_tipos.fuego);
	printf("Planta: %d \n", cant_tipos.planta);
	printf("Roca: %d \n", cant_tipos.roca);
	printf("Electrico: %d \n", cant_tipos.electrico);
	printf("Normal: %d \n", cant_tipos.normal);
	printf("Lucha: %d \n", cant_tipos.lucha);

	cerrar_archivo_csv(archivo);
	pokedex_destruir(pdex);
	return 0;
}
