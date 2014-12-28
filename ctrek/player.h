typedef struct{//inventory
	int cap; //cargo capacity
	int *slot;//stores itemid's
}cargo;
typedef struct{//equipment item ids
	int *weps;
}equipment;
typedef struct{
	//stats
	int mshield;//max shield
	int shield;
	int shieldregen;
	int mhull;//max hull
	int hull;
	int  mhardpoints;
	int speed;
	cargo *inv;
	equipment *equip;
	int *id;
}warship;
typedef struct{
	char *race;
	char *prof;//class
	char *sclass;//spec
	int lvl;
	int xp;
	//primary attributes
	int pilot;
	int combat;
	int science;
	int social;
	//seconday attributes
	//pilot
	int clout;//learn skills/use better equip
	int leadership;//equip bonuses
	int renown; //
	//combt
	int mechapt;//skill/equip
	int ability;//equip bonus
	int intuition;
	//science
	int intelligence; //learn skills/use better equip
	int tech; //equip bonuses
	int innovation;
	//mix sub skills
	int charisma; //pilot/social - dialog bonus
	int commerce; //science/social - market bonus
	int dexterity;//pilot/combat - 
	//
	warship *ship;
	char *skills;
	int *id;
}player;