/*
 * @Copyright 2021 Mihalcea Adrian 334CA
 * Tema 1 SO
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "defines.h"
#include "includes.h"

/*
 * Struct for holding include_paths
 */
struct includes {
	char **paths;
	int size;
};

/*
 * Creates a new includes struct, retuning 0/MEM_ERROR
 */
int new_includes(struct includes **inc)
{
	*inc = malloc(sizeof(struct includes));
	if (*inc == 0)
		return MEM_ERROR;
	(*inc)->paths = malloc(INCLUDE_CHUNK * sizeof(char *));
	if ((*inc)->paths == 0)
		return MEM_ERROR;
	(*inc)->size = 0;

	return 0;
}

/*
 * Frees memory used by an includes struct
 */
void delete_includes(struct includes *inc)
{
	int i;

	for (i = 0; i < inc->size; ++i)
		free(inc->paths[i]);
	free(inc->paths);
	free(inc);
}

/*
 * Adds a new path to the includes struct, returning 0/MEM_ERROR
 */
int add_path(struct includes *inc, char *to_add)
{
	char **to_write;
	int len, new_alloc;

	if (inc->size != 0 && inc->size % INCLUDE_CHUNK == 0) {
		new_alloc = inc->size + INCLUDE_CHUNK;
		inc->paths = realloc(inc->paths, new_alloc * sizeof(char *));
		if (inc->paths == 0)
			return MEM_ERROR;
	}


	to_write = &(inc->paths)[inc->size];
	len = strlen(to_add) + 1;
	*to_write = malloc(len);
	if (*to_write == 0)
		return MEM_ERROR;
	memcpy(*to_write, to_add, len - 1);
	(*to_write)[len - 1] = 0;
	++inc->size;

	return 0;
}

/*
 * Returns 0/IO_ERROR, if successful, opens file into *fp
 */
int open_file(struct includes *inc, char *to_open, FILE **fp, char *w_dir)
{
	int i;
	char tmp[500];

	*fp = fopen(to_open, "r");
	if (*fp != 0)
		return 0;

	/* Try working directory */
	strcpy(tmp, w_dir);
	strcat(tmp, to_open);
	*fp = fopen(tmp, "r");
	if (*fp != 0)
		return 0;

	/* Try all included paths */
	for (i = 0; i < inc->size; ++i) {
		strcpy(tmp, inc->paths[i]);
		strcat(tmp, "/");
		strcat(tmp, to_open);
		*fp = fopen(tmp, "r");
		if (*fp != 0)
			return 0;
	}

	return IO_ERROR;
}

/*
 * Prints the contents of the includes structure
 */
void print_includes(struct includes *inc)
{
	int i;

	printf("->Includes: (%d)\n", inc->size);
	for (i = 0; i < inc->size; ++i)
		printf("%s; ", inc->paths[i]);
	printf("\n");
}
