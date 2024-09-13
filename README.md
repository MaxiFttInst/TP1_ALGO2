<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TP1

## Alumno: Maximiliano Fittipaldi - 111676 - mafittipaldi@fi.uba.ar

- Para ejecutar todo:

```bash
make
```

- Para ejecutar pruebas:

```bash
make clean && make valgrind-alumno
```
- Para ejecutar tp1:

```bash
make clean && make valgrind-tp1
```

- Para ejecutar ejemplo:
```bash
make clean && make valgrind-ejemplo
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
	int ultimo_encontrado;
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

##### Complejidad
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
	if (pokedex->lista != NULL) {
		pokedex->lista->poke = pokemon;
		pokedex->lista->poke.nombre = calloc(
			strlen(pokemon.nombre) + 1, sizeof(char));
		strcpy(pokedex->lista->poke.nombre, pokemon.nombre);
		pokedex->lista->siguiente = NULL;
	}
} else { ...
```
Creamos un nodo pokemon y lo ponemos en la cabeza de la lista, es decir,
en _pokedex->lista_ y decimos que su siguiente es NULL. Verás que también,
luego de asignar el pokemon, creamos una string dinámica y se la asignamos al
nombre, y esto lo hacemos en el resto de casos,
¿Por qué? Podría pasar que el usuario le pase al pokemon una string del
stack, o una string del heap, y si esta última es liberada por el usuario, la
pokedex perderá el nombre de ese pokemon. Por eso, se copia y luego se elimina junto
con los nodos.
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
		if(strcmp(nodo_actual->poke.nombre, pokemon.nombre) <= 0){
			nuevo_nodo->poke = pokemon;
			nuevo_nodo->poke.nombre = calloc(
				strlen(pokemon.nombre) + 1,
				sizeof(char));
			strcpy(nuevo_nodo->poke.nombre,
			       pokemon.nombre);
			nodo_actual->siguiente = nuevo_nodo;
		} else {
			nodo_aux.poke = nodo_actual->poke;
			nodo_actual->poke = pokemon;
			nodo_actual->poke.nombre = calloc(
				strlen(pokemon.nombre) + 1,
				sizeof(char));
			strcpy(nodo_actual->poke.nombre,
			       pokemon.nombre);
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
	nodo_actual->poke.nombre =
		calloc(strlen(pokemon.nombre) + 1,
		       sizeof(char));
	strcpy(nodo_actual->poke.nombre,
	       pokemon.nombre);
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
	nuevo_nodo->poke.nombre =
		calloc(strlen(pokemon.nombre) + 1,
		       sizeof(char));
	strcpy(nuevo_nodo->poke.nombre, pokemon.nombre);
	direccion_lista = nodo_actual->siguiente;
	nodo_actual->siguiente = nuevo_nodo;
	nuevo_nodo->siguiente = direccion_lista;
	posicionado = true;
}
```
En este caso, estamos contemplando lo siguiente:
pokemon.nombre < pokemon_actual.nombre < suguiente_pokemon.nombre.
Muy parecido al anterior, de hecho, igual, sólo que sin el otro nodo auxiliar.

##### Cierre
Por último si no pudimos hacer alguna de las operaciones anteriores, vamos
al siguiente nodo.
```c

	if(!posicionado)
		nodo_actual = nodo_actual->siguiente;
	}
}
	return true;
```
##### Complejidad

Es complejidad O(n) porque, en el peor de los casos, se recorre toda la lista.

#### pokedex_cantidad_pokemones: O(1)
Simplemente toma la cantidad almacenada
##### Complejidad
Es O(1), dado que, en cualquier caso, sólo debe leer una variable.

#### pokedex_buscar_pokemon: O(n)

##### Complejidad
Es O(n), pues necesita iterar hasta encontrar al pokemon.

#### pokedex_iterar_pokemones: O(n)

##### Complejidad
Es O(n). Si una función no frena el bucle, se iteran todos los pokemones.
No obstante, hay que tener en cuenta que su complejidad es variable, porque depende
del usuario, es decir, si este involucra una función O(n^2) para cada pokemon, la
complejidad resultaría en O(n^3).

#### pokedex_destruir: O(n)


##### Complejidad
Es O(n), porque debe pasar por todos los nodos para liberarlos.

#### ¿Por qué se eligió esta estructura?
Dado que no se trata de un arreglo dinámico, sino que cada nodo apunta a su siguiente,
sólo hacemos el pedido de memoria para insertar un nuevo nodo, por ende,
*1)* el sistema operativo no debe buscar un trozo de memoria contigua sin utilizar
para añadir nuevos pokemones.
Por otro lado,  *2)* el orenamiento no requiere de hacer desplazamientos,
es decir, si tuviéramos un arreglo dinámico y quisiéramos insertar un elemento _e_ en _x_
posición (exceptuando el final), deberíamos desplazar el resto de
los elementos delante de _e_ _i_+1 posiciones, donde _i_ es la posición actual de cada
elemento.

#### Diagrama de la memoria de pokedex
<img src="img/heap_stack_pokedex.drawio.svg" alt="heap y stack de la pokedex" width="100%"/>

### CSV
#### abrir_archivo_csv: O(1)
##### Complejidad
Dado que siempre tiene que realizar la misma operación, sin importar los parámetros
que reciba, deducimos que la complejidad es O(1)
#### leer_linea_csv: O(n)
Para leer una linea, debemos tener un vector de lecturas que nos permita
almacenar los valores leídos.
```c
char **lecturas = calloc(columnas, sizeof(char *));
for (int i = 0; i < columnas; i++)
	lecturas[i] = calloc(100, sizeof(char));
```
También, desde luego, unos string que nos permitan separar columna por columna.
```c
char parser[10] = "%[^;];";
parser[3] = archivo->separador;
parser[5] = archivo->separador;
char parser_nl[10] = "%[^\n]\n";
```
_parser_ lee todo hasta que llegue al separador (';' por defecto) y _parser_nl_
lee hasta un '\n'
##### Ciclo de lectura
```c
while (i < columnas) {
	if (i == columnas - 1)
		cols_leidas +=
			fscanf(archivo->file, parser_nl, lecturas[i]);
	else {
		cols_leidas +=
			fscanf(archivo->file, parser, lecturas[i]);
	}
	if (cols_leidas != EOF && funciones[i] != NULL) {
		resultado = funciones[i](lecturas[i], ctx[i]);
		if (resultado)
			cols_exitosamente_leidas++;
	}
	i++;
}
```
Leemos de _archivo->file_ la primera columna con _parser_. Lo siguiente
es corroborar que cols_leidas sea distinto de EOF (-1) y que la función pasada
por el usuario no sea NULL. Si el resultado de la función fue positivo, entonces
seguimos iterando, sino, cortamos en esa columna.

Hay que tener en cuenta que se cuenta una _lectura exitosa_ si en el bucle se
pudo leer el string y se le pudo aplicar la función.
Finalmente, se libera la memoria, se corrobora también que se hayan leído todas
las columnas, caso contrario, se la función devuelve cero. Este caso probablemente
se trate de EOF (End Of File).

##### Complejidad
Es O(n) porque que debe iterar columna por columna para aplicar las funciones del usuario.
Aunque, al igual que con *pokedex_iterar_pokemones*, debe tenerse en cuenta la
complejidad extra que añada el usuario.

#### cerrar_archivo_csv: O(1)
Cierra el archivo
##### Complejidad
Dado que siempre tiene que realizar la misma operación, sin importar los parámetros
que reciba, deducimos que la complejidad es O(1). 

#### Diagrama de la memoria del manejo de archivos
<img src="img/heap_stack_csv.drawio.svg" alt="heap y stack de archivos" width="80%"/>

TODO:
- hacer dibujos

