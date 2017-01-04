#define BLACK 30
#define RED 31
#define GREEN 32
#define YELLOW 33
#define BLUE 34
#define MAGENTA 35
#define CYAN 36
#define WHITE 37

#include <iostream>
#include <string>


void clrs() {
  std::cout << "\033[2J";
}
void gotoxy(unsigned int x, unsigned int y){
  std::cout << "\033[" + std::to_string(y) + ";" + std::to_string(y) + "f";
}
void setC(int c) {
  std::cout << "\033[0;" + std::to_string(c) + "m";
}
void setCB(int c, int b) {
  std::cout << "\033[0;" + std::to_string(c) + "m" << "\033[0;" + std::to_string(b + 10) + "m";
}
void clrsCB(int c, int b) {
  setCB(c, b);
  clrs();
}
void cursor() {
  std::cout << "\033[?25l";
  // \033?25h
}
