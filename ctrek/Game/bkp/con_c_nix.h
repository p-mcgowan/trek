#include <stdio.h>
#include <stdlib.h>
#define COL_BLACK 30
#define COL_RED 31
#define COL_GREEN 32
#define COL_YELLOW 33
#define COL_BLUE 34
#define COL_MAGENTA 35
#define COL_CYAN 36
#define COL_WHITE 37
#define COL_GREY 36

#define COL_BLACK_BG 40
#define COL_RED_BG 41
#define COL_GREEN_BG 42
#define COL_YELLOW_BG 43
#define COL_BLUE_BG 44
#define COL_MAGENTA_BG 45
#define COL_CYAN_BG 46
#define COL_WHITE_BG 47
#define COL_GREY_BG 46

// grey = teal, no grey :(
#define STR_COL_INVERT "\033[7m"
#define STR_COL_BLACK "\033[30m"
#define STR_COL_RED "\033[31m"
#define STR_COL_GREEN "\033[32m"
#define STR_COL_YELLOW "\033[33m"
#define STR_COL_BLUE "\033[34m"
#define STR_COL_MAGENTA "\033[35m"
#define STR_COL_CYAN "\033[36m"
#define STR_COL_WHITE "\033[37m"
#define STR_COL_GREY "\033[36m"

#define STR_COL_BLACK_BG "\033[;40m"
#define STR_COL_RED_BG "\033[;41m"
#define STR_COL_GREEN_BG "\033[;42m"
#define STR_COL_YELLOW_BG "\033[;43m"
#define STR_COL_BLUE_BG "\033[;44m"
#define STR_COL_MAGENTA_BG "\033[;45m"
#define STR_COL_CYAN_BG "\033[;46m"
#define STR_COL_WHITE_BG "\033[;47m"
#define STR_COL_GREY_BG "\033[;46m"

#define STR_COL_RESET "\033[0m\033[37;40m"

void gotoXY(unsigned int x, unsigned int y){
  char buf[20];
  snprintf(buf, 19, "\033[%d;%df", y, x);
  printf("%s", buf);
}

void clrs() {
  printf("\033[2J");
  gotoXY(1,1);
}
void setC(int c) {
  char buf[20];
  snprintf(buf, 19, "\033[%dm", c);
  printf("%s", buf);
}

void setCB(int c, int b) {
  char buf[20];
  snprintf(buf, 19, "\033[%d;%dm", c, b + 10);
  printf("%s", buf);
}

void clrsCB(int c, int b) {
  setCB(c, b);
  clrs();
}

void clrLine() {
  printf("\033[2K");
}

void cursor(int toggle) {
  if (toggle == 1) {
    printf("\033[?25h");
  } else {
    printf("\033[?25l");
  }
}
