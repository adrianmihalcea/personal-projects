// @Copyright 2021 Mihalcea Adrian - 334CA

#include "helpers.h"

int GetGenreInt(char genre) {
    switch (genre) {
    case 'h':
        return HORROR;
    case 'c':
        return COMEDY;
    case 'f':
        return FANTASY;
    case 's':
        return SF;
    default:
        return NONE;
    }
}
