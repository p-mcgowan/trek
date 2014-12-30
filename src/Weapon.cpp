/* Trek.cpp
 *
 * Weapon class
 */

CWeapon::CWeapon() {  // params - lvl, raremult...
        std::string path("../lists/weapon/names/");
        wep.type = getRandomStringFromFile("../lists/weapon/names/type");
        path += wep.type;
        wep.stype = getRandomStringFromFile(path);
        wep.setRarity();
        wep.getStats();
        return;
}

/* 
 * 
 */
void setRarity() {  // int mult) {
	int i = rndm(0,1000);
	if (i <= COMMONTHRESH)
		wep.rarity = "common";
	else if (i <= UNCOMMONTHRESH)
		wep.rarity = "uncommon";
	else if (i <= RARETHRESH)
		wep.rarity = "rare";
	else if (i <= EPICTHRESH)
		wep.rarity = "epic";
	else
		wep.rarity = "legendary";
}

/* CWeapon::getBaseStats
 * Sets the weapon's basic stats based on rarity and weapon type/sub type
 */
void CWeapon::getBaseStats() {  // int lvl
	std::ifstream subTypeFile;
	std::string path("../lists/weapon/");
	if (!wep.rarity.compare("legendary")) {  // legendary
		path += "legendary/";
		path += wep.type;
		path += "/stats/";
		path += wep.stype;  // stype holds stats
		// getwepdesc(wep,'l');  // legendary - own writeup
	}
	else {  // everything else
		path += "stats/";
		path += wep.type;
		path += "/";
		path += wep.stype;
		wep.name = wep.stype;
		// getwepdesc(wep'n');  // nornmal
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


/* CWeapon::getStats
 * Sets stat modifiers from file for a weapon, based on rarity
 */
void CWeapon::getStats() {
	std::string name;
	std::string path("../lists/weapon/legendary/");
	if (!wep.rarity.compare("common")) {
		wep.getBaseStats();
		wep.prefix = "";
		wep.suffix = "";
		wep.name = wep.stype;
	}
	else if (!wep.rarity.compare("uncommon")) {
		wep.getBaseStats();
		if (rndm(0,1)) {
			wep.suffix = "";
			wep.prefix = getRandomStringFromFile("../lists/weapon/prefix");
			name += wep.prefix;
			name += " ";
			name += wep.stype;
			wep.name = name;
			wep.getMods('p');
		}else {
			wep.prefix = "";
			wep.suffix = getRandomStringFromFile("../lists/weapon/suffix");
			name += wep.stype;
			name += " ";
			name += wep.suffix;
			wep.name = name;
			wep.getMods('s');
		}
	}
	else if (!wep.rarity.compare("rare")) {
		wep.getBaseStats();
		wep.prefix = getRandomStringFromFile("../lists/weapon/prefix");
		wep.getMods('p');
		wep.suffix = getRandomStringFromFile("../lists/weapon/suffix");
		wep.getMods('s');
		name += wep.prefix;
		name += " ";
		name += wep.stype;
		name += " ";
		name += wep.suffix;
		wep.name = name;

	}
	else if (!wep.rarity.compare("epic")) {
		wep.getBaseStats(wep);
		wep.prefix = getRandomStringFromFile("../lists/weapon/prefix");
		wep.suffix = getRandomStringFromFile("../lists/weapon/suffix");
		wep.getMods('e');
		name += wep.prefix;
		name += " ";
		name += wep.stype;
		name += " ";
		name += wep.suffix;
		wep.name = name;
	}
	else {  // legendary
		wep.getBaseStats(wep);
		path += wep.type;
		path += "/";
		path += wep.stype;
		path += "/names";
		name += getRandomStringFromFile(path);
		wep.name = name;
		wep.prefix = "";
		wep.suffix = "";
		wep.getMods('l');
	}
	return;
}

/* 
 * 
 */
void getMods(std::string const mode) {
	char path[256] = "../lists/weapon/legendary/";
	int i,mods;
	if (mode =='p') {  // prefix -- for uncommon, rare, epic
		findmod("../lists/weapon/prefix",wep.prefix,wep,'n');
	}
	if (mode =='s') {  // suffix -- for uncommon, rare, epic
		findmod("../lists/weapon/suffix",wep.suffix,wep,'n');
	}
	if (mode =='e') {  // epic
		mods = rndm(3,5);
		findmod("../lists/weapon/prefix",wep.prefix,wep,'n');
		findmod("../lists/weapon/suffix",wep.suffix,wep,'n');
		for(i = 0;i<mods;i++) {
			if (rndm(0,1)) {
				findmod("../lists/weapon/suffix", getRandomStringFromFile("../lists/weapon/suffix"),wep,'n');
			}
			else {
				findmod("../lists/weapon/prefix", getRandomStringFromFile("../lists/weapon/prefix"),wep,'n');
			}
		}
	}
	if (mode =='l') {  // legendary - from file
		path += wep.type;
		path += "/";
		path += wep.stype;
		path += "/";
		path += wep.name;
		findmod(path,"",wep,'l');
	}
	return;
}

/* 
 * 
 */
void findmod(char *path,char *matchstr,CWeapon wep,char rare) {
	FILE *stringfile;
	char mod[128] = "",dum[256];
	int val = -1;
	stringfile = fopen(path,"r");
	if (rare! = 'l') {
		fscanf(stringfile,"%[^.]",mod);  // read up to .
		while(strcmp(matchstr,mod)) {
			fgets(dum,256,stringfile);  // clear out the stuff after the .
			fscanf(stringfile,"%[^.]",mod);  // get next name
		}
		fgetc(stringfile);  // clear out .
	}
	fscanf(stringfile,"%d %s",&val,mod);  // at least 1
	while(val! = 0) {
		if (processmods(wep,mod,val)) {
			printf("Mod defect = %s",path);
			while(getchar()! = 'y') {}
		}
		fscanf(stringfile,"%d %s",&val,mod);
	}
	fclose(stringfile);
	return;
}

/* 
 * 
 */
int processmods(CWeapon wep, char *mod, int val) {
	if (!strcmp(mod,"c"))  // values should be scaled by weapon tier
		wep.crit+= (float)((float)1/(float)val);
	else if (!strcmp(mod,"s"))
		wep.shots+= val;
	else if (!strcmp(mod,"md"))
		wep.maxdmg+= val;
	else if (!strcmp(mod,"d"))
		wep.dmg+= val;
	else if (!strcmp(mod,"mu")) {
		if (wep.nupgrades+val< = wep.mupgrades)
			wep.mupgrades+= val;
	}
	else if (!strcmp(mod,"u")) {
		  // could generate upgrade id from file, place in wep
	}
	else if (!strcmp(mod,"rl"))
		wep.rlvl+= val;
	else if (!strcmp(mod,"rc"))
		wep.rclout+= val;
	else if (!strcmp(mod,"rs"))
		wep.rsocial+= val;

	else if (!strcmp(mod,"pil"))
		wep.mods[PIL]+= val;
	else if (!strcmp(mod,"com"))
		wep.mods[COM]+= val;
	else if (!strcmp(mod,"sci"))
		wep.mods[SCI]+= val;
	else if (!strcmp(mod,"soc"))
		wep.mods[SOC]+= val;
	else if (!strcmp(mod,"clo"))
		wep.mods[CLO]+= val;
	else if (!strcmp(mod,"lead"))
		wep.mods[LEAD]+= val;
	else if (!strcmp(mod,"ren"))
		wep.mods[REN]+= val;
	else if (!strcmp(mod,"mec"))
		wep.mods[MEC]+= val;
	else if (!strcmp(mod,"abl"))
		wep.mods[ABL]+= val;
	else if (!strcmp(mod,"intu"))
		wep.mods[INTU]+= val;
	else if (!strcmp(mod,"inte"))
		wep.mods[INTE]+= val;
	else if (!strcmp(mod,"tec"))
		wep.mods[TEC]+= val;
	else if (!strcmp(mod,"inn"))
		wep.mods[INN]+= val;
	else if (!strcmp(mod,"cha"))
		wep.mods[CHA]+= val;
	else if (!strcmp(mod,"comm"))
		wep.mods[COMM]+= val;
	else if (!strcmp(mod,"dex"))
		wep.mods[DEX]+= val;
	else {
		printf("unmatched mod %s value %d\n",mod,val);
		return 1;
	}
	return 0;
}