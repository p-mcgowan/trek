struct Player *currentPlayer;
#define SECURITY_SAFE    1
#define SECURITY_UNSAFE  2
#define SECURITY_INRANGE 3

#define NONE   0
#define PLAYER 1
#define ENEMY  2


#define KEY_ENEMY_MOVE   'E'
#define KEY_SKILLS_ARRAY 'S'
#define KEY_TAKE_DMG     'T'
#define KEY_HUDSTAT      'H'
#define KEY_SCANS        'l'
#define KEY_TORPEDO      'u'
#define KEY_STATS        'p'
#define KEY_SKILLS       'k'
#define KEY_RANGE        'r'
#define KEY_SCAN         'n'
#define KEY_ATTACK       't'
#define KEY_DIVERT       'o'
#define KEY_WARP_UP      'w'
#define KEY_WARP_LEFT    'a'
#define KEY_WARP_RIGHT   'd'
#define KEY_WARP_DOWN    's'
#define KEY_UP           65
#define KEY_DOWN         66
#define KEY_RIGHT        67
#define KEY_LEFT         68
#define KEY_RETURN       10
#define KEY_HELP         '?'
#define KEY_DEBUG ':'
#define KEY_TEST  ';'

#define _HS_I_PLASMA     0
#define _HS_I_DUTERIUM   1
#define _HS_I_LATINUM    2
#define _HS_I_WATER      3
#define _HS_I_URANIUM    4
#define _HS_I_WEAPON1    0
#define _HS_I_WEAPON2    1
#define _HS_I_WEAPON3    2
#define _HS_I_WEAPON4    3
#define _HS_I_WEAPON5    4

// Commodity indeces
#define COMMODITY_SHIELD    1
#define COMMODITY_HULL      2
#define COMMODITY_ENERGY    3
#define COMMODITY_TORPEDOS  4
#define COMMODITY_SHIP_TYPE 5
#define COMMODITY_PLASMA    6
#define COMMODITY_DUTERIUM  7
#define COMMODITY_LATINUM   8
#define COMMODITY_WATER     9
#define COMMODITY_URANIUM   10
#define COMMODITY_WEAPONS   11
//#define COMMODITY_WEAPON1   11
//#define COMMODITY_WEAPON2   12
//#define COMMODITY_WEAPON3   13
//#define COMMODITY_WEAPON4   14
//#define COMMODITY_WEAPON5   15

#define HS_DATE       currentPlayer->date
#define HS_SHIP       currentPlayer->ship
#define HS_SHIELD     currentPlayer->ship->shield
#define HS_HULL       currentPlayer->ship->hull
#define HS_SYS_X      currentPlayer->systemX
#define HS_SYS_Y      currentPlayer->systemY
#define HS_GAL_X      currentPlayer->galaxyX
#define HS_GAL_Y      currentPlayer->galaxyY
#define HS_SECURITY   currentPlayer->security
#define HS_ENERGY     currentPlayer->ship->energy
#define HS_TORPEDOS   currentPlayer->ship->torpedos
#define HS_CREDITS    currentPlayer->credits
#define HS_CARGO      currentPlayer->ship->cargo()
#define HS_PLASMA     currentPlayer->ship->commodities[_HS_I_PLASMA]
#define HS_DUTERIUM   currentPlayer->ship->commodities[_HS_I_DUTERIUM]
#define HS_LATINUM    currentPlayer->ship->commodities[_HS_I_LATINUM]
#define HS_WATER      currentPlayer->ship->commodities[_HS_I_WATER]
#define HS_URANIUM    currentPlayer->ship->commodities[_HS_I_URANIUM]
#define HS_SHIP_TYPE  currentPlayer->ship->shipType
#define HS_WEAPON1    currentPlayer->ship->weapons[0]
#define HS_WEAPON2    currentPlayer->ship->weapons[1]
#define HS_WEAPON3    currentPlayer->ship->weapons[2]
#define HS_WEAPON4    currentPlayer->ship->weapons[3]
#define HS_WEAPON5    currentPlayer->ship->weapons[4]
#define HS_WEAPONS    currentPlayer->ship->weapons
#define HS_NWEAPONS   currentPlayer->ship->weaponCount()
#define HS_UP_DMG     currentPlayer->ship->upgrades[0]
#define HS_UP_RANGE   currentPlayer->ship->upgrades[1]
#define HS_UP_HULL    currentPlayer->ship->upgrades[2]
#define HS_UP_SHIELD  currentPlayer->ship->upgrades[3]
#define HS_UP_CARGO   currentPlayer->ship->upgrades[4]
#define HS_UP_TORP    currentPlayer->ship->upgrades[5]
#define HS_UP_ENGINE  currentPlayer->ship->upgrades[6]
#define HS_DEPLETION  currentPlayer->ship->depletion
#define HS_IN_RANGE   currentPlayer->ship->nGunsInRange
#define HS_PASSENGERS currentPlayer->ship->passengers
#define HS_ACHIEV     currentPlayer->achievements
#define HS_SKILLS     currentPlayer->skills
#define HS_PASSENGERS currentPlayer->ship->passengers
#define HS_LASTBUY    currentPlayer->commodityLastBuy
#define HS_POS        currentPlayer->pos()


#define ACHIEVE_I_DISTANCE 0
#define ACHIEVE_I_SYSTEMS  1
#define ACHIEVE_I_PLANETS  2
#define ACHIEVE_I_SHIPS    3
#define ACHIEVE_I_WEAPONS  4
#define ACHIEVE_I_KILLS    5
#define ACHIEVE_I_SIZE     50

#define LASTBUY_PLASMA     currentPlayer->commodityLastBuy[0]
#define LASTBUY_DUTERIUM   currentPlayer->commodityLastBuy[1]
#define LASTBUY_LATINUM    currentPlayer->commodityLastBuy[2]
#define LASTBUY_WATER      currentPlayer->commodityLastBuy[3]
#define LASTBUY_URANIUM    currentPlayer->commodityLastBuy[4]

#define SKILLS_I_SIZE      50

/** ACHIEVEMENTS
0: distance travelled
1: systems visited
2: planets visited
3: ships commanded
4: weapons obtained
5: kills - leads to combat status
**/

#define ACHIEVE_DISTANCE currentPlayer->achievements[0]
#define ACHIEVE_SYSTEMS  currentPlayer->achievements[1]
#define ACHIEVE_PLANETS  currentPlayer->achievements[2]
#define ACHIEVE_SHIPS    currentPlayer->achievements[3]
#define ACHIEVE_WEAPONS  currentPlayer->achievements[4]
#define ACHIEVE_KILLS    currentPlayer->achievements[5]

struct System *currentSystem;

#define EN_POS         currentSystem->enemy->pos()
#define EN_SYS_X       currentSystem->enemy->systemX
#define EN_SYS_Y       currentSystem->enemy->systemY
#define EN_SHIP_TYPE   currentSystem->enemy->ship->shipType
#define EN_SHIP        currentSystem->enemy->ship
#define EN_TILE        currentSystem->enemy->tile
#define EN_WEAPONS     currentSystem->enemy->ship->weapons
#define EN_NWEAPONS    currentSystem->enemy->ship->weaponCount()
#define EN_WEAPON1     currentSystem->enemy->ship->weapons[0]
#define EN_WEAPON2     currentSystem->enemy->ship->weapons[1]
#define EN_WEAPON3     currentSystem->enemy->ship->weapons[2]
#define EN_WEAPON4     currentSystem->enemy->ship->weapons[3]
#define EN_WEAPON5     currentSystem->enemy->ship->weapons[4]
#define EN_IN_RANGE    currentSystem->enemy->ship->nGunsInRange
#define EN_SHIELD      currentSystem->enemy->ship->shield
#define EN_HULL        currentSystem->enemy->ship->hull

int commodityAvg[LASTBUY_I_SIZE] = { 12, 35, 85, 425, 860 };

#define CAP_AVG         commodityAvg
#define CAP_AVGPLASMA   commodityAvg[0]
#define CAP_AVGDUTERIUM commodityAvg[1]
#define CAP_AVGLATINUM  commodityAvg[2]
#define CAP_AVGWATER    commodityAvg[3]
#define CAP_AVGURANIUM  commodityAvg[4]


typedef struct Player {
  int credits;
  int date;
  int galaxyX;
  int galaxyY;
  int pSystem;
  int security;
  int systemX;
  int systemY;
  int tile;
  int achievements[ACHIEVE_I_SIZE];
  int commodityLastBuy[LASTBUY_I_SIZE];
  int skills[SKILLS_I_SIZE];
  Ship *ship;


  static Player *newPlayer() {
    Debug.log(DEBUG_DEFAULT, "static Player* Player::newPlayer()");
    Player *p = (Player*)malloc(sizeof(Player));
    p->initialize(1);
    return p;
  }


  static Player *newEnemy(int type) {
    Debug.log(DEBUG_DEFAULT, "static Player* Player::newEnemy(%d)", type);
    Player *e = (Player*)malloc(sizeof(Player));
    memset(e->achievements, 0, ACHIEVE_I_SIZE * sizeof(int));
    memset(e->skills, 0, SKILLS_I_SIZE * sizeof(int));
    memset(e->commodityLastBuy, 0, LASTBUY_I_SIZE * sizeof(int));
    e->tile = 0;
    e->ship = Ship::newShip(type);
    return e;
  }


  void initialize(int shipType) {
    Debug.log(DEBUG_DEFAULT, "void Player::initialize(%d)", shipType);
    memset(achievements, 0, ACHIEVE_I_SIZE * sizeof(int));
    memset(skills, 0, SKILLS_I_SIZE * sizeof(int));
    memset(commodityLastBuy, 0, LASTBUY_I_SIZE * sizeof(int));
    date     = 407595;
    systemX  = 4;
    systemY  = 4;
    galaxyX  = 0;
    galaxyY  = 0;
    security = 0;
    credits  = 500;
    tile     = 0;
    achievements[ACHIEVE_I_SYSTEMS] = 1;
    achievements[ACHIEVE_I_SHIPS]   = 1;
    achievements[ACHIEVE_I_WEAPONS] = 1;
    ship = Ship::newShip(shipType);
  }


  static Player *load() {
    Debug.log(DEBUG_DEFAULT, "static Player* Player::load()");
    Player *p = newPlayer();
    FILE *fp = fopen("saves/player.trek", "rb");
    if (fp == NULL) {
      Debug.log(DEBUG_DEFAULT, "Error opening player file for reading");
      return NULL;
    }
    fread(p, sizeof(Player), 1, fp);
    fclose(fp);
    return p;
  }


  static Player *loadEnemy(FILE *fp) {
    Debug.log(DEBUG_DEFAULT, "static Player* Player::loadEnemy()");
    Player *p = newPlayer();
    if (fp == NULL) {
      Debug.log(DEBUG_DEFAULT, "Error opening player file for reading");
      return NULL;
    }
    fread(p, sizeof(Player), 1, fp);
    return p;
  }


  int saveEnemy(FILE *fp) {
    Debug.log(DEBUG_DEFAULT, "int Player::saveEnemy()");
    if (fp == NULL) {
      Debug.log(DEBUG_DEFAULT, "Error opening enemy file for writing");
      return 0;
    }
    fwrite(this, sizeof(Player), 1, fp);
    return 1;
  }


  int save() {
    Debug.log(DEBUG_DEFAULT, "int Player::save()");
    FILE *fp = fopen("saves/player.trek", "wb");
    if (fp == NULL) {
      Debug.log(DEBUG_DEFAULT, "Error opening player file for writing");
      return 0;
    }
    fwrite(this, sizeof(Player), 1, fp);
    fclose(fp);
    return 1;
  }


  static void unload(Player *p) {
    Debug.log(DEBUG_DEFAULT, "static void Player::unload(%d)", p);
    if (p == NULL) {
      return;
    }
    free(p);
  }


  int pos() {
    Debug.log(DEBUG_DEFAULT, "int Player::pos()");
    return systemX + 10 * systemY;
  }


  int setPos(int p) {
    Debug.log(DEBUG_DEFAULT, "int Player::setPos(%d)", p);
    if (p < 0 || p > 99) {
      return 0;
    }
    systemX = p % 10;
    systemY = p / 10;
    return 1;
  }


  int setGalaxyPos(int x, int y) {
    Debug.log(DEBUG_DEFAULT, "int Player::setGalaxyPos(%d, %d)", x, y);
    galaxyX = x;
    galaxyY = y;
    return 1;
  }


  int setGalaxyPos(int p) {
    Debug.log(DEBUG_DEFAULT, "int Player::setGalaxyPos(%d)", p);
    if (p < 0 || p > 99) {
      return 0;
    }
    galaxyX = p % 10;
    galaxyY = p / 10;
    return 1;
  }

} Player;

/*
CAP_CARGO = 100; // cargo
capacityArray[2] = 1000;
capacityArray[3] = 5000;
capacityArray[4] = 10000;
capacityArray[5] = 100000;

HS_SHIELD = (debug == 1) ? 0 : 10000; //shields
HS_HULL = (debug == 1) ? 100 : 4000; //hull
// 3, 4, 5, 6 location
// 7 = security
HS_ENERGY = (debug == 1) ? 400 : 60000; //energy
HS_TORPEDOS = (debug == 1) ? 0 : 100; //torps
HS_CREDITS = (debug == 1) ? 500 : 1000000000; //monies
HS_PLASMA = 0;//warp plasma
HS_DUTERIUM = 0;//duterium
HS_LATINUM = 0;//latinum
HS_WATER = 0;//water
HS_URANIUM = 0;//uranium ore
// 16, 17, 18 = planets, enemies, stellar objects
HS_SHIP_TYPE = (debug == 1) ? 1 : 5; //ship class 1-5
// 20 = enemy location
HS_WEAPON1 = (debug == 1) ? 1 : 3; //21-25 = weapons (1-3)
HS_WEAPON2 = (debug == 1) ? 0 : 3;
HS_WEAPON3 = (debug == 1) ? 0 : 3;
HS_WEAPON4 = (debug == 1) ? 0 : 3;
HS_WEAPON5 = (debug == 1) ? 0 : 3;
HS_NWEAPONS = (debug == 1) ? 1 : 5; //total weapons
HS_UP_DMG = 0;//damage upgrades
HS_UP_RANGE = 0;//range upgrade
HS_UP_HULL = 0;//hull upgrade
HS_UP_SHIELD = 0;//shield upgrade
HS_UP_CARGO = 0;//cargo upgrade
HS_UP_TORP = 0;//torpedo upgrade
HS_UP_ENGINE = 0;//engine upgrade
// HS_DEPLETION = energy depletion
// HS_IN_RANGE = guns in range
HS_PASSENGERS = 0;//passengers
// shuttle, cargo, freight, frigate, destroyer
// shields1 = 0, 300, 500, 1000, 10000            1
// hull2 = 100, 500, 700, 2000, 4000              2
// energy8 = 0, 1000, 2000, 4000, 30000           3
// torps9 = 0, 5, 10, 40, 100                     4
// cargo11-15 = 100, 1000, 3000, 100000, 50000    5
// ship range = 6-10                          6
// cannon hardpoints = 16-20                  7

#define COMMODITY_SHIELD    1
#define COMMODITY_HULL      2
#define COMMODITY_ENERGY    3
#define COMMODITY_TORPEDOS  4
#define COMMODITY_SHIP_TYPE 5
#define COMMODITY_PLASMA    6
#define COMMODITY_DUTERIUM  7
#define COMMODITY_LATINUM   8
#define COMMODITY_WATER     9
#define COMMODITY_URANIUM   10
#define COMMODITY_WEAPONS   11
CAP_CARGO = 100; // cargo
capacityArray[2] = 1000;
capacityArray[3] = 5000;
capacityArray[4] = 10000;
capacityArray[5] = 100000;
*/
