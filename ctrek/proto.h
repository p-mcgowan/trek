int rndm(int,int);
void clrin();
void clr();
void gotoxy(int,int);
void drawgrid(int,int,int,int,int);
int fcountstrings(char*);
char *fgetrandomstring(char*);
void basestats(weapon*,int,int);
void getstats(weapon*);
char *getrarity();
void getmods(weapon*,char);
void findmod(char*,char*,weapon*,char);
int processmods(weapon*,char*,int);
