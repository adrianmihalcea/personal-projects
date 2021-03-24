/*
 * @Copyright 2021 Mihalcea Adrian 334CA
 * Tema 1 SO
 */

#pragma once

#define BUCKETS_NO 100
#define BUCKET_CHUNK 10

struct hashmap;

int new_map(struct hashmap **map);

void delete_map(struct hashmap *map);

int has_key(struct hashmap *map, char *key, char **value);

int put_key(struct hashmap *map, char *key, char *value);

int remove_key(struct hashmap *map, char *key);

int hash_key(char *key);

void print_map(struct hashmap *map);
