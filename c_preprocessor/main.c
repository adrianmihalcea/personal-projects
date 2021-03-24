/*
 * @Copyright 2021 Mihalcea Adrian 334CA
 * Tema 1 SO
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "hashmap.h"
#include "includes.h"
#include "process.h"

/*
 * Returns 0 for succes, 1 for failure, MEM_ERROR
 */
int add_define(struct hashmap *defines, char *command)
{
	int len, i = 0, ok;
	char *tmp;

	len = strlen(command);
	if (len == 0)
		return 1;
	while (i < len) {
		if (command[i] == '=')
			break;
		++i;
	}

	/* "Empty" define */
	if (i == len) {
		ok = put_key(defines, command, "\0");
	} else {
		tmp = malloc(i + 1);
		if (tmp == 0)
			return MEM_ERROR;
		memcpy(tmp, command, i);
		tmp[i] = 0;
		ok = put_key(defines, tmp, command + i + 1);
		if (ok == MEM_ERROR)
			return MEM_ERROR;
		free(tmp);
	}

	return 0;
}

/*
 * Returns 1 for bad arguments, 0 for successful read
 */
int parse_arguments(struct hashmap *defines, char **infile, char **outfile,
int argc, char **argv, struct includes *inc)
{
	int i = 1, len, len2, ok, args_empty = 0;

	while (i < argc) {
		len = strlen(argv[i]) + 1;
		if (argv[i][0] != '-') {
			if (args_empty == 2)
				return ARG_ERROR;
			if (args_empty == 0) {
				*infile = malloc(len);
				if (*infile == 0)
					return MEM_ERROR;
				memcpy(*infile, argv[i], len);
				++i;
				++args_empty;
				continue;
			} else {
				*outfile = malloc(len);
				if (*outfile == 0)
					return MEM_ERROR;
				memcpy(*outfile, argv[i], len);
				++i;
				++args_empty;
				continue;
			}
		}

		switch (argv[i][1]) {
		case 'D':
			/* the mapping is in the next argument */
			if (len == 3) {
				ok = add_define(defines, argv[i + 1]);
				if (ok == MEM_ERROR)
					return MEM_ERROR;
				i += 2;
			} else {
				ok = add_define(defines, argv[i] + 2);
				if (ok == MEM_ERROR)
					return MEM_ERROR;
				i += 1;
			}
			if (ok != 0)
				return ok;
			continue;
		case 'I':
			ok = add_path(inc, argv[i + 1]);
			if (ok != 0)
				return ok;
			i += 2;
			continue;
		case 'o':
			/* argumentul este urmatorul */
			if (len == 3) {
				len2 = strlen(argv[i + 1]) + 1;
				*outfile = malloc(len2);
				if (*outfile == 0)
					return MEM_ERROR;
				memcpy(*outfile, argv[i + 1], len2);
				i += 2;
			} else {
				*outfile = malloc(len - 2);
				if (*outfile == 0)
					return MEM_ERROR;
				memcpy(*outfile, argv[i] + 2, len - 2);
				i += 1;
			}
			continue;
		default:
			return ARG_ERROR;
		}
	}

	return 0;
}

/*
 * Frees all dynamic memory
 */
void deallocate_memory(struct hashmap *defines, char *infile, char *outfile,
struct includes *inc, char **data, int lines, char *w_dir)
{
	int i;

	delete_map(defines);
	delete_includes(inc);
	free(infile);
	free(outfile);
	free(w_dir);

	for (i = 0; i < lines; ++i)
		free(data[i]);
	free(data);
}

/*
 * Attempts to write data to outfile, returns 0/IO_ERROR
 */
int write_outfile(char *outfile, char **data, int lines)
{
	int i;
	FILE *fp = 0;

	if (outfile)
		fp = fopen(outfile, "w");
	else
		fp = stdout;
	if (fp == 0)
		return IO_ERROR;

	for (i = 0; i < lines; ++i)
		fprintf(fp, "%s", data[i]);

	fclose(fp);
	return 0;
}

int main(int argc, char **argv)
{
	struct hashmap *defines;
	struct includes *inc;
	char *infile = 0, *outfile = 0, **data, *w_dir;
	int lines, ok, len;

	lines = 0; w_dir = 0;

	ok = new_map(&defines);
	if (ok == MEM_ERROR)
		return MEM_ERROR;
	ok = new_includes(&inc);
	if (ok == MEM_ERROR)
		return MEM_ERROR;
	ok = parse_arguments(defines, &infile, &outfile, argc, argv, inc);
	if (ok == MEM_ERROR)
		return MEM_ERROR;
	if (ok != 0)
		return 5;
	if (infile) {
		len = strlen(infile);
		w_dir = malloc(len + 1);
		if (w_dir == 0)
			return MEM_ERROR;
		strcpy(w_dir, infile);
		while (len > 0 && w_dir[len - 1] != '/')
			--len;
		w_dir[len] = 0;
	}
	ok = process(infile, &data, &lines, defines, inc, w_dir);
	if (ok != 0)
		return ok;

	ok = write_outfile(outfile, data, lines);
	if (ok != 0)
		return ok;

	deallocate_memory(defines, infile, outfile, inc, data, lines, w_dir);
	return 0;
}
