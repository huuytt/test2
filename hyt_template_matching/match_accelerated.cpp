#include <vector>

#include "constants.h"
#include "fast_match.h"

bool Match_accelerated(unsigned char s[256][256],unsigned char t[64][64], int& retX, int& retY) {
    Image vs(256, 256);
    Image vt(64, 64);
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            vs[i][j] = s[i][j];
        }
    }
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            vt[i][j] = t[i][j];
        }
    }
    auto result = fastMatch(vs, vt);
    fprintf(stderr, "Score=%f\n", result.score);
    if (result.score > 0.9) {
        retX = result.x;
        retY = result.y;
        return true;
    }
    else {
        return false;
    }
}