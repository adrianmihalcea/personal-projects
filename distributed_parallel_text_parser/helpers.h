// @Copyright 2021 Mihalcea Adrian - 334CA

#pragma once

#include <string>

enum Genres {
    NONE, HORROR, COMEDY, FANTASY, SF
};

int GetGenreInt(char genre);

const int kNoGenres = 4;
const int kMaster = 0;
const int kLinesPerThread = 20;
const std::string kHeader[] = {"nada\n", "horror\n", "comedy\n", "fantasy\n", "science-fiction\n"};
