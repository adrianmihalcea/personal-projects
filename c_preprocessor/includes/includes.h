/*
 * @Copyright 2021 Mihalcea Adrian 334CA
 * Tema 1 SO
 */

#pragma once

#define INCLUDE_CHUNK 10

struct includes;

int new_includes(struct includes **inc);

void delete_includes(struct includes *inc);

int add_path(struct includes *inc, char *to_add);

int open_file(struct includes *inc, char *to_open, FILE **fp, char *w_dir);

void print_includes(struct includes *inc);
