<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TP1

# ESTO ES UN TEMPLATE, COMPLETAR LO QUE HAGA FALTA, ELIMINAR LO QUE SOBRE (INCLUIDA ESTA LÍNEA)

## Alumno: Maximiliano Fittipaldi - 111676 - mafittipaldi@fi.uba.ar

- Para compilar:

```bash
línea de compilación
```

- Para ejecutar:

```bash
línea de ejecución
```

- Para ejecutar con valgrind:
```bash
línea con valgrind
```

---

##  Funcionamiento y respuesta a preguntas teóricas

### Pokedex

La pokedex consiste en: 

```c
struct nodo_pokemon {
	struct pokemon poke;
	void* siguiente;	
};
struct pokedex {
	size_t cantidad;
	struct nodo_pokemon *lista;
};
```
Es decir, la pokedex almacena el número de pokemones en _cantidad_
y tiene un struct que apunta al primer nodo pokemon, el cual posee el
pokemon propiamente dicho y un puntero al siguiente nodo. De esa forma,
todos los nodos están conectados.
Esta estructura fue utilizada por las ventajas que brinda, aunque se mencionarán
al final, dado que es mejor primero ver su funcionamiento para entender sus cualidades.

#### pokedex_crear: O(1)
```c
struct pokedex *pokedex_crear()
{
	struct pokedex *nueva_pdex = calloc(1, sizeof(struct pokedex));
	if (nueva_pdex != NULL){
		nueva_pdex->cantidad = 0;
		nueva_pdex->lista = NULL;
	}
	return nueva_pdex;
}
```
Dado que no recibe parámetros y la inicialización siempre es igual, la complejidad
resulta en O(1)

#### pokedex_agregar_pokemon: O(n)

Esta función agrega y ordena por nombre de pokemones.
Puede verse como varios casos separados:
1. La lista está vacía
2. El siguiente nodo es NULL
3. tiene más de un nodo y el que se quiere insertar es mayor al
primero pero menor al segundo
4. tiene más de un nodo y el que se quiere insertar está
entre estos dos nodos.

##### 1
```c
...
	if(pokedex->lista == NULL){
		pokedex->lista = calloc(1, sizeof(struct nodo_pokemon));
		if(pokedex->lista != NULL){
			pokedex->lista->poke = pokemon;
			pokedex->lista->siguiente = NULL;
		}
	} else { ...
```
Creamos un nodo pokemon y lo ponemos en la cabeza de la lista, es decir,
en _pokedex->lista_ y decimos que su siguiente es NULL.
##### Preliminares
Antes de seguir, veamos las declaraciones que son comunes para el resto de los
puntos.
```c
...
	} else {
		struct nodo_pokemon *direccion_lista = NULL;
		struct nodo_pokemon nodo_aux = {.poke = __nulo, .siguiente = NULL};
		struct nodo_pokemon *nodo_actual = pokedex->lista;
		struct nodo_pokemon *nuevo_nodo = calloc(1, sizeof(struct nodo_pokemon));
		if (nuevo_nodo == NULL) return false;

		bool posicionado = false;

		while(!posicionado && nodo_actual != NULL){ ...
```
En este caso vamos a tener varios nodos.
- direccion_lista: será para insertar el nuevo nodo sin perder el resto
- nodo_aux: servirá para hacer cambios entre nodos.
- nodo_actual: es el que se usará para recorrer toda la lista.
- nuevo_nodo: el nodo que efectivamente se insertará con el nuevo pokemon.

Luego verificamos que _nuevo_nodo_ sea válido y declaramos una variable _posicionado_
para saber cuándo salir del bucle que manejará las 3 condiciones restantes. Este
último iterará hasta que se haya posicionado o hasta que el nodo actual sea NULL.

##### 2
```c
	if(nodo_actual->siguiente == NULL){
		if(strcmp(nodo_actual->poke.nombre, pokemon.nombre) < 0){
			nuevo_nodo->poke = pokemon;
			nodo_actual->siguiente = nuevo_nodo;
		} else {
			nodo_aux.poke = nodo_actual->poke;
			nodo_actual->poke = pokemon;
			nuevo_nodo->poke = nodo_aux.poke;
			nodo_actual->siguiente = nuevo_nodo;
		}
		posicionado = true;
```
Acá pueden pasar dos cosas: el pokemon del nuevo nodo es mayor o menor al actual.
Si el pokemon es mayor al correspondiente al nodo actual, el nuevo nodo se anida
al siguiente del nodo actual. Caso contrario, _nodo_actual_ toma los valores
del nuevo nodo y este se anida al nodo actual.
##### 3
```c

} else if(strcmp(nodo_actual->poke.nombre, pokemon.nombre) > 0
    && strcmp(pokemon.nombre, ((struct nodo_pokemon*)(nodo_actual->siguiente))->poke.nombre) < 0){
        direccion_lista = nodo_actual->siguiente;
        nodo_aux.poke = nodo_actual->poke;
        nodo_actual->poke = pokemon;
        nuevo_nodo->poke = nodo_aux.poke;
        nodo_actual->siguiente = nuevo_nodo;
        nuevo_nodo->siguiente = direccion_lista;
        posicionado = true;
```
En este caso, verificamos que: 
pokemon_actual.nombre > pokemon.nombre < suguiente_pokemon.nombre.
Si esto es cierto, necesitamos que la estructura quede así:
pokemon.nombre < pokemon_actual.nombre < suguiente_pokemon.nombre.
y anidar.
Para eso, damos vuelta los valores con un nodo auxiliar, _nodo_actual_ pasa a apuntar
a _nuevo_nodo_ y este último apunta a _dirección_lista_, una variable auxiliar que
nos ayuda a no perder el registro del resto de nodos.

##### 4
```c
} else if(strcmp(nodo_actual->poke.nombre, pokemon.nombre) < 0
    && strcmp(pokemon.nombre, ((struct nodo_pokemon*)(nodo_actual->siguiente))->poke.nombre) < 0){
        nuevo_nodo->poke = pokemon;
        direccion_lista = nodo_actual->siguiente;
        nodo_actual->siguiente = nuevo_nodo;
        nuevo_nodo->siguiente = direccion_lista;
        posicionado = true;
}
```
En este caso, estamos contemplando lo siguiente:
pokemon.nombre < pokemon_actual.nombre < suguiente_pokemon.nombre.
Muy parecido al anterior, de hecho, igual, sólo que sin el otro nodo auxiliar.

