/*****************************************************************
* WinConsole.c: Demonstrates using the Win32 console API to
* manipulate console output.
* Program by: NickDMax
http://www.dreamincode.net/code/snippet921.htm
*****************************************************************/

#include <windows.h>
#include <stdio.h>

#define BLACK 30
#define RED 31
#define GREEN 32
#define YELLOW 33
#define BLUE 34
#define MAGENTA 35
#define CYAN 36
#define WHITE 37

//This will clear the console while setting the forground and
//  background colors.
void clrsc(int ForgC, int BackC) {
  WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD coord = {0, 0};
  DWORD count;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  SetConsoleTextAttribute(hStdOut, wColor);
  if(GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
    FillConsoleOutputCharacter(hStdOut, (TCHAR) 32, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
    FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
    SetConsoleCursorPosition(hStdOut, coord);
  }
  return;
}

//This will clear the console.
void clrs() {
  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD coord = {0, 0};
  DWORD count;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if(GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
    FillConsoleOutputCharacter(hStdOut, (TCHAR) 32, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
    FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
    SetConsoleCursorPosition(hStdOut, coord);
  }
  return;
}

//This will set the position of the cursor
void gotoxy(int x, int y) {
  COORD coord = {x, y};
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
  return;
}

//This will set the forground color for printing in a console window.
void setC(int ForgC) {
  WORD wColor;
  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if(GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
     wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
     SetConsoleTextAttribute(hStdOut, wColor);
  }
  return;
}

//This will set the forground and background color for printing in a console window.
void setCB(int ForgC, int BackC) {
  WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
  return;
}