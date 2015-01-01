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
	int i = rndm(0, 1000);
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
	std::string wepName;
	wep.getBaseStats();
	if (!rarity.compare("common")) {
		prefix = "";
		suffix = "";
		name = stype;
	}
	else if (!rarity.compare("uncommon")) {
		if (rndm(0, 1)) {
			suffix = "";
			prefix = getRandomStringFromFile(PREFIX_FILE);
			wepName += prefix + " " + stype;
			name = wepName;
			wep.applyMods();
		}else {
			prefix = "";
			suffix = getRandomStringFromFile(SUFFIX_FILE);
			wepName += stype + " " + suffix;
			name = wepName;
			wep.applyMods();
		}
	}
	else if (!rarity.compare("rare")) {
		prefix = getRandomStringFromFile(PREFIX_FILE);
		suffix = getRandomStringFromFile(SUFFIX_FILE);
		wep.applyMods();
		wepName += prefix + " " + stype + " " + suffix;
		name = wepName;

	}
	else if (!rarity.compare("epic")) {
		mods = rndm(4, 6);
		for(i = 0; i < mods; i++) {
			if (rndm(0, 1)) {
				suffix = "";
				prefix = getRandomStringFromFile(PREFIX_FILE);
			}
			else {
				suffix = getRandomStringFromFile(SUFFIX_FILE);;
				prefix = "";
			}
			wep.applyMods();
		}
		prefix = getRandomStringFromFile(PREFIX_FILE);
		suffix = getRandomStringFromFile(SUFFIX_FILE);
		wep.applyMods();
		wepName += prefix + " " + stype + " " + suffix;
		name = wepName;
	}
	else {
		std::string path("../lists/weapon/legendary/");
		path += type + "/" + stype + "/names";
		name += getRandomStringFromFile(path);
		prefix = "";
		suffix = "";
		wep.applyMods();
	}
	return;
}

/* CWeapon::getBaseStats
 * Sets the weapon's basic stats based on rarity and weapon type/sub type
 */
void CWeapon::getBaseStats() {  // int lvl
	std::ifstream subTypeFile;
	std::string path("../lists/weapon/");
	if (!rarity.compare("legendary")) {
		path += "legendary/" + type + "/stats/" + stype;
	}
	else {
		path += "stats/" + type + "/" + stype;
	}
	subTypeFile.open(path.c_str());
	subTypeFile >> dmg;
	subTypeFile >> maxdmg;
	subTypeFile >> shots;
	subTypeFile >> crit;
	subTypeFile >> mupgrades;
	istream::getline(desc, subTypeFile);
	subTypeFile.close();
	tier = mupgrades - 1;
	nupgrades = rndm(0, mupgrades);
	return;
}

/* CWeapon::applyMods
 * 
 */
void CWeapon::applyMods() {
	std::ifstream modFile;
	std::string path;
	std::string modName, modValue, modLine, delimiter = " ";
	if (rarity.compare("legendary")) {
		path = "../lists/weapon/legendary/" + type + "/" + stype + "/" + name;
		istream::getline(path.c_str());
		// process single line of mods
	}
	else {
		if (prefix != "") {
			modFile.open(PREFIX_FILE);
		}
		if (suffix != "") {
			modFile.open(SUFFIX_FILE);
			istream::getline(modFile, modLine);
			size_t pos = 0;
			while ((pos = modLine.find(delimiter)) != std::string::npos) {
    			NAME = modLinesubstr(0, pos);
    			modLine.erase(0, pos + delimiter.length());
    			pos = modLine.find(delimiter)
    			VAL = modLine.substr(0, pos);
    			modLine.erase(0, pos + delimiter.length());
    			processTokenPair();
			}
		}
	}
	modFile.close();
	return;
}

/* CWeapon::processmods
 * 
 */
int CWeapon::processmods() {
	switch(modToIndex[modType]) {
		case DMG:
			dmg += modValue;
		case CRIT:
			crit += (float)((float)1/(float)val);
			break;
		case SHOTS:
			shots += val;
			break;
		case MAXDMG:
			maxdmg += val;
			break;
		case MAXUPGRADES:
			if (nupgrades + val <= mupgrades)
				mupgrades += val;
			break;
		case LVLREQ:
			rlvl += val;
			break;
		case CLOUTREQ:
			rclout += val;
			break;
		case SOCIALREQ:
			rsocial += val;
			break;
		case PILOT:
		case COMBAT:
		case SCIENCE:
		case SOCIAL:
		case CLOUT:
		case LEADERSHIP:
		case REKNOWN:
		case MECHAPT:
		case ABL:
		case INTUITION:
		case INTELECT:
		case TECHAPT:
		case INNOVATION:
		case CHARISMA:
		case COMMERCE:
		case DEXTERITY:
			mods[modToIndex[modType]] += modValue;
			break;
		default:
			std::cout << "unmatched mod pair" << modType << modToIndex[modType];
			return 1;
			break;
	}
	return 0;
}
