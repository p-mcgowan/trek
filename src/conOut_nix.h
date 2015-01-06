// FG Colours
#define BLACK 30
#define RED 31
#define GREEN 32
#define YELLOW 33
#define BLUE 34
#define MAGENTA 35
#define CYAN 36
#define WHITE 37
/*#define BLACK "\x1b[30m"
#define BGC_BLACK "\x1b[40m"
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
*/
void clrsc(int c, int b) {  
  setCB(c, b);
  printf("\x1b[2J");
}
void clrs() {
  printf("\x1b[2J");
}
void gotoxy(unsigned int x, unsigned int y){
  printf("\x1b[%d;%df", y, x);
}
void setC(int c) {
  char col[11] = {'\\',}
  printf("%s",Colour);
}
void setCB(int c, int b) {
  //#define BLACK "\x1b[30m"
//#define BGC_BLACK "\x1b[40m"
  setC(c);

  char col[11] = {'\\', 'x', '1', 'b', '[', 40 'm'
  strcpy(col,Colour);
  col[2]='4';  // replace fg with bg
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
void gotoXY(int x, int y) {
  printf("%c[%d;%df",0x1B,y,x);
}
*/