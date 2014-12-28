int rndm(int min, int max){
	if ( (unsigned)time(NULL) != seed ){
		srand((unsigned)time(NULL));
		seed=(unsigned)time(NULL);
	}
	return min +rand()%(max-min+1);
}
void gotoxy(int x,int y){
	printf("%c[%d;%df",0x1B,y,x);
	return;
}
void clrin(){
	gotoxy(1,23);//\e[2K clears line
	printf("\e[J");
	gotoxy(1,23);
}
void clr(){
	printf("\e[1;1H\e[2J");
	return;
}
void drawgrid(int topleftx, int toplefty, int width, int height,int clear){
	int i,j;//x,y=(1,1),width=3,height=3
	gotoxy(topleftx,toplefty);
	for (i=0;i<width;i++){
		if(i==0||i==width-1)
			printf("+");
		else
			printf("-");
	}
	for (i=0;i<height-2;i++){
		gotoxy(topleftx,toplefty+i+1);
		printf("|");
		if(clear){
			for(j=topleftx+1;j<width;j++)
				printf(" ");
		}
		else{
			gotoxy(topleftx+width-1,toplefty+i+1);
			printf("|");
		}
	}
	gotoxy(topleftx, height-toplefty+1);
	for (i=0;i<width;i++){
		if (i==0||i==width-1)
			printf("+");
		else
			printf("-");
	}
}
