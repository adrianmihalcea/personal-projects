/*
 * @Copyright 2021 Mihalcea Adrian 334CA
 * Tema 1 SO
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "hashmap.h"

/*
 * A struct for hashmap containers
 */
struct bucket {
	int size;
	int allocated;
	char **keys;
	char **values;
};

/*
 * Struct for a <char*, char*> hashmap
 */
struct hashmap {
	struct bucket *buckets;
};

/*
 * Allocates memory for a new hashmap and returns a pointer to it
 * Returns 0 for successful allocation, 12 for memory error
 */
int new_map(struct hashmap **map)
{
	int i;

	*map = malloc(sizeof(struct hashmap));
	if (*map == 0)
		return MEM_ERROR;
	(*map)->buckets = malloc(BUCKETS_NO * sizeof(struct bucket));
	if ((*map)->buckets == 0)
		return MEM_ERROR;
	for (i = 0; i < BUCKETS_NO; ++i) {
		(*map)->buckets[i].size = 0;
		(*map)->buckets[i].allocated = 0;
	}

	return 0;
}

/*
 * Deallocates all the map's memory
 */
void delete_map(struct hashmap *map)
{
	int i, j;
	struct bucket *curr_bkt;

	for (i = 0; i < BUCKETS_NO; ++i) {
		curr_bkt = &map->buckets[i];
		for (j = 0; j < curr_bkt->size; ++j) {
			free(curr_bkt->keys[j]);
			free(curr_bkt->values[j]);
		}
		if (curr_bkt->size != 0) {
			free(curr_bkt->keys);
			free(curr_bkt->values);
		}
	}
	free(map->buckets);
	free(map);
}

/*
 * Returns 0/1 if key not found / found. * Returns 12 for memory error.
 * Value is placed in value pointer.
 * If found, allocates memory for value (needs to be freed)
 */
int has_key(struct hashmap *map, char *key, char **value)
{
	int i, bucket_index = hash_key(key) % BUCKETS_NO;
	struct bucket *bkt;

	bkt = &map->buckets[bucket_index];

	if (bkt->size == 0)
		return 0;
	for (i = 0; i < bkt->size; ++i) {
		if (strcmp(key, bkt->keys[i]) != 0)
			continue;
		*value = malloc((strlen(bkt->values[i]) + 1) * sizeof(char));
		if (*value == 0)
			return MEM_ERROR;
		memcpy(*value, bkt->values[i], strlen(bkt->values[i]) + 1);
		return 1;
	}

	return 0;
}

/*
 * Inserts key and returns: 0 - already present key, 1 - successfull insertion,
 * MEM_ERROR - memory error.
 */
int put_key(struct hashmap *map, char *key, char *value)
{
	int has, bucket_index, key_len, value_len;
	char *tmp;
	struct bucket *bkt;

	has = has_key(map, key, &tmp);
	if (has) {
		free(tmp);
		return 0;
	}
	bucket_index = hash_key(key) % BUCKETS_NO;
	bkt = &map->buckets[bucket_index];

	if (bkt->size == 0) {
		bkt->keys = malloc(BUCKET_CHUNK * sizeof(char *));
		if (bkt->keys == 0)
			return MEM_ERROR;
		bkt->values = malloc(BUCKET_CHUNK * sizeof(char *));
		if (bkt->values == 0)
			return MEM_ERROR;
		bkt->allocated = BUCKET_CHUNK;
	} else if (bkt->size == bkt->allocated) {
		bkt->keys = realloc(bkt->keys, (BUCKET_CHUNK * bkt->allocated)
			* sizeof(char *));
		if (bkt->keys == 0)
			return MEM_ERROR;
		bkt->values = realloc(bkt->values,
			(BUCKET_CHUNK * bkt->allocated) * sizeof(char *));
		if (bkt->values == 0)
			return MEM_ERROR;
		bkt->allocated *= BUCKET_CHUNK;
	}

	key_len = strlen(key);
	value_len = strlen(value);
	bkt->keys[bkt->size] = malloc((key_len + 1));
	if (bkt->keys[bkt->size] == 0)
		return MEM_ERROR;
	bkt->values[bkt->size] = malloc((value_len + 1));
	if (bkt->values[bkt->size] == 0)
		return MEM_ERROR;
	memcpy(bkt->keys[bkt->size], key, key_len);
	memcpy(bkt->values[bkt->size], value, value_len);
	bkt->keys[bkt->size][key_len] = 0;
	bkt->values[bkt->size][value_len] = 0;
	++(bkt->size);

	return 1;
}

/*
 * Removes the key, value pair (if found) and rearranges bucket,
 * returning 1/0/MEM_ERROR for succes/not_found/MEM_ERROR
 */
int remove_key(struct hashmap *map, char *key)
{
	int ok, hash, i = 0;
	char *tmp;
	struct bucket *bkt;

	ok = has_key(map, key, &tmp);
	if (ok != 1)
		return ok;
	free(tmp);

	hash = hash_key(key) % BUCKETS_NO;
	bkt = &map->buckets[hash];
	while (i < bkt->size && strcmp(bkt->keys[i], key) != 0)
		++i;
	while (i < bkt->size - 1) {
		bkt->keys[i] = bkt->keys[i - 1];
		bkt->values[i] = bkt->values[i - 1];
		++i;
	}
	if (i == bkt->size - 1) {
		free(bkt->keys[i]);
		free(bkt->values[i]);
	}
	--(bkt->size);
	if (bkt->size == 0) {
		free(bkt->keys);
		free(bkt->values);
	}

	return 0;
}

/*
 * A hash function for strings
 */
int hash_key(char *key)
{
	int hash = 7;
	unsigned int i;

	for (i = 0; i < strlen(key); i++)
		hash = (hash * 31 + key[i]) % 997;

	return hash;
}

/*
 * Prints the hashmap to stdout
 */
void print_map(struct hashmap *map)
{
	int i, j;
	struct bucket *bkt;

	printf("Map:\n");
	for (i = 0; i < BUCKETS_NO; ++i) {
		bkt = &map->buckets[i];
		for (j = 0; j < bkt->size; ++j)
			printf("%s: %s\n", bkt->keys[j], bkt->values[j]);
	}
}
