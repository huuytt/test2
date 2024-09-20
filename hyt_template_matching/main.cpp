#include<iostream>
#include"data.h"
#include"match.h"
#include"match_accelerated.h"


int main() {
	int ansx, ansy;

	if (Match(cImageTarget,cImageTemplate, ansx,ansy)) {
		std::cout <<"利用最基础的模板匹配算法得到的x和y坐标为: " <<ansx << " " << ansy << std::endl;
	}

	if (Match_accelerated(cImageTarget, cImageTemplate, ansx, ansy)) {
		std::cout << "利用加速的模板匹配算法得到的x和y坐标为: " << ansx << " " << ansy << std::endl;
	}


}