#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>  // std::count - fileUtils.h
#include <vector>
#include <string>
#ifndef MSWINDOWS
  extern "C" {
    #include "getch.h"
  }
  #include "conOut_nix.h"
#else
  extern "C" {
    #include <conio.h>
    #include "conOut_win.h"
  }
#endif

#define LOGDN(m) do {if(debug) {std::cout << m;}} while(0)
#define LOGD(m) do {if(debug) {std::cout << m << std::endl;}} while(0)
#define LOGW(m) do {std::cout << m << std::endl;} while(0)
#define LOGERR(m, n) std::cout << m << std::endl; exit (n);

bool seeded = false;
bool debug = true;

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
