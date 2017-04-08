#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "global.h"
#include "defs.h"
#include "equipment.h"
#include "proto.h"
#include "gutils.h"
#include "wepgen.h"

int main(){
    
    
    weapon **weps=(weapon**)malloc(10*sizeof(weapon*));
    FILE *fp=fopen("defs.h","r");char mod[128],d1[128],d2[128];
    int i,j=0,k,l;//use structs for id's maybe?
    //char t=getch();
    //printf("%c",t);
    //t=getch();
    //printf("%c",t);//for later...*/
    
    while(1){
        weps=(weapon**)realloc(weps,(j+1)*sizeof(weapon*));
        //weps[j]=(weapon*)malloc(sizeof(weapon));
        weps[j]=wepgen(/*&seed*/);
        for(i=0;i<=j;i++){
            i=j;////// 1 at a time
            printf("\n%s\n%s tier %d %s",weps[i]->name,weps[i]->rarity,weps[i]->tier,weps[i]->stype);
            printf("\ndmg: %i-%i\nshots: %i\ncrit: %1.2f\nupgrades: %i/%i%s\n",weps[i]->dmg,weps[i]->maxdmg,weps[i]->shots,weps[i]->crit,weps[i]->nupgrades,weps[i]->mupgrades,weps[i]->desc);
            for(k=0;k<20;k++){
                if (weps[i]->mods[k]!=0){    
                    for(l=0;l<=k;l++){
                        fscanf(fp,"%s %s %s",d1,mod,d2);
                    }
                    printf("%s %i\n",mod,weps[i]->mods[k]);
                }
                fseek(fp,0,0);
            }
            printf("\n");
            if(!strcmp(weps[i]->type,"nuclear"))
                printf("oh shit its a nuke");
        }
        getchar();
        j++;
    }
    return 0;
}
