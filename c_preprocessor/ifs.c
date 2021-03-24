/*
 * @Copyright 2021 Mihalcea Adrian 334CA
 * Tema 1 SO
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashmap.h"
#include "defines.h"
#include "ifs.h"
#include "process.h"

/*
 * Returns truth value of condition/MEM_ERROR
 */
int evaluate(char *line, struct hashmap *defines, enum chunk_types type)
{
	int skip, cond_len, numeral, ok;
	char *cond, *value;

	value = 0;

	if (type == IF)
		skip = strlen("#if ");
	else
		skip = strlen("#elif ");
	cond_len = strcspn(line + skip, separators);
	cond = calloc(cond_len + 1, 1);
	if (cond == 0)
		return MEM_ERROR;
	strncpy(cond, line + skip, cond_len);
	numeral = atoi(cond);
	if (numeral) {
		free(cond);
		return 1;
	}
	ok = has_key(defines, cond, &value);
	free(cond);
	if (ok == MEM_ERROR)
		return MEM_ERROR;
	if (ok) {
		numeral = atoi(value);
		free(value);
		return (numeral > 0);
	}

	return 0;
}

/*
 * Returns truth value of condition/MEM_ERROR
 */
int evaluate_def(char *line, struct hashmap *defines, enum chunk_types type)
{
	int skip, cond_len;
	char *cond, *value;

	value = 0;

	if (type == IFDEF)
		skip = strlen("#ifdef ");
	else
		skip = strlen("#ifndef ");
	cond_len = strcspn(line + skip, separators);
	cond = calloc(cond_len + 1, 1);
	if (cond == 0)
		return MEM_ERROR;
	strncpy(cond, line + skip, cond_len);
	skip = has_key(defines, cond, &value);
	free(cond);
	if (skip == MEM_ERROR)
		return MEM_ERROR;
	if (skip) {
		free(value);
		return type == IFDEF;
	}

	return type == IFNDEF;
}

/* finds each line's type */
enum chunk_types line_type(char *line)
{
	int len, tkn_len, i, ret;
	char tkn[100];

	len = strlen(line); i = 0; ret = CONTENT;

	while (i < len && line[i] == ' ')
		++i;
	tkn_len = strcspn(line + i, separators);
	strncpy(tkn, line + i, tkn_len);
	tkn[tkn_len] = 0;
	if (strcmp(tkn, "#if") == 0)
		ret = IF;
	if (strcmp(tkn, "#elif") == 0)
		ret = ELIF;
	if (strcmp(tkn, "#else") == 0)
		ret = ELSE;
	if (strcmp(tkn, "#endif") == 0)
		ret = ENDIF;
	if (strcmp(tkn, "#ifdef") == 0)
		ret = IFDEF;
	if (strcmp(tkn, "#ifndef") == 0)
		ret = IFNDEF;

	return ret;
}

/*
 * Processes if statement, returns 0/BAD_IF
 */
int process_ifs(char **data, int line, struct hashmap *defines)
{
	int i, ok, write, found;
	char *ln;
	enum chunk_types type;

	i = line; write = 0; found = 0;
	while (1) {
		ln = data[i];
		type = line_type(ln);
		if (type == ENDIF) {
			ln[0] = 0;
			break;
		}
		if (write) {
			if (type != CONTENT) {
				write = 0;
				ln[0] = 0;
			}
			++i;
			continue;
		}
		if (!found) {
			if (type == ELSE) {
				ln[0] = 0;
				write = 1;
				found = 1;
				++i;
				continue;
			}
			if (type == IF || type == ELIF) {
				ok = evaluate(ln, defines, type);
				if (ok == MEM_ERROR)
					return MEM_ERROR;
				if (ok == 1) {
					ln[0] = 0;
					write = 1;
					found = 1;
					++i;
					continue;
				}
			}
		}
		ln[0] = 0;
		++i;
	}

	return 0;
}

/*
 * Processes ifdef statement, returns 0/BAD_IF
 */
int process_ifdefs(char **data, int line, struct hashmap *defines)
{
	int i, ok, write, found;
	char *ln;
	enum chunk_types type;

	i = line;
	write = 0;
	found = 0;
	while (1) {
		ln = data[i];
		type = line_type(ln);
		if (type == ENDIF) {
			ln[0] = 0;
			break;
		}
		if (write) {
			if (type == IF || type == IFDEF || type == IFNDEF) {
				while (line_type(data[i]) != ENDIF)
					++i;
				++i;
				continue;
			}
			if (type != CONTENT) {
				write = 0;
				ln[0] = 0;
			}
			++i;
			continue;
		}
		if (!found) {
			if (type == ELSE) {
				ln[0] = 0;
				write = 1;
				found = 1;
				++i;
				continue;
			}
			if (type == IFDEF || type == IFNDEF) {
				ok = evaluate_def(ln, defines, type);
				if (ok == MEM_ERROR)
					return MEM_ERROR;
				if (ok == 1) {
					ln[0] = 0;
					write = 1;
					found = 1;
					++i;
					continue;
				}
			}
		}
		ln[0] = 0;
		++i;
	}

	return 0;
}
