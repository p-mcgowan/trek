#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include "outputManipulationFunctions.h"
#include "trek.h"
char systemMap[100], universe[100];
int hudStats[100],commerceArray[100],outfitArray[100],shipArray[100],capacityArray[50],enemyArray[10],commodityLastBuy[5],achievements[50],skillsArray[50];
int lastSeed,docked=0;
// on enter planet, switch hud option
int main()
{
    while(1){
		int nothing=ClearConsoleToColors(15,0);
		///int i;for(i=0;i<20;i++){SetColor(i);printf("%i   ",i);}system("pause");
		int debug=preGameMenu();
		if (debug==0){return 0;}
		initialize(3);
		initStats(debug);
		systemMap[45]='P';
		universe[45]='P';
		systemMap[35]='O';
		/*systemMap[23]='O';
		systemMap[99]=3;
		enemyArray[1]=3;
		enemyArray[8]=capacityArray[10+enemyArray[1]];///shield
		enemyArray[9]=capacityArray[20+enemyArray[1]];///hull*/
		choiceMenu(debug);
	}
    //game();
    return 0;
}
void hitAnim(int playerHit){///hs3,4
    if(playerHit==1){

    }
    else{

    }
}
int preGameMenu(void){
    char choice='5', debug=1;
    while (choice!='0'&&choice!='1'){
        printf("Choose an option:\n");
        printf("1: Play Game\n");
        printf("2: Toggle Debug (");(debug==1)?printf("OFF)\n"):printf("ON) \n");
        printf("0: Quit\n");
        choice=getch();
        if(choice=='2'){debug*=-1;}
        ClearConsole();
    }
    if (choice!='0'){return debug;}
    else {return 0;}
}
void seed(void)
{
	/*elapsed_seconds = (double)(end-start)/1000;
	clock_t end;
	start = clock();*/
    if (clock()!=lastSeed)
    {
        srand(clock());
        lastSeed=clock();
    }
}
int randomNumber(int max)
{
    seed();
    return (float)rand()/32767*max;
}
void choiceMenu(int debug) // debug on right
{
    int i,scans=1,hudstat,value;
    char choice,exit;
    printMap(1,0);
    do
    {
        (skillsArray[0]!=0)?skillsHUD(50,skillsArray[0]):HUD(50);
        choice=-1;
        gotoXY(1,26);(scans==1)?printf("1: Long Range Scans\t"):printf("1: Short Range Scans\t");printf("2: Torpedos");
        gotoXY(1,27);printf("3: Cycle HUDs\t\t");/*printf("3: Enemy Move\t\t");*/printf("4: Range Check");
        gotoXY(1,28);printf("5: Scan Enemy\t\t");printf("6: Deal damage");/*printf("6: Take damage");*/
        gotoXY(1,29);printf("7: Divert Power\t");printf("(W/A/S/D): Warp");
        (debug==-1)?debugMenu():null();
		gotoXY(1,32);choice=getch();choice=toupper(choice);
        gotoXY(1,32);printf("                                    ");
        gotoXY(1,33);printf("                                    ");
        gotoXY(1,34);printf("                                    ");///replace docking options with getch()
        switch (choice)
        {
        case 87: ///wasd
            scans=1;
            move('N');
            printMap(1,0);
            break;
        case 68:
            scans=1;
            move('E');
            printMap(1,0);
            break;
        case 83:
            scans=1;
            move('S');
            printMap(1,0);
            break;
        case 65:
            scans=1;
            move('W');
            printMap(1,0);
            break;
        case 49:///1 - maps
            (scans==1)?printMap(2,0):printMap(1,0);
            scans*=-1;
            break;
        case 50:///2 - torps
            (hudStats[7]!=1)?firetorpedo():(clearCom(51),gotoXY(52,27),printf("No enemies nearby; firing torpedos unadvisable."));
            printMap(1,0);
            break;
        case 51:///3 - skill huds
            skillsArray[0]=(skillsArray[0]!=5)?skillsArray[0]+1:0;///page control--lim=skillarray
            (docked==1)?clearHUD(65):clearHUD(50);
            break;
        case 52:///4 - range
            scan(1,hudStats[19],1);
            break;
        case 53:///5 - scan enemy
            scan(2,hudStats[19],1);
            break;
		case 54:///6 - deal
            scan(1,hudStats[19],0);
            fire(1);
            printMap(1,0);
            break;
        case 55:///7 - divert
            gotoXY(1,32);printf("Ammount to divert to shields:");
            scanf("%i",&value);clean();gotoXY(1,32);printf("                                              ");
            (hudStats[8]==0)?clearCom(51),gotoXY(52,27),printf("Power not available."):divertPower(value);
            break;
			// 1:scans,2:torp,3:huds,4:range,5:scan,6:deal,7:divert
			// F:enemymove,g:skilss,h:take,i:hudstats
		case 'F'://f:70
			if(debug!=-1){clearCom(52);gotoXY(52,27);printf("Input invalid, please enter again.");break;}
			if(hudStats[7]!=1){
                enemyMovement();
                printMap(1,0);
            }
			break;
		case 'G':///g - skills
			if(debug!=-1){clearCom(52);gotoXY(52,27);printf("Input invalid, please enter again.");break;}
            gotoXY(1,32);printf("skillsArray[");scanf("%i",&hudstat);clean();(hudstat>9)?gotoXY(15,32):gotoXY(14,32);printf("]=");scanf("%i",&value);clean();
            (hudstat>0&&hudstat<=50)?(skillsArray[hudstat]=value):(clearCom(51),gotoXY(52,27),printf("Invalid skill array."));
            gotoXY(1,32);printf("                               ");
			break;
		case 'H': // h - take
			if(debug!=-1){clearCom(52);gotoXY(52,27);printf("Input invalid, please enter again.");break;}
            clearCom(51);
            fire(2);
            printMap(1,0);
            break;
		case 'I': ///9 - hudstat
			if(debug!=-1){clearCom(52);gotoXY(52,27);printf("Input invalid, please enter again.");break;}
            gotoXY(1,32);printf("hudStat[");scanf("%i",&hudstat);clean();(hudstat>9)?gotoXY(11,32):gotoXY(10,32);printf("]=");scanf("%i",&value);clean();
            (hudstat>0&&hudstat<33)?(hudStats[hudstat]=value):(clearCom(51),gotoXY(52,27),printf("Invalid hudstat."));
            gotoXY(1,32);printf("                               ");
            (hudstat==19)?printMap(1,0):null();
            break;
        default:
            if (choice!=48){clearCom(52);gotoXY(52,27);printf("Input invalid, please enter again.");}
            break;
        }
    }while (choice!=48);
    gotoXY(1,32);printf("Are you sure you want to end the program?");
    exit=getch();exit=toupper(exit);
    if(exit!='Y'){gotoXY(1,32);printf("                                               ");choiceMenu(debug);}
	return;
}
void debugMenu(){
	//prints debug options on right
	gotoXY(106,3);printf("F: Enemy Move");
	gotoXY(106,4);printf("G: SkillsArray[x]=y");
	gotoXY(106,5);printf("H: Take Damage");
	gotoXY(106,6);printf("I: hudStat[x]=y");
	return;
}
void initialize(int whichArray)
{
    int i;
    for(i=0;i<100;i++)
    {
        (whichArray==1)?(systemMap[i]!='P')?systemMap[i]=0:null():null();
        (whichArray==3)?hudStats[i]=0,universe[i]=0,systemMap[i]=0,achievements[i]=0,skillsArray[i]=0:null();
    }
    (whichArray==3)?achievements[1]=1,achievements[3]=1,achievements[4]=1:null();///initialize systems visited
}
void null(void){;}
void game(void)
{
    initialize(3);
    printMap(1,0);
    systemObjects();
    while (1)
    {
        move(moveInput());
        printMap(1,0);
    }
}
char moveInput(void)///change to com log
{
    char direction=0,temp;
    gotoXY(1,32);printf("Move: (N/E/S/W/0)");
    while(1)
    {
        scanf("%c",&temp);
        clean();
        temp=toupper(temp);
        if (temp=='0'){break;}
        if (temp=='N'||temp=='E'||temp=='S'||temp=='W')
        {
            direction=temp;
            break;
        }
        else
        {
            gotoXY(1,32);printf("Again:                        ");gotoXY(8,32);
        }
    }
    gotoXY(1,32);printf("                                    ");
    return direction;
}
void move(char toWhere)
{///move to com log
    int nothing=clearCom(51);
    int i,j;
    for(i=0;i<100;i++)
    {
        if (systemMap[i]=='P')
        {
            if (toWhere=='N')
            {
                if (i>9&&(systemMap[i-10]<1||systemMap[i-10]>5))//move up
                {
                    (systemMap[i-10]=='O')?(hudStats[7]!=1)?clearCom(51),gotoXY(52,27),printf("Docking request denied when enemy vessles nearby."):planetMenu():(systemMap[i-10]>5&&systemMap[i-10]<11)?loot():(systemMap[i]=0,systemMap[i-10]='P',achievements[0]++);
                    break;
                }
                if (i<=9)//move uni up
                {
                    for (j=0;j<100;j++)
                    {
                        if (universe[j]=='P'&&j>9)//find p, move up
                        {
                            systemMap[i]=0;
                            systemMap[i+90]='P';//map to bottom
                            universe[j]=0;
                            universe[j-10]='P';
                            initialize(1);
                            achievements[0]++;
                            achievements[1]++;
                            systemObjects();
                            break;
                        }
                        if (universe[j]=='P'&&j<=9)
                        {
                            clearCom(51);gotoXY(52,27);printf("You are tying to leave federation space...");
                            gotoXY(52,28);printf("this is ill advised.");
                            break;
                        }
                    }
                }
                break;
            }
            if (toWhere=='E')
            {
                if (i%10!=9&&(systemMap[i+1]<1||systemMap[i+1]>5))
                {
                    (systemMap[i+1]=='O')?(hudStats[7]!=1)?clearCom(51),gotoXY(52,27),printf("Docking request denied when enemy vessles nearby."):planetMenu():(systemMap[i+1]>5&&systemMap[i+1]<11)?loot():(systemMap[i]=0,systemMap[i+1]='P',achievements[0]++);
                    break;
                }
                if (i%10==9)
                {
                    for (j=0;j<100;j++)
                    {
                        if (universe[j]=='P'&&j%10!=9)
                        {
                            systemMap[i]=0;
                            systemMap[i-9]='P';
                            universe[j]=0;
                            universe[j+1]='P';
                            initialize(1);
                            achievements[0]++;
                            achievements[1]++;
                            systemObjects();
                            break;
                        }
                        if (universe[j]=='P'&&j%10==9)
                        {
                            clearCom(51);gotoXY(52,27);printf("You are tying to leave federation space...");
                            gotoXY(52,28);printf("this is ill advised.");
                            break;
                        }
                    }
                }
                break;
            }
            if (toWhere=='S')
            {
                if (i<90&&(systemMap[i+10]<1||systemMap[i+10]>5))//move down
                {
                    (systemMap[i+10]=='O')?(hudStats[7]!=1)?clearCom(51),gotoXY(52,27),printf("Docking request denied when enemy vessles nearby."):planetMenu():(systemMap[i+10]>5&&systemMap[i+10]<11)?loot():(systemMap[i]=0,systemMap[i+10]='P',achievements[0]++);
                }
                if (i>=90)//move uni down
                {
                    for (j=0;j<100;j++)
                    {
                        if (universe[j]=='P'&&j<90)//find p, move down
                        {
                            systemMap[i]=0;
                            systemMap[i-90]='P';//map to top
                            universe[j]=0;
                            universe[j+10]='P';
                            achievements[0]++;
                            achievements[1]++;
                            initialize(1);
                            systemObjects();
                            break;
                        }
                        if (universe[j]=='P'&&j>=90)
                        {
                            clearCom(51);gotoXY(52,27);printf("You are tying to leave federation space...");
                            gotoXY(52,28);printf("this is ill advised.");
                            break;
                        }
                    }
                }
                break;
            }
            if (toWhere=='W')
            {
                if (i%10!=0&&(systemMap[i-1]<1||systemMap[i-1]>5))
                {
                    (systemMap[i-1]=='O')?(hudStats[7]!=1)?clearCom(51),gotoXY(52,27),printf("Docking request denied when enemy vessles nearby."):planetMenu():(systemMap[i-1]>5&&systemMap[i-1]<11)?loot():(systemMap[i]=0,systemMap[i-1]='P',achievements[0]++);
                    break;
                }
                if (i%10==0)
                {
                    for (j=0;j<100;j++)
                    {
                        if (universe[j]=='P'&&j%10!=0)
                        {
                            systemMap[i]=0;
                            systemMap[i+9]='P';
                            universe[j]=0;
                            universe[j-1]='P';
                            achievements[0]++;
                            achievements[1]++;
                            initialize(1);
                            systemObjects();
                            break;
                        }
                        if (universe[j]=='P'&&j%10==0)
                        {
                            clearCom(51);gotoXY(52,27);printf("You are tying to leave federation space...");
                            gotoXY(52,28);printf("this is ill advised.");
                            break;
                        }
                    }
                }
                break;
            }
        }
    }
}
void printMap(int whichMap,int playerScanning)///scan=0,1,2 (player)
{
    int i,j;
    gotoXY(1,1);(whichMap==1)?printf("                System Map"):printf("                Galaxy Map");
    gotoXY(1,2);printf("    1   2   3   4   5   6   7   8   9  10");
    gotoXY(1,3);printf("  +---+---+---+---+---+---+---+---+---+---+");
    for(i=0;i<10;i++)
    {
        gotoXY(1,4+2*i);(i!=9)?printf("%i |",i+1):printf("%i|",i+1);//row
        for(j=0;j<10;j++)
        {
            (((i*10)+j>89||(i*10)+j<10||((i*10)+j)%10==0||((i*10)+j)%10==9)&&whichMap==2)?SetColorAndBackground(15,4):null();
            (whichMap==1)?charColour(systemMap[(i*10)+j],playerScanning,(i*10)+j):charColour(universe[(i*10)+j],playerScanning,(i*10)+j);
        }
        gotoXY(1,5+2*i);printf("  +---+---+---+---+---+---+---+---+---+---+");
    }
}
void charColour(int arrayValue,int playerScanning,int arraySlot)
{
    (playerScanning!=0&&rangeScan(playerScanning,arraySlot)!=0)?SetColorAndBackground(15,4):null();
    (arrayValue>=1&&arrayValue<=5)?SetColor(12),printShip(arrayValue):null();
    (arrayValue>=6&&arrayValue<=10)?SetColor(8),printShip(arrayValue):null();
    (arrayValue=='O')?SetColor(10),printf(" %c ",arrayValue):null();
    (arrayValue=='P')?SetColor(14),printShip(hudStats[19]):null();
    (arrayValue==0)?SetColor(15),printf(" %c ",arrayValue):null();
    SetColorAndBackground(15,0);printf("|");
}
int rangeScan(int playerScanning,int arraySlot)
{
    int i,range1=0,range2=0,range3=0,xDifference,yDifference,inRange=0;
	float distance;
    for(i=1;i<=5;i++)
    {
        ((hudStats[20+i]==1&&playerScanning==1)||(enemyArray[i+2]==1&&playerScanning==2))?range1++:null();
        ((hudStats[20+i]==2&&playerScanning==1)||(enemyArray[i+2]==2&&playerScanning==2))?range2++:null();
        ((hudStats[20+i]==3&&playerScanning==1)||(enemyArray[i+2]==3&&playerScanning==2))?range3++:null();
    }
    if(playerScanning==1)
    {
        xDifference=(arraySlot%10-hudStats[20]%10>0)?arraySlot%10-hudStats[20]%10:hudStats[20]%10-arraySlot%10;
        yDifference=(arraySlot/10-hudStats[20]/10>0)?arraySlot/10-hudStats[20]/10:hudStats[20]/10-arraySlot/10;
        xDifference-=hudStats[28];yDifference-=hudStats[28];
    }
    if(playerScanning==2)
    {
        xDifference=(enemyArray[0]%10-arraySlot%10>0)?enemyArray[0]%10-arraySlot%10:arraySlot%10-enemyArray[0]%10;
        yDifference=(enemyArray[0]/10-arraySlot/10>0)?enemyArray[0]/10-arraySlot/10:arraySlot/10-enemyArray[0]/10;
    }
    distance=sqrt(pow((double)xDifference,2)+pow((double)yDifference,2));
    distance=(distance-(int)distance>=0.5)?distance+(1-(distance-(int)distance)):distance-(distance-(int)distance);
    (distance<=3&&range1!=0)?inRange=1:null();
    (distance<=5&&range2!=0)?inRange=1:null();
    (distance<=7&&range3!=0)?inRange=1:null();
    return inRange;
}
void printShip(int ship)
{
    (ship==0)?printf(" . "):null();
    (ship==1||ship==6)?printf(" %c ",127):null();
    (ship==2||ship==7)?printf(" %c ",30):null();
    (ship==3||ship==8)?printf(".%c.",18):null();
    (ship==4||ship==9)?printf("%c%c%c",174,18,175):null();
    (ship==5||ship==10)?printf("%c%c%c",17,18,16):null();
}
void clean (void)
{
    char dummy;
    do
    {
        scanf("%c",&dummy);
    }
    while(dummy != '\n');
}
void systemObjects(void)
{
    int nothing=initEnemy(0);
    int numberOfPlanets=0, chance, i, placePlanet, enemyLocation=0,derelictLocation;
    randomNumber(100);randomNumber(100);randomNumber(100);
    chance=randomNumber(100);
    if (chance>50)
    {
        numberOfPlanets=(chance-30)/20;
    }
    for(i=0;i<numberOfPlanets;i++)
    {
        placePlanet=randomNumber(100);
        while (systemMap[placePlanet]!=0)
        {
            placePlanet=randomNumber(100);
        }
        systemMap[placePlanet]='O';
    }
    chance=randomNumber(100);
    if (((hudStats[5]==1||hudStats[5]==10||hudStats[6]==1||hudStats[6]==10)&&chance>30)||chance>75)
    {
        enemyLocation=randomNumber(99);
        while (systemMap[enemyLocation]!=0)
        {
            enemyLocation=randomNumber(99);
        }
        systemMap[enemyLocation]=(hudStats[5]==1||hudStats[5]==10||hudStats[6]==1||hudStats[6]==10)?2+randomNumber(4):randomNumber(5);
        enemyArray[0]=enemyLocation;
        enemyArray[1]=systemMap[enemyLocation];
        enemyArray[2]=0;
        for(i=1;i<=enemyArray[1];i++)///1,2,3,4,5
        {
            enemyArray[3]=(i<3)?i:1+randomNumber(2);
            enemyArray[4]=(randomNumber(100)>40&&i>1)?(i>3)?randomNumber(3):randomNumber(2):0;
            enemyArray[5]=(randomNumber(100)>60&&i>2)?(i>3)?randomNumber(3):randomNumber(2):0;
            enemyArray[6]=(randomNumber(100)>70&&i>3)?randomNumber(3):0;
            enemyArray[7]=(randomNumber(100)>70&&i>4)?randomNumber(3):0;
        }
        enemyArray[8]=capacityArray[10+enemyArray[1]];///shield
        enemyArray[9]=capacityArray[20+enemyArray[1]];///hull
    }
    chance=randomNumber(100);
    if(chance>90&&enemyArray[1]==0){
        derelictLocation=randomNumber(99);
        while(systemMap[derelictLocation]!=0)
        {
            derelictLocation=randomNumber(100);
        }
        enemyArray[0]=derelictLocation;
        systemMap[derelictLocation]=6+randomNumber(4);
        enemyArray[1]=systemMap[derelictLocation];
        enemyArray[2]=0;
    }
    hudStats[18]=randomNumber(10);
    hudStats[0]+=1+randomNumber(9);
}
void clearHUD(int x){
    int /*i,*/j;
    for(j=2;j<=23;j++){
        gotoXY(x,j);printf("                                                       ");
    }
}
void printProgressBar(int skillSlot){
    int i;
    printf("  ");
    for(i=1;i<=10;i++){((skillsArray[skillSlot]%10)>=i)?SetColorAndBackground(15,10):SetColorAndBackground(15,0);printf("  ");SetColorAndBackground(15,0);printf("|");}
}
void skillsHUD(int x,int page){
    int i,j,p=9;
    gotoXY(x,2);
    printf(" Skills:");gotoXY(x+48,2);printf("Page %i",page);
    gotoXY(x,3);
    printf("+-----------------------------------------------------+");

    for(j=4;j<=22;j+=2){
    gotoXY(x,j);printf("  Skill %i",10*page-p);gotoXY(x,j+1);
    printProgressBar(10*page-p);printf("\tLevel %i (%i %c)\t",skillsArray[10*page-p]/10+1,(skillsArray[10*page-p]%10)*10,37);
    p--;}
    for(i=4;i<32;i++){gotoXY(x,i);printf("|");gotoXY(x+54,i);printf("|");}
    gotoXY(x,24);printf("+-----------------------------------------------------+");
    gotoXY(x,25);printf(" Communications Log:");
    gotoXY(x,26);printf("+-----------------------------------------------------+");
    ///for(i=27;i<32;i++){gotoXY(x,i);printf("|");gotoXY(x+54,i);printf("|");}
    gotoXY(x,32);printf("+-----------------------------------------------------+");
    ///for(i=4;i<=23;i++){gotoXY(x,i);printf("|");gotoXY(x+54,i);printf("|");}
}
void HUD(int x)
{
    int i;findPlayer();
    gotoXY(x,2);
    printf(" Helm Console:");
    gotoXY(x,3);
    printf("+-----------------------------------------------------+");
    gotoXY(x,4);
    printf("| Stardate: %i.%i     ",hudStats[0]/10,hudStats[0]%10);gotoXY(x+25,4);printf("Ship Class:                  ");gotoXY(x+37,4);(hudStats[19]==0)?printf("Escape Pod"):shipClass(hudStats[19]);gotoXY(x+54,4);printf("|");
    gotoXY(x,5);
    printf("| System Coordinates: (%i,%i)  ",hudStats[3],hudStats[4]);gotoXY(x+54,5);printf("|");
    gotoXY(x,6);
    printf("| Galaxy Coordinates: (%i,%i)  ",hudStats[5],hudStats[6]);(hudStats[5]==1||hudStats[5]==10||hudStats[6]==1||hudStats[6]==10)?gotoXY(x+23,6),SetColor(12),printf("%i,%i",hudStats[5],hudStats[6]),SetColor(15),printf(")"):null();gotoXY(x+54,6);printf("|");
    gotoXY(x,7);
    printf("+-----------------------------------------------------+");
    gotoXY(x,8);
    printf(" Tactical Ship Status:");
    gotoXY(x,9);
    printf("+-----------------------------------------------------+");
    gotoXY(x,10);
    printf("| Shields:               Hull Integrity:              ");gotoXY(x+41,10);(hudStats[19]==0)?printf("---"):hudColours(hudStats[2],capacityArray[20+hudStats[19]]);gotoXY(x+12,10);(hudStats[19]==0)?printf("---"):hudColours(hudStats[1],capacityArray[10+hudStats[19]]);gotoXY(x+54,10);printf("|");
    gotoXY(x,11);
    printf("| Security:              Energy:                      ");gotoXY(x+33,11);(hudStats[19]==0)?printf("---"):hudColours(hudStats[8],capacityArray[30+hudStats[19]]);gotoXY(x+12,11);securityStatus();gotoXY(x+54,11);printf("|");
    gotoXY(x,12);
    printf("| Torpedos:              Weapons:                     ");gotoXY(x+34,12);(hudStats[19]==0)?printf("---"):printf("%i/%i   ",gunCount(),capacityArray[5+hudStats[19]]);(enemyArray[1]>0&&enemyArray[1]<6)?rangeCheck(1,1),printf("("),SetColor(gunRange(gunCount(),hudStats[39])),printf("%i ",hudStats[39]),SetColor(15),printf("in range)"):null();gotoXY(x+12,12);(hudStats[19]==0)?printf("---"):hudColours(hudStats[9],capacityArray[40+hudStats[19]]);gotoXY(x+54,12);printf("|");
    gotoXY(x,13);
    printf("+-----------------------------------------------------+");
    gotoXY(x,14);
    printf(" Cargo:                                               ");gotoXY(x+25,14);(hudStats[19]==0)?printf("---"):printf("(%i/%i units)",hudStats[11]+hudStats[12]+hudStats[13]+hudStats[14]+hudStats[15],capacityArray[hudStats[19]]+100*hudStats[31]);
    gotoXY(x,15);
    printf("+-----------------------------------------------------+");
    gotoXY(x,16);
    printf("| Credits:               Warp Plasma:                  ");gotoXY(x+11,16);prefix(hudStats[10],'n');gotoXY(x+38,16);(hudStats[19]==0)?printf("---"):prefix(hudStats[11],'L');gotoXY(x+54,16);printf("|");
    gotoXY(x,17);
    printf("| Duterium:              Latinum:                      ");gotoXY(x+12,17);(hudStats[19]==0)?printf("---"):prefix(hudStats[12],'g');gotoXY(x+34,17);(hudStats[19]==0)?printf("---"):prefix(hudStats[13],'g');gotoXY(x+54,17);printf("|");
    gotoXY(x,18);
    printf("| Water:                 Uranium Ore:                  ");gotoXY(x+9,18);(hudStats[19]==0)?printf("---"):prefix(hudStats[14],'L');gotoXY(x+38,18);(hudStats[19]==0)?printf("---"):prefix(hudStats[15],'g');gotoXY(x+54,18);printf("|");
    gotoXY(x,19);
    printf("+-----------------------------------------------------+");
    gotoXY(x,20);
    printf(" System Peripherals:");
    gotoXY(x,21);
    printf("+-----------------------------------------------------+");
    gotoXY(x,22);
    printf("| Planets: %i ",hudStats[16]);gotoXY(x+20,22);printf("Enemies: %i",hudStats[17]);gotoXY(x+54,22);printf("|");
    gotoXY(x,23);
    printf("| Inactive Stellar Objects: %i  ",hudStats[18]);gotoXY(x+54,23);printf("|");
    gotoXY(x,24);
    printf("+-----------------------------------------------------+");
    gotoXY(x,25);
    printf(" Communications Log:");
    gotoXY(x,26);
    printf("+-----------------------------------------------------+");
    for(i=27;i<32;i++){gotoXY(x,i);printf("|");gotoXY(x+54,i);printf("|");}
    gotoXY(x,32);
    printf("+-----------------------------------------------------+");
}
int gunRange(int totalGuns,int gunsInRange){
	if(gunsInRange==0){return 12;} // red no guns in range
	if(gunsInRange!=totalGuns){return 14;} // yellow, not all guns in range
	if(gunsInRange==totalGuns){return 10;} // green, all guns in range
	// 10 grn, 14 yell
}
int gunCount(void){
    int i,guns=0;
    for(i=21;i<=25;i++){
        if(hudStats[i]==1||hudStats[i]==2||hudStats[i]==3){guns++;}
    }
    hudStats[26]=guns;
    return guns;
}
void prefix(int arrayValue,char unit)
{
    int pointValue,decimal,zero=0;
    char unitPrefix;
    if (arrayValue>=1000)
    {
        pointValue=arrayValue/1000;
        decimal=(arrayValue%1000)/10;
        zero=(decimal<10)?1:0;

        unitPrefix='k';
    }
    if (arrayValue>=1000000)
    {
        zero=0;
        pointValue=arrayValue/1000000;
        decimal=(arrayValue%1000000)/10000;
        zero=(decimal<10)?1:0;
        unitPrefix='M';
    }
    if (unit=='g')
    {
        (arrayValue<1000)?printf("%i kg",arrayValue):(zero==0)?printf("%i.%i%c kg",pointValue,decimal,unitPrefix):printf("%i.0%i%c kg",pointValue,decimal,unitPrefix);
    }
    if (unit=='L')
    {
        (arrayValue<1000)?printf("%i L",arrayValue):(zero==0)?printf("%i.%i%c L",pointValue,decimal,unitPrefix):printf("%i.0%i%c L",pointValue,decimal,unitPrefix);
    }
    if (unit=='n')
    {
        (arrayValue<1000)?printf("%i",arrayValue):(zero==0)?printf("%i.%i%c",pointValue,decimal,unitPrefix):printf("%i.0%i%c",pointValue,decimal,unitPrefix);
    }
}
void findPlayer(void)
{
    int i;
    hudStats[16]=0;//planets
    hudStats[17]=0;//enemies
    for (i=0;i<100;i++)
    {
        if (systemMap[i]=='P')
        {
            hudStats[3]=i%10+1;
            hudStats[4]=i/10+1;
            hudStats[20]=i;
        }
        if (systemMap[i]>0&&systemMap[i]<6)
        {
            enemyArray[0]=i;
            enemyArray[1]=systemMap[i];
            hudStats[17]++;
        }
        if (universe[i]=='P')
        {
            hudStats[5]=i%10+1;
            hudStats[6]=i/10+1;
        }
        (systemMap[i]=='O')?hudStats[16]++:null();
    }
}
void securityStatus(void)
{
    int i,status=1;
    for (i=0;i<100;i++)
    {
        if (systemMap[i]>=1&&systemMap[i]<=5)
        {
            status=2;
            (rangeCheck(2,1)!=0)?status=3:null();
            break;
        }
    }
    if (status==1)
    {
        (hudStats[19]==0)?printf("---"):(SetColor(10),printf("Green"));
    }
    if (status==2)
    {
        (hudStats[19]==0)?printf("---"):(SetColor(14),printf("Yellow"));
    }
    if (status==3)
    {
        (hudStats[19]==0)?printf("---"):(SetColor(12),printf("Red"));
    }
    hudStats[7]=status;
    SetColor(15);
}
void planetHUD(void)
{
    int i,j;
    gotoXY(0,3);printf("+-----------------------------------------------------------+");
    for(i=4;i<32;i++)
    {
        gotoXY(0,i);printf("|                                                           |");
    }
    gotoXY(0,26);printf("+-----------------------------------------------------------+");
    gotoXY(0,32);printf("+-----------------------------------------------------------+");
    for(i=33;i<38;i++)
    {
        gotoXY(0,i);
        for(j=0;j<60;j++)
        {
            printf(" ");
        }
    }
    (skillsArray[0]!=0)?skillsHUD(65,skillsArray[0]):HUD(65);
}
void planetMenu(void)
{
    int choice,i,reward;docked=1;
    achievements[2]++;
    randomNumber(100);randomNumber(100);randomNumber(100);
    ClearConsole();
    planetHUD();
    gotoXY(15,15);printf("Docking, please wait.");
    initCommerce();
    initOutfit();
    initShips();
    (hudStats[40]!=0)?(reward=hudStats[40]*(8000+randomNumber(4000)),clearCom(66),gotoXY(67,27),printf("Your passenger"),(hudStats[40]!=1)?printf("s"):null(),printf(" rewarded you with "),prefix(reward,'n'),printf(" credits.",reward),hudStats[10]+=reward,hudStats[40]=0):null();
    do
    {
        planetHUD();
        gotoXY(3,27);printf("1: Shipyard\t\t\t");printf("2: Outfitter");gotoXY(3,29);printf("3: Commercial Sector\t\t");printf("4: Bar\t\t");printf("0: Undock");
        gotoXY(3,31);choice=getch()-'0';clearCom(66);

        switch (choice)
        {
        case 1:
            shipyardMenu();
            break;
        case 2:
            outfitterMenu();
            break;
        case 3:
            commerceMenu();
            break;
        case 4:
            barMenu();
            break;
        default:
            if (choice!=0)
                clearCom(66);gotoXY(67,27);printf("Invalid choice.");
            break;
        }
    }while(choice!=0);
    for(i=0;i<100;i++)
    {
        commerceArray[i]=0;
        outfitArray[i]=0;
        shipArray[i]=0;
    }
    ClearConsole();docked=0;hudStats[0]++;
}
void shipyardMenu(void)
{
    int choice,i,shuttle=-1,cargo=-1,freighter=-1,frigate=-1,destroyer=-1,submenu,ship,cost;
    do
    {
        planetHUD();
        gotoXY(3,5);
        printf("Ships avaliable at this location:");
        i=6;
        (shipArray[0]==1)?i++,gotoXY(3,i),printf("%i: NXC-01: P-Class Shuttle",i-6),shuttle=i-6:null();
        (shipArray[2]==1)?i++,gotoXY(3,i),printf("%i: Expanse 5060: F-Class Cargo Drone",i-6),cargo=i-6:null();
        (shipArray[4]==1)?i++,gotoXY(3,i),printf("%i: Axess DN-5: F-Class Freighter",i-6),freighter=i-6:null();
        (shipArray[6]==1)?i++,gotoXY(3,i),printf("%i: Green Goose FX-1: C-Class Frigate",i-6),frigate=i-6:null();
        (shipArray[8]==1)?i++,gotoXY(3,i),printf("%i: Imperium Industry FT9: C-Class Destroyer",i-6),destroyer=i-6:null();
        (i==6)?gotoXY(3,7),printf("No ships available."):null();
        gotoXY(3,27);(i!=6)?(i==7)?printf("1: "),(shuttle==1)?printf("Shuttle"):(cargo==1)?printf("Cargo Drone"):(freighter==1)?printf("Freighter"):printf("Destroyer"),printf("\t\t0: Back"):printf("1-%i: Select Ship\t\t0: Back",i-6):printf("0: Back");gotoXY(3,29);choice=getch()-'0';clearCom(66);
        if (choice==shuttle||choice==cargo||choice==freighter||choice==frigate||choice==destroyer)
        {
            do
            {
                planetHUD();
                gotoXY(3,5);printf("Ship stats:");
                ship=(choice==shuttle)?1:(choice==cargo)?2:(choice==freighter)?3:(choice==frigate)?4:5;
                shipInfo(ship);///1,3,5,7,9
                gotoXY(3,27);printf("1: Buy\t0: Back");
                gotoXY(3,28);submenu=getch()-'0';clearCom(66);
                if(submenu==1){
                    if (hudStats[10]+resaleValue()<shipArray[ship*2-1]){
                        clearCom(66);gotoXY(67,27);printf("Insufficient credits.");
                    }
                    else{
                        cost=(shipArray[ship*2-1]-resaleValue()>0)?shipArray[ship*2-1]-resaleValue():0;
                        printBuySell('b','s',hudStats[19],ship,cost);
                        hudStats[10]-=cost; hudStats[19]=ship;
                        hudStats[1]=capacityArray[10+hudStats[19]];hudStats[2]=capacityArray[20+hudStats[19]];hudStats[8]=capacityArray[30+hudStats[19]];hudStats[9]=0;
                        hudStats[21]=0;hudStats[22]=0;hudStats[23]=0;hudStats[24]=0;hudStats[25]=0;hudStats[26]=0;
                        for(i=27;i<=32;i++){hudStats[i]=0;}
                        achievements[3]++;
                    }
                }
                if(submenu!=1&&submenu!=0){clearCom(66);gotoXY(67,27);printf("Invalid selection.");}
            }while(submenu!=0);clearCom(66);
        }
        if(choice!=shuttle&&choice!=cargo&&choice!=freighter&&choice!=frigate&&choice!=destroyer)
        {
            clearCom(66);gotoXY(67,27);printf("Invalid selection.");
        }
    }while(choice!=0);clearCom(66);
}
void outfitterMenu(void)
{
    int i=0,choice=0,submenu=0,subsubmenu=0,subsubsubmenu=0,quantity=0,supply=2,repair=2,upgrade=2,bought=0,cost=0;
    char buymax='0';
    do
    {
        planetHUD();
        gotoXY(3,5);///moving text
        printf("Oufitter services at this location:");
        i=6;
        (outfitArray[0]==1)?i++,gotoXY(3,i),printf("%i: Repairs",i-6),repair=i-6:null();
        (outfitArray[3]==1)?i++,gotoXY(3,i),printf("%i: Resupply",i-6),supply=i-6:null();
        (outfitArray[8]==1)?i++,gotoXY(3,i),printf("%i: Upgrades",i-6),upgrade=i-6:null();
        (i==6)?gotoXY(3,7),printf("No outfitter services available."):null();
        gotoXY(3,27);(i!=6)?(i==7)?printf("1: "),(repair==1)?printf("Repair Service\t0: Back"):(supply==1)?printf("Resupply Service\t0: Back"):printf("Upgrade Service\t0: Back"):printf("1-%i: Service Selection\t0: Back",i-6):printf("0: Back");
        gotoXY(3,29);choice=getch()-'0';clearCom(66);
        if (choice==repair)//1 shileds, 2 hull, 8 energy
        {
            do
            {
                planetHUD();
                gotoXY(3,5);printf("Repairs:");
                gotoXY(3,7);printf("1: Hull Repairs (%i/hour)",outfitArray[1]*hudStats[19]);
                gotoXY(3,8);printf("2: Power Cell Replacement (%i/hour)",outfitArray[2]*hudStats[19]);
                gotoXY(3,27);printf("1-2: Select Repair\t\t0: Back");gotoXY(3,29);
                submenu=getch()-'0';clearCom(66);
                if (submenu==1)
                {
                    gotoXY(3,27);printf("Hours: ");
                    scanf("%i", &quantity);clean();clearCom(66);
                    if(hudStats[10]>quantity*outfitArray[1]*hudStats[19]&&checkCapacity(2,quantity,hudStats[19])==1)
                    {
                        hudStats[2]+=quantity;
                        hudStats[10]-=quantity*outfitArray[1]*hudStats[19];
                        printBuySell('b','h',quantity,outfitArray[1]*hudStats[19],0);
                    }
                    else
                    {
                        clearCom(66);gotoXY(67,27);printf("Insufficient credits or space.");
                        gotoXY(3,27);printf("Do you want to buy the max ammount (y/n)?");buymax=getch();
                        if (buymax=='y'){
                            if(hudStats[10]/(outfitArray[1]*hudStats[19])>=capacityArray[20+hudStats[19]]-hudStats[2]){
                                bought=capacityArray[20+hudStats[19]]-hudStats[2];
                                cost=outfitArray[1]*hudStats[19];
                                printBuySell('b','h',bought,cost,0);
                                hudStats[2]+=bought;
                                hudStats[10]-=cost*bought;
                            } ///more money than hull
                            else{
                                bought=hudStats[10]/(outfitArray[1]*hudStats[19]);
                                cost=outfitArray[1]*hudStats[19];
                                printBuySell('b','h',bought,cost,0);
                                hudStats[2]+=bought;
                                hudStats[10]-=cost*bought;
                            }
                        }
                        else {clearCom(66);}
                    }
                }
                if (submenu==2)
                {///cap = 11 == 10+ship
                    gotoXY(3,27);printf("Hours:");
                    scanf("%i", &quantity);clean();gotoXY(3,27);printf("          ");clearCom(66);
                    if(hudStats[10]>quantity*outfitArray[2]*hudStats[19]&&checkCapacity(3,quantity,hudStats[19])==1)
                    {
                        hudStats[8]+=quantity;
                        hudStats[10]-=quantity*outfitArray[2]*hudStats[19];
                        printBuySell('b','e',quantity,outfitArray[2]*hudStats[19],0);
                    }
                    else
                    {
                        clearCom(66);gotoXY(67,27);printf("Insufficient credits or space.");
                        gotoXY(3,27);printf("Do you want to buy the max ammount (y/n)?");buymax=getch();
                        if (buymax=='y'){
                            if(hudStats[10]/(outfitArray[2]*hudStats[19])>=capacityArray[30+hudStats[19]]-hudStats[8]){
                                bought=capacityArray[30+hudStats[19]]-hudStats[8];
                                cost=outfitArray[2]*hudStats[19];
                                printBuySell('b','e',bought,cost,0);
                                hudStats[8]+=bought;
                                hudStats[10]-=cost*bought;
                            } ///more money than shield
                            else{
                                bought=hudStats[10]/(outfitArray[2]*hudStats[19]);
                                cost=outfitArray[2]*hudStats[19];
                                printBuySell('b','e',bought,cost,0);
                                hudStats[8]+=bought;
                                hudStats[10]-=cost*bought;
                            }
                        }
                        else {clearCom(66);}
                    }
                }
                if(submenu!=1&&submenu!=2){clearCom(66);gotoXY(67,27);printf("Invalid selection.");}
            }while(submenu!=0);clearCom(66);
        }
        if (choice==supply)
        {
            do
            {
                planetHUD();
                gotoXY(3,5);printf("Resupply:");
                gotoXY(3,7);printf("1: Buy Torpedos (%i/unit)",outfitArray[4]);
                gotoXY(3,8);printf("2: Sell Torpedos (%i/unit)", outfitArray[4]);
                gotoXY(3,9);printf("3: Buy Weapons");
                gotoXY(3,10);printf("4: Sell Weapons");
                gotoXY(3,27);printf("1-4: Buy/Sell Supplies\t\t0: Back");gotoXY(3,29);submenu=getch()-'0';clearCom(66);
                if (submenu==1)
                {
                    gotoXY(3,27);printf("Torpedo quantity to buy: ");
                    scanf("%i", &quantity);clean();clearCom(66);
                    if(hudStats[10]<quantity*outfitArray[4]||checkCapacity(4,quantity,hudStats[19])==0)
                    {
                        clearCom(66);gotoXY(67,27);printf("Insufficient credits or space.");
                        gotoXY(3,27);printf("Do you want to buy the max ammount (y/n)?");buymax=getch();
                        if (buymax=='y'){
                            if(hudStats[10]/outfitArray[4]>=capacityArray[40+hudStats[19]]-hudStats[9]){
                                bought=capacityArray[40+hudStats[19]]-hudStats[9];
                                printBuySell('b','t',bought,outfitArray[4],0);
                                hudStats[9]+=bought;
                                hudStats[10]-=(capacityArray[40+hudStats[19]]-hudStats[9])*outfitArray[4];
                            }
                            else{
                                bought=hudStats[10]/outfitArray[4];
                                printBuySell('b','t',bought,outfitArray[4],0);
                                hudStats[9]+=bought;
                                hudStats[10]-=(hudStats[10]/outfitArray[4])*outfitArray[4];
                            }
                        }
                        else {clearCom(66);}
                    }
                    else
                    {
                        printBuySell('b','t',quantity,outfitArray[4],0);
                        hudStats[9]+=quantity;
                        hudStats[10]-=quantity*outfitArray[4];;
                    }
                }
                if(submenu==2)
                {
                    gotoXY(3,27);printf("Torpedo quantity to sell: ");
                    scanf("%i", &quantity);clean();gotoXY(3,27);printf("              ");clearCom(66);
                    if (hudStats[9]<quantity)
                    {
                        clearCom(66);gotoXY(67,27);printf("Insufficient supply");
                        gotoXY(3,27);printf("Do you want to sell all your torpedos (y/n)?");buymax=getch();
                        if (buymax=='y'){
                            printBuySell('s','t',hudStats[9],hudStats[9]*outfitArray[4],0);
                            hudStats[9]-=hudStats[9];
                            hudStats[10]-=hudStats[9]*outfitArray[4];
                        }
                        else {clearCom(66);}
                    }
                    else
                    {
                        printBuySell('s','t',quantity,quantity*outfitArray[4],0);
                        hudStats[9]-=quantity;
                        hudStats[10]+=quantity*outfitArray[4];
                    }
                }
                if(submenu==3)
                {
                    do
                    {
                        planetHUD();
                        gotoXY(3,5);printf("Weapons for sale:");
                        gotoXY(3,7);printf("1: Pew-Pew SK-337 Lance (");prefix(outfitArray[5],'n');printf(")");
                        gotoXY(15,9);printf("O---[========>");
                        gotoXY(3,12);printf("2: N00bzorz ID 10-T Killer (");prefix(outfitArray[6],'n');printf(")");
                        gotoXY(15,14);printf("CXXXX%c==%c=%c=============%c",186,18,18,175);
                        gotoXY(3,17);printf("3: Pwnzorz 1337 Cannon (");prefix(outfitArray[7],'n');printf(")");
                        gotoXY(15,19);printf("CXXXX[==%c=%c=============]",18,18);
                        gotoXY(15,20);printf("                  __|  |__");gotoXY(15,21);printf("                 |        |",92);gotoXY(15,22);printf("                 |________|");
                        gotoXY(3,27);printf("1-3: Weapon info\t\t0: Back");gotoXY(3,29);subsubmenu=getch()-'0';clearCom(66);
                        if(subsubmenu==1)
                        {
                            do
                            {
                                planetHUD();
                                gotoXY(3,5);printf("Pew-Pew SK-337 Lance\t\t");prefix(outfitArray[5],'n');
                                gotoXY(3,7);printf("Damage: 30");
                                gotoXY(3,8);printf("Range: 30km");
                                gotoXY(3,9);printf("Energy depletion: 10 MJ");
                                gotoXY(3,10);printf("Compatibility: All standard regulation federation ships.");
                                gotoXY(3,27);printf("1: Buy\t0: Back");
                                gotoXY(3,28);subsubsubmenu=getch()-'0';gotoXY(3,28);printf("  ");clearCom(66);
                                if(subsubsubmenu==1)
                                {
                                    if(hudStats[10]<outfitArray[5]||hudStats[26]==capacityArray[5+hudStats[19]])
                                    {
                                        clearCom(66);gotoXY(67,27);printf("Insufficient funds or space.");
                                    }
                                    else
                                    {
                                        cost=outfitArray[5];
                                        printBuySell('b','g',1,cost,0);
                                        hudStats[10]-=cost;
                                        for(i=1;i<6;i++){
                                            if(hudStats[20+i]==0){hudStats[20+i]=1;break;}
                                        }
                                        hudStats[20+i]=1;
                                        hudStats[26]++;
                                        achievements[4]++;
                                    }
                                }
                                if(subsubsubmenu!=0&&subsubsubmenu!=1){clearCom(66);gotoXY(67,27);printf("Invalid selection.");}
                            }while(subsubsubmenu!=0);clearCom(66);
                        }
                        if(subsubmenu==2)
                        {
                            do
                            {
                                planetHUD();
                                gotoXY(3,5);printf("N00bzorz ID10-T Killer\t\t");prefix(outfitArray[6],'n');
                                gotoXY(3,7);printf("Damage: 50");
                                gotoXY(3,8);printf("Range: 50km");
                                gotoXY(3,9);printf("Energy depletion: 50 MJ");
                                gotoXY(3,10);printf("Compatibility: F or C class federation ships.");
                                gotoXY(3,27);printf("1: Buy\t0: Back");
                                gotoXY(3,28);subsubsubmenu=getch()-'0';gotoXY(3,28);printf("  ");clearCom(66);
                                if(subsubsubmenu==1)
                                {
                                    if(hudStats[10]<outfitArray[6]||hudStats[26]==capacityArray[5+hudStats[19]]||hudStats[19]==1)
                                    {
                                        clearCom(66);gotoXY(67,27);printf("Insufficient funds, space, or compatibility.");
                                    }
                                    else
                                    {
                                        cost=outfitArray[6];
                                        printBuySell('b','g',2,cost,0);
                                        hudStats[10]-=cost;
                                        for(i=1;i<6;i++){
                                            if(hudStats[20+i]==0){hudStats[20+i]=2;break;}
                                        }
                                        hudStats[26]++;
                                        achievements[4]++;
                                    }
                                }
                                if(subsubsubmenu!=0&&subsubsubmenu!=1){clearCom(66);gotoXY(67,27);printf("Invalid selection.");}
                            }while(subsubsubmenu!=0);clearCom(66);
                        }
                        if(subsubmenu==3)
                        {
                            do
                            {
                                planetHUD();
                                gotoXY(3,5);printf("Pwnzorz 1337 Cannon\t\t");prefix(outfitArray[7],'n');
                                gotoXY(3,7);printf("Damage: 100");
                                gotoXY(3,8);printf("Range: 70km");
                                gotoXY(3,9);printf("Energy depletion: 100 MJ");
                                gotoXY(3,10);printf("Compatibility: C class federation ships.");
                                gotoXY(3,27);printf("1: Buy\t0: Back");
                                gotoXY(3,28);subsubsubmenu=getch()-'0';gotoXY(3,28);printf("  ");clearCom(66);
                                if(subsubsubmenu==1)
                                {
                                    if(hudStats[10]<outfitArray[7]||hudStats[26]==capacityArray[5+hudStats[19]]||hudStats[19]<4)
                                    {
                                        clearCom(66);gotoXY(67,27);printf("Insufficient funds, space, or compatibility.");
                                    }
                                    else
                                    {
                                        cost=outfitArray[7];
                                        printBuySell('b','g',3,cost,0);
                                        hudStats[10]-=cost;
                                        for(i=1;i<6;i++){
                                            if(hudStats[20+i]==0){hudStats[20+i]=3;break;}
                                        }
                                        hudStats[26]++;
                                        achievements[4]++;
                                    }
                                }
                                if(subsubsubmenu!=0&&subsubsubmenu!=1){clearCom(66);gotoXY(67,27);printf("Invalid selection.");}
                            }while(subsubsubmenu!=0);clearCom(66);
                        }
                        if(subsubmenu!=1&&subsubmenu!=2&&subsubmenu!=3){gotoXY(67,27);printf("Invalid selection.");}
                    }while(subsubmenu!=0);clearCom(66);
                }
                if(submenu==4)
                {
                    do
                    {
                        planetHUD();
                        gotoXY(3,5);printf("Your Weapons:");
                        gotoXY(3,7);(hudStats[21]!=0)?printf("1: Sell Type %i Weapon in slot 1 (",hudStats[21]),prefix(outfitArray[4+hudStats[21]]/2,'n'),printf(")"):printf("No weapon in slot 1");
                        gotoXY(3,8);(hudStats[22]!=0)?printf("2: Sell Type %i Weapon in slot 2 (",hudStats[22]),prefix(outfitArray[4+hudStats[22]]/2,'n'),printf(")"):printf("No weapon in slot 2");
                        gotoXY(3,9);(hudStats[23]!=0)?printf("3: Sell Type %i Weapon in slot 3 (",hudStats[23]),prefix(outfitArray[4+hudStats[23]]/2,'n'),printf(")"):printf("No weapon in slot 3");
                        gotoXY(3,10);(hudStats[24]!=0)?printf("4: Sell Type %i Weapon in slot 4 (",hudStats[24]),prefix(outfitArray[4+hudStats[24]]/2,'n'),printf(")"):printf("No weapon in slot 4");
                        gotoXY(3,11);(hudStats[25]!=0)?printf("5: Sell Type %i Weapon in slot 5 (",hudStats[25]),prefix(outfitArray[4+hudStats[25]]/2,'n'),printf(")"):printf("No weapon in slot 5");
                        gotoXY(3,13);printf("0: Back");
                        gotoXY(3,27);printf("1-5: Sell weapon in slot\t\t0: Back");gotoXY(3,29);subsubmenu=getch()-'0';gotoXY(3,27);clearCom(66);
                        if(subsubmenu>0&&subsubmenu<6&&hudStats[20+subsubmenu]==0)
                        {
                            clearCom(66);gotoXY(67,27);printf("There is no weapon in that slot.");
                        }
                        if(subsubmenu>0&&subsubmenu<6&&hudStats[20+subsubmenu]!=0)
                        {
                            cost=outfitArray[4+hudStats[20+subsubmenu]]/2;
                            printBuySell('s','g',hudStats[20+subsubmenu],cost,0);
                            hudStats[10]+=cost;
                            hudStats[20+subsubmenu]=0;hudStats[26]--;
                        }
                        if(subsubmenu<0||subsubmenu>5){clearCom(66);gotoXY(67,27);printf("Invalid selection.");}
                    }while(subsubmenu!=0);clearCom(66);
                }
                if(submenu!=1&&submenu!=2&&submenu!=3&&submenu!=4){clearCom(66);gotoXY(67,27);printf("Invalid selection.");}
            }while (submenu!=0);clearCom(66);
        }
        if (choice==upgrade)
        {
            do
            {
                planetHUD();
                gotoXY(3,5);printf("Upgrades:");
                gotoXY(3,7);printf("1: Ammunition Upgrade"); ///damage
                gotoXY(3,8);printf("2: Cannon Barrel Upgrade"); ///range
                gotoXY(3,9);printf("3: Advanced Hull Plating Upgrade"); ///damage dampening w/o shields
                gotoXY(3,10);printf("4: Oscillating Shield Upgrade"); ///dampening w/ shields
                gotoXY(3,11);printf("5: Cargo Refit"); ///capacity
                gotoXY(3,12);printf("6: Advanced Torpedo Upgrade"); ///torps
                gotoXY(3,13);printf("7: Port and Polish Engine Upgrade"); ///turns
                gotoXY(3,27);printf("1-7: Upgrade Select\t\t0: Back");gotoXY(3,29);submenu=getch()-'0';clearCom(66);
                if(submenu==1)
                {
                    do///tab upgrades
                    {
                        planetHUD();
                        gotoXY(3,5);printf("Ammunition Upgrade:");
                        gotoXY(3,7);printf("Replaces the standard issue impact");///27
                        gotoXY(3,8);printf("casings with armour piercing ones.");
                        gotoXY(3,17);printf("Effect: +10 damage per upgrade");
                        gotoXY(3,18);printf("Max: 5 upgrades");
                        gotoXY(3,19);printf("Cost: 100,000");
                        gotoXY(3,20);printf("Compatibility: C or F class ships");
                        gotoXY(3,23);printf("Currently equipped: %i",hudStats[27]);
                        gotoXY(3,27);printf("1: Buy\t0: Back");
                        gotoXY(3,28);subsubmenu=getch()-'0';gotoXY(3,28);clearCom(66);
                        if(subsubmenu==1)
                        {
                            if(hudStats[10]<100000||hudStats[27]==5||hudStats[19]==1)
                            {
                                clearCom(66);gotoXY(67,27);printf("Insufficient funds or maximum/incompatible upgrade");
                            }
                            if(hudStats[10]>100000&&hudStats[27]<5&&hudStats[19]>1)
                            {
                                clearCom(66);gotoXY(67,27);printf("You bought an ammunition upgrade");
                                gotoXY(67,28);printf("for 100k credits");
                                hudStats[10]-=100000;hudStats[27]++;
                            }
                        }
                        if(subsubmenu!=1&&subsubmenu!=0){clearCom(66);gotoXY(67,27);printf("Invalid selection.");}
                    }while(subsubmenu!=0);clearCom(66);
                }
                if(submenu==2)
                {
                    do
                    {
                        planetHUD();
                        gotoXY(3,5);printf("Cannon Barrel Upgrade:");
                        gotoXY(3,7);printf("Replaces the stock steel barrels on weapons");///28
                        gotoXY(3,8);printf("with a high efficiency carbon alloy.");
                        gotoXY(3,17);printf("Effect: +10 km range per upgrade");
                        gotoXY(3,18);printf("Max: 2 upgrades");
                        gotoXY(3,19);printf("Cost: 100,000");
                        gotoXY(3,20);printf("Compatibility: C or F class ships");
                        gotoXY(3,23);printf("Currently equipped: %i",hudStats[28]);
                        gotoXY(3,27);printf("1: Buy\t0: Back");
                        gotoXY(3,28);subsubmenu=getch()-'0';gotoXY(3,28);clearCom(66);
                        if(subsubmenu==1)
                        {
                            if(hudStats[10]<100000||hudStats[28]==2||hudStats[19]==1)
                            {
                                clearCom(66);gotoXY(67,27);printf("Insufficient funds or maximum/incompatible upgrade");
                            }
                            if(hudStats[10]>100000&&hudStats[28]<2&&hudStats[19]>1)
                            {
                                clearCom(66);gotoXY(67,27);printf("You bought an ammunition upgrade");
                                gotoXY(67,28);printf("for 100k credits");
                                hudStats[10]-=100000;hudStats[28]++;
                            }
                        }
                        if(subsubmenu!=1&&subsubmenu!=0){clearCom(66);gotoXY(67,27);printf("Invalid selection.");}
                    }while(subsubmenu!=0);clearCom(66);
                }
                if(submenu==3)
                {
                    do
                    {
                        planetHUD();
                        gotoXY(3,5);printf("Advanced Hull Plating Upgrade:");
                        gotoXY(3,7);printf("Replaces stock hull struts with a high-");///29
                        gotoXY(3,8);printf("tensile strength tritanium alloy.");
                        gotoXY(3,17);printf("Effect: +10 hull damage dampening per hit");
                        gotoXY(3,18);printf("Max: 3 upgrades");
                        gotoXY(3,19);printf("Cost: 250,000");
                        gotoXY(3,20);printf("Compatibility: C or F class ships");
                        gotoXY(3,23);printf("Currently equipped: %i",hudStats[29]);
                        gotoXY(3,27);printf("1: Buy\t0: Back");
                        gotoXY(3,28);subsubmenu=getch()-'0';gotoXY(3,28);
                        clearCom(66);
                        if(subsubmenu==1)
                        {
                            if(hudStats[10]<250000||hudStats[29]==3||hudStats[19]==1)
                            {
                                clearCom(66);gotoXY(67,27);printf("Insufficient funds or maximum/incompatible upgrade");
                            }
                            if(hudStats[10]>250000&&hudStats[29]<3&&hudStats[19]>1)
                            {
                                clearCom(66);gotoXY(67,27);printf("You bought an ammunition upgrade");
                                gotoXY(67,28);printf("for 250k credits");
                                hudStats[10]-=250000;hudStats[29]++;
                            }
                        }
                        if(subsubmenu!=1&&subsubmenu!=0){clearCom(66);gotoXY(67,27);printf("Invalid selection.");}
                    }while(subsubmenu!=0);clearCom(66);
                }
                if(submenu==4)
                {
                    do
                    {
                        planetHUD();
                        gotoXY(3,5);printf("Oscillating Shield Upgrade:");
                        gotoXY(3,7);printf("Reconfigures shielding protocols to");///30
                        gotoXY(3,8);printf("react quicker to phase fluctuations.");
                        gotoXY(3,17);printf("Effect: +10 shield damage dampening per hit");
                        gotoXY(3,18);printf("Max: 3 upgrades");
                        gotoXY(3,19);printf("Cost: 250,000");
                        gotoXY(3,20);printf("Compatibility: C or F class ships");
                        gotoXY(3,23);printf("Currently equipped: %i",hudStats[30]);
                        gotoXY(3,27);printf("1: Buy\t0: Back");
                        gotoXY(3,28);subsubmenu=getch()-'0';gotoXY(3,28);clearCom(66);
                        if(subsubmenu==1)
                        {
                            if(hudStats[10]<250000||hudStats[30]==3||hudStats[19]==1)
                            {
                                clearCom(66);gotoXY(67,27);printf("Insufficient funds or maximum/incompatible upgrade");
                            }
                            if(hudStats[10]>250000&&hudStats[30]<3&&hudStats[19]>1)
                            {
                                clearCom(66);gotoXY(67,27);printf("You bought an ammunition upgrade");
                                gotoXY(67,28);printf("for 250k credits");
                                hudStats[10]-=250000;hudStats[30]++;
                            }
                        }
                        if(subsubmenu!=1&&subsubmenu!=0){clearCom(66);gotoXY(67,27);printf("Invalid selection.");}
                    }while(subsubmenu!=0);clearCom(66);
                }
                if(submenu==5)
                {
                    do
                    {
                        planetHUD();
                        gotoXY(3,5);printf("Cargo Refit:");
                        gotoXY(3,7);printf("Reconfigures cargo bays and consolidates");///31
                        gotoXY(3,8);printf("surrounding areas of unused space.");
                        gotoXY(3,17);printf("Effect: +100 cargo space");
                        gotoXY(3,18);printf("Max: 5 upgrades");
                        gotoXY(3,19);printf("Cost: 150,000");
                        gotoXY(3,20);printf("Compatibility: C or F class ships");
                        gotoXY(3,23);printf("Currently equipped: %i",hudStats[31]);
                        gotoXY(3,27);printf("1: Buy\t0: Back");
                        gotoXY(3,28);subsubmenu=getch()-'0';gotoXY(3,28);clearCom(66);
                        if(subsubmenu==1)
                        {
                            if(hudStats[10]<150000||hudStats[31]==5||hudStats[19]==1)
                            {
                                clearCom(66);gotoXY(67,27);printf("Insufficient funds or maximum/incompatible upgrade");
                            }
                            if(hudStats[10]>150000&&hudStats[31]<5&&hudStats[19]>1)
                            {
                                clearCom(66);gotoXY(67,27);printf("You bought an ammunition upgrade");
                                gotoXY(67,28);printf("for 150k credits");
                                hudStats[10]-=150000;hudStats[31]++;
                            }
                        }
                        if(subsubmenu!=1&&subsubmenu!=0){clearCom(66);gotoXY(67,27);printf("Invalid selection.");}
                    }while(subsubmenu!=0);clearCom(66);
                }
                if(submenu==6)
                {
                    do
                    {
                        planetHUD();
                        gotoXY(3,5);printf("Advanced Torpedo Upgrade:");
                        gotoXY(3,7);printf("Exchanges standard javelin torpedos for photonic ones");///32
                        gotoXY(3,8);printf("and recalibrates torpedo bays for optimum speed.");
                        gotoXY(3,17);printf("Effect: +20 torpedo damage");
                        gotoXY(3,18);printf("Max: 3 upgrades");
                        gotoXY(3,19);printf("Cost: 500,000");
                        gotoXY(3,20);printf("Compatibility: C class ships");
                        gotoXY(3,23);printf("Currently equipped: %i",hudStats[32]);
                        gotoXY(3,27);printf("1: Buy\t0: Back");
                        gotoXY(3,28);subsubmenu=getch()-'0';gotoXY(3,28);clearCom(66);
                        if(subsubmenu==1)
                        {
                            if(hudStats[10]<500000||hudStats[32]==3||hudStats[19]<4)
                            {
                                clearCom(66);gotoXY(67,27);printf("Insufficient funds or maximum/incompatible upgrade");
                            }
                            if(hudStats[10]>500000&&hudStats[32]<3&&hudStats[19]>3)
                            {
                                clearCom(66);gotoXY(67,27);printf("You bought an ammunition upgrade");
                                gotoXY(67,28);printf("for 500k credits");
                                hudStats[10]-=500000;hudStats[32]++;
                            }
                        }
                        if(subsubmenu!=1&&subsubmenu!=0){clearCom(66);gotoXY(67,27);printf("Invalid selection.");}
                    }while(subsubmenu!=0);clearCom(66);
                }
                if(submenu==7)
                {
                    do
                    {
                        planetHUD();
                        gotoXY(3,5);printf("Port and Polish Engine Upgrade:");
                        gotoXY(3,7);printf("Standard cleaning and optimization of all");///32
                        gotoXY(3,8);printf("engine and propulsion systems. After upgrade");
                        gotoXY(3,9);printf("your ship will fly like she's just come out");
                        gotoXY(3,10);printf("of ATLAS Shipyards.");
                        gotoXY(3,17);printf("Effect: +1 move per turn");
                        gotoXY(3,18);printf("Max: 2 upgrades");
                        gotoXY(3,19);printf("Cost: 750,000");
                        gotoXY(3,20);printf("Compatibility: C class ships");
                        gotoXY(3,23);printf("Currently equipped: %i",hudStats[33]);
                        gotoXY(3,27);printf("1: Buy\t0: Back");
                        gotoXY(3,28);subsubmenu=getch()-'0';gotoXY(3,28);clearCom(66);
                        if(subsubmenu==1)
                        {
                            if(hudStats[10]<750000||hudStats[33]==2||hudStats[19]<4)
                            {
                                clearCom(66);gotoXY(67,27);printf("Insufficient funds or maximum/incompatible upgrade");
                            }
                            if(hudStats[10]>750000&&hudStats[33]<2&&hudStats[19]>3)
                            {
                                clearCom(66);gotoXY(67,27);printf("You bought an engine upgrade");
                                gotoXY(67,28);printf("for 750k credits");
                                hudStats[10]-=750000;hudStats[33]++;
                            }
                        }
                        if(subsubmenu!=1&&subsubmenu!=0){clearCom(66);gotoXY(67,27);printf("Invalid selection.");}
                    }while(subsubmenu!=0);clearCom(66);
                }
                if(submenu<0||submenu>7){clearCom(66);gotoXY(67,27);printf("Invalid Selection");}
            }while(submenu!=0);clearCom(66);
        }
        if (choice!=0&&choice!=supply&&choice!=repair&&choice!=upgrade)
        {
            clearCom(66);gotoXY(67,27);printf("Invalid selection.");
        }
    }while (choice!=0);clearCom(66);
}
void initOutfit(void)
{
    if (randomNumber(100)>20){
        outfitArray[0]=1;
        Sleep(100);printf(".");
    }
    outfitArray[1]=5+randomNumber(10);
    Sleep(100);printf(".");
    outfitArray[2]=10+randomNumber(10);
    Sleep(100);printf(".");

    if (randomNumber(100)>50){
        outfitArray[3]=1;
        Sleep(100);printf(".");
    }
    outfitArray[4]=900+randomNumber(200);///torps
    Sleep(100);printf(".");
    outfitArray[5]=80000+randomNumber(20000);///cannon 1
    Sleep(100);printf(".");
    outfitArray[6]=100000+randomNumber(100000);///cannon 2
    Sleep(100);printf(".");
    outfitArray[7]=5000000+randomNumber(5000000);///cannon 3
    Sleep(100);printf(".");

    if (randomNumber(100)>70){
        outfitArray[8]=1;
    }
}
int cargoLeft(void){return (capacityArray[hudStats[19]]+100*hudStats[31]-hudStats[11]-hudStats[12]-hudStats[13]-hudStats[14]-hudStats[15]);}
void commerceMenu(void)
{/// (1-69,1-29) <-wtf is this shit
    int choice,submenu,i,quantity,plas,duter,lat,water,ore,bought,profit;
    char buymax;
    do
    {
        planetHUD();
        gotoXY(3,5);printf("Commodities Center:");
        gotoXY(3,7);printf("Available commodities:");i=8;
        (commerceArray[0]==1)?i++,gotoXY(3,i),printf("\tWarp plasma."):null();
        (commerceArray[2]==1)?i++,gotoXY(3,i),printf("\tDuterium."):null();
        (commerceArray[4]==1)?i++,gotoXY(3,i),printf("\tLatinum."):null();
        (commerceArray[6]==1)?i++,gotoXY(3,i),printf("\tWater."):null();
        (commerceArray[8]==1)?i++,gotoXY(3,i),printf("\tUranium Ore."):null();
        (i==6)?i++,gotoXY(3,9),printf("  No commodities for sale."):null();

        gotoXY(3,27);printf("1: Buy\t\t");printf("2: Sell\t\t");printf("0: Back");
        gotoXY(3,29);choice=getch()-'0';clearCom(66);

        switch (choice)
        {
        case 1:
            do
            {
                planetHUD();
                plas=-1;duter=-1;lat=-1;water=-1;ore=-1;
                gotoXY(3,5);printf("Commodities Available for Purchase:");i=6;
                (commerceArray[0]==1)?i++,plas=i-6,gotoXY(3,i),printf("%i: Buy Warp Plasma (%i/L)",i-6,commerceArray[1]),printDealArrow(commerceArray[1],commodityLastBuy[1],1):null();
                (commerceArray[2]==1)?i++,duter=i-6,gotoXY(3,i),printf("%i: Buy Duterium (%i/kg)",i-6,commerceArray[3]),printDealArrow(commerceArray[3],commodityLastBuy[2],1):null();
                (commerceArray[4]==1)?i++,lat=i-6,gotoXY(3,i),printf("%i: Buy Latinum (%i/kg)",i-6,commerceArray[5]),printDealArrow(commerceArray[5],commodityLastBuy[3],1):null();
                (commerceArray[6]==1)?i++,water=i-6,gotoXY(3,i),printf("%i: Buy Water (%i/L)",i-6,commerceArray[7]),printDealArrow(commerceArray[7],commodityLastBuy[4],1):null();
                (commerceArray[8]==1)?i++,ore=i-6,gotoXY(3,i),printf("%i: Buy Uranium Ore (%i/kg)",i-6,commerceArray[9]),printDealArrow(commerceArray[9],commodityLastBuy[5],1):null();
                (i==6)?i++,gotoXY(3,7),printf("No available commodities."):null();
                gotoXY(3,27);(i!=7)?printf("1-%i: Select commodity\t\t0: Back",i-6):(printf("1: Buy "),(plas!=-1)?printf("Warp Plasma"):(duter!=-1)?printf("Duterium"):(lat!=-1)?printf("Latinum"):(water!=-1)?printf("Water"):printf("Uranium Ore"));
                gotoXY(3,29);submenu=getch()-'0';gotoXY(3,2);clearCom(66);
                if(submenu==plas)
                {
                    gotoXY(3,27);printf("Litres of warp plasma to purchase: ");
                    scanf("%i", &quantity);clean();clearCom(66);
                    while(quantity>100000000){gotoXY(67,27);printf("Invalid ammount, try again.");gotoXY(3,27);printf("Litres of warp plasma to purchase: ");scanf("%i",&quantity);}
                    if (hudStats[10]<(commerceArray[1]*quantity)||checkCapacity(5,quantity,hudStats[19])==0)
                    {
                        clearCom(66);gotoXY(67,27);printf("Insufficient credits or space.");
                        gotoXY(3,27);printf("Do you want to buy the max ammount(y/n)?");
                        buymax=getch();
                        if(buymax=='y'){
                            if(hudStats[10]/commerceArray[1]>=cargoLeft()){///more money that cargo
                                hudStats[10]-=cargoLeft()*commerceArray[1];
                                commodityLastBuy[1]=(cargoLeft()!=0)?commerceArray[1]:commodityLastBuy[1];
                                printBuySell('b','p',cargoLeft(),commerceArray[1],0);
                                hudStats[11]+=cargoLeft();
                            }///buy max cargo
                            else{
                                printBuySell('b','p',hudStats[10]/commerceArray[1],commerceArray[1],0);///buysell, commodity, quantity, profitLoss
                                commodityLastBuy[1]=(hudStats[10]/commerceArray[1]!=0)?commerceArray[1]:commodityLastBuy[1];
                                hudStats[11]+=hudStats[10]/commerceArray[1];
                                hudStats[10]-=hudStats[10]/commerceArray[1]*commerceArray[1];
                            }
                        }
                        else {clearCom(66);}
                    }
                    else
                    {
                        printBuySell('b','p',quantity,commerceArray[1],0);///buysell, commodity, quantity, profitLoss
                        hudStats[11]+=quantity;
                        hudStats[10]-=(commerceArray[1]*quantity);
                        commodityLastBuy[1]=(quantity!=0)?commerceArray[1]:commodityLastBuy[1];
                    }
                }
                if(submenu==duter)
                {
                    gotoXY(3,27);printf("Kilograms of duterium to purchase: ");
                    scanf("%i", &quantity);clean();clearCom(66);
                    while(quantity>100000000){gotoXY(67,27);printf("Invalid ammount, try again.");gotoXY(3,30);printf("Kilograms of duterium to purchase: ");scanf("%i",&quantity);}
                    if (hudStats[10]<(commerceArray[3]*quantity)||checkCapacity(5,quantity,hudStats[19])==0)
                    {
                        clearCom(66);gotoXY(67,27);printf("Insufficient credits or space.");
                        gotoXY(3,27);printf("Do you want to buy the max ammount(y/n)?");
                        buymax=getch();
                        if(buymax=='y'){
                            if(hudStats[10]/commerceArray[3]>=cargoLeft()){///more money that cargo
                                hudStats[10]-=cargoLeft()*commerceArray[3];
                                commodityLastBuy[2]=(cargoLeft()!=0)?commerceArray[3]:commodityLastBuy[2];
                                printBuySell('b','d',cargoLeft(),commerceArray[3],0);
                                hudStats[12]+=cargoLeft();
                            }///buy max cargo
                            else{
                                printBuySell('b','d',hudStats[10]/commerceArray[3],commerceArray[3],0);
                                commodityLastBuy[2]=(hudStats[10]/commerceArray[3]!=0)?commerceArray[3]:commodityLastBuy[2];
                                hudStats[12]+=hudStats[10]/commerceArray[3];
                                hudStats[10]-=hudStats[10]/commerceArray[3]*commerceArray[3];
                            }
                        }
                        else {clearCom(66);}
                    }
                    else
                    {
                        printBuySell('b','d',quantity,commerceArray[3],0);
                        hudStats[12]+=quantity;
                        hudStats[10]-=(commerceArray[3]*quantity);
                        commodityLastBuy[2]=(quantity!=0)?commerceArray[3]:commodityLastBuy[2];
                    }

                }
                if(submenu==lat)
                {
                    gotoXY(3,27);printf("Kilograms of latinum to purchase: ");
                    scanf("%i", &quantity);clean();clearCom(66);
                    while(quantity>100000000){gotoXY(67,27);printf("Invalid ammount, try again.");gotoXY(3,30);printf("Kilograms of latinum to purchase: ");scanf("%i",&quantity);}
                    if (hudStats[10]<(commerceArray[5]*quantity)||checkCapacity(5,quantity,hudStats[19])==0)
                    {
                        clearCom(66);gotoXY(67,27);printf("Insufficient credits or space.");
                        gotoXY(3,27);printf("Do you want to buy the max ammount(y/n)?");
                        buymax=getch();
                        if(buymax=='y'){
                            if(hudStats[10]/commerceArray[5]>=cargoLeft()){///more money that cargo
                                hudStats[10]-=cargoLeft()*commerceArray[5];
                                commodityLastBuy[3]=(cargoLeft()!=0)?commerceArray[5]:commodityLastBuy[3];
                                printBuySell('b','l',cargoLeft(),commerceArray[5],0);
                                hudStats[13]+=cargoLeft();
                            }///buy max cargo
                            else{
                                printBuySell('b','l',hudStats[10]/commerceArray[5],commerceArray[5],0);
                                commodityLastBuy[3]=(hudStats[10]/commerceArray[5]!=0)?commerceArray[5]:commodityLastBuy[3];
                                hudStats[13]+=hudStats[10]/commerceArray[5];
                                hudStats[10]-=hudStats[10]/commerceArray[5]*commerceArray[5];
                            }
                        }
                        else {clearCom(66);}
                    }
                    else
                    {
                        printBuySell('b','l',quantity,commerceArray[5],0);
                        hudStats[13]+=quantity;
                        hudStats[10]-=(commerceArray[5]*quantity);
                        commodityLastBuy[3]=(quantity!=0)?commerceArray[5]:commodityLastBuy[3];
                    }

                }
                if(submenu==water)
                {
                    gotoXY(3,27);printf("Litres of water to purchase: ");
                    scanf("%i", &quantity);clean();clearCom(66);
                    while(quantity>100000000){gotoXY(67,27);printf("Invalid ammount, try again.");gotoXY(3,27);printf("Litres of water to purchase: ");scanf("%i",&quantity);}
                    if (hudStats[10]<(commerceArray[7]*quantity)||checkCapacity(5,quantity,hudStats[19])==0)
                    {
                        clearCom(66);gotoXY(67,27);printf("Insufficient credits or space.");
                        gotoXY(3,27);printf("Do you want to buy the max ammount(y/n)?");
                        buymax=getch();
                        if(buymax=='y'){
                            if(hudStats[10]/commerceArray[7]>=cargoLeft()){///more money that cargo
                                hudStats[10]-=cargoLeft()*commerceArray[7];
                                commodityLastBuy[4]=(cargoLeft()!=0)?commerceArray[7]:commodityLastBuy[4];
                                printBuySell('b','w',cargoLeft(),commerceArray[7],0);
                                hudStats[14]+=cargoLeft();
                            }///buy max cargo
                            else{
                                printBuySell('b','w',hudStats[10]/commerceArray[7],commerceArray[7],0);
                                commodityLastBuy[4]=(hudStats[10]/commerceArray[7]!=0)?commerceArray[7]:commodityLastBuy[4];
                                hudStats[14]+=hudStats[10]/commerceArray[7];;
                                hudStats[10]-=hudStats[10]/commerceArray[7]*commerceArray[7];
                            }
                        }
                        else {clearCom(66);}
                    }
                    else
                    {
                        printBuySell('b','w',quantity,commerceArray[7],0);
                        hudStats[14]+=quantity;
                        hudStats[10]-=(commerceArray[7]*quantity);
                        commodityLastBuy[4]=(quantity!=0)?commerceArray[7]:commodityLastBuy[4];
                    }
                }
                if(submenu==ore)
                {
                    gotoXY(3,27);printf("Kilograms of uranium ore to purchase: ");
                    scanf("%i", &quantity);clean();clearCom(66);
                    while(quantity>100000000){gotoXY(67,27);printf("Invalid ammount, try again.");gotoXY(3,27);printf("Kilograms of uranium ore to purchase: ");scanf("%i",&quantity);}
                    if (hudStats[10]<(commerceArray[9]*quantity)||checkCapacity(5,quantity,hudStats[19])==0)
                    {
                        clearCom(66);gotoXY(67,27);printf("Insufficient credits or space.");
                        gotoXY(3,27);printf("Do you want to buy the max ammount(y/n)?");
                        buymax=getch();
                        if(buymax=='y'){
                            if(hudStats[10]/commerceArray[9]>=cargoLeft()){///more money that cargo
                                hudStats[10]-=cargoLeft()*commerceArray[9];
                                commodityLastBuy[5]=(cargoLeft()!=0)?commerceArray[9]:commodityLastBuy[5];
                                printBuySell('b','u',cargoLeft(),commerceArray[9],0);
                                hudStats[15]+=cargoLeft();
                            }///buy max cargo
                            else{
                                printBuySell('b','u',hudStats[10]/commerceArray[9],commerceArray[9],0);
                                commodityLastBuy[5]=(hudStats[10]/commerceArray[9]!=0)?commerceArray[9]:commodityLastBuy[5];
                                hudStats[15]+=hudStats[10]/commerceArray[9];
                                hudStats[10]-=(hudStats[10]/commerceArray[9])*commerceArray[9];
                            }
                        }
                        else {clearCom(66);}
                    }
                    else
                    {
                        printBuySell('b','u',quantity,commerceArray[9],0);
                        hudStats[15]+=quantity;
                        hudStats[10]-=(commerceArray[9]*quantity);
                        commodityLastBuy[5]=(quantity!=0)?commerceArray[9]:commodityLastBuy[5];
                    }

                }
                if(submenu!=plas&&submenu!=duter&&submenu!=lat&&submenu!=water&&submenu!=ore){clearCom(66);gotoXY(67,27);printf("Invalid selection.");}
            }while(submenu!=0);clearCom(66);
            break;///plas,duter,lat,water,ore
        case 2:
            do
            {
                planetHUD();///11-15
                gotoXY(3,5);printf("Commodities available to Sell:");
                i=6,plas=-1,duter=-1,lat=-1,water=-1,ore=-1;
                (hudStats[11]!=0)?(i++,gotoXY(3,i),plas=i-6,(commerceArray[0]==1)?(printf("%i: Sell Warp Plasma (%i/L)",i-6,commerceArray[1]),printDealArrow(commerceArray[1],commodityLastBuy[1],2)):(printf("%i: Sell Warp Plasma (%i/L)",i-6,commerceArray[1]*2),printDealArrow(2,1,2))):null();
                (hudStats[12]!=0)?(i++,gotoXY(3,i),duter=i-6,(commerceArray[2]==1)?(printf("%i: Sell Duterium (%i/kg)",i-6,commerceArray[3]),printDealArrow(commerceArray[3],commodityLastBuy[2],2)):(printf("%i: Sell Duterium (%i/kg)",i-6,commerceArray[3]*2),printDealArrow(2,1,2))):null();
                (hudStats[13]!=0)?(i++,gotoXY(3,i),lat=i-6,(commerceArray[4]==1)?(printf("%i: Sell Latinum (%i/kg)",i-6,commerceArray[5]),printDealArrow(commerceArray[5],commodityLastBuy[3],2)):(printf("%i: Sell Latinum (%i/kg)",i-6,commerceArray[5]*2),printDealArrow(2,1,2))):null();
                (hudStats[14]!=0)?(i++,gotoXY(3,i),water=i-6,(commerceArray[6]==1)?(printf("%i: Sell Water (%i/L)",i-6,commerceArray[7]),printDealArrow(commerceArray[7],commodityLastBuy[4],2)):(printf("%i: Sell Water (%i/L)",i-6,commerceArray[7]*2),printDealArrow(2,1,2))):null();
                (hudStats[15]!=0)?(i++,gotoXY(3,i),ore=i-6,(commerceArray[8]==1)?(printf("%i: Sell Uranium Ore (%i/kg)",i-6,commerceArray[9]),printDealArrow(commerceArray[9],commodityLastBuy[5],2)):(printf("%i: Sell Uranium Ore (%i/kg)",i-6,commerceArray[9]*2),printDealArrow(2,1,2))):null();
                gotoXY(3,27);(i>7)?printf("1-%i: Select commodity\t\t0: Back",i-6):(i==6)?gotoXY(3,7),printf("No commodities to sell."),gotoXY(3,27),printf("0: Back"):(printf("1: Sell "),(plas!=-1)?printf("Warp Plasma"):(duter!=-1)?printf("Duterium"):(lat!=-1)?printf("Latinum"):(water!=-1)?printf("Water"):printf("Uranium Ore"),printf("\t\t0: Back"));
                gotoXY(3,29);submenu=getch()-'0';gotoXY(3,27);clearCom(66);
                if(submenu==plas&&hudStats[11]!=0)
                {
                    gotoXY(3,27);printf("Litres of warp plasma to sell: ");
                    scanf("%i", &quantity);clean();clearCom(66);
                    while(quantity>100000000){gotoXY(67,27);printf("Invalid ammount, try again.");gotoXY(3,27);printf("Litres of warp plasma to sell: ");scanf("%i",&quantity);}
                    if(hudStats[11]<quantity)
                    {
                        clearCom(66);gotoXY(67,27);printf("Insufficient supply.");
                        gotoXY(3,27);printf("Do you want to sell all your Warp Plasma (y/n)?");
                        buymax=getch();
                        if(buymax=='y'){
                            profit=(commerceArray[0]==1)?(commerceArray[1]*hudStats[11]):(commerceArray[1]*2*hudStats[11]);
                            hudStats[10]+=profit;
                            printBuySell('s','p',hudStats[11],profit,0);
                            hudStats[11]=0;
                            commodityLastBuy[1]=(hudStats[11]==0)?capacityArray[46]:commodityLastBuy[1];
                        }
                        else {clearCom(66);}
                    }
                    else
                    {
                        profit=(commerceArray[0]==1)?(commerceArray[1]*quantity):(commerceArray[1]*2*quantity);
                        printBuySell('s','p',quantity,profit,0);
                        hudStats[11]-=quantity;
                        hudStats[10]+=profit;
                        commodityLastBuy[1]=(hudStats[11]==0)?capacityArray[46]:commodityLastBuy[1];
                    }
                }
                if(submenu==duter&&hudStats[12]!=0)
                {
                    gotoXY(3,27);printf("Kilograms of duterium to sell: ");
                    scanf("%i", &quantity);clean();clearCom(66);
                    while(quantity>100000000){gotoXY(67,27);printf("Invalid ammount, try again.");gotoXY(3,30);printf("Kilograms of duterium to sell: ");gotoXY(19,27);scanf("%i",&quantity);}
                    if(hudStats[12]<quantity)
                    {
                        clearCom(66);gotoXY(67,27);printf("Insufficient supply");
                        gotoXY(3,27);printf("Do you want to sell all your Duterium (y/n)?");
                        buymax=getch();
                        if(buymax=='y'){
                            profit=(commerceArray[2]==1)?(commerceArray[3]*hudStats[12]):(commerceArray[3]*2*hudStats[12]);
                            hudStats[10]+=profit;
                            printBuySell('s','d',hudStats[12],profit,0);
                            hudStats[12]=0;
                            commodityLastBuy[2]=(hudStats[12]==0)?capacityArray[47]:commodityLastBuy[2];
                        }
                        else {clearCom(66);}
                    }
                    else
                    {
                        profit=(commerceArray[2]==1)?(commerceArray[3]*quantity):(commerceArray[3]*2*quantity);
                        printBuySell('s','d',quantity,profit,0);
                        hudStats[12]-=quantity;
                        hudStats[10]+=profit;
                        commodityLastBuy[2]=(hudStats[12]==0)?capacityArray[47]:commodityLastBuy[2];
                    }
                }
                if(submenu==lat&&hudStats[13]!=0)
                {
                    gotoXY(3,27);printf("Kilograms of latinum to sell: ");
                    scanf("%i", &quantity);clean();clearCom(66);
                    while(quantity>100000000){gotoXY(67,27);printf("Invalid ammount, try again.");gotoXY(3,30);printf("Kilograms of latinum to sell: ");scanf("%i",&quantity);}
                    if(hudStats[13]<quantity)
                    {
                        clearCom(66);gotoXY(67,27);printf("Insufficient supply");
                        gotoXY(3,27);printf("Do you want to sell all your Latinum (y/n)?");
                        buymax=getch();
                        if(buymax=='y'){
                            profit=(commerceArray[4]==1)?(commerceArray[5]*hudStats[13]):(commerceArray[5]*2*hudStats[13]);
                            hudStats[10]+=profit;
                            printBuySell('s','l',hudStats[13],profit,0);
                            hudStats[13]=0;
                            commodityLastBuy[3]=(hudStats[13]==0)?capacityArray[48]:commodityLastBuy[3];
                        }
                        else {clearCom(66);}
                    }
                    else
                    {
                        profit=(commerceArray[4]==1)?(commerceArray[5]*quantity):(commerceArray[5]*2*quantity);
                        printBuySell('s','l',quantity,profit,0);
                        hudStats[13]-=quantity;
                        hudStats[10]+=profit;
                        commodityLastBuy[3]=(hudStats[13]==0)?capacityArray[48]:commodityLastBuy[3];
                    }
                }
                if(submenu==water&&hudStats[14]!=0)
                {
                    gotoXY(3,27);printf("Litres of water to sell: ");
                    scanf("%i", &quantity);clean();clearCom(66);
                    while(quantity>100000000){gotoXY(67,27);printf("Invalid ammount, try again.");gotoXY(3,27);printf("Litres of water to sell: ");scanf("%i",&quantity);}
                    if(hudStats[14]<quantity)
                    {
                        clearCom(66);gotoXY(67,27);printf("Insufficient supply");
                        gotoXY(3,27);printf("Do you want to sell all your Duterium (y/n)?");
                        buymax=getch();
                        if(buymax=='y'){
                            profit=(commerceArray[6]==1)?(commerceArray[7]*hudStats[14]):(commerceArray[7]*2*hudStats[14]);
                            hudStats[10]+=profit;
                            printBuySell('s','w',hudStats[14],profit,0);
                            hudStats[14]=0;
                            commodityLastBuy[4]=(hudStats[14]==0)?capacityArray[49]:commodityLastBuy[4];
                        }
                        else {clearCom(66);}
                    }
                    else
                    {
                        profit=(commerceArray[6]==1)?(commerceArray[7]*quantity):(commerceArray[7]*2*quantity);
                        printBuySell('s','w',quantity,profit,0);
                        hudStats[14]-=quantity;
                        hudStats[10]+=profit;
                        commodityLastBuy[4]=(hudStats[14]==0)?capacityArray[49]:commodityLastBuy[4];
                    }
                }
                if(submenu==ore&&hudStats[15]!=0)
                {
                    gotoXY(3,27);printf("Kilograms of uranium ore to sell: ");
                    scanf("%i", &quantity);clean();clearCom(66);
                    while(quantity>100000000){gotoXY(67,27);printf("Invalid ammount, try again.");gotoXY(3,30);printf("Kilograms of uranium ore to sell: ");scanf("%i",&quantity);}
                    if(hudStats[15]<quantity)
                    {
                        clearCom(66);gotoXY(67,27);printf("Insufficient supply");
                        gotoXY(3,27);printf("Do you want to sell all your Uranium Ore (y/n)?");
                        buymax=getch();
                        if(buymax=='y'){
                            profit=(commerceArray[8]==1)?(commerceArray[9]*hudStats[15]):(commerceArray[9]*2*hudStats[15]);
                            hudStats[10]+=profit;
                            printBuySell('s','u',hudStats[15],profit,0);
                            hudStats[15]=0;
                            commodityLastBuy[5]=(hudStats[15]==0)?capacityArray[50]:commodityLastBuy[5];
                        }
                        else {clearCom(66);}
                    }
                    else
                    {
                        profit=(commerceArray[8]==1)?(commerceArray[9]*quantity):(commerceArray[9]*2*quantity);
                        printBuySell('s','u',quantity,profit,0);
                        hudStats[15]-=quantity;
                        hudStats[10]+=profit;
                        commodityLastBuy[5]=(hudStats[15]==0)?capacityArray[50]:commodityLastBuy[5];
                    }
                }
                if(submenu!=plas&&submenu!=duter&&submenu!=lat&&submenu!=water&&submenu!=ore){clearCom(66);gotoXY(67,27);printf("Invalid selection.");}
            }while(submenu!=0);clearCom(66);
            break;
        default:
            if (choice!=0)
            {
                clearCom(66);gotoXY(67,27);printf("Invalid selection.");
            }
            break;
        }
    }while(choice!=0);clearCom(66);
}//printBuySell('s','u',hudStats[15],profit);
void printBuySell(char buySell,char commodity,int quantity,int costProfit,int shipCost){
    char hudStat,unit;
    if (commodity=='p'){hudStat=11;unit='L';}
    if (commodity=='d'){hudStat=12;unit='g';}
    if (commodity=='l'){hudStat=13;unit='g';}
    if (commodity=='w'){hudStat=14;unit='L';}
    if (commodity=='u'){hudStat=15;unit='g';}
    if (commodity=='h'){hudStat=2;unit='n';}
    if (commodity=='e'){hudStat=8;unit='n';}
    if (commodity=='t'){hudStat=9;unit='n';}
    clearCom(66);gotoXY(67,27);

    if (commodity=='g'&&buySell=='b'){printf("You bought a class %i cannon for ",quantity);prefix(costProfit,'n');printf(" credits.");gotoXY(67,28);printf("Should look nice on that shiny ");shipClass(hudStats[19]);printf(".");return;}
    if (commodity=='g'&&buySell=='s'){printf("You sold your class %i cannon for ",quantity);prefix(costProfit,'n');printf(" credits.");return;}
    if (commodity=='s'){printf("You sold your ");shipClass(quantity);printf(" and bought a");
    gotoXY(67,28);shipClass(costProfit);printf(" for ");prefix(shipCost,'n');printf(" credits.");return;}

    if(buySell=='s'&&quantity!=0){printf("You sold ");prefix(quantity,unit);}
    if(buySell=='s'&&quantity==0){printf("You did not sell any ");}
    if(buySell=='b'&&quantity!=0){printf("You bought ");prefix(quantity,unit);}
    if(buySell=='b'&&quantity==0){printf("You did not purchase any ");}
    if(commodity=='p'){(quantity!=0)?printf(" of Warp Plasma."):printf("Warp Plasma");}
    if(commodity=='d'){(quantity!=0)?printf(" of Duterium."):printf("Duterium");}
    if(commodity=='l'){(quantity!=0)?printf(" of Latinum."):printf("Latinum");}
    if(commodity=='w'){(quantity!=0)?printf(" of Water."):printf("Water");}
    if(commodity=='u'){(quantity!=0)?printf(" of Uranium Ore."):printf("Uranium Ore");}
    if(commodity=='h'){(quantity!=0)?printf(" hours of hull repairs."),hudStats[0]+=quantity/24:printf("hull repairs.");}
    if(commodity=='e'){(quantity!=0)?printf(" hours of energy cell replacements."),hudStats[0]+=quantity/24:(commodity=='e'&&quantity==0)?printf("energy cell replacements."):null();}
    if(commodity=='t'){(quantity!=0)?printf(" torpedos."):printf("torpedos.");}
    gotoXY(67,28);
    if(buySell=='s'&&quantity!=0){printf("You made ");prefix(costProfit,'n');printf(" credits.");}
    if(buySell=='b'&&quantity!=0){printf("You paid ");prefix(quantity*costProfit,'n');printf(" credits.");}
}
void initCommerce(void){
    if (randomNumber(100)>20)
    {
        commerceArray[0]=1;
    }
    Sleep(100);printf(".");
    commerceArray[1]=5+randomNumber(15);
    if (randomNumber(100)>30)
    {
        commerceArray[2]=1;
    }
    Sleep(100);printf(".");
    commerceArray[3]=20+randomNumber(30);
    if (randomNumber(100)>40)
    {
        commerceArray[4]=1;
    }
    Sleep(100);printf(".");
    commerceArray[5]=70+randomNumber(30);
    if (randomNumber(100)>50)
    {
        commerceArray[6]=1;
    }
    Sleep(100);printf(".");
    commerceArray[7]=350+randomNumber(150);
    if (randomNumber(100)>75)
    {
        commerceArray[8]=1;
    }
    commerceArray[9]=700+randomNumber(300);
}
void shipClass(int shipType)
{
    switch(shipType)
    {
    case 1:
        printf("Shuttle (P)");
        break;
    case 2:
        printf("Cargo Drone (F)");
        break;
    case 3:
        printf("Freighter (F)");
        break;
    case 4:
        printf("Frigate (C)");
        break;
    case 5:
        printf("Destroyer (C)");
        break;
    }
}
void initShips(void)
{
    if (randomNumber(100)>20)
    {
        shipArray[0]=1;
    }
    Sleep(100);printf(".");
    shipArray[1]=10000+randomNumber(1000);
    if (randomNumber(100)>40)
    {
        shipArray[2]=1;
    }
    Sleep(100);printf(".");
    shipArray[3]=100000+randomNumber(10000);
    if (randomNumber(100)>60)
    {
        shipArray[4]=1;
    }
    Sleep(100);printf(".");
    shipArray[5]=1000000+randomNumber(100000);
    if (randomNumber(100)>80)
    {
        shipArray[6]=1;
    }
    Sleep(100);printf(".");
    shipArray[7]=10000000+randomNumber(10000000);
    if (randomNumber(100)>90)
    {
        shipArray[8]=1;
    }
    Sleep(100);printf(".");
    shipArray[9]=100000000+randomNumber(100000000);
}
int checkCapacity(int slot,int value,int ship)
{
    int check=1;
    if(((slot==1||slot==2)&&(value+hudStats[slot]>capacityArray[10*slot+ship]))||((slot==3||slot==4)&&(value+hudStats[slot+5]>capacityArray[10*slot+ship])))
    {
        check=0;
    }
    if(slot==5&&(value+hudStats[11]+hudStats[12]+hudStats[13]+hudStats[14]+hudStats[15]+100*hudStats[31])>capacityArray[ship])
    {
        check=0;
    }
    return check;
}
void shipInfo(int ship)
{
    int total;
    gotoXY(3,7);printf("Cost before resale: ");prefix(shipArray[ship*2-1],'n');
    gotoXY(3,9);shipClass(ship);gotoXY(40,9);printf("Your Ship:");
    gotoXY(3,11);printf("Shield Power: ");gotoXY(26,11);printf("%i",capacityArray[10+ship]);gotoXY(40,11);printf("%i",capacityArray[10+hudStats[19]]);
    gotoXY(3,12);printf("Hull Strength: ");gotoXY(26,12);printf("%i",capacityArray[20+ship]);gotoXY(40,12);printf("%i",capacityArray[20+hudStats[19]]);
    gotoXY(3,13);printf("Battery Charge: ");gotoXY(26,13);printf("%i",capacityArray[30+ship]);gotoXY(40,13);printf("%i",capacityArray[30+hudStats[19]]);
    gotoXY(3,14);printf("Torpedo Bay Capacity: ");gotoXY(26,14);printf("%i",capacityArray[40+ship]);gotoXY(40,14);printf("%i",capacityArray[40+hudStats[19]]);
    gotoXY(3,15);printf("Cargo Hold Capacity: ");gotoXY(26,15);printf("%i",capacityArray[ship]);gotoXY(40,15);printf("%i",capacityArray[hudStats[19]]);
    gotoXY(3,16);printf("Turret Hardpoints: ");gotoXY(26,16);printf("%i",capacityArray[5+ship]);gotoXY(40,16);printf("%i",capacityArray[5+hudStats[19]]);
    gotoXY(3,18);printf("Current Ship Value: ");prefix(resaleValue(),'n');
    gotoXY(3,19);printf("Total Cost: ");(shipArray[ship*2-1]-resaleValue()>0)?prefix(shipArray[ship*2-1]-resaleValue(),'n'):printf("0");
}
int resaleValue(void)///torps???????!?!?!?
{
    int guns=0,i,upgrades;
    float shipCondition;
    upgrades=50000*(hudStats[27]+hudStats[28])+125000*(hudStats[29]+hudStats[30])+75000*hudStats[31]+250000*hudStats[32];
    shipCondition=(hudStats[1]+hudStats[2]+hudStats[8])/(capacityArray[hudStats[19]+10]+capacityArray[hudStats[19]+20]+capacityArray[hudStats[19]+30]);
    if(achievements[3]!=1){
        for(i=1;i<=5;i++)
        {
            guns+=(hudStats[20+i]==1)?outfitArray[5]:0;
            guns+=(hudStats[20+i]==2)?outfitArray[6]:0;
            guns+=(hudStats[20+i]==3)?outfitArray[7]:0;
        }
        return ((shipCondition*shipArray[2*hudStats[19]-1]/4)+shipArray[2*hudStats[19]-1]/4+guns/2+upgrades+(hudStats[9]*outfitArray[4])/2);
    }
    return ((shipCondition*shipArray[2*hudStats[19]-1]/4)+shipArray[2*hudStats[19]-1]/4);
}
void enemyMovement(void)
{
    int xDistance,yDistance,lastSpot;
    xDistance=(enemyArray[0]%10-hudStats[20]%10>=0)?enemyArray[0]%10-hudStats[20]%10:hudStats[20]%10-enemyArray[0]%10;
    yDistance=(enemyArray[0]/10-hudStats[20]/10>=0)?enemyArray[0]/10-hudStats[20]/10:hudStats[20]/10-enemyArray[0]/10;
    if (xDistance>=yDistance)
    {
        if(enemyArray[0]%10<hudStats[20]%10&&systemMap[enemyArray[0]+1]!='P')
        {
            lastSpot=enemyArray[2];
            enemyArray[2]=systemMap[enemyArray[0]+1];
            systemMap[enemyArray[0]+1]=enemyArray[1];
            systemMap[enemyArray[0]]=lastSpot;
        }
        if(enemyArray[0]%10>hudStats[20]%10&&systemMap[enemyArray[0]-1]!='P')
        {
            lastSpot=enemyArray[2];
            enemyArray[2]=systemMap[enemyArray[0]-1];
            systemMap[enemyArray[0]-1]=enemyArray[1];
            systemMap[enemyArray[0]]=lastSpot;
        }
    }
    if (xDistance<yDistance)
    {
        if(enemyArray[0]/10<hudStats[20]/10&&systemMap[enemyArray[0]+10]!='P')
        {
            lastSpot=enemyArray[2];
            enemyArray[2]=systemMap[enemyArray[0]+10];
            systemMap[enemyArray[0]+10]=enemyArray[1];
            systemMap[enemyArray[0]]=lastSpot;
        }
        if(enemyArray[0]/10>hudStats[20]/10&&systemMap[enemyArray[0]-10]!='P')
        {
            lastSpot=enemyArray[2];
            enemyArray[2]=systemMap[enemyArray[0]-10];
            systemMap[enemyArray[0]-10]=enemyArray[1];
            systemMap[enemyArray[0]]=lastSpot;
        }
    }
}
void scan(int player,int ship,int scan)
{
    int i;
    if(player==1)/// F can scan shields/hull, C weaps, range, P no scans
    {
        clearCom(51);///2=none,1=limited,0=out of range,3=in range
        gotoXY(52,27);
        (rangeCheck(1,1)!=0&&hudStats[7]!=1)?printf("Potential Damage Output: %i",rangeCheck(1,1)),gotoXY(52,28):null();///2=none,1=limited,0=out of range,3=in range
        (hudStats[26]==0)?printf("You have no weapons."):((hudStats[7]==1)?printf("No enemies nearby."):((hudStats[39]==0)?printf("Hostile Out of Range."):printf("Hostile In Range.")));
        (hudStats[38]==1&&hudStats[7]!=1&&hudStats[26]!=0)?gotoXY(52,29),printf("Limited energy available."):(hudStats[38]==2&&hudStats[7]!=1&&hudStats[26]!=0)?gotoXY(52,28),printf("Insufficient energy available."):null();
        (scan==1)?printMap(1,1):null();
    }
    if(player==2)
    {
        clearCom(51);gotoXY(52,27);
        if(enemyArray[1]>0&&enemyArray[1]<6)
        {
            if(ship==0||ship==1){printf("No scanning capabilities in this ship.");printMap(1,0);return;}
            if(ship==2||ship==3){printf("Shield:%i Hull:%i",enemyArray[8],enemyArray[9]);}
            if(ship==4||ship==5){printf("Gun Types: ");
            for(i=3;i<=enemyArray[1]+2;i++){(enemyArray[i]!=0)?printf("%i ",enemyArray[i]):null();}///printf("%i:%i",i,enemyArray[i]);}
            gotoXY(52,28);printf("Shield Power:%i\tHull Strength:%i",enemyArray[8],enemyArray[9]);}
            gotoXY(52,29);printf("Potential Damage Output: %i",rangeCheck(2,1));
            gotoXY(52,30);(rangeCheck(2,1)==0)?printf("Out of Range."):printf("In Range.");///8,9
            printMap(1,2);
        }
        else{printf("No hostiles to scan.");}
    }
}
int rangeCheck(int player,int scans)
{
    int i,range1=0,range2=0,range3=0,xDifference,yDifference,damage=0,energyDepletion=0,track1=0,track2=0,track3=0;///hudstats21-25 weaps, 28 range up  27dmg///4,6,10//20,50,100
    float distance;
	hudStats[38]=0;///reinit energy depletion
    hudStats[39]=0;///reinit guns in range
    for(i=1;i<=5;i++)
    {
        ((hudStats[20+i]==1&&player==1)||(enemyArray[i+2]==1&&player==2))?range1++:null();
        ((hudStats[20+i]==2&&player==1)||(enemyArray[i+2]==2&&player==2))?range2++:null();
        ((hudStats[20+i]==3&&player==1)||(enemyArray[i+2]==3&&player==2))?range3++:null();
    }
    xDifference=(enemyArray[0]%10-hudStats[20]%10>0)?enemyArray[0]%10-hudStats[20]%10:hudStats[20]%10-enemyArray[0]%10;
    yDifference=(enemyArray[0]/10-hudStats[20]/10>0)?enemyArray[0]/10-hudStats[20]/10:hudStats[20]/10-enemyArray[0]/10;
    distance=sqrt(pow((double)xDifference,2)+pow((double)yDifference,2));
    distance=(distance-(int)distance>=0.5)?distance+(1-(distance-(int)distance)):distance-(distance-(int)distance);
    if(player==1)
    {
        (distance<=3+hudStats[28])?hudStats[39]+=range1:null();
        (distance<=5+hudStats[28])?hudStats[39]+=range2:null();
        (distance<=7+hudStats[28])?hudStats[39]+=range3:null();
        energyDepletion+=10*range1;energyDepletion+=50*range2;energyDepletion+=100*range3;
        if(energyDepletion>hudStats[8])
        {
            energyDepletion=hudStats[8];
            for(i=0;i<5;i++)
            {
                if(energyDepletion>=100*(range3-i)&&(range3-i)>0&&track1==0)
                {
                    damage+=(range3-i)*100;
                    energyDepletion-=(range3-i)*100;
                    hudStats[38]+=energyDepletion;
                    track1=1;
                }
            }
            for(i=0;i<5;i++)
            {
                if(energyDepletion>=50*(range2-i)&&(range2-i)>0&&track2==0)
                {
                    damage+=(range2-i)*50;
                    energyDepletion-=(range2-i)*50;
                    hudStats[38]+=energyDepletion;
                    track2=1;
                }
            }
            for(i=0;i<5;i++)
            {
                if(energyDepletion>=10*(range1-i)&&(range1-i)>0&&track3==0)
                {
                    damage+=(range1-i)*30;
                    energyDepletion-=(range1-i)*10;
                    hudStats[38]+=energyDepletion;
                    track3=1;
                }
            }
            (scans==0)?hudStats[8]=energyDepletion:null();
            hudStats[38]=(scans==1)?(track1==0&&track2==0&&track3==0)?2:1:hudStats[38];
        }
        else
        {
            (distance<=3)?damage+=30*range1:null();///0=fire,1=scan,2=info
            (distance<=5)?damage+=50*range2:null();
            (distance<=7)?damage+=100*range3:null();
            (scans==0)?hudStats[8]-=energyDepletion:null();///1=none 2=lim
        }
        (damage!=0)?damage+=hudStats[27]*10:null();
        (scans==2)?hudStats[38]=10*range1+50*range2+100*range3,damage=30*range1+50*range2+100*range3:null();
        hudStats[39]=(scans==2)?(range3!=0)?7:(range2!=0)?5:(range1!=0)?3:0:hudStats[39];
    }
    if(player==2)
    {
        (distance<=3)?damage+=30*range1:null();
        (distance<=5)?damage+=50*range2:null();
        (distance<=7)?damage+=100*range3:null();
    }
    return damage;
}
void fire(int playerFiring)
{
    int damage,crit;
    if(hudStats[7]==1)
    {
        ///gotoXY(52,27);printf("There are no hostiles to shoot at.");
        return;
    }
    if(rangeCheck(playerFiring,1)!=0)
    {
        clearCom(51);
        randomNumber(100);randomNumber(100);randomNumber(100);
        if(randomNumber(100)<10&&rangeCheck(playerFiring,1)!=0)
        {
            gotoXY(52,27);(playerFiring==1)?printf("You miss. "):printf("Enemy misses.");
            rangeCheck(playerFiring,0);
            return;
        }
        damage=rangeCheck(playerFiring,1);
        damage*=(playerFiring==2)?0.9:1;
        damage-=damage/10+randomNumber(damage/5);
        crit=randomNumber(100);
        if(crit>=95&&rangeCheck(playerFiring,1)!=0)
        {
            damage*=2;
            gotoXY(52,27);(playerFiring==1)?printf("You hit a critical system! "):printf("Enemy hits a critical system! ");
        }
        if(crit<95&&rangeCheck(playerFiring,1)!=0)
        {
            gotoXY(52,27);(playerFiring==1)?printf("You hit. "):printf("Enemy hits. ");
            (playerFiring==1)?hitAnim(2):hitAnim(1);
        }
        (damage!=0)?rangeCheck(playerFiring,0),gotoXY(52,28),printf("%i damage was done.",damage):null();
        damageReport(damage,playerFiring);
    }
}
void firetorpedo(void)
{
    int damage=0,xDifference,yDifference;
	float distance=0;
	int nothing=clearCom(51);
	nothing=gotoXY(52,27);
    randomNumber(100);randomNumber(100);randomNumber(100);
    if(hudStats[9]==0){
        printf("You have no torpedos to fire.");return;}
    xDifference=(enemyArray[0]%10-hudStats[20]%10>0)?enemyArray[0]%10-hudStats[20]%10:hudStats[20]%10-enemyArray[0]%10;
    yDifference=(enemyArray[0]/10-hudStats[20]/10>0)?enemyArray[0]/10-hudStats[20]/10:hudStats[20]/10-enemyArray[0]/10;
    distance=sqrt(pow((double)xDifference,2)+pow((double)yDifference,2));///30,50,100
    ///float maxDistance=sqrt(162);///32,+20 ///distance=2-12.7 ~ 10.7-0
    damage=50-3*randomNumber((int)distance)+2*randomNumber((int)distance)+20*hudStats[32];

    if(randomNumber(100)<2*(int)distance){
        printf("Torpedo missed.");hudStats[9]--;return;}
    else
    {
        if(randomNumber(100)>95)
        {
            printf("Torpedo hits critical systems.");
            damage*=2;
        }
        else
        {
            printf("Torpedo hits.");
        }
        damageReport(damage,1);
        gotoXY(52,28);printf("Enemy takes %i damage.",damage);
        if(distance<2)
        {
            printf(" You take %i splash damage.",damage/2);
            damageReport(damage/2,2);
        }
    }
    hudStats[9]--;
}
void damageReport(int damage,int player)
{
    if(player==2)///hull:2[29] shields:1[30]
    {
        if(damage-10*hudStats[30]>hudStats[1])///shields down
        {
            if(damage-hudStats[1]-10*hudStats[29]>=hudStats[2])///death
            {
                gotoXY(52,29);printf("Your ship was destroyed.");
                shipDestroyed();
                return;
            }
            if(damage-hudStats[1]-10*hudStats[29]<hudStats[2])///hull damage
            {
                hudStats[2]-=damage-hudStats[1]-10*hudStats[29];
                hudStats[1]=0;
                return;
            }
        }
        if(damage-10*hudStats[30]<=hudStats[1])///shields damaged
        {
            hudStats[1]-=damage-10*hudStats[30];
            return;
        }
    }
    if(player==1)///hull:9 shields:8
    {
        if(damage>enemyArray[8])///shields down
        {
            if(damage-enemyArray[8]>=enemyArray[9])///death
            {
                enemyArray[8]=0;enemyArray[9]=0;
                gotoXY(52,29);printf("The enemy ship was destroyed.");
                achievements[5]++;
                hudStats[7]=1;initEnemy(1);
                return;
            }
            if(damage-enemyArray[8]<enemyArray[9])///hull damage
            {
                enemyArray[9]-=damage-enemyArray[8];
                enemyArray[8]=0;
                return;
            }
        }
        if(damage<=enemyArray[8])///shields damaged
        {
            enemyArray[8]-=damage;
            return;
        }
    }
}
void hudColours(int currentHud,int maxHud)///3,5,9
{
    if(maxHud!=0)
    {
        int percent=currentHud*100/maxHud; 
        (percent==100)?SetColor(15):(percent>75)?SetColor(10):(percent>50)?SetColor(14):SetColor(12);
    }
    printf("%i",currentHud);SetColor(15);printf("/%i",maxHud);
}
int initEnemy(int died)
{
    int i,location,ship;
    (died==1)?ship=enemyArray[1]+5:null();
    (died==1)?location=enemyArray[0]:null();
    for(i=0;i<10;i++)
    {
        (i!=2)?enemyArray[i]=0:null();
    }
    (died==1)?systemMap[location]=ship:null();
    (died==1)?enemyArray[1]=ship:null();
    (died==1)?enemyArray[0]=location:null();
	return 0;
}
int clearCom(int startX){///27+5
    int i;for(i=27;i<=31;i++){gotoXY(startX,i);printf("                                                     ");}
    if(docked==1){for(i=27;i<=31;i++){gotoXY(0,i);printf("|                                                           ");}}
    if(docked==1){gotoXY(0,32);printf("+-----------------------------------------------------------+");}
	return 0;
}
void shipDestroyed(void){///1,2,8,9,11,12,13,14,15,19,21-26,27-32
    int i;
    achievements[3]++;
    hudStats[1]=0;hudStats[2]=0;hudStats[8]=0;hudStats[9]=0;hudStats[7]=1;
    for(i=11;i<16;i++){hudStats[i]=0;}hudStats[19]=0;
    for(i=21;i<33;i++){hudStats[i]=0;}
}
void divertPower(int quantity){///s1 e8 ///cap 10
    int diverted=0;
    clearCom(51);gotoXY(52,27);
    if(hudStats[8]<quantity)///less energy than asked
    {
        if(capacityArray[10+hudStats[19]]-hudStats[1]<=hudStats[8])///enough energy to charge shield
        {
            diverted=capacityArray[10+hudStats[19]]-hudStats[1];
            hudStats[8]-=capacityArray[10+hudStats[19]]-hudStats[1];
            hudStats[1]=capacityArray[10+hudStats[19]];
            printf("%i power was diverted to shields.",diverted);gotoXY(52,28);
            printf("Shields Full.");
        }
        else ///take all energy avail
        {
            diverted=hudStats[8];
            hudStats[1]+=hudStats[8];
            hudStats[8]=0;
            printf("%i power was available.",diverted);gotoXY(52,28);
            printf("Shields Increased.");
        }
    }
    else///energy>asked
    {
        if(capacityArray[10+hudStats[19]]-hudStats[1]>quantity)///less energy than shield
        {
            diverted=quantity;
            hudStats[8]-=quantity;
            hudStats[1]+=quantity;
            printf("%i power was diverted to shields.",diverted);gotoXY(52,28);
            printf("Shields Increased.");
        }
        else ///more energy than shield
        {
            diverted=capacityArray[10+hudStats[19]]-hudStats[1];
            hudStats[8]-=capacityArray[10+hudStats[19]]-hudStats[1];
            hudStats[1]=capacityArray[10+hudStats[19]];
            printf("%i power was diverted to shields.",diverted);gotoXY(52,28);
            printf("Shields Full.");
        }
    }
}
void loot(void)///monies,commodities,power cells,tritanium plating,torps
{
    int money=0,cargo=0,cargoQuant=0,energy=0,hull=0,torps=0,i=1,damage;///11-15,e:8,2:h,t:9
    clearCom(51);
    randomNumber(100);randomNumber(100);randomNumber(100);
    if(randomNumber(100)<10&&hudStats[19]!=0){
        gotoXY(52,27);printf("Aboard the vessel you found a pilot who claims they");
        gotoXY(52,28);printf("will pay you to bring them to the nearest planet.");
        systemMap[enemyArray[0]]=enemyArray[2];initEnemy(0);hudStats[40]++;return;
    }
    if(randomNumber(100)>95&&hudStats[19]!=0){
        gotoXY(52,27);printf("Boarding the vessel tripped a security protocol.");
        damage=randomNumber(hudStats[1]+hudStats[2]+10*hudStats[19]*(enemyArray[1]-6));///damage fukd
        gotoXY(52,28);
        (damage!=0)?(printf("%i damage was done.",damage),damageReport(damage,2)):printf("Fortunately the system was damaged and malfunctioned.");
        systemMap[enemyArray[0]]=enemyArray[2];
        initEnemy(0);
        return;
    }
    if(randomNumber(100)>50){
        money=20+randomNumber(100);
        (enemyArray[1]==7)?money+=randomNumber(500):null();
        (enemyArray[1]==8)?money+=500+randomNumber(1000):null();
        (enemyArray[1]==9)?money+=1000+randomNumber(5000):null();
        (enemyArray[1]==10)?money+=2000+randomNumber(10000):null();
        lootPrint(i);i++;printf("%i Credits",money);
        hudStats[10]+=money;
    }
    if(randomNumber(100)>50&&hudStats[19]!=0){
        cargo=1+randomNumber(4);
        cargoQuant=(enemyArray[1]-5)*randomNumber(5);
        if(cargoQuant+hudStats[11]+hudStats[12]+hudStats[13]+hudStats[14]+hudStats[15]>capacityArray[hudStats[19]]+100*hudStats[31])
        {
            cargoQuant=capacityArray[hudStats[19]]+100*hudStats[31]-hudStats[11]-hudStats[12]-hudStats[13]-hudStats[14]-hudStats[15];
        }
        (cargoQuant!=0)?lootPrint(i),i++,printf("%i ",cargoQuant),lootPrint(cargo+5),hudStats[10+cargo]+=cargoQuant:null();
    }
    if(randomNumber(100)>65&&hudStats[19]!=1&&enemyArray[1]!=6&&hudStats[19]!=0){
        energy=(enemyArray[1]-5)*randomNumber(100);
        if(energy>capacityArray[30+hudStats[19]]-hudStats[8])
        {
            energy=capacityArray[30+hudStats[19]]-hudStats[8];
        }
        (energy!=0)?lootPrint(i),i++,printf("%i Power Cells",energy),hudStats[8]+=energy:null();
    }
    if(randomNumber(100)>70&&hudStats[19]!=0){
        hull=(enemyArray[1]-5)*(randomNumber(30));
        if(hull>capacityArray[20+hudStats[19]]-hudStats[2])
        {
            hull=capacityArray[20+hudStats[19]]-hudStats[2];
        }
        (hull!=0)?lootPrint(i),i++,printf("%i Tritanium Plates",hull),hudStats[2]+=hull:null();
    }
    if(randomNumber(100)>85&&hudStats[19]!=1&&hudStats[19]!=0){
        torps=randomNumber(enemyArray[1]);
        if(torps>capacityArray[40+hudStats[19]]-hudStats[9])
        {
            torps=capacityArray[40+hudStats[19]]-hudStats[9];
        }
        (torps!=0)?lootPrint(i),i++,printf("%i Torpedos",torps),hudStats[9]+=torps:null();
    }
    gotoXY(52,27);(i==1)?printf("No loot found."):printf("You looted the vessel and found:");
    systemMap[enemyArray[0]]=enemyArray[2];
    initEnemy(0);
}
void lootPrint(int number)
{
    (number==1)?gotoXY(52,29):null();
    (number==2)?gotoXY(52,30):null();
    (number==3)?gotoXY(52,31):null();
    (number==4)?gotoXY(78,29):null();
    (number==5)?gotoXY(78,30):null();//war,dut,lat,wat,ur
    (number==6)?printf("Warp Plasma"):null();
    (number==7)?printf("Duterium"):null();
    (number==8)?printf("Latinum"):null();
    (number==9)?printf("Water"):null();
    (number==10)?printf("Uranium Ore"):null();
}
void printDealArrow(int price,int lastBuy,int buyOrSell)///30,22,31--12,15,10
{
    if(buyOrSell==1){
        (price>lastBuy)?(SetColor(12),printf(" %c",30)):null();
        (price==lastBuy)?(printf(" %c",22)):null();
        (price<lastBuy)?(SetColor(10),printf(" %c",31)):null();}
    if(buyOrSell==2){
        (price<lastBuy)?(SetColor(12),printf(" %c",31)):null();
        (price==lastBuy)?(printf(" %c",22)):null();
        (price>lastBuy)?(SetColor(10),printf(" %c",30)):null();}
    SetColor(15);
}
void barMenu(void)
{///1525
    int choice;
    planetHUD();
    do
    {
        planetHUD();
        gotoXY(3,5);printf("Bar:");
        gotoXY(3,7);printf("1: Ship Logs");
        gotoXY(3,8);printf("2: Achievements");
        gotoXY(3,9);printf("3: Gamble");
        gotoXY(3,27);printf("1-3: Select Passtime\t\t0: Back");gotoXY(3,29);choice=getch()-'0';clearCom(66);

        switch (choice)
        {
        case 1:
            (hudStats[19]!=0)?printLog():(clearCom(66),gotoXY(67,27),printf("Ship logs not available in this vessel."));
            break;
        case 2:
            achievementsMenu();
            break;
        case 3:
            gamblingMenu();
            break;
        default:
            if(choice!=0){clearCom(66);gotoXY(67,27);printf("Invalid selection.");}
        }
    }while(choice!=0);
    //gamble,crew?
}
void printLog(void)
{
    int page=1,choice,i,j,guns;
    do
    {
        planetHUD();///max 24
        if(page==1)
        {
            gotoXY(3,5);printf("Ship Logs: Current as of %i.%i",hudStats[0]/10,hudStats[0]%10);
            gotoXY(3,8);printf("General Information:");
            gotoXY(3,10);printf("Exploration Logs:");
            gotoXY(3,11);printf("\tTime in Space: %i day",hudStats[0]-407594);(hudStats[0]-407594>1)?printf("s"):null();
            gotoXY(3,12);printf("\tDistance Travelled: %i km",achievements[0]*10);///in move - a[0]
            gotoXY(3,13);printf("\tSystems Visited: %i",achievements[1]);///in move - a[1]
            gotoXY(3,14);printf("\tPlanets Visited: %i",achievements[2]);///in planet menu
            gotoXY(3,16);printf("Combat Logs:");
            gotoXY(3,17);printf("\tShips Commanded: %i",achievements[3]);///in outfitter
            gotoXY(3,18);printf("\tWeapons Obtained: %i",achievements[4]);///outfit
            gotoXY(3,19);printf("\tEnemies Ships Destroyed: %i",achievements[5]);///damagereport
            gotoXY(3,20);printf("\tCombat Status: Legionnaire");
        }
        if(page==2)
        {
            i=0;
            gotoXY(3,5);printf("Outfit Details:");
            gotoXY(3,8);printf("Ship Class: Section %i ",hudStats[19]);shipClass(hudStats[19]),printf(" Class");
            gotoXY(3,10);printf("Armaments:");
            gotoXY(3,11);printf("\tShip Hardpoints: %i",capacityArray[5+hudStats[19]]);
            if(hudStats[26]==0){
                gotoXY(3,11+i);printf("\tNo modules currently equipped.");}
            else{
                gotoXY(3,11+i);printf("\tCurrent installations: %i",hudStats[26]);guns=0;
                for(j=21;j<26;j++)
                {
                    (hudStats[j]==1)?guns++:null();
                }
                (guns!=0)?i++,gotoXY(3,11+i),printf("\t\t%i Class 1 type pulse cannon",guns),(guns>1)?printf("s."):printf("."):null();
                guns=0;
                for(j=21;j<26;j++)
                {
                    (hudStats[j]==2)?guns++:null();
                }
                (guns!=0)?i++,gotoXY(3,11+i),printf("\t\t%i Class 2 type pulse cannon",guns),(guns>1)?printf("s."):printf("."):null();
                guns=0;
                for(j=21;j<26;j++)
                {
                    (hudStats[j]==3)?guns++:null();
                }
                (guns!=0)?i++,gotoXY(3,11+i),printf("\t\t%i Class 3 type pulse cannon",guns),(guns>1)?printf("s."):printf("."):null();}
            (hudStats[26]!=0)?i++,gotoXY(3,11+i),rangeCheck(1,2),printf("\tMaximum energy draw: %i MJ",hudStats[38]):null();
            (hudStats[26]!=0)?i++,gotoXY(3,11+i),printf("\tPotential damage output: %i",rangeCheck(1,2)):null();
            (hudStats[19]!=1)?i++,gotoXY(3,11+i),printf("\tStandard Sigma Shipyards torpedo bays: %i",hudStats[19]-1):null();
        }
        if(page==3)
        {//armour pierce+10,carbonalloy+10,tritaniumhullstruts+10,shieldprotocol+10,reconfigure+100,,photons+20
            i=0;
            gotoXY(3,5);printf("Ship Enhancements:");///27-32//damage,range,hull,shield,cargo,torp
            gotoXY(3,7+i);(hudStats[27]!=0)?(printf("%i Pulse cannon armour piercing shell upgrades.",hudStats[27]),i++,
                                             gotoXY(3,7+i),printf("\t%i extra damage per shot.",hudStats[27]*10),i+=2):null();
            gotoXY(3,7+i);(hudStats[28]!=0)?(printf("%i Pulse cannon carbon alloy barrel upgrades.",hudStats[28]),i++,
                                             gotoXY(3,7+i),printf("\t%i km range added.",hudStats[28]*10),i+=2):null();
            gotoXY(3,7+i);(hudStats[29]!=0)?(printf("%i Tritanium hull strut upgrades.",hudStats[29]),i++,
                                             gotoXY(3,7+i),printf("\t%i damage dampening on hull per shot.",hudStats[29]*10),i+=2):null();
            gotoXY(3,7+i);(hudStats[30]!=0)?(printf("%i Shileding reaction protocol reconfigurations.",hudStats[30]),i++,
                                             gotoXY(3,7+i),printf("\t%i damage deflection on shield per shot.",hudStats[30]*10),i+=2):null();
            gotoXY(3,7+i);(hudStats[31]!=0)?(printf("%i Cargo bay reconfigurations and expansion processes.",hudStats[31]),i++,
                                             gotoXY(3,7+i),printf("\t%i extra cargo capacity.",hudStats[31]*100),i+=2):null();
            gotoXY(3,7+i);(hudStats[32]!=0)?(printf("%i Photonic torpedo upgrades.",hudStats[32]),i++,
                                             gotoXY(3,7+i),printf("\t%i extra damage per torpedo.",hudStats[32]*20),i+=2):null();
            gotoXY(3,7+i);(i==0)?printf("No upgrades at this time."):null();
        }
        gotoXY(3,27);(page==1)?printf("0: Back\t\t2: Next"):(page==3)?printf("1: Previous\t\t0: Back"):printf("1: Previous\t\t0: Back\t\t2: Next");
        gotoXY(3,28);choice=getch()-'0';gotoXY(3,28);clearCom(66);gotoXY(67,27);
        (choice==1)?(page==1)?printf("Cannot go back a page"):page--:null();
        (choice==2)?(page==3)?printf("Cannot go forward a page"):page++:null();
        (choice!=1&&choice!=2&&choice!=0)?gotoXY(67,27),printf("Invalid selection"):null();
    }while(choice!=0);

    ///outfit log - guns equipped, upgrades equipped, average damage output, max range, combat/trade status/title
}
void achievementsMenu(void){
    int page=1,choice,slot,value;
    do
    {
        planetHUD();///dis=0,1=syst,2=plan,3=ships,4=weps,5=kills
        if(page==1){
            gotoXY(3,5);printf("Basic Achievements:");
            gotoXY(5,7);(achievements[0]>=20)?SetColor(15):SetColor(8);printf("The Traveler");printf("\t\t%i/20 km travelled",achievements[0]);
            gotoXY(5,8);(achievements[1]>=10)?SetColor(15):SetColor(8);printf("Discoverer");printf("\t\t\t%i/10 systems visited",achievements[1]);
            gotoXY(5,9);(achievements[2]>=15)?SetColor(15):SetColor(8);printf("Land Harr");printf("\t\t\t%i/15 planets landed on",achievements[2]);
            gotoXY(5,10);(achievements[3]>=2)?SetColor(15):SetColor(8);printf("Able Captain");printf("\t\t%i/2 ships owned",achievements[3]);
            gotoXY(5,11);(achievements[4]>=5)?SetColor(15):SetColor(8);printf("Rifleman");printf("\t\t\t%i/5 weapons obtained",achievements[4]);
            gotoXY(5,12);(achievements[5]>=10)?SetColor(15):SetColor(8);printf("Brute");printf("\t\t\t%i/10 enemies killed",achievements[5]);
        }
        if(page==2){
            gotoXY(3,5);printf("Intermediate Achievements:");
            gotoXY(5,7);(achievements[0]>=50)?SetColor(15):SetColor(8);printf("Well Travelled");printf("\t\t%i/50 km travelled",achievements[0]);
            gotoXY(5,8);(achievements[1]>=25)?SetColor(15):SetColor(8);printf("Explorer");printf("\t\t\t%i/25 systems visited",achievements[1]);
            gotoXY(5,9);(achievements[2]>=35)?SetColor(15):SetColor(8);printf("Sphere Hunter");printf("\t\t%i/35 planets landed on",achievements[2]);
            gotoXY(5,10);(achievements[3]>=4)?SetColor(15):SetColor(8);printf("Admiral");printf("\t\t\t%i/4 ships owned",achievements[3]);
            gotoXY(5,11);(achievements[4]>=15)?SetColor(15):SetColor(8);printf("Arms Dealer");printf("\t\t%i/15 weapons obtained",achievements[4]);
            gotoXY(5,12);(achievements[5]>=25)?SetColor(15):SetColor(8);printf("Soldier");printf("\t\t\t%i/25 enemiies killed",achievements[5]);
        }
        if(page==3){
            gotoXY(3,5);printf("Advanced Achievements:");
            gotoXY(5,7);(achievements[0]>=150)?SetColor(15):SetColor(8);printf("Not Lost In Space");printf("\t\t%i/150 km travelled",achievements[0]);
            gotoXY(5,8);(achievements[1]>=50)?SetColor(15):SetColor(8);printf("Found Your Hemingway");printf("\t%i/50 system visited",achievements[1]);
            gotoXY(5,9);(achievements[2]>=75)?SetColor(15):SetColor(8);printf("Diplomat");printf("\t\t\t%i/75 planets landed on",achievements[2]);
            gotoXY(5,10);(achievements[3]>=5)?SetColor(15):SetColor(8);printf("Enshiplopedia");printf("\t\t%i/5 ships owned",achievements[3]);
            gotoXY(5,11);(achievements[4]>=30)?SetColor(15):SetColor(8);printf("Gun Fettish");printf("\t\t%i/30 weapons obtained",achievements[4]);
            gotoXY(5,12);(achievements[5]>=50)?SetColor(15):SetColor(8);printf("Warren Piece");printf("\t\t%i/50 enemies killed",achievements[5]);
            ///silver tounge
        }
        SetColor(15);
        gotoXY(3,27);(page==1)?printf("0: Back\t\t2: Next"):(page==3)?printf("1: Previous\t\t0: Back"):printf("1: Previous\t\t0: Back\t\t2: Next");
        gotoXY(3,29);printf("5: achievements[x]=y");
        gotoXY(3,30);choice=getch()-'0';clearCom(66);gotoXY(67,27);
        if(choice==5){gotoXY(3,28);printf("achievements[");scanf("%i",&slot);(slot>9)?gotoXY(18,28):gotoXY(17,28);printf("]=");scanf("%i",&value);
            if(slot>50||value>200){gotoXY(67,27);printf("Invalid slot/value");}else{achievements[slot]=value;}}
        (choice==1)?(page==1)?printf("Cannot go back a page"):page--:null();
        (choice==2)?(page==3)?printf("Cannot go forward a page"):page++:null();
        (choice!=1&&choice!=2&&choice!=0&&choice!=5)?printf("Invalid selection"):null();
    }while(choice!=0);
}
void gamblingMenu(void){
    int choice;
    do{
        planetHUD();


        gotoXY(3,27);printf("Under Development\t\t0: Back");
        choice=getch()-'0';
        (choice!=1&&choice!=2&&choice!=0&&choice!=5)?printf("Invalid selection"):null();
    }while(choice!=0);
}
void initStats(int debug)///debug = -1 on, 1 off
{
    hudStats[0]=407595;//date
    hudStats[1]=(debug==1)?0:10000;//shields
    hudStats[2]=(debug==1)?100:4000;//hull
    ///3,4,5,6 location
    ///7 = security
    hudStats[8]=(debug==1)?400:60000;//energy
    hudStats[9]=(debug==1)?0:100;//torps
    hudStats[10]=(debug==1)?500:1000000000;//monies
    hudStats[11]=0;//warp plasma
    hudStats[12]=0;//duterium
    hudStats[13]=0;//latinum
    hudStats[14]=0;//water
    hudStats[15]=0;//uranium ore
    ///16,17,18 = planets,enemies,stellar objects
    hudStats[19]=(debug==1)?1:5;//ship class 1-5
    ///20 = enemy location
    hudStats[21]=(debug==1)?1:3;//21-25=weapons (1-3)
    hudStats[22]=(debug==1)?0:3;
    hudStats[23]=(debug==1)?0:3;
    hudStats[24]=(debug==1)?0:3;
    hudStats[25]=(debug==1)?0:3;
    hudStats[26]=(debug==1)?1:5;//total weapons
    hudStats[27]=0;//damage upgrades
    hudStats[28]=0;//range upgrade
    hudStats[29]=0;//hull upgrade
    hudStats[30]=0;//shield upgrade
    hudStats[31]=0;//cargo upgrade
    hudStats[32]=0;//torpedo upgrade
    hudStats[33]=0;//engine upgrade

    ///hudStats[38]=energy depletion
    ///hudStats[39]=guns in range
    hudStats[40]=0;//passengers


    ///shuttle,cargo,freight,frigate,destroyer
    ///shields1=0,300,500,1000,10000            1
    ///hull2=100,500,700,2000,4000              2
    ///energy8=0,1000,2000,4000,30000           3
    ///torps9=0,5,10,40,100                     4
    ///cargo11-15=100,1000,3000,100000,50000    5
    ///ship range=6-10                          6
    ///cannon hardpoints=16-20                  7
    capacityArray[1]=100;///cargo
    capacityArray[2]=1000;
    capacityArray[3]=5000;
    capacityArray[4]=10000;
    capacityArray[5]=100000;
    capacityArray[6]=1;///hardpoints
    capacityArray[7]=2;
    capacityArray[8]=3;
    capacityArray[9]=4;
    capacityArray[10]=5;
    capacityArray[11]=0;///shields
    capacityArray[12]=300;
    capacityArray[13]=500;
    capacityArray[14]=1000;
    capacityArray[15]=10000;
    capacityArray[21]=100;///hull
    capacityArray[22]=500;
    capacityArray[23]=700;
    capacityArray[24]=2000;
    capacityArray[25]=4000;
    capacityArray[31]=400;///energy
    capacityArray[32]=2000;
    capacityArray[33]=4000;
    capacityArray[34]=8000;
    capacityArray[35]=60000;
    capacityArray[41]=0;///torps
    capacityArray[42]=5;
    capacityArray[43]=10;
    capacityArray[44]=40;
    capacityArray[45]=100;
    capacityArray[46]=12;///commodity mids
    capacityArray[47]=35;
    capacityArray[48]=85;
    capacityArray[49]=425;
    capacityArray[50]=850;
    commodityLastBuy[1]=12;
    commodityLastBuy[2]=35;
    commodityLastBuy[3]=85;
    commodityLastBuy[4]=425;
    commodityLastBuy[5]=850;
    (debug!=1)?achievements[0]=randomNumber(150):null();
    (debug!=1)?achievements[1]=randomNumber(50):null();
    (debug!=1)?achievements[2]=randomNumber(75):null();
    (debug!=1)?achievements[3]=randomNumber(5):null();
    (debug!=1)?achievements[4]=randomNumber(30):null();
    (debug!=1)?achievements[5]=randomNumber(50):null();
    if(debug!=1){int i=1;for(;i<=51;i++){skillsArray[i]=randomNumber(50);}}

}
/**            ACHIEVEMENTS
0: distance travelled
1: systems visited
2: planets visited
3: ships commanded
4: weapons obtained
5: kills - leads to combat status
**/
