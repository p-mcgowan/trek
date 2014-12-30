/* Trek.cpp CWeapon
 * 
 * Random weapon generator and utilities
 */

#include "global.h"

// Modifier definitions
#define PIL 0
#define COM 1
#define SCI 2
#define SOC 3
#define CLO 4
#define LEAD 5
#define REN 6
#define MEC 7
#define ABL 8
#define INTU 9
#define INTE 10
#define TEC 11
#define INN 12
#define CHA 13
#define COMM 14
#define DEX 15
#define DMG 0

// Prefix and suffix list file locations
#define PREFIX_FILE "../lists/weapon/prefix"
#define SUFFIX_FILE "../lists/weapon/suffix"

// Thresholds for rarities [0-1000]
#define COMMONTHRESH 750
#define UNCOMMONTHRESH 900
#define RARETHRESH 983
#define EPICTHRESH 994


class CWeapon {

	private:
		std::string type;
		std::string sType;
		std::string prefix;
		std::string suffix;
		std::string name;
		std::string desc;  // weapon writup
		int lvl;
		int tier;
		  // base stats (lvl + rarity mult)
		float crit;
		int shots;  // hits per turn
		int maxdmg;
		int dmg;
		std::string ammoType;  // 
		int mupgrades;  // max # sockets
		int nupgrades;  // actual # sockets
		int *upgrades;  // sockets holding upgrade id
		int rarity;  // 0-1000
		int rlvl;  // lvl req
		int rclout;  // combat subskill req;
		int rsocial;
		int *mods;  // other buffs from upgrades?
		int *id;  // may not use?

	public:
		std::string getType() {return type;}
		std::string getStype() {return sType;}
		std::string getPrefix() {return prefix;}
		std::string getSuffix() {return suffix;}
		std::string getName() {return name;}
		std::string getDesc() {return desc;}
		  // getDescription()  // full printout
		void getBaseStats(CWeapon wep);
		void setBaseStats(weapon *wep);
		void setRarity();
		void getmods(weapon *wep, char mode);
		void findmod(char *path,char *matchstr,weapon *wep,char rare);
		int processmods(weapon *wep, char *mod, int val);
		int getwepdesc(weapon *wep);

 };