void gotoXY(int x, int y) {
  printf("%c[%d;%df",0x1B,y,x);
}
void ClearConsole(void);
void SetColorAndBackground(int ch, int bg) {
  std::string c = "\033[1;" + std::to_string(ch) + 'm';
  std::string b = "\033[7;" + std::to_string(bg) + 'm';
// \033[7;#m - colors the background according to #
}
void ClearConsoleToColors(int ch, int bg) {

}
void SetColor(int colour){
//#define RED "\033[1;31m"
  std::string c = "\033[1;" + std::to_string(i) + 'm';
  std::cout << c;
}