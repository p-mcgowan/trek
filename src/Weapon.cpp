/* Trek.cpp - CWeapon
 *
 * Random weapon generator class
 */

#include "Weapon.h"

 class CWeapon;

CWeapon::CWeapon() {  // params - lvl, raremult...
    std::string path("../lists/weapon/names/");
    this->type = getRandomStringFromFile("../lists/weapon/names/type");
    path += this->type;
    this->sType = getRandomStringFromFile(path);
    this->setRarity();
    this->setStats();
    return;
}

/* CWeapon::setRarity 
 * 
 * Applies a random rarity to a weapon
 */
void CWeapon::setRarity() {  // int mult) {
	int i = rndm(0, 1000);
	if (i <= COMMONTHRESH)
		this->rarity = "common";
	else if (i <= UNCOMMONTHRESH)
		this->rarity = "uncommon";
	else if (i <= RARETHRESH)
		this->rarity = "rare";
	else if (i <= EPICTHRESH)
		this->rarity = "epic";
	else
		this->rarity = "legendary";
}

/* CWeapon::setStats
 *
 * Applies base stats and modifiers from subtype file for a weapon,
 * based on rarity
 */
void CWeapon::setStats() {
	std::string path, modLine;
	this->getBaseStats();
	if (!this->rarity.compare("common")) {
		this->prefix = "";
		this->suffix = "";
		//this->name = this->sType;
	}
	else if (!this->rarity.compare("uncommon")) {
		if (rndm(0, 1)) {
			this->suffix = "";
			this->prefix = getRandomStringFromFile(PREFIX_FILE);
			this->name = this->prefix + " " + this->sType;
			this->processModLine(getModLine(PREFIX_FILE, this->prefix));
		}else {
			this->prefix = "";
			this->suffix = getRandomStringFromFile(SUFFIX_FILE);
			//this->name = this->sType + " " + this->suffix;
			this->processModLine(getModLine(SUFFIX_FILE, this->suffix));
		}
	}
	else if (!this->rarity.compare("rare")) {
		this->prefix = getRandomStringFromFile(PREFIX_FILE);
		this->suffix = getRandomStringFromFile(SUFFIX_FILE);
		this->processModLine(getModLine(SUFFIX_FILE, this->suffix));
		this->processModLine(getModLine(PREFIX_FILE, this->prefix));
		//wepName += this->prefix + " " + this->sType + " " + this->suffix;
		//this->name = wepName;

	}
	else if (!this->rarity.compare("epic")) {
		int mods = rndm(4, 6);
		for(int i = 0; i < mods; i++) {
			if (rndm(0, 1)) {
				this->prefix = getRandomStringFromFile(PREFIX_FILE);
				this->processModLine(getModLine(PREFIX_FILE, this->prefix));
			}
			else {
				this->suffix = getRandomStringFromFile(SUFFIX_FILE);;
				this->processModLine(getModLine(SUFFIX_FILE, this->suffix));
			}
		}
		this->prefix = getRandomStringFromFile(PREFIX_FILE);
		this->suffix = getRandomStringFromFile(SUFFIX_FILE);
		this->processModLine(getModLine(SUFFIX_FILE, this->suffix));
		this->processModLine(getModLine(PREFIX_FILE, this->prefix));
		//this->name = wepName;
	}
	else {
		std::string path("../lists/weapon/legendary/");
		path += this->type + "/" + this->sType + "/names";
		this->name += getRandomStringFromFile(path);
		this->prefix = "";
		this->suffix = "";
		path = "../lists/weapon/legendary/" + this->type + "/" + this->sType + "/" + this->name;
		std::ifstream legendaryFile(path.c_str());
		std::getline(legendaryFile, modLine);
		this->processModLine(modLine);
	}
	if(this->rarity.compare("legendary"))
		this->name = this->prefix + " " + this->sType + " " + this->suffix;
	return;
}

/* CWeapon::getModLine
 * 
 * Searches for the matching string in this->prefix or this->suffix file and
 * returns the corresponding line of mods after it
 */
std::string CWeapon::getModLine(std::string path, std::string toFind) {
	std::ifstream modFile(path.c_str());
	std::string modLine;
	std::getline(modFile, modLine);
	while (!modLine.compare(toFind)) {
		std::getline(modFile, modLine);
		std::getline(modFile, modLine);
	}
	std::getline(modFile, modLine);
	modFile.close();
	return modLine;
}

/* CWeapon::getBaseStats
 *
 * Sets the weapon's basic stats based on this->rarity and weapon
 * this->type/sub this->type
 */
void CWeapon::getBaseStats() {  // int lvl
	std::ifstream subTypeFile;
	std::string path("../lists/weapon/");
	if (!this->rarity.compare("legendary")) {
		path += "legendary/" + this->type + "/stats/" + this->sType;
	}
	else {
		path += "stats/" + this->type + "/" + this->sType;
	}
	subTypeFile.open(path.c_str());
	subTypeFile >> this->dmg;
	subTypeFile >> this->maxdmg;
	subTypeFile >> this->shots;
	subTypeFile >> this->crit;
	subTypeFile >> this->mupgrades;
	std::getline(subTypeFile, this->desc);
	subTypeFile.close();
	this->tier = mupgrades - 1;
	this->nupgrades = rndm(0, this->mupgrades);
	return;
}

/* CWeapon::processModLine
 *
 * Reads valur, this->name pairs from string and passes them to processing
 */
void CWeapon::processModLine(std::string modLine) {
	std::string modName, modValue, delimiter = " ";

	size_t pos = 0;
	while ((pos = modLine.find(delimiter)) != std::string::npos) {
		modValue = modLine.substr(0, pos);
		modLine.erase(0, pos + delimiter.length());
		pos = modLine.find(delimiter);
		modLine.erase(0, pos + delimiter.length());
		
		pos = modLine.find(delimiter);
		modName = modLine.substr(0, pos);
		modLine.erase(0, pos + delimiter.length());
		pos = modLine.find(delimiter);
		modLine.erase(0, pos + delimiter.length());
		applyMods(modName, std::stoi(modValue));
	}
	return;
}

/* CWeapon::applyMods
 * 
 */
int CWeapon::applyMods(std::string modName, int modValue) {
	switch(modToIndex[modName]) {
		case DMG:
			this->dmg += modValue;
		case CRIT:
			this->crit += (float)((float)1/(float)modValue);
			break;
		case SHOTS:
			this->shots += modValue;
			break;
		case MAXDMG:
			this->maxdmg += modValue;
			break;
		case MAXUPGRADES:
			if (nupgrades + modValue <= mupgrades)
				mupgrades += modValue;
			break;
		case LVLREQ:
			this->rlvl += modValue;
			break;
		case CLOUTREQ:
			this->rclout += modValue;
			break;
		case SOCIALREQ:
			this->rsocial += modValue;
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
			this->mods[modToIndex[modName]] += modValue;
			break;
		default:
			std::cout << "unmatched mod pair" << modName << " => " << modToIndex[modName];
			return 1;
			break;
	}
	return 0;
}