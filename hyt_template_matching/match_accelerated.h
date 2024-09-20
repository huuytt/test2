#ifndef MATCH_ACCELERATED_H
#define MATCH_ACCELERATED_H

#include <vector>

#include "image.hpp"


bool Match_accelerated(unsigned char s[256][256], unsigned char t[64][64], int& retX, int& retY);

#endif 
