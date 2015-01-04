#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>  // std::count - fileUtils.h
#include <assert.h>

bool seeded = false;

int rndm(int low, int high) {
	if (!seeded) {
		srand(time(NULL));
		seeded = true;
	}
	return low + (rand() % (int)(high - low));
}