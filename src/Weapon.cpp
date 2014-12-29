/** Trek.cpp
 *
 *  Weapon class
 */
 
 class CWeapon {

	private:
		std::string type;
		std::string sType;
		std::string prefix;
		std::string suffix;
		std::string name;
		std::string desc;//weapon writup
		int lvl;
		int tier;
		//base stats (lvl + rarity mult)
		float crit;
		int shots; //hits per turn
		int maxdmg;
		int dmg;
		std::string ammoType; //
		int mupgrades;//max # sockets
		int nupgrades;//actual # sockets
		int *upgrades;//sockets holding upgrade id
		int rarity;//0-1000
		int rlvl;//lvl req
		int rclout; //combat subskill req;
		int rsocial;
		int *mods; //other buffs from upgrades?
		int *id; //may not use?

	public:
		std::string getType(){return type;}
		std::string getStype(){return sType;}
		std::string getPrefix()return prefix;}
		std::string getSuffix(){return suffix;}
		std::string getName(){return name;}
		std::string getDesc(){return desc;}
		//getDescription()  //full printout
		void getBaseStats(CWeapon wep);

 }
 
void getBaseStats(CWeapon wep){//int lvl
	std::ifstream subTypeFile;
	std::string path ("lists/weapon/");
	if(!(wep.rarity.compare("legendary"))){//legendary
		path += "legendary/";
		path += wep.type;
		path += "/stats/";
		path += wep.stype; //stype holds stats
		//getwepdesc(wep,'l'); //legendary - own writeup
	}
	else{ //everything else
		path += "stats/";
		path += wep.type;
		path += "/";
		path += wep.stype;
		wep.name = wep.stype;
		//getwepdesc(wep'n'); //nornmal
	}
	subTypeFile.open(path.c_str());
	subTypeFile >> wep.dmg;
	subTypeFile >> wep.maxdmg;
	subTypeFile >> wep.shots;
	subTypeFile >> wep.crit;
	subTypeFile >> wep.mupgrades;
	std::getline(wep.desc, subTypeFile);
	subTypeFile.close();
	wep.tier = wep.mupgrades - 1;
	wep.nupgrades = rndm(0, wep.mupgrades);
	return;
}

void getStats(CWeapon wep){
	std::string name;
	std::string path("lists/weapon/legendary/");
	if(!wep.rarity.compare("common")) {
		getBaseStats(wep);
		wep.prefix="";
		wep.suffix="";
		wep.name=wep.stype;
	}
	else if(!wep.rarity.compare("uncommon")){
		getBaseStats(wep);
		if(rndm(0,1)){
			wep.suffix="";
			wep.prefix=fgetrandomstring("lists/weapon/prefix");
			strcat(name,wep.prefix);
			strcat(name," ");
			strcat(name,wep.stype);
			wep.name=name;
			getMods(wep,'p');
		}else{
			wep.prefix="";
			wep.suffix=fgetrandomstring("lists/weapon/suffix");
			strcat(name,wep.stype);
			strcat(name," ");
			strcat(name,wep.suffix);
			wep.name=name;
			getMods(wep,'s');
		}
	}
	else if(!wep.rarity.compare("rare")){
		getBaseStats(wep);
		wep.prefix=fgetrandomstring("lists/weapon/prefix");
		getMods(wep,'p');
		wep.suffix=fgetrandomstring("lists/weapon/suffix");
		getMods(wep,'s');
		strcat(name,wep.prefix);
		strcat(name," ");
		strcat(name,wep.stype);
		strcat(name," ");
		strcat(name,wep.suffix);
		wep.name=name;
		
	}
	else if(!wep.rarity.compare("epic")){
		getBaseStats(wep);
		wep.prefix=fgetrandomstring("lists/weapon/prefix");
		wep.suffix=fgetrandomstring("lists/weapon/suffix");
		getMods(wep,'e');
		strcat(name,wep.prefix);
		strcat(name," ");
		strcat(name,wep.stype);
		strcat(name," ");
		strcat(name,wep.suffix);
		wep.name=name;
	}
	else{//legendary
		getBaseStats(wep);
		strcat(path,wep.type);
		strcat(path,"/");
		strcat(path,wep.stype);
		strcat(path,"/names");
		strcat(name,fgetrandomstring(path));
		wep.name=name;
		wep.prefix="";
		wep.suffix="";
		getMods(wep,'l');
	}
	return;
}
/**
char *getrarity(){//int mult){
	int i=rndm(0,994);
	if (i<=750)
		return "common";
	else if(i<=900)
		return "uncommon";
	else if(i<=983)
		return "rare";
	else if(i<=994)
		return "epic";
	else
		return "legendary";
}
void getMods(CWeapon wep, char mode){
	char path[256]="lists/weapon/legendary/";
	int i,mods;
	if (mode=='p'){//prefix -- for uncommon, rare, epic
		findmod("lists/weapon/prefix",wep.prefix,wep,'n');
	}
	if (mode=='s'){//suffix -- for uncommon, rare, epic
		findmod("lists/weapon/suffix",wep.suffix,wep,'n');
	}
	if (mode=='e'){//epic
		mods=rndm(3,5);
		findmod("lists/weapon/prefix",wep.prefix,wep,'n');
		findmod("lists/weapon/suffix",wep.suffix,wep,'n');
		for(i=0;i<mods;i++){
			if(rndm(0,1)){
				findmod("lists/weapon/suffix",fgetrandomstring("lists/weapon/suffix"),wep,'n');
			}
			else{
				findmod("lists/weapon/prefix",fgetrandomstring("lists/weapon/prefix"),wep,'n');
			}
		}
	}	
	if (mode=='l'){//legendary - from file
		strcat(path,wep.type);
		strcat(path,"/");
		strcat(path,wep.stype);
		strcat(path,"/");
		strcat(path,wep.name);
		findmod(path,"",wep,'l');
	}
	return;
}
void findmod(char *path,char *matchstr,CWeapon wep,char rare){
	FILE *stringfile;
	char mod[128]="",dum[256];
	int val=-1;
	stringfile=fopen(path,"r");
	if(rare!='l'){
		fscanf(stringfile,"%[^.]",mod); //read up to .
		while(strcmp(matchstr,mod)){
			fgets(dum,256,stringfile);//clear out the stuff after the .
			fscanf(stringfile,"%[^.]",mod); //get next name
		}
		fgetc(stringfile);//clear out .
	}
	fscanf(stringfile,"%d %s",&val,mod);//at least 1
	while(val!=0){
		if(processmods(wep,mod,val)){
			printf("Mod defect=%s",path);
			while(getchar()!='y'){}
		}
		fscanf(stringfile,"%d %s",&val,mod);
	}
	fclose(stringfile);
	return;
}
int processmods(CWeapon wep, char *mod, int val){
	if(!strcmp(mod,"c")) // values should be scaled by weapon tier
		wep.crit+=(float)((float)1/(float)val);
	else if(!strcmp(mod,"s"))
		wep.shots+=val;
	else if(!strcmp(mod,"md"))
		wep.maxdmg+=val;
	else if(!strcmp(mod,"d"))
		wep.dmg+=val;
	else if(!strcmp(mod,"mu")){
		if (wep.nupgrades+val<=wep.mupgrades)
			wep.mupgrades+=val;
	}
	else if(!strcmp(mod,"u")){
		//could generate upgrade id from file, place in wep
	}
	else if(!strcmp(mod,"rl"))
		wep.rlvl+=val;
	else if(!strcmp(mod,"rc"))
		wep.rclout+=val;
	else if(!strcmp(mod,"rs"))
		wep.rsocial+=val;

	else if(!strcmp(mod,"pil"))
		wep.mods[PIL]+=val;
	else if(!strcmp(mod,"com"))
		wep.mods[COM]+=val;
	else if(!strcmp(mod,"sci"))
		wep.mods[SCI]+=val;
	else if(!strcmp(mod,"soc"))
		wep.mods[SOC]+=val;
	else if(!strcmp(mod,"clo"))
		wep.mods[CLO]+=val;
	else if(!strcmp(mod,"lead"))
		wep.mods[LEAD]+=val;
	else if(!strcmp(mod,"ren"))
		wep.mods[REN]+=val;
	else if(!strcmp(mod,"mec"))
		wep.mods[MEC]+=val;
	else if(!strcmp(mod,"abl"))
		wep.mods[ABL]+=val;
	else if(!strcmp(mod,"intu"))
		wep.mods[INTU]+=val;
	else if(!strcmp(mod,"inte"))
		wep.mods[INTE]+=val;
	else if(!strcmp(mod,"tec"))
		wep.mods[TEC]+=val;
	else if(!strcmp(mod,"inn"))
		wep.mods[INN]+=val;
	else if(!strcmp(mod,"cha"))
		wep.mods[CHA]+=val;
	else if(!strcmp(mod,"comm"))
		wep.mods[COMM]+=val;
	else if(!strcmp(mod,"dex"))
		wep.mods[DEX]+=val;
	else{ 
		printf("unmatched mod %s value %d\n",mod,val);
		return 1;
	}
	return 0;
}*/





