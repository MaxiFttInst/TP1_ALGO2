#include "pokedex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct nodo_pokemon {
	struct pokemon poke;
	void *siguiente;
};
struct pokedex {
	size_t cantidad;
	int ultimo_encontrado;
	struct nodo_pokemon *lista;
};

struct pokemon __nulo = { .nombre = "",
			  .tipo = 0,
			  .fuerza = 0,
			  .destreza = 0,
			  .resistencia = 0 };
struct pokedex *pokedex_crear()
{
	struct pokedex *nueva_pdex = calloc(1, sizeof(struct pokedex));
	if (nueva_pdex != NULL) {
		nueva_pdex->cantidad = 0;
		nueva_pdex->ultimo_encontrado = -1;
		nueva_pdex->lista = NULL;
	}
	return nueva_pdex;
}

bool pokedex_agregar_pokemon(struct pokedex *pokedex, struct pokemon pokemon)
{
	if (pokedex == NULL)
		return false;
	if(pokemon.nombre == NULL)
		return false;
	pokedex->cantidad++;
	if (pokedex->lista == NULL) {
		pokedex->lista = calloc(1, sizeof(struct nodo_pokemon));
		if (pokedex->lista != NULL) {
			pokedex->lista->poke = pokemon;
			pokedex->lista->poke.nombre = calloc(strlen(pokemon.nombre) + 1, sizeof(char));
			strcpy(pokedex->lista->poke.nombre, pokemon.nombre);
			pokedex->lista->siguiente = NULL;
		}
	} else {
		struct nodo_pokemon *direccion_lista = NULL;
		struct nodo_pokemon nodo_aux = { .poke = __nulo,
						 .siguiente = NULL };
		struct nodo_pokemon *nodo_actual = pokedex->lista;
		struct nodo_pokemon *nuevo_nodo =
			calloc(1, sizeof(struct nodo_pokemon));
		if (nuevo_nodo == NULL)
			return false;

		bool posicionado = false;

		while (!posicionado && nodo_actual != NULL) {
			if (nodo_actual->siguiente == NULL) {
				if (strcmp(nodo_actual->poke.nombre,
					   pokemon.nombre) < 0) {
					nuevo_nodo->poke = pokemon;
					nuevo_nodo->poke.nombre = calloc(strlen(pokemon.nombre) + 1, sizeof(char));
					strcpy(nuevo_nodo->poke.nombre, pokemon.nombre);
					nodo_actual->siguiente = nuevo_nodo;
				} else {
					nodo_aux.poke = nodo_actual->poke;
					nodo_actual->poke = pokemon;
					nodo_actual->poke.nombre = calloc(strlen(pokemon.nombre) + 1, sizeof(char));
					strcpy(nodo_actual->poke.nombre, pokemon.nombre);
					nuevo_nodo->poke = nodo_aux.poke;
					nodo_actual->siguiente = nuevo_nodo;
				}
				posicionado = true;
			} else if (strcmp(nodo_actual->poke.nombre,
					  pokemon.nombre) > 0 &&
				   strcmp(pokemon.nombre,
					  ((struct nodo_pokemon
						    *)(nodo_actual->siguiente))
						  ->poke.nombre) < 0) {
				direccion_lista = nodo_actual->siguiente;
				nodo_aux.poke = nodo_actual->poke;
				nodo_actual->poke = pokemon;
				nodo_actual->poke.nombre = calloc(strlen(pokemon.nombre) + 1, sizeof(char));
				strcpy(nodo_actual->poke.nombre, pokemon.nombre);
				nuevo_nodo->poke = nodo_aux.poke;
				nodo_actual->siguiente = nuevo_nodo;
				nuevo_nodo->siguiente = direccion_lista;
				posicionado = true;

			} else if (strcmp(nodo_actual->poke.nombre,
					  pokemon.nombre) < 0 &&
				   strcmp(pokemon.nombre,
					  ((struct nodo_pokemon
						    *)(nodo_actual->siguiente))
						  ->poke.nombre) < 0) {
				nuevo_nodo->poke = pokemon;
				nuevo_nodo->poke.nombre = calloc(strlen(pokemon.nombre) + 1, sizeof(char));
				strcpy(nuevo_nodo->poke.nombre, pokemon.nombre);
				direccion_lista = nodo_actual->siguiente;
				nodo_actual->siguiente = nuevo_nodo;
				nuevo_nodo->siguiente = direccion_lista;
				posicionado = true;
			}
			if (!posicionado)
				nodo_actual = nodo_actual->siguiente;
		}
	}
	return true;
}

size_t pokedex_cantidad_pokemones(struct pokedex *pokedex)
{
	if (pokedex == NULL)
		return 0;
	return pokedex->cantidad;
}

const struct pokemon *pokedex_buscar_pokemon(struct pokedex *pokedex,
					     const char *nombre)
{
	if (pokedex == NULL)
		return NULL;

	bool encontrado = false;
	int contador = 0;
	const struct pokemon *buscado = NULL;
	const struct nodo_pokemon *nodo_actual = pokedex->lista;
	while (nodo_actual != NULL && !encontrado) {
		if (strcmp(nodo_actual->poke.nombre, nombre) == 0) {
			if(pokedex->ultimo_encontrado < contador)
			{
				buscado = &(nodo_actual->poke);
				encontrado = true;
				pokedex->ultimo_encontrado = contador;
			}
		}
		nodo_actual = nodo_actual->siguiente;
		contador++;
	}
	return buscado;
}

size_t pokedex_iterar_pokemones(struct pokedex *pokedex,
				bool (*funcion)(struct pokemon *, void *),
				void *ctx)
{
	if (pokedex == NULL)
		return 0;

	struct nodo_pokemon *nodo_actual = pokedex->lista;
	size_t pokemones_iterados = 0;
	bool seguir_iterando = true;
	while (nodo_actual != NULL && seguir_iterando) {
		seguir_iterando = funcion(&nodo_actual->poke, ctx);
		nodo_actual = nodo_actual->siguiente;
		pokemones_iterados++;
	}
	return pokemones_iterados;
}

void pokedex_destruir(struct pokedex *pokedex)
{
	if (pokedex == NULL)
		return;

	if (pokedex->cantidad == 0) {
		free(pokedex);
		return;
	}
	struct nodo_pokemon *nodo_actual = pokedex->lista;
	struct nodo_pokemon *nodo_a_borrar = NULL;
	while (nodo_actual != NULL) {
		nodo_a_borrar = nodo_actual;
		nodo_actual = nodo_a_borrar->siguiente;
		free(nodo_a_borrar->poke.nombre);
		free(nodo_a_borrar);
	}
	free(pokedex);
}
