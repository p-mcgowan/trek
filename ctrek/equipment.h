typedef struct{
	//detail stats
	char *type;
	char *stype;
	char *prefix;
	char *suffix;
	char *name;
	char *desc;//weapon writup
	int lvl;
	int tier;
	//base stats (lvl + rarity mult)
	float crit;
	int shots; //hits per turn
	int maxdmg;
	int dmg;
	int ammo; //how much ammo for this type in inventory -- have elsewhere?
	int mupgrades;//max # sockets
	int nupgrades;//actual # sockets
	int *upgrades;//sockets holding upgrade id
	char *rarity;//0-1000
	int rlvl;//lvl req
	int rclout; //combat subskill req;
	int rsocial;
	int *mods; //other buffs from upgrades?
	int *id; //may not use?
}weapon;
typedef struct{
	//mods - base stats, player stats, abils?
}wupgrade;
typedef struct{
	char *type;//head,chest,shield...
	char *stype;//circlet,horned helm...
	char *tier;//cloth,leather,scale,chain,plate
	char *prefix;
	char *suffix;
	int lvl;
	float crit;//crit resist
	int armourpts;
	int armourmult;
	int rarity;//0-5 ?
	int msockets;//max # sockets
	int nsockets;//actual # sockets
	int *sockets;//sockets holding gem id
	int maxdurability;
	int durability;
	char **mods;
	int *id;
}hull;
typedef struct{ //neck, lring, rring
	char *type;//ring,amulet...
	char *stype;//quarts,ruby
	char *prefix;
	char *suffix;
	int lvl;
	int rarity;//0-5 ?
	char **mods;
	int *id;
}acc;
