#define CAP_I_CARGO       4
#define CAP_I_ENERGY      2
#define CAP_I_HARDPOINT   5
#define CAP_I_HULL        1
#define CAP_I_SHIELD      0
#define CAP_I_TORP        3

#define CAP_CARGO       currentPlayer->ship->capacity[CAP_I_CARGO]
#define CAP_ENERGY      currentPlayer->ship->capacity[CAP_I_ENERGY]
#define CAP_HARDPOINT   currentPlayer->ship->capacity[CAP_I_HARDPOINT]
#define CAP_HULL        currentPlayer->ship->capacity[CAP_I_HULL]
#define CAP_SHIELD      currentPlayer->ship->capacity[CAP_I_SHIELD]
#define CAP_TORP        currentPlayer->ship->capacity[CAP_I_TORP]

#define SHIP_ESCAPE_POD 0
#define SHIP_DESTROYER  5
#define SHIP_DERELICT_DESTROYER  10

#define CAP_I_SIZE         6
#define UPGRADES_I_SIZE    10
#define COMMODITIES_I_SIZE 10
#define WEAPONS_I_SIZE     5
#define LASTBUY_I_SIZE     5

typedef struct Ship {
  int depletion;
  int energy;
  int hull;
  int nGunsInRange;
  int passengers;
  int shield;
  int shipType;
  int torpedos;
  int commodities[COMMODITIES_I_SIZE];
  int upgrades[UPGRADES_I_SIZE];
  int weapons[WEAPONS_I_SIZE];
  int capacity[CAP_I_SIZE];

  static Ship *newShip(int type) {
    Debug.log(DEBUG_DEFAULT, "static int Ship::newShip(%d)", type);
    if (type < SHIP_ESCAPE_POD || type > SHIP_DERELICT_DESTROYER) {
      Debug.log(DEBUG_DEFAULT, "Error: invalid ship type %d", type);
      return NULL;
    }
    Ship *s = (Ship*)malloc(sizeof(Ship));
    s->setType(type);
    s->depletion    = 0;
    s->nGunsInRange = 0;
    s->passengers   = 0;
    s->torpedos     = 0;
    memset(s->commodities, 0, COMMODITIES_I_SIZE * sizeof(int));
    memset(s->weapons, 0, WEAPONS_I_SIZE * sizeof(int));
    return s;
  }


  int setType(int type) {
    Debug.log(DEBUG_DEFAULT, "int Ship::setType(%d)", type);
    if (type < SHIP_ESCAPE_POD || type > SHIP_DERELICT_DESTROYER) {
      Debug.log(DEBUG_DEFAULT, "Error invalid ship type %d", type);
      return -1;
    }
    memset(upgrades, 0, UPGRADES_I_SIZE * sizeof(int));
    memset(capacity, 0, CAP_I_SIZE * sizeof(int));

    int i;
    for (i = 0; i < CAP_I_SIZE; i++) {
      capacity[i] = Ship::shipCapacity(type, i);
    }
    if (type > SHIP_DESTROYER) {
      hull   = 0;
      shield = 0;
      energy = 0;
    } else {
      hull   = capacity[CAP_I_HULL];
      shield = capacity[CAP_I_SHIELD];
      energy = capacity[CAP_I_ENERGY];
    }
    shipType = type;
    return 1;
  }


  int destroy() {
    Debug.log(DEBUG_DEFAULT, "int Ship::destroy()");
    memset(commodities, 0, COMMODITIES_I_SIZE * sizeof(int));
    setType(0);
    return 1;
  }


  void clearWeapons() {
    Debug.log(DEBUG_DEFAULT, "void Ship::clearWeapons()");
    memset(weapons, 0, 5 * sizeof(int));
  }


  int cargo() {
    Debug.log(DEBUG_DEFAULT, "int Ship::cargo()");
    int i, total = 0;
    for (i = 0; i < COMMODITIES_I_SIZE; i++) {
      total += commodities[i];
    }
    return total;
  }

  int cargoLeft() {
    Debug.log(DEBUG_DEFAULT, "int Ship::cargoLeft()");
    return shipCapacity(shipType, CAP_I_CARGO) - cargo();
  }


  int weaponCount() {
    Debug.log(DEBUG_DEFAULT, "int Ship::weaponCount()");
    int i, count = 0;
    for (i = 0; i < WEAPONS_I_SIZE; i++) {
      if (weapons[i]) {
        count++;
      }
    }
    return count;
  }


  static const char* shipClass(int shipType) {
    Debug.log(DEBUG_DEFAULT, "static const char* Ship::shipClass(%d)", shipType);
    switch (shipType) {
      case 1:
      return "P";
      case 2:
      return "F";
      case 3:
      return "F";
      case 4:
      return "C";
      case 5:
      return "C";
      default:
      return "E";
    }
  }


  static const char* shipName(int shipType) {
    Debug.log(DEBUG_DEFAULT, "static const char* Ship::shipName(%d)", shipType);
    switch (shipType) {
      case 1:
        return "Shuttle (P)";
      case 2:
        return "Cargo Drone (F)";
      case 3:
        return "Freighter (F)";
      case 4:
        return "Frigate (C)";
      case 5:
        return "Destroyer (C)";
      default:
        return "Escape Pod (H)";
    }
  }


  static int shipCapacity(int shipType, int index) {
    Debug.log(DEBUG_DEFAULT, "static int Ship::shipCapacity(%d, %d)", shipType, index);

    // shield, hull, energy, torps, cargo, hardpoints
    int capacities[10][CAP_I_SIZE] = {
      { 0,     0,    0,      0,   0,      0 },
      { 0,     100,  1000,   0,   100,    1 },
      { 300,   500,  3000,   5,   1000,   2 },
      { 500,   700,  10000,  10,  3000,   3 },
      { 1000,  2000, 20000,  40,  100000, 4 },
      { 10000, 4000, 50000,  100, 500000, 5 },
      { 0,     0,    0,      0,   0,      0 },
      { 0,     0,    0,      0,   0,      0 },
      { 0,     0,    0,      0,   0,      0 },
      { 0,     0,    0,      0,   0,      0 }
    };

    if (shipType < SHIP_ESCAPE_POD || shipType > SHIP_DERELICT_DESTROYER) {
      Debug.log(DEBUG_DEFAULT, "Error invalid ship type %d", shipType);
      return 0;
    }
    if (index < 0 || index > CAP_I_SIZE) {
      Debug.log(DEBUG_DEFAULT, "Error invalid capacity index %d", index);
      return 0;
    }
    return capacities[shipType][index];
  }

} Ship;
