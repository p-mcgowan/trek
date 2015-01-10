/* Trek.cpp - CWeapon
 * 
 * Random weapon generator and utilities
 */

#include "global.h"
#include "fileUtils.h"

// Stats definitions
#define PILOT     0
#define COMBAT    1
#define SCIENCE   2
#define SOCIAL    3
#define CLOUT     4
#define LEADERSHIP  5
#define REKNOWN   6
#define MECHAPT   7
#define ABILITY   8
#define INTUITION   9
#define INTELECT  10
#define TECHAPT   11
#define INNOVATION  12
#define CHARISMA  13
#define COMMERCE  14
#define DEXTERITY   15
#define DMG     16
#define CRIT    17
#define SHOTS   18
#define MAXDMG    19
#define MAXUPGRADES 20
#define LVLREQ    21
#define CLOUTREQ  22
#define SOCIALREQ 23


// Stat file locations
#define RELATIVE_LISTS "../lists/"
#define WNAMES_FILE RELATIVE_LISTS"weapon/names/"
#define WTYPE_FILE WNAMES_FILE"type"
#define WPREFIX_FILE RELATIVE_LISTS"weapon/prefix"
#define WSUFFIX_FILE RELATIVE_LISTS"weapon/suffix"
#define WUNIQUES_FILE RELATIVE_LISTS"weapon/uniques/"
#define WUNIQUENAMES_FILE RELATIVE_LISTS"weapon/uniques/names"
#define WLEGENDARY_FILE RELATIVE_LISTS"weapon/legendary/"

// Thresholds for rarities [0-1000]
#define COMMONTHRESH 750
#define UNCOMMONTHRESH 900
#define RARETHRESH 983
#define EPICTHRESH 994

class CWeapon {

  private:
    std::string type = "";
    std::string sType = "";
    std::string prefix = "";
    std::string suffix = "";
    std::string name = "";
    std::string desc = "";  // weapon writup
    std::string rarity = "";  // common, uncommon, rare, unique, epic, legendary
    std::string ammoType;  // 
    int lvl = 0;
    int tier = 0;
    float crit = 0;
    int shots = 0;  // hits per turn
    int maxdmg = 0;
    int dmg = 0;
    int mupgrades = 0;  // max # sockets
    int nupgrades = 0;  // actual # sockets
    int rlvl = 0;  // lvl req
    int rclout = 0;  // combat subskill req;
    int rsocial = 0;
    std::vector<std::pair<std::string, std::string>> upgrades;  // STAT, value
    std::vector<std::pair<std::string, std::string>> stats;

  public:
    CWeapon();
    CWeapon(std::string uniqueName);  // unique from file
    CWeapon(  // Fully custom, from file or called from uniqeu
      std::string type,
      std::string sType,
      std::string prefix,
      std::string suffix,
      std::string name,
      std::string desc,
      std::string ammoType,
      std::string rarity,
      int lvl,
      int tier,
      float crit,
      int shots,
      int maxdmg,
      int dmg,
      int mupgrades,
      int nupgrades,
      std::vector<std::pair<std::string, std::string>> upgrades,
      int rlvl,
      int rclout,
      int rsocial,
      std::vector<std::pair<std::string, std::string>> stats);

    // Getters
    std::string getType() {return type;}
    std::string getSType() {return sType;}
    std::string getPrefix() {return prefix;}
    std::string getSuffix() {return suffix;}
    std::string getName() {return name;}
    std::string getDesc() {return desc;}
    std::string getAmmoType() {return ammoType;}
    std::string getRarity() {return rarity;}
    int getLvl() {return lvl;}
    int getTier() {return tier;}
    float getCrit() {return crit;}
    int getShots() {return shots;}
    int getMaxdmg() {return maxdmg;}
    int getDmg() {return dmg;}
    int getMupgrades() {return mupgrades;}
    int getNupgrades() {return nupgrades;}
    int getRlvl() {return rlvl;}
    int getRclout() {return rclout;}
    int getRsocial() {return rsocial;}
    std::vector<std::pair<std::string, std::string>> getStats() {return stats;}
    std::vector<std::pair<std::string, std::string>> getUpgrades() {return upgrades;}
		//std::string getStat(std::string);

    // Setters
    void setRarity();
    void setStats();
    void setBaseStats();
    std::string getStatsLine(std::string path, std::string toFind);
    void processStatsLine(std::string modLine);

    // Manipulators
    void applyStats(std::string statsName, int statsValue);
    void applyUpgrade(std::pair<std::string, int> statPair);
    void removeUpgrade(std::pair<std::string, int> statPair);
 };

/* Map strings from mod files to array indeces
 *
 */
 std::map<std::string, int> statToIndex = {
  {"PILOT", 0},
  {"COMBAT", 1},
  {"SCIENCE", 2},
  {"SOCIAL", 3},
  {"CLOUT", 4},
  {"LEADERSHIP", 5},
  {"REKNOWN", 6},
  {"MECHAPT", 7},
  {"ABILITY", 8},
  {"INTUITION", 9},
  {"INTELECT", 10},
  {"TECHAPT", 11},
  {"INNOVATION", 12},
  {"CHARISMA", 13},
  {"COMMERCE", 14},
  {"DEXTERITY", 15},
  {"DMG", 16},
  {"CRIT", 17},
  {"SHOTS", 18},
  {"MAXDMG", 19},
  {"MAXUPGRADES", 20},
  {"LVLREQ", 21},
  {"CLOUTREQ", 22},
  {"SOCIALREQ", 23}
};
 std::map<int, std::string> indexToStat = {
  {0, "PILOT"},
  {1, "COMBAT"},
  {2, "SCIENCE"},
  {3, "SOCIAL"},
  {4, "CLOUT"},
  {5, "LEADERSHIP"},
  {6, "REKNOWN"},
  {7, "MECHAPT"},
  {8, "ABILITY"},
  {9, "INTUITION"},
  {10, "INTELECT"},
  {11, "TECHAPT"},
  {12, "INNOVATION"},
  {13, "CHARISMA"},
  {14, "COMMERCE"},
  {15, "DEXTERITY"},
  {16, "DMG"},
  {17, "CRIT"},
  {18, "SHOTS"},
  {19, "MAXDMG"},
  {20, "MAXUPGRADES"},
  {21, "LVLREQ"},
  {22, "CLOUTREQ"},
  {23, "SOCIALREQ"}
};

std::map<std::string, std::string> statToDataType = {
  {"PILOT", "int"},
  {"COMBAT", "int"},
  {"SCIENCE", "int"},
  {"SOCIAL", "int"},
  {"CLOUT", "int"},
  {"LEADERSHIP", "int"},
  {"REKNOWN", "int"},
  {"MECHAPT", "int"},
  {"ABILITY", "int"},
  {"INTUITION", "int"},
  {"INTELECT", "int"},
  {"TECHAPT", "int"},
  {"INNOVATION", "int"},
  {"CHARISMA", "int"},
  {"COMMERCE", "int"},
  {"DEXTERITY", "int"},
  {"DMG", "int"},
  {"CRIT", "float"},
  {"SHOTS", "int"},
  {"MAXDMG", "int"},
  {"MAXUPGRADES", "int"},
  {"LVLREQ", "int"},
  {"CLOUTREQ", "int"},
  {"SOCIALREQ", "int"}
};
