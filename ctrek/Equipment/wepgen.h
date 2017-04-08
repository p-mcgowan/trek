#include "wutils.h"

weapon *wepgen(){//,int lvl,int raremult){
	weapon *wep=(weapon*)malloc(sizeof(weapon));
	//initialize all pointers and anything that is not set by default
	wep->mods=(int*)calloc(20,sizeof(int));
	wep->desc=(char*)calloc(256,sizeof(char));
	char path[128]="../../lists/weapon/names/";
	wep->type=fgetrandomstring("../../lists/weapon/names/type");
	strcat(path,wep->type);
	wep->stype=fgetrandomstring(path);
	wep->rarity=getrarity(/*raremult*/);
	getstats(wep);
	return wep;
}
