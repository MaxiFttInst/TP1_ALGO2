#include "pokedex.h"
#include <stdlib.h>
#include <string.h>

struct nodo_pokemon {
	struct pokemon poke;
	void* siguiente;	
};
struct pokedex {
	size_t cantidad;
	struct nodo_pokemon *lista;
};

struct pokemon __nulo = {
	.nombre = "",
	.tipo = 0,
	.fuerza = 0,
	.destreza = 0,
	.resistencia = 0
};
bool pokemones_son_iguales(struct pokemon poke1, struct pokemon poke2){
	return strcmp(poke1.nombre, poke2.nombre) == 0
		&& poke1.tipo == poke2.tipo
		&& poke1.resistencia == poke2.resistencia
		&& poke1.destreza == poke2.destreza
		&& poke1.fuerza == poke2.fuerza;
}
struct pokedex *pokedex_crear()
{
	struct pokedex *nueva_pdex = calloc(1, sizeof(struct pokedex));
	if (nueva_pdex != NULL){
		nueva_pdex->cantidad = 0;
		nueva_pdex->lista = NULL;
	}
	return nueva_pdex;
}

bool pokedex_agregar_pokemon(struct pokedex *pokedex, struct pokemon pokemon)
{
	if (pokedex == NULL)
		return false;
	pokedex->cantidad++;
	if(pokedex->lista == NULL){
		pokedex->lista = calloc(1, sizeof(struct nodo_pokemon));
		pokedex->lista->poke = pokemon;
		pokedex->lista->siguiente = NULL;
	} else {
		struct nodo_pokemon *direccion_lista = pokedex->lista;
		struct nodo_pokemon *nuevo_nodo = calloc(1, sizeof(struct nodo_pokemon));
		nuevo_nodo->poke = pokemon;
		pokedex->lista = nuevo_nodo;
		nuevo_nodo->siguiente = direccion_lista;
	}
	return true;
}

size_t pokedex_cantidad_pokemones(struct pokedex *pokedex)
{
	return pokedex->cantidad;
}

const struct pokemon *pokedex_buscar_pokemon(struct pokedex *pokedex,
					     const char *nombre)
{
	bool encontrado = false;
	const struct pokemon *buscado = NULL;
	const struct nodo_pokemon *nodo_actual = pokedex->lista;
	while(nodo_actual != NULL && !encontrado){
		if(strcmp(nodo_actual->poke.nombre, nombre) == 0){
			buscado = &(nodo_actual->poke);
			encontrado = true;
		}
		nodo_actual = nodo_actual->siguiente;
	}
	return buscado;
}


size_t pokedex_iterar_pokemones(struct pokedex *pokedex,
				bool (*funcion)(struct pokemon *, void *),
				void *ctx)
{
	return 0;
}

void pokedex_destruir(struct pokedex *pokedex)
{
	if(pokedex->cantidad == 0){
		free(pokedex);
		return;
	}
	struct nodo_pokemon *nodo_actual = pokedex->lista;
	struct nodo_pokemon *nodo_a_borrar = NULL;
	while(nodo_actual != NULL){
		nodo_a_borrar = nodo_actual;
		nodo_actual = nodo_a_borrar->siguiente;
		free(nodo_a_borrar);
	}
	free(pokedex);
	
}
