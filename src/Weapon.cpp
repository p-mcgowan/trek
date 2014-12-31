/* Trek.cpp
 *
 * Weapon class
 */

CWeapon::CWeapon() {  // params - lvl, raremult...
    std::string path("../lists/weapon/names/");
    type = getRandomStringFromFile("../lists/weapon/names/type");
    path += type;
    stype = getRandomStringFromFile(path);
    wep.setRarity();
    wep.setStats();
    return;
}

/* 
 * 
 */
void setRarity() {  // int mult) {
	int i = rndm(0,1000);
	if (i <= COMMONTHRESH)
		rarity = "common";
	else if (i <= UNCOMMONTHRESH)
		rarity = "uncommon";
	else if (i <= RARETHRESH)
		rarity = "rare";
	else if (i <= EPICTHRESH)
		rarity = "epic";
	else
		rarity = "legendary";
}

/* CWeapon::setStats
 * Sets stat modifiers from file for a weapon, based on rarity
 */
void CWeapon::setStats() {
	std::string name;
	if (!rarity.compare("common")) {
		wep.getBaseStats();
		prefix = "";
		suffix = "";
		name = stype;
	}
	else if (!rarity.compare("uncommon")) {
		wep.getBaseStats();
		if (rndm(0,1)) {
			suffix = "";
			prefix = getRandomStringFromFile(PREFIX_FILE);
			name += prefix + " " + stype;
			name = name;
			wep.findMod(PREFIX_FILE,prefix,wep,'n');
		}else {
			prefix = "";
			suffix = getRandomStringFromFile(SUFFIX_FILE);
			name += stype + " " + suffix;
			name = name;
			wep.findMod(SUFFIX_FILE,suffix,wep,'n');
		}
	}
	else if (!rarity.compare("rare")) {
		wep.getBaseStats();
		prefix = getRandomStringFromFile(PREFIX_FILE);
		suffix = getRandomStringFromFile(SUFFIX_FILE);
		wep.findMod();
		wep.findMod();
		name += prefix + " " + stype + " " + suffix;
		name = name;

	}
	else if (!rarity.compare("epic")) {
		wep.getBaseStats();
		prefix = getRandomStringFromFile(PREFIX_FILE);
		suffix = getRandomStringFromFile(SUFFIX_FILE);
		name += prefix + " " + stype + " " + suffix;
		name = name;

		mods = rndm(3,5);
		wep.findMod(PREFIX_FILE,prefix,wep,'n');
		wep.findMod(SUFFIX_FILE,suffix,wep,'n');
		for(i = 0;i<mods;i++) {
			if (rndm(0,1)) {
				wep.findMod();
			}
			else {
				wep.findMod();
			}
		}
	}
	else {  // legendary
		std::string path("../lists/weapon/legendary/");
		wep.getBaseStats();
		path += type + "/" + stype + "/names";
		name += getRandomStringFromFile(path);
		name = name;
		prefix = "";
		suffix = "";
		path += type + "/" + stype + "/" + name;
		wep.findMod(path,"",wep,'l');
	}
	return;
}

/* CWeapon::getBaseStats
 * Sets the weapon's basic stats based on rarity and weapon type/sub type
 */
void CWeapon::getBaseStats() {  // int lvl
	std::ifstream subTypeFile;
	std::string path("../lists/weapon/");
	if (!rarity.compare("legendary")) {  // legendary
		path += "legendary/" + type + "/stats/" + stype;  // stype holds stats
		// wep.getwepdesc(wep,'l');  // legendary - own writeup
	}
	else {  // everything else
		path += "stats/" + type + "/" + stype;
		name = stype;
		// wep.getwepdesc(wep'n');  // nornmal
	}
	subTypeFile.open(path.c_str());
	subTypeFile >> dmg;
	subTypeFile >> maxdmg;
	subTypeFile >> shots;
	subTypeFile >> crit;
	subTypeFile >> mupgrades;
	std::getline(desc, subTypeFile);
	subTypeFile.close();
	tier = mupgrades - 1;
	nupgrades = rndm(0, mupgrades);
	return;
}

/* CWeapon::findmod
 * 
 */
void CWeapon::findmod(std::string path, std::string matchstr) {
	std::ifstream stringFile(path.c_str());
	std::string buffer;
	if (rarity.compare("legendary")) {
		getline(path.c_str(), buffer);
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

/* CWeapon::processmods
 * 
 */
int CWeapon::processmods(CWeapon wep, char *mod, int val) {
	if (!strcmp(mod,"c"))  // values should be scaled by weapon tier
		crit+= (float)((float)1/(float)val);
	else if (!strcmp(mod,"s"))
		shots+= val;
	else if (!strcmp(mod,"md"))
		maxdmg+= val;
	else if (!strcmp(mod,"d"))
		dmg+= val;
	else if (!strcmp(mod,"mu")) {
		if (nupgrades+val< = mupgrades)
			mupgrades+= val;
	}
	else if (!strcmp(mod,"u")) {
		  // could generate upgrade id from file, place in wep
	}
	else if (!strcmp(mod,"rl"))
		rlvl+= val;
	else if (!strcmp(mod,"rc"))
		rclout+= val;
	else if (!strcmp(mod,"rs"))
		rsocial+= val;

	else if (!strcmp(mod,"pil"))
		mods[PIL]+= val;
	else if (!strcmp(mod,"com"))
		mods[COM]+= val;
	else if (!strcmp(mod,"sci"))
		mods[SCI]+= val;
	else if (!strcmp(mod,"soc"))
		mods[SOC]+= val;
	else if (!strcmp(mod,"clo"))
		mods[CLO]+= val;
	else if (!strcmp(mod,"lead"))
		mods[LEAD]+= val;
	else if (!strcmp(mod,"ren"))
		mods[REN]+= val;
	else if (!strcmp(mod,"mec"))
		mods[MEC]+= val;
	else if (!strcmp(mod,"abl"))
		mods[ABL]+= val;
	else if (!strcmp(mod,"intu"))
		mods[INTU]+= val;
	else if (!strcmp(mod,"inte"))
		mods[INTE]+= val;
	else if (!strcmp(mod,"tec"))
		mods[TEC]+= val;
	else if (!strcmp(mod,"inn"))
		mods[INN]+= val;
	else if (!strcmp(mod,"cha"))
		mods[CHA]+= val;
	else if (!strcmp(mod,"comm"))
		mods[COMM]+= val;
	else if (!strcmp(mod,"dex"))
		mods[DEX]+= val;
	else {
		printf("unmatched mod %s value %d\n",mod,val);
		return 1;
	}
	return 0;
}
