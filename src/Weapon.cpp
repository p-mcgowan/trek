/* Trek.cpp - CWeapon
 *
 * Random weapon generator class
 */

#include "Weapon.h"

 class CWeapon;

CWeapon::CWeapon() {  // params - lvl, raremult...
    std::string path("../lists/weapon/names/");
    this->type = getRandomStringFromFile("../lists/weapon/names/type", TYPE);
    path += this->type;
    this->sType = getRandomStringFromFile(path, TYPE);
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
	std::string path, statsLine;
	this->getBaseStats();
	if (!this->rarity.compare("common")) {
		this->prefix = "";
		this->suffix = "";
		//this->name = this->sType;
	}
	else if (!this->rarity.compare("uncommon")) {
		if (rndm(0, 1)) {
			this->suffix = "";
			this->prefix = getRandomStringFromFile(PREFIX_FILE, NAME);
			this->name = this->prefix + " " + this->sType;
			this->processStatsLine(getStatsLine(PREFIX_FILE, this->prefix));
		}else {
			this->prefix = "";
			this->suffix = getRandomStringFromFile(SUFFIX_FILE, NAME);
			//this->name = this->sType + " " + this->suffix;
			this->processStatsLine(getStatsLine(SUFFIX_FILE, this->suffix));
		}
	}
	else if (!this->rarity.compare("rare")) {
		this->prefix = getRandomStringFromFile(PREFIX_FILE, NAME);
		this->suffix = getRandomStringFromFile(SUFFIX_FILE, NAME);
		this->processStatsLine(getStatsLine(SUFFIX_FILE, this->suffix));
		this->processStatsLine(getStatsLine(PREFIX_FILE, this->prefix));
		//wepName += this->prefix + " " + this->sType + " " + this->suffix;
		//this->name = wepName;

	}
	else if (!this->rarity.compare("epic")) {
		int mods = rndm(4, 6);
		for(int i = 0; i < mods; i++) {
			if (rndm(0, 1)) {
				this->prefix = getRandomStringFromFile(PREFIX_FILE, NAME);
				this->processStatsLine(getStatsLine(PREFIX_FILE, this->prefix));
			}
			else {
				this->suffix = getRandomStringFromFile(SUFFIX_FILE, NAME);
				this->processStatsLine(getStatsLine(SUFFIX_FILE, this->suffix));
			}
		}
		this->prefix = getRandomStringFromFile(PREFIX_FILE, NAME);
		this->suffix = getRandomStringFromFile(SUFFIX_FILE, NAME);
		this->processStatsLine(getStatsLine(SUFFIX_FILE, this->suffix));
		this->processStatsLine(getStatsLine(PREFIX_FILE, this->prefix));
		//this->name = wepName;
	}
	else {
		std::string path("../lists/weapon/legendary/");
		path += this->type + "/" + this->sType + "/names";
		this->name += getRandomStringFromFile(path, TYPE);
		this->prefix = "";
		this->suffix = "";
		path = "../lists/weapon/legendary/" + this->type + "/" + this->sType + "/" + this->name;
		std::ifstream legendaryFile(path.c_str());
		std::getline(legendaryFile, statsLine);
		this->processStatsLine(statsLine);
	}
	if(this->rarity.compare("legendary"))
		this->name = this->prefix + " " + this->sType + " " + this->suffix;
	return;
}

/* CWeapon::getStatsLine
 * 
 * Searches for the matching string in this->prefix or this->suffix file and
 * returns the corresponding line of stat bonuses after it
 */
std::string CWeapon::getStatsLine(std::string path, std::string toFind) {
	std::ifstream statsFile(path.c_str());
	std::string statsLine;
	std::getline(statsFile, statsLine);
	while (statsLine.compare(toFind)) {
		std::getline(statsFile, statsLine);
		std::getline(statsFile, statsLine);
	}
	std::getline(statsFile, statsLine);
	statsFile.close();
	std::cout << "stats: " << statsLine << std::endl;
	return statsLine;
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

/* CWeapon::processStatsLine
 *
 * Reads valur, this->name pairs from string and passes them to processing
 */
void CWeapon::processStatsLine(std::string statsLine) {
	std::string statsName, statsValue, delimiter = " ";
	size_t pos = 0;
	while ((pos = statsLine.find(delimiter)) != std::string::npos) {
		statsValue = statsLine.substr(0, pos);
		statsLine.erase(0, pos + delimiter.length());
		pos = statsLine.find(delimiter);

		statsName = statsLine.substr(0, pos);
		statsLine.erase(0, pos + delimiter.length());
		applyStats(statsName, std::stoi(statsValue));
	}
	return;
}

/* CWeapon::applyStats
 * 
 */
int CWeapon::applyStats(std::string statsName, int statsValue) {
	switch(statToIndex[statsName]) {
		case DMG:
			this->dmg += statsValue;
		case CRIT:
			this->crit += (float)((float)1/(float)statsValue);
			break;
		case SHOTS:
			this->shots += statsValue;
			break;
		case MAXDMG:
			this->maxdmg += statsValue;
			break;
		case MAXUPGRADES:
			if (nupgrades + statsValue <= mupgrades)
				mupgrades += statsValue;
			break;
		case LVLREQ:
			this->rlvl += statsValue;
			break;
		case CLOUTREQ:
			this->rclout += statsValue;
			break;
		case SOCIALREQ:
			this->rsocial += statsValue;
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
			this->stats[statToIndex[statsName]] += statsValue;
			break;
		default:
			std::cout << "unmatched stats pair" << statsName << " => " << statToIndex[statsName];
			return 1;
			break;
	}
	return 0;
}