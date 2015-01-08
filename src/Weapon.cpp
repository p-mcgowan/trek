/* Trek.cpp - CWeapon
 *
 * Random weapon generator class
 */

#include "Weapon.h"

 class CWeapon;

/*  Default weapon
 *
 *  Creates a random generated weapon
 */
CWeapon::CWeapon() {  // params - lvl, raremult... - laser type energy cost
  this->type = getRandomStringFromFile(WTYPE_FILE, false);
  std::string path = WNAMES_FILE + this->type;
  this->sType = getRandomStringFromFile(path, false);
  this->setRarity();
  this->setStats();
  return;
}

/*  Unique weapon
 *
 *  Creates a unique generated weapon from file
 */
CWeapon::CWeapon(std::string uniqueName) {  // unique
  this->rarity = "unique";
  if (uniqueName == "") // random unique
    this->name = getRandomStringFromFile(WUNIQUENAMES_FILE, false);
  else
    this->name = uniqueName;
  this->setStats();
  return;
}

/*  Custom weapon
 *
 *  Creates a fully customized weapon
 */
/*CWeapon::CWeapon(  // custom weapon, crafting?
  std::string type,
  std::string sType,
  std::string prefix,
  std::string suffix,
  std::string name,
  std::string desc,
  int lvl,
  int tier,
  float crit,
  int shots,
  int maxdmg,
  int dmg,
  std::string ammoType,
  int mupgrades,
  int nupgrades,
  int *upgrades,
  std::string rarity,
  int rlvl,
  int rclout,
  int rsocial,
  std::initializer_list <std::string> stats
  ) {
  this->type = type;
  this->sType = sType;
  this->prefix = prefix;
  this->suffix = suffix;
  this->name = name;
  this->desc = desc;
  this->lvl = lvl;
  this->tier = tier;
  this->crit = crit;
  this->shots = shots;
  this->maxdmg = maxdmg;
  this->dmg = dmg;
  this->ammoType = ammoType;
  this->mupgrades = mupgrades;
  this->nupgrades = nupgrades;
  this->*upgrades = *upgrades;
  this->rarity = rarity;
  this->rlvl = rlvl;
  this->rclout = rclout;
  this->rsocial = rsocial;
  for (auto i : stats)
    stats[modtoIndex[i]] = std::stoi(++i);
  return;
}*/

/* CWeapon::setRarity 
 * 
 * Applies a random rarity to a weapon
 */
void CWeapon::setRarity() {  // int mult) {
  int i = rndm(0, 1000);//S + COMMONTHRESH/2;
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
  this->setBaseStats();
  if (this->rarity == "common") {
    this->prefix = "";
    this->suffix = "";
  }
  else if (this->rarity == "uncommon") {
    if (rndm(0, 1)) {
      this->suffix = "";
      this->prefix = getRandomStringFromFile(WPREFIX_FILE);
      this->processStatsLine(getStatsLine(WPREFIX_FILE, this->prefix));
    } else {
      this->prefix = "";
      this->suffix = getRandomStringFromFile(WSUFFIX_FILE);
      this->processStatsLine(getStatsLine(WSUFFIX_FILE, this->suffix));
    }
  }
  else if (this->rarity == "rare") {
    this->prefix = getRandomStringFromFile(WPREFIX_FILE);
    this->suffix = getRandomStringFromFile(WSUFFIX_FILE);
    this->processStatsLine(getStatsLine(WSUFFIX_FILE, this->suffix));
    this->processStatsLine(getStatsLine(WPREFIX_FILE, this->prefix));
  }
  else if (this->rarity == "epic") {
    int mods = rndm(4, 6);
    for(int i = 0; i < mods; i++) {
      if (rndm(0, 1)) {
        this->prefix = getRandomStringFromFile(WPREFIX_FILE);
        this->processStatsLine(getStatsLine(WPREFIX_FILE, this->prefix));
      }
      else {
        this->suffix = getRandomStringFromFile(WSUFFIX_FILE);
        processStatsLine(getStatsLine(WSUFFIX_FILE, this->suffix));
      }
    }
    this->prefix = getRandomStringFromFile(WPREFIX_FILE);
    this->suffix = getRandomStringFromFile(WSUFFIX_FILE);
    this->processStatsLine(getStatsLine(WSUFFIX_FILE, this->suffix));
    this->processStatsLine(getStatsLine(WPREFIX_FILE, this->prefix));
  }
  else if (this->rarity == "unique") {
    std::string path(WUNIQUES_FILE + this->name + "/stats");
    std::ifstream uniqueStats(path);
    if (!uniqueStats.is_open())
      LOGERR("setStats: Error opening file for reading: \"" << path << "\"", 1);
    std::string statsLine;
    std::getline(uniqueStats, statsLine);
    processStatsLine(statsLine);
    this->prefix = "";
    this->suffix = "";
  }
  else {  // legendary
    std::string path(WLEGENDARY_FILE);
    path += this->type + "/" + this->sType + "/names";
    this->name = getRandomStringFromFile(path);
    this->prefix = "";
    this->suffix = "";
    this->processStatsLine(getStatsLine(path, this->name));
  }
  if (this->rarity == "legendary") {
    name = this->sType;
    if (this->prefix != "")
      this->name = this->prefix + " " + this->name;
    if (this->suffix != "")
      this->name += + " " + this->suffix;
  }
  return;
}

/* CWeapon::getStatsLine
 * 
 * Searches for the matching string in this->prefix or this->suffix file and
 * returns the corresponding line of stat bonuses after it
 */
std::string CWeapon::getStatsLine(std::string path, std::string toFind) {
  std::ifstream statsFile(path);
  if (!statsFile.is_open())
    LOGERR("getStatsLine: Error opening file for reading: " << path, 1);
  std::string statsLine;
  std::getline(statsFile, statsLine);
  //statsLine = trimCRLF(statsLine);
  while (statsLine != toFind) {
    std::getline(statsFile, statsLine);
    std::getline(statsFile, statsLine);
    //statsLine = trimCRLF(statsLine);
  }
  std::getline(statsFile, statsLine);
  statsFile.close();
  return statsLine;//trimCRLF(statsLine);
}

/* CWeapon::setBaseStats
 *
 * Sets the weapon's basic stats based on this->rarity and weapon
 * this->type/sub this->type
 */
void CWeapon::setBaseStats() {  // int lvl
  std::string path(RELATIVE_LISTS"weapon/");
  if (this->rarity == "legendary") {
    path += "legendary/" + this->type + "/" + this->sType + "/stats";
  }
  else if (this->rarity == "unique") {
    path += "uniques/" + this->name + "/basestats";
  }
  else {
    path += "stats/" + this->type + "/" + this->sType;
  }
  std::ifstream subTypeFile(path);
  if (!subTypeFile.is_open())
    LOGERR("setBaseStats: Error opening file for reading: " << path, 1);
  subTypeFile >> this->dmg;
  subTypeFile >> this->maxdmg;
  subTypeFile >> this->shots;
  subTypeFile >> this->crit;
  subTypeFile >> this->mupgrades;
  std::getline(subTypeFile, this->desc);  // clear out newline
  std::getline(subTypeFile, this->desc);
  subTypeFile >> this->ammoType;
  subTypeFile >> this->rlvl;
  subTypeFile >> this->rclout;
  subTypeFile >> this->rsocial;
  subTypeFile.close();
  this->tier = mupgrades - 1;
  this->nupgrades = rndm(0, this->mupgrades);
  return;
}

/* CWeapon::processStatsLine
 *
 * Reads value, this->name pairs from string and passes them to processing
 */
void CWeapon::processStatsLine(std::string statsLine) {
  //statsLine = trimCRLF(statsLine);
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
 * Decodes a <string, int> pair and applies it to the weapon
 */
int CWeapon::applyStats(std::string statsName, int statsValue) {
  switch(statToIndex[statsName]) {
    case DMG:
      this->dmg += statsValue;
    case CRIT:
      this->crit += statsValue;
      break;
    case SHOTS:
      this->shots += statsValue;
      break;
    case MAXDMG:
      this->maxdmg += statsValue;
      break;
    case MAXUPGRADES:
      this->mupgrades += statsValue;
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
      LOGW("unmatched stats pair" << statsName << " => " << statToIndex[statsName]);
      return 1;
      break;
  }
  return 0;
}

/* CWeapon::applyUpgrades
 * 
 * Adds stats to a weapon from STAT, VALUE pair
 */
void CWeapon::applyUpgrade(std::pair<std::string, int> statPair) {
  applyStats(std::get<0>(statPair), std::get<1>(statPair));
}
