#include "pa2m.h"
#include "src/csv.h"
#include "src/pokedex.h"
#include <string.h>
void abrirUnArchivoInexistenteDebeRetornarNull()
{
	struct archivo_csv *archivo =
		abrir_archivo_csv("noexiste/noexiste/noexiste/no", ';');
	pa2m_afirmar(archivo == NULL, "El archivo no existe y se retorna NULL");
}
// bool preservar(const char * s, void  * ctx);
// bool convertir_a_tipo(const char * s, void  * ctx);
// bool convertir_a_int(const char * s, void  * ctx);
// void seAbreUnArchivo_seLeenLasLineas()
// {
//
// 	struct archivo_csv *archivo =
// 		abrir_archivo_csv("ejemplos/pokedex.csv", ';');
// 	pa2m_afirmar(archivo != NULL, "Archivo abierto exitosamente");
// 	
// 	bool (*array_funcs[5])();
// 	array_funcs[0] = preservar;
// 	array_funcs[1] = convertir_a_tipo;
// 	array_funcs[2] = convertir_a_int;
// 	array_funcs[3] = convertir_a_int;
// 	array_funcs[4] = convertir_a_int;
// 	struct pokemon poke = {
// 		.nombre = "Charmander",
// 		.tipo = TIPO_ELECTRICO,
// 		.fuerza = 45,
// 		.destreza = 10,
// 		.resistencia = 20
// 	};
// 	void *contexto[5] = {&(poke.nombre),&(poke.tipo),
// 		&(poke.fuerza),&(poke.destreza),&(poke.resistencia)};
//
// 	size_t lineas = leer_linea_csv(archivo, 4, array_funcs, contexto);
// 	bool linea_leida_bien = false;
// 	if (strcmp(poke.nombre, "Pikachu") == 0
// 		&& poke.tipo == TIPO_ELECTRICO
// 		&& poke.resistencia == 20
// 		&& poke.destreza == 15
// 		&& poke.fuerza == 17)
// 			linea_leida_bien = true;
// 	pa2m_afirmar(linea_leida_bien, "Archivo abierto exitosamente");
// 	pa2m_afirmar(lineas == 1, "Se leyo 1 linea");
// 	cerrar_archivo_csv(archivo);
//
// }
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
		.nombre = "Charmander",
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
		.nombre = "Charmander",
		.tipo = TIPO_ELECTRICO,
		.fuerza = 45,
		.destreza = 10,
		.resistencia = 20
	};
	struct pokemon poke2 = {
		.nombre = "Algo",
		.tipo = TIPO_ROCA,
		.fuerza = 45,
		.destreza = 10,
		.resistencia = 20
	};
	struct pokemon poke3 = {
		.nombre = "Igna",
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
		.nombre = "Charmander",
		.tipo = TIPO_ELECTRICO,
		.fuerza = 45,
		.destreza = 10,
		.resistencia = 20
	};
	struct pokemon poke2 = {
		.nombre = "Algo",
		.tipo = TIPO_ROCA,
		.fuerza = 45,
		.destreza = 10,
		.resistencia = 20
	};
	struct pokemon poke3 = {
		.nombre = "Igna",
		.tipo = TIPO_FUEGO,
		.fuerza = 45,
		.destreza = 10,
		.resistencia = 20
	};
	struct pokemon poke4 = {
		.nombre = "uyiouyuio",
		.tipo = TIPO_FUEGO,
		.fuerza = 45,
		.destreza = 10,
		.resistencia = 20
	};
	struct pokemon poke5 = {
		.nombre = "fdghdfg",
		.tipo = TIPO_FUEGO,
		.fuerza = 45,
		.destreza = 10,
		.resistencia = 20
	};
	struct pokemon poke6 = {
		.nombre = "hkljl",
		.tipo = TIPO_FUEGO,
		.fuerza = 45,
		.destreza = 10,
		.resistencia = 20
	};
	struct pokemon poke7 = {
		.nombre = "asdfasf",
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
int main()
{
	pa2m_nuevo_grupo("Pruebas de archivos CSV");
	abrirUnArchivoInexistenteDebeRetornarNull();
	pa2m_nuevo_grupo("Pruebas de pokedex");
	seCreaLaPokedex();
	seAgregaUnPokemon();
	seCuentanLosPokemones();
	seEncuentraElPokemonBuscado();
	return pa2m_mostrar_reporte();
}
