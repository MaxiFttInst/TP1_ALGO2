#include "pa2m.h"
#include "src/csv.h"
#include "src/pokedex.h"
#include <stdio.h>
#include <string.h>

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

char *asignar_nombre(char *nombre)
{
	char *ptr = calloc(1, sizeof(char) * (strlen(nombre) + 1));
	strcpy(ptr, nombre);
	return ptr;
}

bool pokemones_son_iguales(struct pokemon poke1, struct pokemon poke2)
{
	return strcmp(poke1.nombre, poke2.nombre) == 0 &&
	       poke1.tipo == poke2.tipo &&
	       poke1.resistencia == poke2.resistencia &&
	       poke1.destreza == poke2.destreza && poke1.fuerza == poke2.fuerza;
}
struct ctx_evaluar_poke {
	size_t actual;
	size_t cantidad_bien;
	char nombres[7][20];
};
bool evaluar_poke(struct pokemon *poke, void *ctx)
{
	struct ctx_evaluar_poke *cep = ctx;
	bool iguales = (strcmp(poke->nombre, cep->nombres[cep->actual]) == 0);
	printf("Nombre: %s \n", poke->nombre);
	if (iguales)
		(cep->cantidad_bien)++;
	(cep->actual)++;
	return true;
}

void abrirUnArchivoInexistenteDebeRetornarNull()
{
	struct archivo_csv *archivo =
		abrir_archivo_csv("noexiste/noexiste/noexiste/no", ';');
	pa2m_afirmar(archivo == NULL, "El archivo no existe y se retorna NULL");
}

void seAbreUnArchivo_seLeenLasLineas()
{
	struct archivo_csv *archivo =
		abrir_archivo_csv("ejemplos/pokedex.csv", ';');
	pa2m_afirmar(archivo != NULL, "Archivo abierto exitosamente");

	bool (*funciones[5])(const char *, void *) = { crear_string_nuevo,
						       leer_caracter, leer_int,
						       leer_int, leer_int };
	struct pokemon poke = { .nombre = "Charmander",
				.tipo = TIPO_ELECTRICO,
				.fuerza = 45,
				.destreza = 10,
				.resistencia = 20 };
	void *contexto[5] = { &(poke.nombre), &(poke.tipo), &(poke.fuerza),
			      &(poke.destreza), &(poke.resistencia) };

	size_t lineas = leer_linea_csv(archivo, 5, funciones, contexto);
	pa2m_afirmar(lineas == 5, "Se leyo 1 linea con 5 columnas");
	bool linea_leida_bien = false;
	if (strcmp(poke.nombre, "Pikachu") == 0 &&
	    poke.tipo == TIPO_ELECTRICO && poke.fuerza == 20 &&
	    poke.destreza == 15 && poke.resistencia == 17)
		linea_leida_bien = true;
	pa2m_afirmar(linea_leida_bien,
		     "La linea se leyó y se asignaron los valores");
	free(poke.nombre);
	cerrar_archivo_csv(archivo);
}

void dosColumnasCuandoLaTercerFuncionEsNula()
{
	struct archivo_csv *archivo =
		abrir_archivo_csv("ejemplos/correcto.txt", ';');
	pa2m_afirmar(archivo != NULL, "Archivo abierto exitosamente");

	bool (*funciones[3])(const char *, void *) = { crear_string_nuevo,
						       leer_caracter, NULL };
	struct pokemon poke = { .nombre = NULL,
				.tipo = '\0',
				.fuerza = 0,
				.destreza = 0,
				.resistencia = 0 };
	void *contexto[3] = { &(poke.nombre), &(poke.tipo), &(poke.fuerza) };

	size_t columnas = leer_linea_csv(archivo, 3, funciones, contexto);
	bool linea_leida_bien = false;
	if (strcmp(poke.nombre, "Pikachu") == 0 &&
	    poke.tipo == TIPO_ELECTRICO && poke.fuerza == 0)
		linea_leida_bien = true;
	pa2m_afirmar(funciones[2] == NULL, "La tercera función es NULL");
	pa2m_afirmar(poke.fuerza == 0,
		     "el valor original de fuerza no fue cambiado");
	pa2m_afirmar(columnas == 2, "Se leyó 1 linea con 2 columnas");
	pa2m_afirmar(linea_leida_bien,
		     "La linea se leyó y se asignaron los valores");
	free(poke.nombre);
	cerrar_archivo_csv(archivo);
}
void ceroLineasCuandoEsElFinal()
{
	struct archivo_csv *archivo =
		abrir_archivo_csv("ejemplos/correcto.txt", ';');
	struct pokemon poke = { .nombre = NULL,
				.tipo = '\0',
				.fuerza = 0,
				.destreza = 0,
				.resistencia = 0 };
	bool (*funciones[3])(const char *, void *) = { crear_string_nuevo,
						       leer_caracter,
						       leer_int };
	void *contexto[3] = { &(poke.nombre), &(poke.tipo), &(poke.fuerza) };
	while (leer_linea_csv(archivo, 3, funciones, contexto) == 3)
		free(poke.nombre);
	size_t columnas = 0;
	columnas = leer_linea_csv(archivo, 3, funciones, contexto);
	pa2m_afirmar(columnas == 0,
		     "Se intentó leer el final y el resultado es cero");
	cerrar_archivo_csv(archivo);
}
void seCreaLaPokedex()
{
	printf("PRUEBA CREACIÓN POKEDEX \n");
	struct pokedex *pokedex = pokedex_crear();
	pa2m_afirmar(pokedex != NULL, "La pokedex se ha creado");
	pokedex_destruir(pokedex);
}

void seAgregaUnPokemon()
{
	printf("PRUEBA AGREGAR UN POKEMON \n");
	struct pokedex *pokedex = pokedex_crear();
	struct pokemon poke = { .nombre = asignar_nombre("Charmander"),
				.tipo = TIPO_ELECTRICO,
				.fuerza = 45,
				.destreza = 10,
				.resistencia = 20 };
	bool agregado = pokedex_agregar_pokemon(pokedex, poke);
	pa2m_afirmar(agregado == true, "Pokemon agregado a la pokedex");
	free(poke.nombre);
	pokedex_destruir(pokedex);
}

void seCuentanLosPokemones()
{
	printf("PRUEBA CONTEO POKEMONES \n");
	struct pokedex *pokedex = pokedex_crear();
	struct pokemon poke1 = { .nombre = asignar_nombre("Charmander"),
				 .tipo = TIPO_ELECTRICO,
				 .fuerza = 45,
				 .destreza = 10,
				 .resistencia = 20 };
	struct pokemon poke2 = { .nombre = asignar_nombre("Algo"),
				 .tipo = TIPO_ROCA,
				 .fuerza = 45,
				 .destreza = 10,
				 .resistencia = 20 };
	struct pokemon poke3 = { .nombre = "Igna",
				 .tipo = TIPO_FUEGO,
				 .fuerza = 45,
				 .destreza = 10,
				 .resistencia = 20 };
	pokedex_agregar_pokemon(pokedex, poke1);
	pokedex_agregar_pokemon(pokedex, poke2);
	pokedex_agregar_pokemon(pokedex, poke3);
	pa2m_afirmar(pokedex_cantidad_pokemones(pokedex) == 3,
		     "Hay 3 pokemones");
	free(poke1.nombre);
	free(poke2.nombre);
	pokedex_destruir(pokedex);
}
void seEncuentranTodosLosPokemones()
{
	printf("PRUEBA SE ENCUENTRAN TODOS LOS POKEMONES \n");
	struct pokedex *pokedex = pokedex_crear();
	struct pokemon poke1 = { .nombre = asignar_nombre("Charmander"),
				 .tipo = TIPO_ELECTRICO,
				 .fuerza = 45,
				 .destreza = 10,
				 .resistencia = 20 };
	struct pokemon poke2 = { .nombre = asignar_nombre("Algo"),
				 .tipo = TIPO_ROCA,
				 .fuerza = 45,
				 .destreza = 10,
				 .resistencia = 20 };
	struct pokemon poke3 = { .nombre = asignar_nombre("Igna"),
				 .tipo = TIPO_FUEGO,
				 .fuerza = 45,
				 .destreza = 10,
				 .resistencia = 20 };
	struct pokemon poke4 = { .nombre = asignar_nombre("Zapatao"),
				 .tipo = TIPO_FUEGO,
				 .fuerza = 45,
				 .destreza = 10,
				 .resistencia = 20 };
	struct pokemon poke5 = { .nombre = asignar_nombre("Ragnarok"),
				 .tipo = TIPO_FUEGO,
				 .fuerza = 45,
				 .destreza = 10,
				 .resistencia = 20 };
	struct pokemon poke6 = { .nombre = asignar_nombre("Loco"),
				 .tipo = TIPO_FUEGO,
				 .fuerza = 45,
				 .destreza = 10,
				 .resistencia = 20 };
	struct pokemon poke7 = { .nombre = asignar_nombre("Serpentine"),
				 .tipo = TIPO_FUEGO,
				 .fuerza = 45,
				 .destreza = 10,
				 .resistencia = 20 };
	pokedex_agregar_pokemon(pokedex, poke1);
	pokedex_agregar_pokemon(pokedex, poke3);
	pokedex_agregar_pokemon(pokedex, poke2);
	pokedex_agregar_pokemon(pokedex, poke4);
	pokedex_agregar_pokemon(pokedex, poke5);
	pokedex_agregar_pokemon(pokedex, poke6);
	pokedex_agregar_pokemon(pokedex, poke7);
	struct pokemon pokemones[7] = { poke1, poke2, poke3, poke4,
					poke5, poke6, poke7 };
	char nombres[7][20] = { "Charmander", "Algo", "Igna",	   "Zapatao",
				"Ragnarok",   "Loco", "Serpentine" };

	const struct pokemon *poke_encontrado;

	for (int i = 0; i < 7; i++) {
		poke_encontrado = pokedex_buscar_pokemon(pokedex, nombres[i]);
		pa2m_afirmar(poke_encontrado != NULL, "El pokemon no es NULL");
		if (poke_encontrado == NULL)
			return;
		pa2m_afirmar(pokemones_son_iguales(*poke_encontrado,
						   pokemones[i]),
			     "Pokemon encontrado");
	}

	const struct pokemon *no_existe;
	no_existe = pokedex_buscar_pokemon(pokedex, "NOEXISTE");
	pa2m_afirmar(no_existe == NULL,
		     "NULL: no se ha encontrado al pokemon que no existe");
	free(poke1.nombre);
	free(poke2.nombre);
	free(poke3.nombre);
	free(poke4.nombre);
	free(poke5.nombre);
	free(poke6.nombre);
	free(poke7.nombre);
	pokedex_destruir(pokedex);
}

void seMuestranLosPokes()
{
	printf("PRUEBA POKEMONES ORDENADOS \n");
	struct pokedex *pokedex = pokedex_crear();
	struct pokemon poke1 = { .nombre = asignar_nombre("Charmander"),
				 .tipo = TIPO_ELECTRICO,
				 .fuerza = 45,
				 .destreza = 10,
				 .resistencia = 20 };
	struct pokemon poke2 = { .nombre = asignar_nombre("Algo"),
				 .tipo = TIPO_ROCA,
				 .fuerza = 45,
				 .destreza = 10,
				 .resistencia = 20 };
	struct pokemon poke3 = { .nombre = asignar_nombre("Igna"),
				 .tipo = TIPO_FUEGO,
				 .fuerza = 45,
				 .destreza = 10,
				 .resistencia = 20 };
	struct pokemon poke4 = { .nombre = asignar_nombre("Barco"),
				 .tipo = TIPO_FUEGO,
				 .fuerza = 45,
				 .destreza = 10,
				 .resistencia = 20 };
	struct pokemon poke5 = { .nombre = asignar_nombre("Flaco"),
				 .tipo = TIPO_FUEGO,
				 .fuerza = 45,
				 .destreza = 10,
				 .resistencia = 20 };
	struct pokemon poke6 = { .nombre = asignar_nombre("Zequiel"),
				 .tipo = TIPO_FUEGO,
				 .fuerza = 45,
				 .destreza = 10,
				 .resistencia = 20 };
	struct pokemon poke7 = { .nombre = asignar_nombre("Astro"),
				 .tipo = TIPO_FUEGO,
				 .fuerza = 45,
				 .destreza = 10,
				 .resistencia = 20 };
	pokedex_agregar_pokemon(pokedex, poke1);
	pokedex_agregar_pokemon(pokedex, poke3);
	pokedex_agregar_pokemon(pokedex, poke2);
	pokedex_agregar_pokemon(pokedex, poke4);
	pokedex_agregar_pokemon(pokedex, poke5);
	pokedex_agregar_pokemon(pokedex, poke6);
	pokedex_agregar_pokemon(pokedex, poke7);
	struct ctx_evaluar_poke cep = { .cantidad_bien = 0,
					.actual = 0,
					.nombres = { "Algo", "Astro", "Barco",
						     "Charmander", "Flaco",
						     "Igna", "Zequiel" } };
	pokedex_iterar_pokemones(pokedex, evaluar_poke, &cep);
	free(poke1.nombre);
	free(poke2.nombre);
	free(poke3.nombre);
	free(poke4.nombre);
	free(poke5.nombre);
	free(poke6.nombre);
	free(poke7.nombre);
	pa2m_afirmar(cep.cantidad_bien == 7,
		     "Los nombres están ordenados en forma creciente");
	pokedex_destruir(pokedex);
}
void cincoPokesConMismoPunteroaNombre()
{
	printf("PRUEBA CINCO POKEMONES CON EL MISMO NOMBRE \n");
	struct pokedex *pokedex = pokedex_crear();

	char *nombre = calloc(10, sizeof(char));
	strcpy(nombre, "MISMO");
	struct pokemon poke1 = { .nombre = nombre,
				 .tipo = TIPO_ELECTRICO,
				 .fuerza = 45,
				 .destreza = 3,
				 .resistencia = 20 };
	struct pokemon poke2 = { .nombre = nombre,
				 .tipo = TIPO_ELECTRICO,
				 .fuerza = 45,
				 .destreza = 1,
				 .resistencia = 20 };
	struct pokemon poke3 = { .nombre = nombre,
				 .tipo = TIPO_ELECTRICO,
				 .fuerza = 1,
				 .destreza = 10,
				 .resistencia = 20 };
	struct pokemon poke4 = { .nombre = nombre,
				 .tipo = TIPO_ELECTRICO,
				 .fuerza = 45,
				 .destreza = 10,
				 .resistencia = 20 };
	struct pokemon poke5 = { .nombre = nombre,
				 .tipo = TIPO_ELECTRICO,
				 .fuerza = 45,
				 .destreza = 23,
				 .resistencia = 18 };
	pokedex_agregar_pokemon(pokedex, poke1);
	pokedex_agregar_pokemon(pokedex, poke2);
	pokedex_agregar_pokemon(pokedex, poke3);
	pokedex_agregar_pokemon(pokedex, poke4);
	pokedex_agregar_pokemon(pokedex, poke5);
	const struct pokemon *poke6 = pokedex_buscar_pokemon(pokedex, "MISMO");
	bool son_iguales = pokemones_son_iguales(*poke6, poke1);
	pa2m_afirmar(son_iguales, "Se ha encontrado al pokemon 1");
	const struct pokemon *poke7 = pokedex_buscar_pokemon(pokedex, "MISMO");
	son_iguales = pokemones_son_iguales(*poke7, poke2);
	pa2m_afirmar(son_iguales, "Se ha encontrado al pokemon 2");
	const struct pokemon *poke8 = pokedex_buscar_pokemon(pokedex, "MISMO");
	son_iguales = pokemones_son_iguales(*poke8, poke3);
	pa2m_afirmar(son_iguales, "Se ha encontrado al pokemon 3");
	const struct pokemon *poke9 = pokedex_buscar_pokemon(pokedex, "MISMO");
	son_iguales = pokemones_son_iguales(*poke9, poke4);
	pa2m_afirmar(son_iguales, "Se ha encontrado al pokemon 4");
	const struct pokemon *poke10 = pokedex_buscar_pokemon(pokedex, "MISMO");
	son_iguales = pokemones_son_iguales(*poke10, poke5);
	pa2m_afirmar(son_iguales, "Se ha encontrado al pokemon 5");
	pokedex_destruir(pokedex);
	free(nombre);
}
int main()
{
	pa2m_nuevo_grupo("Pruebas de archivos CSV");
	abrirUnArchivoInexistenteDebeRetornarNull();
	seAbreUnArchivo_seLeenLasLineas();
	pa2m_nuevo_grupo("Pruebas de lectura de CSV con trampa");
	dosColumnasCuandoLaTercerFuncionEsNula();
	ceroLineasCuandoEsElFinal();
	pa2m_nuevo_grupo("Pruebas de pokedex");
	seCreaLaPokedex();
	seAgregaUnPokemon();
	seCuentanLosPokemones();
	seEncuentranTodosLosPokemones();
	seMuestranLosPokes();
	cincoPokesConMismoPunteroaNombre();
	return pa2m_mostrar_reporte();
}
