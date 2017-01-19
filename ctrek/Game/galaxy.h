#ifndef PLANET_MAX
  #define PLANET_MAX 3
#endif

/*
 *
 */
typedef struct GalaxyNodePlanet {
  int shipyard;
  int outfitter;
  int commerce;
} GalaxyNodePlanet;


/*
 *
 */
typedef struct GalaxyNode {
  GalaxyNodePlanet *planets[PLANET_MAX];
  int x;
  int y;
  // enemies?
  // faction?
} GalaxyNode;


/*
 *
 */
typedef struct Galaxy {
  GalaxyNode **nodes;
  int nNodes;


  /*
   *
   */
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


  /*
   * Write data for current System to file
   */
  static int writeSystem() {
    Debug.log(DEBUG_DEFAULT, "static int Galaxy::writeSystem()");
    // Galaxy *g = (Galaxy*)malloc(sizeof(Galaxy));

    return 0;
  }


  /*
   *
   */
  static int readGalaxy() {
    Debug.log(DEBUG_DEFAULT, "static int Galaxy::readGalaxy()");
    
    return 0;
  }

} Galaxy;

