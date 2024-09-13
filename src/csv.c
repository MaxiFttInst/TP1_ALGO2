#include "csv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct archivo_csv {
	FILE *file;
	char separador;
};

struct archivo_csv *abrir_archivo_csv(const char *nombre_archivo,
				      char separador)
{
	struct archivo_csv *archivo = calloc(1, sizeof(struct archivo_csv));

	archivo->file = fopen(nombre_archivo, "r");
	archivo->separador = separador;

	if (archivo->file == NULL) {
		free(archivo);
		archivo = NULL;
	}
	return archivo;
}

size_t leer_linea_csv(struct archivo_csv *archivo, size_t columnas,
		      bool (*funciones[])(const char *, void *), void *ctx[])
{
	if (archivo == NULL)
		return 0;

	int cols_leidas = 0;
	int cols_exitosamente_leidas = 0;
	bool resultado = true;

	char **lecturas = calloc(columnas, sizeof(char *));
	for (int i = 0; i < columnas; i++)
		lecturas[i] = calloc(100, sizeof(char));
	char parser[10] = "%[^;];";
	parser[3] = archivo->separador;
	parser[5] = archivo->separador;
	char parser_nl[10] = "%[^\n]\n";
	size_t i = 0;
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
	for (int i = 0; i < columnas; i++)
		free(lecturas[i]);
	free(lecturas);
	if (cols_leidas != columnas)
		cols_exitosamente_leidas = 0;

	return (size_t)cols_exitosamente_leidas;
}

void cerrar_archivo_csv(struct archivo_csv *archivo)
{
	if (archivo == NULL)
		return;

	fclose(archivo->file);
	free(archivo);
}

// archivo_csv
//	- separador
//	- FILE
//
//
