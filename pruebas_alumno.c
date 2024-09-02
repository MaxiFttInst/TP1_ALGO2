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
	pokedex_agregar_pokemon(pokedex, poke1);
	pokedex_agregar_pokemon(pokedex, poke2);
	pokedex_agregar_pokemon(pokedex, poke3);
	const struct pokemon *poke4 = pokedex_buscar_pokemon(pokedex, "Algo");
	pa2m_afirmar( poke4 != NULL, "El pokemon no es NULL");
	if(poke4 == NULL) return;

	bool son_iguales = false;
	if (strcmp(poke4->nombre, poke2.nombre) == 0
		&& poke4->tipo == poke2.tipo
		&& poke4->resistencia == poke2.resistencia
		&& poke4->destreza == poke2.destreza
		&& poke4->fuerza == poke2.fuerza)
		son_iguales = true;
	pa2m_afirmar( son_iguales, "Pokemon encontrado");

	poke4 = pokedex_buscar_pokemon(pokedex, "NOEXISTE");
	pa2m_afirmar( poke4 == NULL, "NULL: no se ha encontrado al pokemon que no existe");
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
