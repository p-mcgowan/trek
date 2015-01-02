#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>  // std::count - fileUtils.h

double seed=0;

int rndm(int low, int high) {
	if (time(NULL) != seed)
		srand(time(NULL));
	return low + rand()%high;
}