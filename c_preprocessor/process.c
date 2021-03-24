/*
 * @Copyright 2021 Mihalcea Adrian 334CA
 * Tema 1 SO
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hashmap.h"
#include "defines.h"
#include "ifs.h"
#include "includes.h"
#include "process.h"

#define LINE_ALLOCATE 100
#define LINE_LENGTH 200

/*
 * Function to pre-process a file (will be used recursively).
 * Returns 0/1/MEM_ERROR for success/fail/MEM_ERROR
 */
int process(char *infile, char ***data, int *lines, struct hashmap *defines,
struct includes *inc, char *w_dir)
{
	int ok, i, start_pos, len, tkn_len;
	char **line, tkn[100], *value;

	ok = load_file(infile, data, lines, inc, w_dir);
	if (ok != 0)
		return ok;

	for (i = 0; i < *lines; ++i) {
		start_pos = 0;
		line = &(*data)[i];
		len = strlen(*line);

		/* skip all white spaces */
		while (start_pos < len && (*line)[start_pos] == ' ')
			++start_pos;
		/* skip empty lines */
		if (start_pos == len)
			continue;

		if ((*line)[0] == '#') {
			ok = process_directive(*data, start_pos, i, defines,
			inc, w_dir);
			if (ok != 0)
				return ok;
			--i;
			continue;
		}

		/* go over each token and replace if necessary */
		while (start_pos < len) {
			tkn_len = strcspn(*line + start_pos, separators);
			strncpy(tkn, *line + start_pos, tkn_len);
			tkn[tkn_len] = 0;
			value = 0;
			ok = has_key(defines, tkn, &value);
			if (ok > 1)
				return ok;
			if (ok) {
				/* poate trebuie sa realoc */
				ok = replace_mapping(line, start_pos, tkn_len,
				value,
					&start_pos);
				if (ok != 0)
					return ok;
				free(value);
				--i;
				continue;
			}
			start_pos += tkn_len + 1;
		}
	}

	return 0;
}

/*
 * Processes a pre-processor directive and 0/MEM_ERROR/IO_ERROR
 */
int process_directive(char **data, int start_pos, int i,
struct hashmap *defines, struct includes *inc, char *w_dir)
{
	int tkn_length, ok, len_key;
	char token[200], tmp2[100];

	tkn_length = strcspn(data[i] + start_pos, separators);
	strncpy(token, data[i] + start_pos, tkn_length);
	token[tkn_length] = 0;

	if (strcmp(token, "#define") == 0)
		return process_define(data, i, defines);

	if (strcmp(token, "#include") == 0) {
		ok = process_include(inc, &data[i], start_pos
		+ strlen("#include \""),
		defines, w_dir);
		if (ok != 0)
			return 12;
		return ok;
	}

	if (strcmp(token, "#undef") == 0) {
		len_key = strcspn(data[i] + start_pos + strlen("#undef "),
		separators);
		strncpy(tmp2, data[i] + start_pos + strlen("#undef "),
		len_key);
		tmp2[len_key] = 0;
		ok = remove_key(defines, tmp2);
		if (ok > 1)
			return ok;
		data[i][0] = 0;
		return 0;
	}

	if (strcmp(token, "#if") == 0)
		return process_ifs(data, i, defines);

	if (strcmp(token, "#ifdef") == 0 || strcmp(token, "#ifndef") == 0)
		return process_ifdefs(data, i, defines);

	return ARG_ERROR;
}

/*
 * Processes a define directive, returns 0/MEM_ERROR
 */
int process_define(char **data, int cur_line, struct hashmap *defines)
{
	int line_pos, key_pos, value_pos, i, tkn_len, ok;
	char key[100], value[100], *start, other[100], token[100], *tmp;

	line_pos = strlen("#define ");
	key_pos = 0; value_pos = 0; i = 0;
	start = data[cur_line];

	while (start[line_pos] != '\r' && start[line_pos] != '\n'
	&& start[line_pos] != ' ')
		key[key_pos++] = start[line_pos++];
	key[key_pos] = 0;
	value[0] = 0;
	/* empty define */
	if (start[line_pos] == '\r' || start[line_pos] == '\n') {
		value[0] = 0;
		ok = put_key(defines, key, value);
		if (ok > 1)
			return ok;
		start[0] = 0;
		return 0;
	}
	++line_pos;

	while (data[cur_line][line_pos] != '\r'
	&& data[cur_line][line_pos] != '\n') {
		if ((data[cur_line][line_pos] == '\\')
		&& (data[cur_line][line_pos + 1] == '\n'
		|| (data[cur_line][line_pos + 1] == '\r'
		&& data[cur_line][line_pos + 2] == '\n'))) {
			data[cur_line++][0] = 0;
			line_pos = 0;
			continue;
		}
		value[value_pos++] = data[cur_line][line_pos++];
	}
	value[value_pos] = 0;
	data[cur_line][0] = 0;

	/* tokenize value to replace defines */
	while (i < value_pos) {
		tkn_len = strcspn(value + i, separators);
		if (tkn_len == 0) {
			++i;
			continue;
		}
		strncpy(token, value + i, tkn_len);
		token[tkn_len] = 0;
		tmp = 0;
		ok = has_key(defines, token, &tmp);
		if (ok > 1)
			return ok;
		if (ok) {
			strcpy(other, value + i + tkn_len);
			value[i] = 0;
			strcat(value, tmp);
			strcat(value, other);
			free(tmp);
		}
		i += tkn_len + 1;
	}

	ok = put_key(defines, key, value);
	if (ok > 1)
		return ok;

	return 0;
}

/*
 * Processes an include directive, returns 0/ARG_ERROR/IO_ERROR.
 * Assumes final line of .h is an empty line
 */
int process_include(struct includes *inc, char **line, int pos,
struct hashmap *defines, char *w_dir)
{
	char tmp[100];
	int len, i, ok;
	char **data;
	int lines;

	i = 0; lines = 0;

	strcpy(tmp, *line + pos);
	len = strlen(tmp);
	while (i < len && tmp[i] != '\"')
		++i;
	if (i == len)
		return ARG_ERROR;

	tmp[i] = 0;
	/*printf("I will include: %s;\n", tmp);*/
	ok = process(tmp, &data, &lines, defines, inc, w_dir);
	if (ok != 0)
		return ok;

	free(*line);
	*line = calloc(lines * LINE_ALLOCATE, 1);
	if (*line == 0)
		return MEM_ERROR;
	for (i = 0; i < lines; ++i) {
		strcat(*line, data[i]);
		free(data[i]);
	}
	free(data);

	return 0;
}

/*
 * Replaces a known define, returns 0/MEM_ERROR
 */
int replace_mapping(char **line, int start_pos, int tkn_len, char *value,
int *new_pos)
{
	char other[100];
	int line_len, value_len;

	line_len = strlen(*line);
	value_len = strlen(value);
	*line = realloc(*line, line_len + value_len + 1);
	if (*line == 0)
		return MEM_ERROR;
	strcpy(other, *line + start_pos + tkn_len);
	(*line)[start_pos] = 0;
	strcat(*line, value);
	strcat(*line, other);

	*new_pos = start_pos + tkn_len + 1;
	return 0;
}

/* Function to load infile source into ram line by line
 * Returns 0/1/IO_ERROR/MEM_ERROR for success/fail/IO_ERROR/MEM_ERROR
 */
int load_file(char *infile, char ***data, int *lines, struct includes *inc,
char *w_dir)
{
	int ok, len;
	FILE *fp = 0;
	char line[LINE_LENGTH], **cur_line;

	if (infile) {
		ok = open_file(inc, infile, &fp, w_dir);
		if (ok != 0)
			return ok;
	} else {
		fp = stdin;
	}
	if (fp == 0)
		return IO_ERROR;

	*data = malloc(LINE_ALLOCATE * sizeof(char *));
	if (*data == 0)
		return MEM_ERROR;
	while (1) {
		if (fgets(line, LINE_LENGTH, fp) == NULL)
			break;
		if (*lines % LINE_ALLOCATE == LINE_ALLOCATE - 1) {
			*data = realloc(*data, (*lines + 1 + LINE_ALLOCATE)
			* sizeof(char *));
			if (*data == 0)
				return MEM_ERROR;
		}
		len = strlen(line);
		cur_line = &(*data)[*lines];
		*cur_line = malloc(len + 1);
		if (*cur_line == 0)
			return MEM_ERROR;
		memcpy(*cur_line, line, len);
		(*cur_line)[len] = 0;
		++(*lines);
	}
	fclose(fp);
	return 0;
}
