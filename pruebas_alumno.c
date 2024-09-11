#include "pa2m.h"
#include "src/csv.h"
#include "src/pokedex.h"
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

char *asignar_nombre(char *nombre){
	char *ptr = calloc(1, sizeof(char) * (strlen(nombre) + 1));
	strcpy(ptr, nombre);
	return ptr;
}
bool mostrar_poke(struct pokemon *poke, void *ctx){
	printf("%s\n", poke->nombre);
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
						       leer_caracter,
						       leer_int, leer_int, leer_int };
	struct pokemon poke = {
		.nombre = "Charmander",
		.tipo = TIPO_ELECTRICO,
		.fuerza = 45,
		.destreza = 10,
		.resistencia = 20
	};
	void *contexto[5] = {&(poke.nombre),&(poke.tipo),
		&(poke.fuerza),&(poke.destreza),&(poke.resistencia)};

	size_t lineas = leer_linea_csv(archivo, 5, funciones, contexto);
	pa2m_afirmar(lineas == 5, "Se leyo 1 linea con 5 columnas");
	bool linea_leida_bien = false;
	if (strcmp(poke.nombre, "Pikachu") == 0
		&& poke.tipo == TIPO_ELECTRICO
		&& poke.fuerza == 20
		&& poke.destreza == 15
		&& poke.resistencia == 17)
			linea_leida_bien = true;
	pa2m_afirmar(linea_leida_bien, "La linea se leyó y se asignaron los valores");
	free(poke.nombre);
	cerrar_archivo_csv(archivo);

}
void seCreaLaPokedex()
{
	struct pokedex *pokedex = pokedex_crear();
	pa2m_afirmar(pokedex != NULL, "La pokedex se ha creado");
	pokedex_destruir(pokedex);
}

void seAgregaUnPokemon()
{
	struct pokedex *pokedex = pokedex_crear();
	struct pokemon poke = {
		.nombre = asignar_nombre("Charmander"),
		.tipo = TIPO_ELECTRICO,
		.fuerza = 45,
		.destreza = 10,
		.resistencia = 20
	};
	bool agregado = pokedex_agregar_pokemon(pokedex, poke);
	pa2m_afirmar(agregado == true, "Pokemon agregado a la pokedex");
	pokedex_destruir(pokedex);
}

void seCuentanLosPokemones()
{
	struct pokedex *pokedex = pokedex_crear();
	struct pokemon poke1 = {
		.nombre = asignar_nombre("Charmander"),
		.tipo = TIPO_ELECTRICO,
		.fuerza = 45,
		.destreza = 10,
		.resistencia = 20
	};
	struct pokemon poke2 = {
		.nombre = asignar_nombre("Algo"),
		.tipo = TIPO_ROCA,
		.fuerza = 45,
		.destreza = 10,
		.resistencia = 20
	};
	struct pokemon poke3 = {
		.nombre = asignar_nombre("Igna"),
		.tipo = TIPO_FUEGO,
		.fuerza = 45,
		.destreza = 10,
		.resistencia = 20
	};
	pokedex_agregar_pokemon(pokedex, poke1);
	pokedex_agregar_pokemon(pokedex, poke2);
	pokedex_agregar_pokemon(pokedex, poke3);
	pa2m_afirmar(pokedex_cantidad_pokemones(pokedex) == 3, "Hay 3 pokemones");
	pokedex_destruir(pokedex);
}
void seEncuentraElPokemonBuscado()
{
	struct pokedex *pokedex = pokedex_crear();
	struct pokemon poke1 = {
		.nombre = asignar_nombre("Charmander"),
		.tipo = TIPO_ELECTRICO,
		.fuerza = 45,
		.destreza = 10,
		.resistencia = 20
	};
	struct pokemon poke2 = {
		.nombre = asignar_nombre("Algo"),
		.tipo = TIPO_ROCA,
		.fuerza = 45,
		.destreza = 10,
		.resistencia = 20
	};
	struct pokemon poke3 = {
		.nombre = asignar_nombre("Igna"),
		.tipo = TIPO_FUEGO,
		.fuerza = 45,
		.destreza = 10,
		.resistencia = 20
	};
	struct pokemon poke4 = {
		.nombre = asignar_nombre("uyiouyuio"),
		.tipo = TIPO_FUEGO,
		.fuerza = 45,
		.destreza = 10,
		.resistencia = 20
	};
	struct pokemon poke5 = {
		.nombre = asignar_nombre("fdghdfg"),
		.tipo = TIPO_FUEGO,
		.fuerza = 45,
		.destreza = 10,
		.resistencia = 20
	};
	struct pokemon poke6 = {
		.nombre = asignar_nombre("hkljl"),
		.tipo = TIPO_FUEGO,
		.fuerza = 45,
		.destreza = 10,
		.resistencia = 20
	};
	struct pokemon poke7 = {
		.nombre = asignar_nombre("asdfasf"),
		.tipo = TIPO_FUEGO,
		.fuerza = 45,
		.destreza = 10,
		.resistencia = 20
	};
	pokedex_agregar_pokemon(pokedex, poke1);
	pokedex_agregar_pokemon(pokedex, poke3);
	pokedex_agregar_pokemon(pokedex, poke2);
	pokedex_agregar_pokemon(pokedex, poke4);
	pokedex_agregar_pokemon(pokedex, poke5);
	pokedex_agregar_pokemon(pokedex, poke6);
	pokedex_agregar_pokemon(pokedex, poke7);
	const struct pokemon *poke8 = pokedex_buscar_pokemon(pokedex, "Algo");
	pa2m_afirmar( poke8 != NULL, "El pokemon no es NULL");
	if(poke8 == NULL) return;

	bool son_iguales = false;
	if (strcmp(poke8->nombre, poke2.nombre) == 0
		&& poke8->tipo == poke2.tipo
		&& poke8->resistencia == poke2.resistencia
		&& poke8->destreza == poke2.destreza
		&& poke8->fuerza == poke2.fuerza)
		son_iguales = true;
	pa2m_afirmar( son_iguales, "Pokemon encontrado");

	poke8 = pokedex_buscar_pokemon(pokedex, "NOEXISTE");
	pa2m_afirmar( poke8 == NULL, "NULL: no se ha encontrado al pokemon que no existe");
	pokedex_destruir(pokedex);
}

void seMuestranLosPokes()
{
	struct pokedex *pokedex = pokedex_crear();
	struct pokemon poke1 = {
		.nombre = asignar_nombre("Charmander"),
		.tipo = TIPO_ELECTRICO,
		.fuerza = 45,
		.destreza = 10,
		.resistencia = 20
	};
	struct pokemon poke2 = {
		.nombre = asignar_nombre("Algo"),
		.tipo = TIPO_ROCA,
		.fuerza = 45,
		.destreza = 10,
		.resistencia = 20
	};
	struct pokemon poke3 = {
		.nombre = asignar_nombre("Igna"),
		.tipo = TIPO_FUEGO,
		.fuerza = 45,
		.destreza = 10,
		.resistencia = 20
	};
	struct pokemon poke4 = {
		.nombre = asignar_nombre("uyiouyuio"),
		.tipo = TIPO_FUEGO,
		.fuerza = 45,
		.destreza = 10,
		.resistencia = 20
	};
	struct pokemon poke5 = {
		.nombre = asignar_nombre("fdghdfg"),
		.tipo = TIPO_FUEGO,
		.fuerza = 45,
		.destreza = 10,
		.resistencia = 20
	};
	struct pokemon poke6 = {
		.nombre = asignar_nombre("hkljl"),
		.tipo = TIPO_FUEGO,
		.fuerza = 45,
		.destreza = 10,
		.resistencia = 20
	};
	struct pokemon poke7 = {
		.nombre = asignar_nombre("Asdfasf"),
		.tipo = TIPO_FUEGO,
		.fuerza = 45,
		.destreza = 10,
		.resistencia = 20
	};
	pokedex_agregar_pokemon(pokedex, poke1);
	pokedex_agregar_pokemon(pokedex, poke3);
	pokedex_agregar_pokemon(pokedex, poke2);
	pokedex_agregar_pokemon(pokedex, poke4);
	pokedex_agregar_pokemon(pokedex, poke5);
	pokedex_agregar_pokemon(pokedex, poke6);
	pokedex_agregar_pokemon(pokedex, poke7);
	pokedex_iterar_pokemones(pokedex, mostrar_poke, NULL);
	pokedex_destruir(pokedex);
}
int main()
{
	pa2m_nuevo_grupo("Pruebas de archivos CSV");
	abrirUnArchivoInexistenteDebeRetornarNull();
	seAbreUnArchivo_seLeenLasLineas();
	pa2m_nuevo_grupo("Pruebas de pokedex");
	seCreaLaPokedex();
	seAgregaUnPokemon();
	seCuentanLosPokemones();
	seEncuentraElPokemonBuscado();
	seMuestranLosPokes();
	return pa2m_mostrar_reporte();
}
