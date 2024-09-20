#include<algorithm>
#include<iostream>
#include<cmath>
#include<limits>
#include<vector>

#include "constants.h"
#include "image.hpp"


namespace Utils
{
	struct Complex {
		double real, imag;
		Complex() : Complex(0, 0) {}
		Complex(double real_, double imag_) :real(real_), imag(imag_) {}
		Complex operator+(const Complex& other) const { return Complex(real + other.real, imag + other.imag); }
		Complex operator-(const Complex& other) const { return Complex(real - other.real, imag - other.imag); }
		Complex operator*(const Complex& other) const {
			return Complex(real * other.real - imag * other.imag, real * other.imag + imag * other.real);
		}
		Complex& operator+=(const Complex& other) {
			real += other.real;
			imag += other.imag;
			return *this;
		}
		Complex& operator-=(const Complex& other) {
			real -= other.real;
			imag -= other.imag;
			return *this;
		}
		Complex& operator*=(const Complex& other) {
			double new_real = real * other.real - imag * other.imag;
			double new_imag = real * other.imag + imag * other.real;
			real = new_real;
			imag = new_imag;
			return *this;
		}
		Complex operator/=(const double other) {
			real /= other;
			imag /= other;
			return *this;
		}

	};

	void dft(std::vector<Complex>& a, const std::vector<int>& to, bool invert) {
		int n = a.size();

		for (int i = 0; i < n; i++) {
			if (i < to[i]) {
				std::swap(a[i], a[to[i]]);
			}
		}

		for (int len = 1; len < n; len <<= 1) {
			double ang = PI / len * (invert ? -1 : 1);
			Complex wlen(std::cos(ang), std::sin(ang));
			for (int i = 0; i < n; i += 2 * len) {
				Complex w(1, 0);
				for (int j = 0; j < len; j++) {
					Complex u = a[i + j];
					Complex v = a[i + j + len] * w;
					a[i + j] = u + v;
					a[i + j + len] = u - v;
					w *= wlen;
				}
			}
		}
		if (invert) {
			for (Complex& x : a) {
				x /= n;
			}
		}

	}


	std::vector<long long> fft(const std::vector<long long>& a, const std::vector<long long>& b) {
		std::vector<Complex> fa;
		int n = 1, k = 0;
		while (n < int(a.size() + b.size())) {
			n <<= 1;
			k++;
		}
		fa.resize(n);
		for (int i = 0; i < (int)a.size(); i++) {
			fa[i].real = a[i];
			fa[i].imag = b[i];
		}

		std::vector<int> to(n);
		for (int i = 0; i < n; i++) {
			to[i] = (to[i >> 1] >> 1) | ((i & 1) << (k - 1));
		}

		dft(fa, to, false);
		for (int i = 0; i < n; i++) {
			fa[i] *= fa[i];
		}
		dft(fa, to, true);

		std::vector<long long> result(n);
		for (int i = 0; i < n; i++) {
			result[i] = std::round(fa[i].imag / 2);
		}
		return result;
	}



}

using Utils::fft;


struct MatchResult {
	double score;
	int x, y;
};

MatchResult fastMatch(const Image& s, const Image& t) {
	const int S_HEIGHT = s.height;
	const int S_WIDTH = s.width;
	const int T_HEIGHT = t.height;
	const int T_WIDTH = t.width;

	
	if (T_HEIGHT > S_HEIGHT || T_WIDTH > S_WIDTH) {
		return { -std::numeric_limits<double>::infinity(), -1, -1 };
	}

	std::vector<long long>arrS(S_HEIGHT * S_WIDTH, 0);
	std::vector<long long>arrT(S_HEIGHT * S_WIDTH, 0);
	/*std::vector<long long> arrS2(S_HEIGHT * S_WIDTH, 0);*/
	//std::vector<long long> arrPrefix(S_HEIGHT * S_WIDTH, 0);
	std::vector<std::vector<long long>> arrPrefix(S_HEIGHT, std::vector<long long>(S_WIDTH, 0));

	long long sumT2 = 0;
	long long sumS2 = 0;
	for (int i = 0; i < S_HEIGHT; i++) {
		for (int j = 0; j < S_WIDTH; j++) {
			arrS[i * S_WIDTH + j] = s[i][j];
			/*arrS2[i * S_WIDTH + j] = static_cast<long long>(s[i][j]) * s[i][j];*/
			/*arrPrefix[i * S_WIDTH + j] = s[i][j];
			if (i > 0) arrPrefix[i * S_WIDTH + j] += arrPrefix[(i - 1) * S_WIDTH + j];
			if (j > 0)arrPrefix[i * S_WIDTH + j] += arrPrefix[i * S_WIDTH + j - 1];
			if (i > 0 && j > 0) arrPrefix[i * S_WIDTH + j] -= arrPrefix[(i - 1) * S_WIDTH + j - 1];*/

			arrPrefix[i][j] = static_cast<long long>(s[i][j]) * s[i][j];
			if (i > 0) arrPrefix[i][j] += arrPrefix[i - 1][j];
			if(j>0) arrPrefix[i][j]+= arrPrefix[i][j-1];
			if(i>0&&j>0) arrPrefix[i][j]-= arrPrefix[i-1][j-1];


		}
	}

	for (int i = 0; i < T_HEIGHT; i++) {
		for (int j = 0; j < T_WIDTH; j++) {
			arrT[i * S_WIDTH + j]= t[i][j];
			sumT2+= static_cast<long long>(t[i][j]) * t[i][j];

		}
	}



	std::reverse(arrT.begin(), arrT.end());
	auto stq = fft(arrS, arrT);
	const int resHeight = S_HEIGHT - T_HEIGHT + 1;
	const int resWidth = S_WIDTH - T_WIDTH + 1;


	std::vector<std::vector<double>> result(resHeight, std::vector<double>(resWidth, 0));

	for (int bx = 0; bx < resHeight; bx++) {
		for (int by = 0; by < resWidth; by++) {
			/*unsigned long long s1= arrPrefix[(bx + T_HEIGHT - 1) * S_WIDTH + by + T_WIDTH - 1] - arrPrefix[bx * S_WIDTH + by + T_WIDTH - 1] - arrPrefix[(bx + T_HEIGHT - 1) * S_WIDTH + by] + arrPrefix[bx * S_WIDTH + by];*/
			/*unsigned long long s2 = s1 * s1;*/
			unsigned long long s2 = arrPrefix[bx + T_HEIGHT - 1][by + T_WIDTH - 1];
			if (bx > 0) s2 -= arrPrefix[bx - 1][by + T_WIDTH - 1];
			if (by > 0) s2 -= arrPrefix[bx + T_HEIGHT - 1][by - 1];
			if(bx>0&&by>0) s2+=arrPrefix[bx-1][by-1];

			unsigned long long t2 = sumT2;
			long long st = stq[bx * S_WIDTH + by+arrT.size()-1];
			result[bx][by] = st / std::sqrt(static_cast<double>(s2 * t2));

		}
	}

	double bestScore= -std::numeric_limits<double>::infinity();
	int retX = -1, retY = -1;
	for (int bx = 0; bx < resHeight; bx++) {
		for (int by = 0; by < resWidth; by++) {
			double score = result[bx][by];
			if (score > bestScore) {
				bestScore = score;
				retX = bx;
				retY = by;
			}
		}
	}

	return { bestScore,retX,retY };



}