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
#define RELATIVE_LISTS ".."
#define PREFIX_FILE RELATIVE_LISTS"/weapon/prefix"
#define SUFFIX_FILE RELATIVE_LISTS"/weapon/suffix"
#define UNIQUES_FILE RELATIVE_LISTS"/weapon/unique"

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
    int lvl = 0;
    int tier = 0;
      // base stats (lvl + rarity mult)
    float crit = 0;
    int shots = 0;  // hits per turn
    int maxdmg = 0;
    int dmg = 0;
    std::string ammoType;  // 
    int mupgrades = 0;  // max # sockets
    int nupgrades = 0;  // actual # sockets
    std::vector<std::pair<std::string, int>> upgrades;  // STAT, value
    std::string rarity = "";  // common, uncommon, rare, unique, epic, legendary
    int rlvl = 0;  // lvl req
    int rclout = 0;  // combat subskill req;
    int rsocial = 0;
    int stats[20] = {0};  // other buffs from upgrades?

  public:
    CWeapon();
    CWeapon(std::string name);  // unique from file
    CWeapon(  // Fully custom, from file or called from uniqeu
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
      std::initializer_list <std::string> stats);
    std::string getType() {return type;}
    std::string getSType() {return sType;}
    std::string getPrefix() {return prefix;}
    std::string getSuffix() {return suffix;}
    std::string getName() {return name;}
    std::string getDesc() {return desc;}
    int getLvl() {return lvl;}
    int getTier() {return tier;}
    float getCrit() {return crit;}
    int getShots() {return shots;}
    int getMaxdmg() {return maxdmg;}
    int getDmg() {return dmg;}
    std::string getAmmoType() {return ammoType;}
    int getMupgrades() {return mupgrades;}
    int getNupgrades() {return nupgrades;}
    //int *getUpgrades() {return upgrades;}
    std::string getRarity() {return rarity;}
    int getRlvl() {return rlvl;}
    int getRclout() {return rclout;}
    int getRsocial() {return rsocial;}
    int getStats(int i) {return stats[i];}

    void setRarity();
    void setStats();
    std::string getStatsLine(std::string path, std::string toFind);
    void setBaseStats();
    void processStatsLine(std::string modLine);
    int applyStats(std::string modName, int modValue);

 };

/* Map strings from mod files to array indeces
 *
 */
 std::map<std::string,int> statToIndex = {
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
 std::map<int,std::string> indexToStat = {
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
