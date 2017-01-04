//#define HS_ENEMIES     currentSystem->enemy->ship->shipType
#define HS_STELLAR     currentSystem->stellarObjects
#define HS_PLANETS     currentSystem->nPlanets

#define TILE_EMPTY     0
#define TILE_ENEMY1    1
#define TILE_ENEMY2    2
#define TILE_ENEMY3    3
#define TILE_ENEMY4    4
#define TILE_ENEMY5    5
#define TILE_DERELICT1 6
#define TILE_DERELICT2 7
#define TILE_DERELICT3 8
#define TILE_DERELICT4 9
#define TILE_DERELICT5 10
#define TILE_PLANET    'O'
#define TILE_PLAYER    'P'

#define SYSTEM_SIZE 100
#define PLANET_MAX 3

typedef struct System {
  Planet *planets[PLANET_MAX];
  Player *enemy;
  int nPlanets;
  int map[SYSTEM_SIZE];
  int initialized;
  int galaxyX;
  int galaxyY;
  int stellarObjects;

  static System *newSystem(int x, int y) {
    Debug.log(DEBUG_DEFAULT, "static System* System::System(%d, %d)", x, y);
    System *s = (System*)malloc(sizeof(System));
    s->galaxyX = x;
    s->galaxyY = y;
    s->nPlanets = 0;
    memset(s->map, 0, SYSTEM_SIZE * sizeof(int));
    // init enemy
    s->enemy = Player::newEnemy(0);
    s->initialized = 0;

    return s;
  }

  void startingSystem() {
    Debug.log(DEBUG_DEFAULT, "void System::startingSystem()");
    stellarObjects = randomNumber(10);

    map[45] = TILE_PLAYER;
    map[35] = TILE_PLANET;
    currentPlayer->setPos(45);
    currentPlayer->setGalaxyPos(0, 0);

    nPlanets = 1;
    planets[0] = Planet::newPlanet(35);
    stellarObjects = randomNumber(10);

    initialized = 1;
  }

  void initialize() {
    Debug.log(DEBUG_DEFAULT, "void System::initialize()");
    int playerPos = -1;

    stellarObjects = randomNumber(10);

    if (currentPlayer != NULL) {
      playerPos = currentPlayer->pos();
      if (playerPos != -1) {
        map[playerPos] = TILE_PLAYER;
      }
    }
    if (initialized) {
      return;
    }

    int i, derelictLocation;

    int chance = percentile();
    if (chance > 65) {
      nPlanets = Math.min((chance - 30) / 20, PLANET_MAX);
    }
    for (i = 0; i < nPlanets; i++) {
      int placePlanet = randomNumber(100);
      while (map[placePlanet] != 0) {
        placePlanet = randomNumber(100);
      }
      map[placePlanet] = TILE_PLANET;
      planets[i] = Planet::newPlanet(placePlanet);
    }
    chance = percentile();
    int xyEdge = 0;
    //= (HS_GAL_X == 1 || HS_GAL_X == 10 || HS_GAL_Y == 1 || HS_GAL_Y == 10)? 1 : 0;
    if ((xyEdge && chance > 30) || chance > 75) {
      spawnEnemy(xyEdge);
    }

    chance = percentile();
    if (chance > 90 && EN_SHIP_TYPE == 0) {
      derelictLocation = randomNumber(99);
      while (map[derelictLocation] != 0) {
        derelictLocation = randomNumber(100);
      }
      currentSystem->enemy->setPos(derelictLocation);
      map[derelictLocation] = 6 + randomNumber(4);
      EN_SHIP_TYPE = map[derelictLocation];
      EN_TILE = 0;
    }

    HS_DATE += 1 + randomNumber(9);
    initialized = 1;
  }


  Planet *getPlanet(int pos) {
    Debug.log(DEBUG_DEFAULT, "Planet System::getPlanet(%d)", pos);
    int i;
    Planet *p = NULL;
    for (i = 0; i < nPlanets; ++i) {
      if (planets[i]->position == pos) {
        p = planets[i];
      }
    }
    if (p == NULL) {
      Debug.log(DEBUG_DEFAULT, "Error: did not find planet at %d", pos);
    }

    return p;
  }

  int save() {
    Debug.log(DEBUG_DEFAULT, "int System::save()");
    FILE *fp;
    char path[128] = {0};

    FILE *fplayer = fopen("saves/player.trek", "wb");
    if (fplayer == NULL) {
      Debug.log(DEBUG_DEFAULT, "System::save() Error writing to player file");
      return -1;
    }
    fwrite(currentPlayer, sizeof(Player), 1, fplayer);
    fclose(fplayer);

    snprintf(path, 128, "saves/sectors/%d_%d.trek", galaxyX, galaxyY);

    fp = fopen(path, "wb");
    if (fp == NULL) {
      Debug.log(DEBUG_DEFAULT, "System::save() Error writing to file %s", path);
      return -1;
    }
    fwrite(this, sizeof(System), 1, fp);
    fwrite(map, SYSTEM_SIZE * sizeof(int), 1, fp);
    enemy->saveEnemy(fp);
    fwrite(&nPlanets, sizeof(int), 1, fp);
    int i;
    for (i = 0; i < nPlanets; i++) {
      planets[i]->save(fp);
    }
    fclose(fp);

    return 1;
  }

  static System *load(int x, int y) {
    Debug.log(DEBUG_DEFAULT, "System* System::load(%d, %d)", x, y);
    FILE *fp;
    System *s;
    char path[128] = {0};

    snprintf(path, 128, "saves/sectors/%d_%d.trek", x, y);
    fp = fopen(path, "rb");
    if (fp == NULL) {
      Debug.log(DEBUG_DEFAULT, "System::load() could not find %s", path);
      return NULL;
    } else {
      s = System::newSystem(x, y);
      fread(s, sizeof(System), 1, fp);
      fread(s->map, SYSTEM_SIZE * sizeof(int), 1, fp);
      s->enemy = Player::loadEnemy(fp);
      fread(&(s->nPlanets), sizeof(int), 1, fp);
      int i;
      for (i = 0; i < s->nPlanets; i++) {
        s->planets[i] = Planet::load(fp);
      }
      fclose(fp);
      return s;
    }
  }

  static void unload(System *s) {
    Debug.log(DEBUG_DEFAULT, "static void System::unload(%d)", s);
    if (s == NULL) {
      return;
    }
    int i;
    for (i = 0; i < s->nPlanets; i++) {
      Planet::unload(s->planets[i]);
    }
    Player::unload(s->enemy);

    free(s);
    return;
  }


  int initEnemy(int died) {
    Debug.log(DEBUG_DEFAULT, "int initEnemy(%d)", died);
    int location = EN_POS;
    int ship = EN_SHIP_TYPE + 5 * died;
    int tile = EN_TILE;

    if (currentSystem->enemy != NULL) {
      Player::unload(currentSystem->enemy);
    }
    currentSystem->enemy = Player::newEnemy(ship);
    currentSystem->spawnEnemy(0);
    EN_TILE = tile;
    if (died) {
      currentSystem->map[location] = ship;
      enemy->setPos(location);
    }
    return 0;
  }


  static Player * destroyEnemy(Player *enemy) {
    Debug.log(DEBUG_DEFAULT, "static Player * destroyEnemy(%d), enemy");
    int enTile = enemy->tile;
    int enPos  = enemy->pos();
    int enShip = enemy->ship->shipType;
    Player::unload(currentSystem->enemy);
    Player *e = Player::newEnemy(enShip + 5);
    e->setPos(enPos);
    e->tile = enTile;
    currentSystem->map[enPos] = e->ship->shipType;

    ACHIEVE_KILLS++;
    return e;
  }


  int spawnEnemy(int edgeOfSpace) {
    Debug.log(DEBUG_DEFAULT, "int spawnEnemy(%d)", edgeOfSpace);
    int i, enemyLocation, enemyType;
    enemyLocation = randomNumber(99);
    while (currentSystem->map[enemyLocation] != 0) {
      enemyLocation = randomNumber(99);
    }
    enemyType = edgeOfSpace ? 2 + randomNumber(4) : randomNumber(5);
    currentSystem->map[enemyLocation] = enemyType;
    currentSystem->enemy->ship->setType(enemyType);
    currentSystem->enemy->setPos(enemyLocation);
    EN_TILE = 0;
    for (i = 1; i <= EN_SHIP_TYPE; i++) {
      EN_WEAPON1 = (i < 3) ? i : 1 + randomNumber(2);
      EN_WEAPON2 = (percentile() > 40 && i > 1) ?
      (i > 3) ? randomNumber(3) : randomNumber(2) : 0;
      EN_WEAPON3 = (percentile() > 60 && i > 2) ?
      (i > 3) ? randomNumber(3) : randomNumber(2) : 0;
      EN_WEAPON4 = (percentile() > 70 && i > 3) ? randomNumber(3) : 0;
      EN_WEAPON5 = (percentile() > 70 && i > 4) ? randomNumber(3) : 0;
    }
    return 1;
  }

} System;

