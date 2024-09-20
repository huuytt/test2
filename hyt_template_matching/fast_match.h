#ifndef FASTMATCH_H
#define FASTMATCH_H

#include <limits>
#include <vector>

#include "image.hpp"


struct MatchResult {
    double score;
    int x, y;
};


MatchResult fastMatch(const Image& s, const Image& t);

#endif 
