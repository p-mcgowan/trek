#define CLEAR "\x1b[2J"
#define CC_CLEAR "\x1b[0m" // Console Colour CLEAR
// FG Colours
#define BLACK "\x1b[30m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define WHITE "\x1b[37m"
// BG Colours
#define BGC_BLACK "\x1b[40m"
#define BGC_RED "\x1b[41m"
#define BGC_GREEN "\x1b[42m"
#define BGC_YELLOW "\x1b[43m"
#define BGC_BLUE "\x1b[44m"
#define BGC_MAGENTA "\x1b[45m"
#define BGC_CYAN "\x1b[46m"
#define BGC_WHITE "\x1b[47m"

void gotoXY(int x, int y) {
  printf("%c[%d;%df",0x1B,y,x);
}
void clrs(void) {
  printf("\x1b[2J");
}
void setCB(int ch, int bg) {
  std::string c = "\033[1;" + std::to_string(ch) + 'm';
  std::string b = "\033[7;" + std::to_string(bg) + 'm';
// \033[7;#m - Colours the background according to #
}
void clrsc(int ch, int bg) {
}
void SetColour(int colour){
//#define RED "\033[1;31m"
  std::string c = "\033[1;" + std::to_string(colour) + 'm';
  std::cout << c;
}
void clrsc(int c, int b) {  
  printf("\x1b[2J");
}
void gotoxy(unsigned int x, unsigned int y){
  printf("\x1b[%d;%df", y, x);
}
void setC(int c) {
  printf("%s",Colour);
}
void textbackground(char Colour[11]) {
char col[11];
strcpy(col,Colour);
col[2]='4'; // The given Colour will be fg Colour. Replace ’3′ with ’4′.
printf("%s",col);
}
/*
Escape code Use
\x1b[2J Clears the console
\x1bn;mH or \x1bn;mf  Moves the cursor to row n, column m. The values are 1-based, and default to 1 (top left corner) if omitted
\x1b?25l  Hides the cursor (Note: the last character is lowercase ‘L’)
\x1b?25h  Shows the cursor.
\x1b[;km  Where k is the colour (text colours are from 30 to 37 and background colours are from 40 to 47). For the colour codes, see Table 2.
\x1b[5m Blinks slowly
\x1b[6m Blinks rapidly
*/