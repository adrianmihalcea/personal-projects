/*
 * @Copyright 2021 Mihalcea Adrian 334CA
 * Tema 1 SO
 */

#pragma once

#include "hashmap.h"

enum chunk_types {
	CONTENT, IF, ELIF, ELSE, ENDIF, IFDEF, IFNDEF
};

int process_ifs(char **data, int line, struct hashmap *defines);

int process_ifdefs(char **data, int line, struct hashmap *defines);

enum chunk_types line_type(char *line);

int evaluate(char *line, struct hashmap *defines, enum chunk_types type);

int evaluate_def(char *line, struct hashmap *defines, enum chunk_types type);
