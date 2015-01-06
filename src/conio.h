/*Escape code Use
\x1b[2J Clears the console
\x1bn;mH or \x1bn;mf  Moves the cursor to row n, column m. The values are 1-based, and default to 1 (top left corner) if omitted
\x1b?25l  Hides the cursor (Note: the last character is lowercase ‘L')
\x1b?25h  Shows the cursor.
\x1b[;km  Where k is the colour (text colours are from 30 to 37 and background colours are from 40 to 47). For the colour codes, see Table 2.
\x1b[5m Blinks slowly
\x1b[6m Blinks rapidly*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
  // General Utility
#define CLEAR "\x1b[2J"
#define SET11 "\x1b[1;1f"  // Set the Cursor at 1,1
// Text Modification
#define BLINK_SLOW "\x1b[5m"
#define BLINK_RAPID "\x1b[6m"
// Colors
#define CC_CLEAR "\x1b[0m" // Console Color CLEAR
// FG Colours
#define BLACK "\x1b[30m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define WHITE "\x1b[37m"
// FG Intense Colors
#define IBLACK "\x1b[30;1m"
#define IRED "\x1b[31;1m"
#define IGREEN "\x1b[32;1m"
#define IYELLOW "\x1b[33;1m"
#define IBLUE "\x1b[34;1m"
#define IMAGENTA "\x1b[35;1m"
#define ICYAN "\x1b[36;1m"
#define IWHITE "\x1b[37;1m"
// BG Colors
#define BGC_BLACK "\x1b[40m"
#define BGC_RED "\x1b[41m"
#define BGC_GREEN "\x1b[42m"
#define BGC_YELLOW "\x1b[43m"
#define BGC_BLUE "\x1b[44m"
#define BGC_MAGENTA "\x1b[45m"
#define BGC_CYAN "\x1b[46m"
#define BGC_WHITE "\x1b[47m"
// BG Intense Colors
#define BGC_IBLACK "\x1b[40;1m"
#define BGC_IRED "\x1b[41;1m"
#define BGC_IGREEN "\x1b[42;1m"
#define BGC_IYELLOW "\x1b[43;1m"
#define BGC_IBLUE "\x1b[44;1m"
#define BGC_IMAGENTA "\x1b[45;1m"
#define BGC_ICYAN "\x1b[46;1m"
#define BGC_IWHITE "\x1b[47;1m"
// General Utility Functions
void cagxy(unsigned int x, unsigned int y) {  // Clear and Goto X,Y
  printf("%s\x1b[%d;%df", CLEAR, y, x);
}
void clrscr() {  // Clear the Screen
  printf("%s",CLEAR);
}
void gotox(unsigned int x) {
printf("\x1b[%dG", x);
}
void gotoxy(unsigned int x, unsigned int y) {
printf("\x1b[%d;%df", y, x);
}
void nocursor() {
printf("\x1b[?25l");
}
void reset_video() {
printf("\x1b[0m");
}
void showcursor() {
printf("\x1b[?25h");
}
void textcolor(char *color) {
printf("%s",color);
}
void textbackground(char color[11]) {
char col[11];
strcpy(col,color);
col[2]='4'; // The given color will be fg color. Replace '3' with '4'
printf("%s",col);
}

/*
int main() {
textbackground(BLUE);
textcolor(WHITE);
clrscr();
printf("Hello, World!\n");
getch();
return 0;
}
See how short test.c is compared to Code 2!
Now, to compile this code,
1)    Keep both conio.c and conio.h in the same directory where test.c exists.
2)    Compile using the following command:
gcc test.c conio.c -o test
*/
