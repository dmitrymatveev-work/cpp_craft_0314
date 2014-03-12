/*
 * 11.cpp
 *
 *  Created on: Mar 8, 2014
 *      Author: dell
 */
#include <iostream>
#include <string.h>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cmath>

//Задается точность числа
double changePrecision(const double number){
	int tmp=floor(number);
	int tmp1=10000*(number-tmp);

	return tmp+(double)tmp1/10000;
}

//Поиск кодов в паролях
void searchCodesInPass(const size_t sizeCodes, std::fstream &fileIn, std::fstream &fileOut){

	std::vector<double> codes;
	std::vector<double> pass;
	size_t i=0;
	double tmp;

	while(fileIn){
		fileIn>>tmp;
		if(i==0 && !fileIn) {std::cout<<"There is no 'code' data in input file"<<std::endl; exit(1);}
		else if(i==sizeCodes && !fileIn) {std::cout<<"There is no 'pass' data in input file"<<std::endl; exit(1);}
		tmp=changePrecision(tmp);
		if(i<sizeCodes) codes.push_back(tmp);
		else pass.push_back(tmp);
		i++;
	}

	std::vector<double>::iterator it;

	for(it=codes.begin(); it!=codes.end(); it++){
		if(std::find(pass.begin(), pass.end(), *it)==pass.end()) fileOut<<"NO"<<std::endl;
		else fileOut<<"YES"<<std::endl;
	}
}

int main(int argc, char **argv) {
	size_t sizeCodes;

	std::fstream fileIn;
	fileIn.open( BINARY_DIR "/input.txt", std::ios::in);
	if(!fileIn) {std::cout<<"Error path for input.txt"<<std::endl; exit(1);}

	fileIn>>sizeCodes;
	if(!fileIn){std::cout<<"There is no data in input file"<<std::endl; exit(1);}

	std::fstream fileOut;
	fileOut.open( BINARY_DIR "/output.txt", std::ios::out);
	if(!fileOut) {std::cout<<"Error path for output.txt"<<std::endl; exit(1);}

	searchCodesInPass(sizeCodes, fileIn, fileOut);

	fileIn.close();
	fileOut.close();

	return 0;
}



