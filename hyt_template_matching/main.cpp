#include<iostream>
#include"data.h"
#include"match.h"
#include"match_accelerated.h"


int main() {
	int ansx, ansy;

	if (Match(cImageTarget,cImageTemplate, ansx,ansy)) {
		std::cout <<"�����������ģ��ƥ���㷨�õ���x��y����Ϊ: " <<ansx << " " << ansy << std::endl;
	}

	if (Match_accelerated(cImageTarget, cImageTemplate, ansx, ansy)) {
		std::cout << "���ü��ٵ�ģ��ƥ���㷨�õ���x��y����Ϊ: " << ansx << " " << ansy << std::endl;
	}


}