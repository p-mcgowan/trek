#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>  // std::count - fileUtils.h
#include <assert.h>

#define LOGD(m) do { if(debug) {std::cout << m << std::endl} } while (0)

bool seeded = false;

int rndm(int low, int high) {
	if (!seeded) {
		srand(time(NULL));
		seeded = true;
	}
	return low + (rand() % (int)(high - low));
}

std::string trimCRLF(std::string s) {
	s.erase(std::remove(s.begin(), s.end(), '\r'), s.end());
	s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());
	return s;
}