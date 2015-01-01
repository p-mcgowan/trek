/* Trek.cpp - CWeapon
 *
 * Random weapon generator class
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

/* CWeapon::setRarity 
 * 
 * Applies a random rarity to a weapon
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
 *
 * Applies base stats and modifiers from subtype file for a weapon,
 * based on rarity
 */
void CWeapon::setStats() {
	std::string wepName, path, modLine;
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
			wep.processModLine(getModLine(PREFIX_FILE, prefix));
		}else {
			prefix = "";
			suffix = getRandomStringFromFile(SUFFIX_FILE);
			wepName += stype + " " + suffix;
			name = wepName;
			wep.processModLine(getModLine(SUFFIX_FILE, suffix));
		}
	}
	else if (!rarity.compare("rare")) {
		prefix = getRandomStringFromFile(PREFIX_FILE);
		suffix = getRandomStringFromFile(SUFFIX_FILE);
		wep.processModLine(getModLine(SUFFIX_FILE, suffix));
		wep.processModLine(getModLine(PREFIX_FILE, prefix));
		wepName += prefix + " " + stype + " " + suffix;
		name = wepName;

	}
	else if (!rarity.compare("epic")) {
		mods = rndm(4, 6);
		for(i = 0; i < mods; i++) {
			if (rndm(0, 1)) {
				prefix = getRandomStringFromFile(PREFIX_FILE);
				wep.processModLine(getModLine(PREFIX_FILE, prefix));
			}
			else {
				suffix = getRandomStringFromFile(SUFFIX_FILE);;
				wep.processModLine(getModLine(SUFFIX_FILE, suffix));
			}
		}
		prefix = getRandomStringFromFile(PREFIX_FILE);
		suffix = getRandomStringFromFile(SUFFIX_FILE);
		wep.processModLine(getModLine(SUFFIX_FILE, suffix));
		wep.processModLine(getModLine(PREFIX_FILE, prefix));
		wepName += prefix + " " + stype + " " + suffix;
		name = wepName;
	}
	else {
		std::string path("../lists/weapon/legendary/");
		path += type + "/" + stype + "/names";
		name += getRandomStringFromFile(path);
		prefix = "";
		suffix = "";
		path = "../lists/weapon/legendary/" + type + "/" + stype + "/" + name;
		istream::getline(path.c_str(), modLine);
		wep.processModLine(modLine);
	}
	return;
}

/* CWeapon::getModLine
 * 
 * Searches for the matching string in prefix or suffix file and
 * returns the corresponding line of mods after it
 */
std::string CWeapon::getModLine(std::string path, std::string toFind) {
	std::ifstream modFile(path.c_str());
	std::string modLine;
	istream::getline(modFile, modLine);
	while (!modLine.compare(toFind)) {
		istream::getline(modFile, modLine);
		istream::getline(modFile, modLine);
	}
	istream::getline(modFile, modLine);
	modFile.close();
	return modLine;
}

/* CWeapon::getBaseStats
 *
 * Sets the weapon's basic stats based on rarity and weapon
 * type/sub type
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

/* CWeapon::processModLine
 *
 * Reads valur, name pairs from string and passes them to processing
 */
void CWeapon::processModLine(std::string modLine) {
	std::string modName, modValue, delimiter = " ";

	size_t pos = 0;
	while ((pos = modLine.find(delimiter)) != std::string::npos) {
		modValue = modLine.substr(0, pos);
		modLine.erase(0, pos + delimiter.length());
		pos = modLine.find(delimiter)
		modLine.erase(0, pos + delimiter.length());
		
		pos = modLine.find(delimiter);
		modName = modLine.substr(0, pos);
		modLine.erase(0, pos + delimiter.length());
		pos = modLine.find(delimiter)
		modLine.erase(0, pos + delimiter.length());
		applyMods(modName, std::stoi(modValue));
	}
	modFile.close();
	return;
}

/* CWeapon::applyMods
 * 
 */
int CWeapon::applyMods(std::string modName, int modValue) {
	switch(modToIndex[modName]) {
		case DMG:
			dmg += modValue;
		case CRIT:
			crit += (float)((float)1/(float)modValue);
			break;
		case SHOTS:
			shots += modValue;
			break;
		case MAXDMG:
			maxdmg += modValue;
			break;
		case MAXUPGRADES:
			if (nupgrades + modValue <= mupgrades)
				mupgrades += modValue;
			break;
		case LVLREQ:
			rlvl += modValue;
			break;
		case CLOUTREQ:
			rclout += modValue;
			break;
		case SOCIALREQ:
			rsocial += modValue;
			break;
		case PILOT:
		case COMBAT:
		case SCIENCE:
		case SOCIAL:
		case CLOUT:
		case LEADERSHIP:
		case REKNOWN:
		case MECHAPT:
		case ABILITY:
		case INTUITION:
		case INTELECT:
		case TECHAPT:
		case INNOVATION:
		case CHARISMA:
		case COMMERCE:
		case DEXTERITY:
			mods[modToIndex[modName]] += modValue;
			break;
		default:
			std::cout << "unmatched mod pair" << modName << " => " << modToIndex[modName];
			return 1;
			break;
	}
	return 0;
}
