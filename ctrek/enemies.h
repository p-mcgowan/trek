typedef struct{
	char *type;//slele,zomb,wolf
	char *prefix;//angry, dire,champion, ghost --attributes
	char *suffix;//of sorrow,brute,cleric,mage,ranger of fthe flame, vampire --skills
	int maxhp;
	int hp;
	int marmour;
	int armour;//mana shield/armour
	char **spells;//shield,heal fireball--determined by prefix,suffix, type
	int str;//hp,armour,crit res
	int agl;//dodge,turns, ranged skills
	int wis;//mana, spell dmg,spell crit
	float dodge; //melee
	float reflex; //ranged
	float intellect; //spell dodge
	float mhit; //melee chance to hit
	float shit; //spell to hit
	float rhit; //ranged to hit
	float mcrit;//melee crit
	float scrit;//spell crit
	float rcrit;
	float mcritres;//melee crit res
	float scritres;
	int rfire;//fire res
	int rcold;//cold
	int rlight;//lightning
	int rpois;//poison
	int rstun;//stun
	int rbled;//bleeding
	int *id;
}enemy;