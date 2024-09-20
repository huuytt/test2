#include<iostream>
#include"constants.h"





bool Match(unsigned char Target[256][256], unsigned char Template[64][64], int& x, int& y) {


	int bestScore = INT_MAX;
	for (int i = 0; i <= 256 - 64; i++) {
		for (int j = 0; j < 256 - 64; j++) {

			int r = 0;
			long long  st = 0;
			long long s2 = 0;
			long long t2 = 0;
			for (int m = 0; m < 54; m++) {
				for (int n = 0; n < 64; n++) {
					st += static_cast<long long>(Target[i + m][j + n]) * static_cast<long long>(Template[m][n]);
					s2 += static_cast<long long>(Target[i + m][j + n]) * Target[i + m][j + n];
					t2 += static_cast<long long>(Template[m][n]) * Template[m][n];
				}
			}
			r = st / std::sqrt(static_cast<long long>(s2 * t2));

			if (std::abs(r - 1) < bestScore) {
				bestScore = std::abs(r - 1);
				x = j;
				y = i;
			}


		}
	}

	if (bestScore < 0.1) {
		return true;
	}
	else {
		return false;
	}


}