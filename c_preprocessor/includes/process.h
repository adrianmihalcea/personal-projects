/*
 * @Copyright 2021 Mihalcea Adrian 334CA
 * Tema 1 SO
 */

#include "hashmap.h"
#include "includes.h"

#define separators "\t []{}<>=+-*/%!&|^.,:;()\\\n\r"

int process(char *infile, char ***data, int *lines, struct hashmap *defines,
struct includes *inc, char *w_dir);

int load_file(char *infile, char ***data, int *lines, struct includes *inc,
char *w_dir);

int replace_mapping(char **line, int start_pos, int tkn_len, char *value,
int *new_pos);

int process_include(struct includes *inc, char **line, int pos,
struct hashmap *defines, char *w_dir);

int process_define(char **data, int cur_line, struct hashmap *defines);

int process_directive(char **data, int start_pos, int i,
struct hashmap *defines, struct includes *inc, char *w_dir);
