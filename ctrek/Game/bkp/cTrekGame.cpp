#include "cTrekGame.h"

//#define RANDMAX 65535

/** TODO
 * - on enter planet, switch hud option
 * - looting derelict spawns enemy
 */

void test() {
}


void debuggingMenu(char *s, int value) {
  if (strstr(s, "hull") != NULL) {
    HS_HULL = value;
  } else if (strstr(s, "shield") != NULL) {
    HS_SHIELD = value;
  } else if (strstr(s, "energy") != NULL) {
    HS_ENERGY = value;
  } else if (strstr(s, "weapon1") != NULL) {
    HS_WEAPON1 = value;
  } else if (strstr(s, "weapon2") != NULL) {
    HS_WEAPON2 = value;
  } else if (strstr(s, "weapon3") != NULL) {
    HS_WEAPON3 = value;
  } else if (strstr(s, "weapon4") != NULL) {
    HS_WEAPON4 = value;
  } else if (strstr(s, "weapon5") != NULL) {
    HS_WEAPON5 = value;
  } else if (strstr(s, "credits") != NULL) {
    HS_CREDITS = value;
  } else if (strstr(s, "ship") != NULL) {
  currentPlayer->ship->setType(value);
  }
}


int preGameMenu() {
  char choice = -1;
  int debug = 1;
  while (choice < '0' || choice > '2') {
    printf("Choose an option:\n");
    printf("1: New Game\n");
    printf("2: Load Game\n");
    printf("3: Toggle Debug (");
    if (debug == 1) {
      printf("OFF)\n");
    } else {
      printf("ON) \n");
    }
    printf("0: Quit\n");
    choice = getch_arrow();
    Debug.log(DEBUG_DEFAULT, "preGameMenu: %c", choice);
    if (choice == '3') {
      debug *= -1;
    }
    clrs();
  }
  if (choice != '0') {
    if (choice == '1') {
      system("rm -rf saves/*");
      system("mkdir -p saves/sectors");
      Debug.log(DEBUG_DEFAULT, "deleted saves");
      currentPlayer = Player::newPlayer();
      currentSystem = System::newSystem(0, 0);
      currentSystem->startingSystem();
      currentSystem->initialize();
      LASTBUY_PLASMA   = CAP_AVGPLASMA;
      LASTBUY_DUTERIUM = CAP_AVGDUTERIUM;
      LASTBUY_LATINUM  = CAP_AVGLATINUM;
      LASTBUY_WATER    = CAP_AVGWATER;
      LASTBUY_URANIUM  = CAP_AVGURANIUM;

      if (debug == -1) {
        currentPlayer->ship->setType(5);
        HS_CREDITS = 10000000;
        HS_WEAPONS[0] = 3;
        HS_WEAPONS[1] = 3;
        HS_WEAPONS[2] = 3;
        EN_SHIP->setType(4);
        currentSystem->enemy->setPos(46);
        currentSystem->map[46] = 4;
        EN_WEAPON1 = 3;
        EN_WEAPON2 = 3;
      } else {
        currentPlayer->ship->setType(1);
        HS_WEAPONS[0] = 1;
      }

    } else if (choice == '2') {
      Debug.log(DEBUG_DEFAULT, "loading system");
      // load player then load system
      currentPlayer = Player::load();
      if (currentPlayer == NULL) {
        Debug.log(DEBUG_DEFAULT, "Error loading player file");
        return 0;
      }
      currentSystem = System::load(HS_SYS_X, HS_SYS_Y);
      if (currentSystem == NULL) {
        Debug.log(DEBUG_DEFAULT, "Error loading system file %d_%d.trek", HS_SYS_X, HS_SYS_Y);
        return 0;
      }
    }
    return debug;
  } else {
    return 0;
  }
}


int main() {
  Debug.init("debug.log", DEBUG_LOG_MEDIUM);
  Debug.write(DEBUG_DEFAULT, "\n\n=====================NEW LOG=========================\n\n\n");

  while (1) {
    clrsCB(15, 0);
    int debug = preGameMenu();
    if (debug == 0) {
      return 0;
    }
    choiceMenu(debug);
  }
  return 0;
}


void hitAnim(int playerHit) {
  if (playerHit == 1) {

  } else {

  }
}


void help() {
  Debug.log(DEBUG_DEFAULT, "void help()");
  clrs();
  gotoXY(1, 1);
  printf(
  "Controls:\n"
  "----------\n"
  "Use wasd or arrow keys to move\n"
  "%c - Toggle between long and short range scans (maps).\n"
  "%c - Try to fire torpedos.\n"
  "%c - Ship Status\n"
  "%c - Skills\n"
  "%c - Range Check\n"
  "%c - Scan Enemy\n"
  "%c - Attack\n"
  "%c - Divert Power\n"
  "\n"
  "Debugging - \n"
  "----------\n"
  "%c - Enemy Move\n"
  "%c - SkillsArray[x] = y\n"
  "%c - Take Damage\n"
  "%c - hudstat[x] = y\n"
  "%c - Toggle debug\n"
  "\n"
  "Press any key to go back",

  KEY_SCANS,
  KEY_TORPEDO,
  KEY_STATS,
  KEY_SKILLS,
  KEY_RANGE,
  KEY_SCAN,
  KEY_ATTACK,
  KEY_DIVERT,
  // wasd
  KEY_ENEMY_MOVE,
  KEY_SKILLS_ARRAY,
  KEY_TAKE_DMG,
  KEY_HUDSTAT,
  KEY_DEBUG);
}


void choiceMenu(int debug) {
  Debug.log(DEBUG_DEFAULT, "void choiceMenu(%d)", debug);
  int scans = 1 , hudstat, value;
  char choice, exit;
  char debugHudstat[128] = {0};
  printMap(SCAN_TARGET_NONE);
  minimalHUD(HUD_MINIMAL_START);
  comLog(COM_BELOW_MAP);

  do {
    minimalHUD(HUD_MINIMAL_START);
    gotoXY(0,0);
    choice = getch_arrow();
    Debug.log(DEBUG_DEFAULT, "\n\nChoice: %c", choice);
    if (debug) {
      switch (choice) {
      case KEY_ENEMY_MOVE:
        if (HS_SECURITY != 1) {
          enemyMovement();
          printMap(SCAN_TARGET_NONE);
        }
        continue;
        break;
      case KEY_SKILLS_ARRAY:
        comLog(COM_BELOW_MAP);
        printf("skillsArray: ");
        scanf("%i", &hudstat);
        clean();
        printf("|value: ");
        scanf("%i", &value);
        clean();
        if (hudstat > 0 && hudstat <= 50) {
          (HS_SKILLS[hudstat] = value);
        } else {
          comLog(COM_BELOW_MAP);
          printf("Invalid skill array '%d'.", hudstat);
        }
        continue;
        break;
      case KEY_TAKE_DMG:
        comLog(COM_BELOW_MAP);
        fire(2);
        printMap(SCAN_TARGET_NONE);
        continue;
        break;
      case KEY_HUDSTAT:
        comLog(COM_BELOW_MAP);
        printf("hudstat: ");
        scanf("%s", debugHudstat);
        clean();
        printf("|value: ");
        scanf("%i", &value);
        clean();
        debuggingMenu(debugHudstat, value);
        memset(debugHudstat, 0, 128 * sizeof(char));
        printMap(SCAN_TARGET_NONE);
        continue;
        break;
      case KEY_TEST:
        test();
        continue;
        break;
      }
    }
    switch (choice) {
    case KEY_UP:
    case KEY_WARP_UP:
    case KEY_RIGHT:
    case KEY_WARP_RIGHT:
    case KEY_DOWN:
    case KEY_WARP_DOWN:
    case KEY_LEFT:
    case KEY_WARP_LEFT:
      scans = 1;
      move(choice);
      printMap(SCAN_TARGET_NONE);
      minimalHUD(HUD_MINIMAL_START);
      break;
    case KEY_STATS:
      clrs();
      HUD(1, 1);
      getch_arrow();
      clrs();
      printMap(SCAN_TARGET_NONE);
      break;
    case KEY_SCANS:
      if (scans == 1) {
        printMap(SCAN_TARGET_NONE);
      } else {
        printMap(SCAN_TARGET_NONE);
      }
      scans *= -1;
      break;
    case KEY_TORPEDO:
      if (HS_SECURITY != 1) {
        firetorpedo();
      } else {
        comLog(COM_BELOW_MAP);
        printf("No enemies nearby; firing torpedos unadvisable.");
      }
      printMap(SCAN_TARGET_NONE);
      break;
    case KEY_SKILLS:
      HS_SKILLS[0] = (HS_SKILLS[0] != 5) ? HS_SKILLS[0] + 1 : 0;
      break;
    case KEY_RANGE:
      scan(SCAN_TARGET_ENEMY);
      break;
    case KEY_SCAN:
      scan(SCAN_TARGET_PLAYER);
      break;
    case KEY_ATTACK:
      scan(SCAN_TARGET_PLAYER);
      fire(1);
      printMap(SCAN_TARGET_NONE);
      break;
    case KEY_DIVERT:
      comLog(COM_BELOW_MAP);
      printf("Ammount to divert to shields:");
      scanf("%i", &value);
      clean();
      gotoXY(1, 32);
      printf("                        ");
      if (HS_ENERGY == 0) {
        comLog(COM_BELOW_MAP);
        printf("Power not available.");
      } else {
        divertPower(value);
      }
      break;
    case KEY_DEBUG:
      debug *= -1;
      break;
    case KEY_HELP:
      clrs();
      help();
      getch_arrow();
      clrs();
      printMap(SCAN_TARGET_NONE);
      comLog(COM_BELOW_MAP);
      minimalHUD(HUD_MINIMAL_START);
      break;
    default:
      if (choice != '0') {
        comLog(COM_BELOW_MAP); //52
        printf("Input invalid, please enter again.");
      }
      break;
    }
    minimalHUD(HUD_MINIMAL_START);
  } while (choice != '0');

  comLog(COM_BELOW_MAP); //52
  printf("Are you sure you want to end the program?");
  exit = getch_arrow();
  exit = toupper(exit);
  if (exit != 'Y') {
    currentPlayer->save();
    choiceMenu(debug);
  }
  return;
}


void move(char toWhere) { // move to com log
  Debug.log(DEBUG_DEFAULT, "void move(%c)", toWhere);
  int systemPos, systemEdge;
  int nextSystemIndex, systemWrap;
  int nextGalaxyX = HS_GAL_X, nextGalaxyY = HS_GAL_Y;
  comLog(COM_BELOW_MAP);

  //findPlayer();
  //securityStatus();

  systemPos = (HS_SYS_Y - 1) * 10 + (HS_SYS_X - 1);

  switch(toWhere) {
  case KEY_UP:
  case KEY_WARP_UP:
    systemEdge = (HS_SYS_Y == 1);
    nextSystemIndex = -10;
    nextGalaxyY += 1;
    break;
  case KEY_RIGHT:
  case KEY_WARP_RIGHT:
    systemEdge = (HS_SYS_X == 10);
    nextSystemIndex = 1;
    nextGalaxyX += 1;
    break;
  case KEY_DOWN:
  case KEY_WARP_DOWN:
    systemEdge = (HS_SYS_Y == 10);
    nextSystemIndex = 10;
    nextGalaxyY -= 1;
    break;
  case KEY_LEFT:
  case KEY_WARP_LEFT:
  default:
    systemEdge = (HS_SYS_X == 1);
    nextSystemIndex = -1;
    nextGalaxyX -= 1;
    break;
  }
  systemWrap = -9 * nextSystemIndex;

  if (enemyAtTile(systemPos + nextSystemIndex)) {
    return;
  }

  if (!systemEdge) {
    if (getMapTile(systemPos + nextSystemIndex) == TILE_PLANET) {
      if (HS_SECURITY != SECURITY_SAFE) {
        comLog(COM_BELOW_MAP);
        printf("Docking request denied when enemy vessles nearby.");
      } else {
        currentSystem->getPlanet(systemPos + nextSystemIndex)->dock();
        comLog(COM_BELOW_MAP);
      }
    } else if (derelictAtTile(systemPos + nextSystemIndex)) {
      loot();
    } else if (getMapTile(systemPos + nextSystemIndex) == TILE_EMPTY) {
      currentSystem->map[systemPos] = TILE_EMPTY;
      currentSystem->map[systemPos + nextSystemIndex] = TILE_PLAYER;
      ACHIEVE_DISTANCE++;
      //findPlayer();
    } else {
      comLog(COM_BELOW_MAP);
      fprintf(stderr, "Unknown map tile %d",
      getMapTile(systemPos + nextSystemIndex));
      exit(1);
    }
  } else {
    currentSystem->map[systemPos] = TILE_EMPTY;
    currentSystem->save();
    System::unload(currentSystem);
    currentSystem = System::load(nextGalaxyX, nextGalaxyY);
    if (currentSystem == NULL) {
      currentSystem = System::newSystem(nextGalaxyX, nextGalaxyY);
    }
    HS_GAL_X = nextGalaxyX;
    HS_GAL_Y = nextGalaxyY;
    currentSystem->map[systemPos + systemWrap] = TILE_PLAYER;
    //findPlayer();
    currentSystem->initialize();
    ACHIEVE_DISTANCE++;
    ACHIEVE_SYSTEMS++;
    return;
  }
  // Update player location
  //findPlayer();
}


int rangeScan(int playerScanning, int mapIndex) {
  Debug.log(DEBUG_DEFAULT, "int rangeScan(%d, %d)", playerScanning, mapIndex);
  int i, xDifference, yDifference, inRange = 0;
  int ranges[3] = {0};
  float distance;

  int *weaponArray;
  if (playerScanning == SCAN_TARGET_PLAYER) {
    weaponArray = currentPlayer->ship->weapons;
  } else {
    weaponArray = currentSystem->enemy->ship->weapons;
  }

  for (i = 0; i < 5; i++) {
    if (weaponArray[i] != 0) {
      ranges[weaponArray[i]]++;
    }
  }

  // Player
  if (playerScanning == SCAN_TARGET_PLAYER) {
    xDifference = abs(EN_POS % 10 - mapIndex % 10);
    yDifference = abs(EN_POS / 10 - mapIndex / 10);
    xDifference -= HS_UP_RANGE;
    yDifference -= HS_UP_RANGE;
  } else {
    xDifference = abs(HS_SYS_X - 1 - mapIndex % 10);
    yDifference = abs(HS_SYS_Y - 1 - mapIndex / 10);
  }

  xDifference = pow(xDifference, 2);
  yDifference = pow(yDifference, 2);
  distance = Math.round(sqrt(xDifference + yDifference));
  if (distance <= 3 && ranges[0] != 0) {
    inRange = 1;
  }
  if (distance <= 5 && ranges[1] != 0) {
    inRange = 1;
  }
  if (distance <= 7 && ranges[2] != 0) {
    inRange = 1;
  }

  return inRange;
}


void clean () {
  char dummy;
  do {
    scanf("%c", &dummy);
  } while (dummy != '\n');
}


void generateSystem() {
  Debug.log(DEBUG_DEFAULT, "void generateSystem()");
  // save old system and free memory
  currentSystem->save();
  free(currentSystem);
  currentSystem = System::newSystem(HS_GAL_X, HS_GAL_Y);
}


void enemyMovement() {
  Debug.log(DEBUG_DEFAULT, "void enemyMovement()");
  int xDistance, yDistance, tileUnderEnemy;
  int enX, enY, plX, plY;

  enX = EN_POS % 10;
  enY = EN_POS / 10;
  plX = HS_SYS_X - 1;
  plY = HS_SYS_Y - 1;
  xDistance = abs(enX - plX);
  yDistance = abs(enY - plY);

  if (xDistance >= yDistance) {
    if (enX < plX && getMapTile(EN_POS + 1) != 'P') {
      tileUnderEnemy = EN_TILE;
      EN_TILE = getMapTile(EN_POS + 1);
      currentSystem->map[EN_POS + 1] = EN_SHIP_TYPE;
      currentSystem->map[EN_POS] = tileUnderEnemy;
      currentSystem->enemy->setPos(EN_POS + 1);
    }
    if (enX > plX && getMapTile(EN_POS - 1) != 'P') {
      tileUnderEnemy = EN_TILE;
      EN_TILE = getMapTile(EN_POS - 1);
      currentSystem->map[EN_POS - 1] = EN_SHIP_TYPE;
      currentSystem->map[EN_POS] = tileUnderEnemy;
      currentSystem->enemy->setPos(EN_POS - 1);
    }
  } else {
    if (enY < plY && getMapTile(EN_POS + 10) != 'P') {
      tileUnderEnemy = EN_TILE;
      EN_TILE = getMapTile(EN_POS + 10);
      currentSystem->map[EN_POS + 10] = EN_SHIP_TYPE;
      currentSystem->map[EN_POS] = tileUnderEnemy;
      currentSystem->enemy->setPos(EN_POS + 10);
    }
    if (enY > plY && getMapTile(EN_POS - 10) != 'P') {
      tileUnderEnemy = EN_TILE;
      EN_TILE = getMapTile(EN_POS - 10);
      currentSystem->map[EN_POS - 10] = EN_SHIP_TYPE;
      currentSystem->map[EN_POS] = tileUnderEnemy;
      currentSystem->enemy->setPos(EN_POS - 10);
    }
  }
}


void scan(int playerScanning, int ship, int rangeScan) {
  Debug.log(DEBUG_DEFAULT, "void scan(%d, %d, %d)", playerScanning, ship, rangeScan);
  int i;
  int range = rangeCheck(1, 1);
  // F can scan shields/hull, C weaps, range, P no scans
  // 2 = none, 1 = limited, 0 = out of range, 3 = in range
  if (playerScanning == SCAN_PLAYER_RANGE) {
    comLog(COM_BELOW_MAP);
    if (HS_NWEAPONS == 0) {
      printf("You have no weapons.");
    } else if (HS_SECURITY == SECURITY_SAFE) {
      printf("No enemies nearby.");
    } else {
      if (HS_IN_RANGE == 0) {
        printf("Hostile Out of Range.");
      } else {
        printf("Hostile In Range.");
      }
      if (range != 0) {
        printf("Potential Damage Output: %i", range);
      }
    }

    if (HS_DEPLETION == 1 && HS_SECURITY != 1 && HS_NWEAPONS != 0) {
      printf("Limited energy available.");
    } else if (HS_DEPLETION == 2 && HS_SECURITY != 1 && HS_NWEAPONS != 0) {
      printf("Insufficient energy available.");
    }

    if (rangeScan) {
      printMap(playerScanning);
    }
  }

  if (playerScanning == SCAN_ENEMY_RANGE) {
    comLog(COM_BELOW_MAP);
    if (EN_SHIP_TYPE > 0 && EN_SHIP_TYPE < 6) {
      if (ship == 0 || ship == 1) {
        printf("No scanning capabilities in this ship.");
        printMap(SCAN_TARGET_NONE);
        return;
      }

      if (ship == 2 || ship == 3) {
        printf("Shield:%i Hull:%i", EN_SHIELD, EN_HULL);
      }
      if (ship == 4 || ship == 5) {
        printf("Gun Types: ");
        for (i = 0; i <= 5; i++) {
          if (currentSystem->enemy->ship->weapons[i] != 0) {
            printf("%i ", currentSystem->enemy->ship->weapons[i]);
          }
        }
        printf("Shield Power:%i\tHull Strength:%i", EN_SHIELD, EN_HULL);
      }
      printf("Potential Damage Output: %i", rangeCheck(2, 1));
      if (rangeCheck(2, 1) == 0) {
        printf("Out of Range.");
      } else {
        printf("In Range."); // 8, 9
      }
      printMap(playerScanning);
    } else {
      printf("No hostiles to scan.");
    }
  }
}


/**
 *  TODO refactor
 */
int rangeCheck(int scanTarget, int scans) {
  Debug.log(DEBUG_DEFAULT, "int rangeCheck(%d, %d)", scanTarget, scans);
  int i, xDifference, yDifference, damage = 0, energyDepletion = 0,
  track1 = 0, track2 = 0, track3 = 0;
  int ranges[3] = {0};
  int *weaponArray;
  float distance;

  HS_DEPLETION = 0;
  HS_IN_RANGE = 0;

  if (scanTarget == SCAN_PLAYER_RANGE) {
    weaponArray = currentPlayer->ship->weapons;
  } else {
    weaponArray = currentSystem->enemy->ship->weapons;
  }

  for (i = 0; i < 5; i++) {
    if (weaponArray[i] != 0) {
      ranges[weaponArray[i] - 1]++;
    }
  }
  xDifference = pow((double)(EN_POS % 10 - (HS_SYS_X - 1)), 2);
  yDifference = pow((double)(EN_POS / 10 - (HS_SYS_Y - 1)), 2);
  distance = Math.round(sqrt(xDifference + yDifference));

  if (scanTarget == SCAN_PLAYER_RANGE) {
    if (distance <= 3 + HS_UP_RANGE) {
      HS_IN_RANGE += ranges[0];
    }
    if (distance <= 5 + HS_UP_RANGE) {
      HS_IN_RANGE += ranges[1];
    }
    if (distance <= 7 + HS_UP_RANGE) {
      HS_IN_RANGE += ranges[2];
    }

    energyDepletion += 10 * ranges[0];
    energyDepletion += 50 * ranges[1];
    energyDepletion += 100 * ranges[2];
    if (energyDepletion > HS_ENERGY) {
      // Calculate max dmg with remaining energy - TODO refactor
      energyDepletion = HS_ENERGY;
      for (i = 0; i < 5; i++) {
        if (energyDepletion >= 100 * (ranges[2] - i) && (ranges[2] - i) > 0 &&
        track1 == 0) {
          damage += (ranges[2] - i) * 100;
          energyDepletion -= (ranges[2] - i) * 100;
          HS_DEPLETION += energyDepletion;
          track1 = 1;
        }
      }
      for (i = 0; i < 5; i++) {
        if (energyDepletion >= 50 * (ranges[1] - i) && (ranges[1] - i) > 0 &&
        track2 == 0) {
          damage += (ranges[1] - i) * 50;
          energyDepletion -= (ranges[1] - i) * 50;
          HS_DEPLETION += energyDepletion;
          track2 = 1;
        }
      }
      for (i = 0; i < 5; i++) {
        if (energyDepletion >= 10 * (ranges[0] - i) && (ranges[0] - i) > 0 &&
        track3 == 0) {
          damage += (ranges[0] - i) * 30;
          energyDepletion -= (ranges[0] - i) * 10;
          HS_DEPLETION += energyDepletion;
          track3 = 1;
        }
      }
      if (scans == ) {
        HS_ENERGY = energyDepletion;
      }
      HS_DEPLETION = (scans == SCAN_TYPE_RANGE) ?
      (track1 == 0 && track2 == 0 && track3 == 0) ? 2 : 1 : HS_DEPLETION;
    }
  } else {
    if (distance <= 3) {
      damage += 30 * ranges[0];
    } // 0 = fire, 1 = scan, 2 = info
    if (distance <= 5) {
      damage += 50 * ranges[1];
    }
    if (distance <= 7) {
      damage += 100 * ranges[2];
    }
    if (scans == 0) {
      HS_ENERGY -= energyDepletion;
    } // 1 = none 2 = lim
  }
  if (damage != 0) {
    damage += HS_UP_DMG * 10;
  }
  if (scans == SCAN_TYPE_DEPLETION) {
    HS_DEPLETION = 10 * ranges[0] + 50 * ranges[1] + 100 * ranges[2];
    damage = 30 * ranges[0] + 50 * ranges[1] + 100 * ranges[2];
  }
  HS_IN_RANGE = (scans == SCAN_TYPE_DEPLETION) ? (ranges[2] != 0) ? 7 :
  (ranges[1] != 0) ? 5 : (ranges[0] != 0) ? 3 : 0 : HS_IN_RANGE;

  if (scanTarget == SCAN_PLAYER_RANGE) {
    if (distance <= 3) {
      damage += 30 * ranges[0];
    }
    if (distance <= 5) {
      damage += 50 * ranges[1];
    }
    if (distance <= 7) {
      damage += 100 * ranges[2];
    }
  }
  return damage;
}


void fire(int playerFiring) {
  Debug.log(DEBUG_DEFAULT, "void fire(%d)", playerFiring);
  int damage, crit;
  comLog(COM_BELOW_MAP);
  if (HS_NWEAPONS == 0) {
    printf("You habe no weapons.");
    return;
  } else if (HS_SECURITY == 1) {
    printf("There are no hostiles to shoot at.");
    return;
  }
  if (rangeCheck(playerFiring, 1) != 0) {
    if (percentile() < 10 && rangeCheck(playerFiring, 1) != 0) {
      if (playerFiring == 1) {
        printf("You miss. ");
      } else {
        printf("Enemy misses.");
      }
      rangeCheck(playerFiring, 0);
      return;
    }
    damage = rangeCheck(playerFiring, 1);
    damage *= (playerFiring == 2) ? 0.9 : 1;
    damage -= damage / 10 + randomNumber(damage / 5);
    crit = percentile();
    if (crit >= 95 && rangeCheck(playerFiring, 1) != 0) {
      damage *= 2;
      if (playerFiring == 1) {
        printf("You hit a critical system! ");
      } else {
        printf("Enemy hits a critical system! ");
      }
    }
    if (crit < 95 && rangeCheck(playerFiring, 1) != 0) {
      if (playerFiring == 1) {
        printf("You hit. ");
      } else {
        printf("Enemy hits. ");
      }
      if (playerFiring == 1) {
        hitAnim(2);
      } else {
        hitAnim(1);
      }
    }
    if (damage != 0) {
      rangeCheck(playerFiring, 0);
      printfLinesAt(COM_BELOW_MAP_START, "\n%i damage was done.\n", damage);
    }
    damageReport(damage, playerFiring);
  }
}


void firetorpedo() {
  Debug.log(DEBUG_DEFAULT, "void firetorpedo()");
  int damage = 0, xDifference, yDifference;
  float distance = 0;
  comLog(COM_BELOW_MAP);
  if (HS_TORPEDOS == 0) {
    printf("You have no torpedos to fire.");
    return;
  }
  xDifference = pow((double)(EN_POS % 10 - (HS_SYS_X - 1)), 2);
  yDifference = pow((double)(EN_POS / 10 - (HS_SYS_Y - 1)), 2);
  distance = Math.round(sqrt(xDifference + yDifference));
  damage = 50 - 3 * randomNumber((int)distance) +
  2 * randomNumber((int)distance) + 20 * HS_UP_TORP;

  if (percentile() < 2 * (int)distance) {
    printf("Torpedo missed.");
    HS_TORPEDOS--;
    return;
  } else {
    if (percentile() > 95) {
      printf("Torpedo hits critical systems.");
      damage *= 2;
    } else {
      printf("Torpedo hits.");
    }
    printf("Enemy takes %i damage.", damage);
    if (distance < 2) {
      printf(" You take %i splash damage.", damage / 2);
      damageReport(damage / 2, 2);
    }
    damageReport(damage, 1);
  }

  HS_TORPEDOS--;
}


void damageReport(int damage, int player) {
  Debug.log(DEBUG_DEFAULT, "void damageReport(%d, %d)", damage, player);
  // hull:2[29] shields:1[30]
  if (player == 2) {
    // shields down
    if (damage - 10 * HS_UP_SHIELD > HS_SHIELD) {
      // death
      if (damage - HS_SHIELD - 10 * HS_UP_HULL >= HS_HULL) {
        printf("Your ship was destroyed.");
        shipDestroyed();
        return;
      }
      // hull damage
      if (damage - HS_SHIELD - 10 * HS_UP_HULL < HS_HULL) {
        HS_HULL -= damage - HS_SHIELD - 10 * HS_UP_HULL;
        HS_SHIELD = 0;
        return;
      }
    }
    // shields damaged
    if (damage - 10 * HS_UP_SHIELD <= HS_SHIELD) {
      HS_SHIELD -= damage - 10 * HS_UP_SHIELD;
      return;
    }
  }
  // hull:9 shields:8
  if (player == 1) {
    // shields down
    if (damage > EN_SHIELD) {
      // death
      if (damage - EN_SHIELD >= EN_HULL) {
        printf("The enemy ship was destroyed.");
        currentSystem->enemy = System::destroyEnemy(currentSystem->enemy);
        HS_SECURITY = 1;
        return;
      }
      // hull damage
      if (damage - EN_SHIELD < EN_HULL) {
        EN_HULL -= damage - EN_SHIELD;
        EN_SHIELD = 0;
        return;
      }
    }
    // shields damaged
    if (damage <= EN_SHIELD) {
      EN_SHIELD -= damage;
      return;
    }
  }
}


void shipDestroyed() {
  Debug.log(DEBUG_DEFAULT, "void shipDestroyed()");
  //int i;
  ACHIEVE_SHIPS++;
  HS_SHIELD = 0;
  HS_HULL = 0;
  HS_ENERGY = 0;
  HS_TORPEDOS = 0;
  HS_SECURITY = 1;
  /*
  for (i = 11; i < 16; i++) {
    hudstats[i] = 0;
  }
  HS_SHIP_TYPE = 0;
  for (i = 21; i < 33; i++) {
    hudstats[i] = 0;
  }
  */
}


void divertPower(int quantity) {
  Debug.log(DEBUG_DEFAULT, "void divertPower(%d)", quantity);
  int diverted = 0;
  comLog(COM_BELOW_MAP);
  // less energy than asked
  if (HS_ENERGY < quantity) {
    // enough energy to charge shield
    if (CAP_SHIELD - HS_SHIELD <= HS_ENERGY) {
      diverted = CAP_SHIELD - HS_SHIELD;
      HS_ENERGY -= CAP_SHIELD - HS_SHIELD;
      HS_SHIELD += diverted;
      printf("%i power was diverted to shields.", diverted);
      gotoXY(52, 28);
      printf("Shields Full.");
    } else  {
      // take all energy avail
      diverted = HS_ENERGY;
      HS_SHIELD += HS_ENERGY;
      HS_ENERGY = 0;
      printf("%i power was available.", diverted);
      gotoXY(52, 28);
      printf("Shields Increased.");
    }
  } else {
    // energy > asked
    // less energy than shield
    if (CAP_SHIELD - HS_SHIELD > quantity) {
      diverted = quantity;
      HS_ENERGY -= quantity;
      HS_SHIELD += quantity;
      printf("%i power was diverted to shields.", diverted);
      gotoXY(52, 28);
      printf("Shields Increased.");
    } else {
      // more energy than shield
      diverted = CAP_SHIELD - HS_SHIELD;
      HS_ENERGY -= CAP_SHIELD - HS_SHIELD;
      HS_SHIELD = CAP_SHIELD;
      printf("%i power was diverted to shields.", diverted);
      gotoXY(52, 28);
      printf("Shields Full.");
    }
  }
  healthBars();
}


void loot() {
  Debug.log(DEBUG_DEFAULT, "void loot()");
  int lootCount = 0;
  int foundLoot = 0;
  char str[BUFFER_MEDIUM] = {0};
  char *msg = str;
  int msgLen = 0;
  comLog(COM_BELOW_MAP);

  if (percentile() < 10 && HS_SHIP_TYPE != 0) {
    printLinesAt(COM_BELOW_MAP_START,
    "Aboard the vessel you found a pilot who claims they\n"
    "will pay you to bring them to the nearest planet.");
    Debug.log(DEBUG_DEFAULT, "enemy tile: %d, enemy pos: %d", EN_TILE, EN_POS);
    currentSystem->map[EN_POS] = EN_TILE;
    Player::unload(currentSystem->enemy);
    currentSystem->enemy = Player::newEnemy(0);
    HS_PASSENGERS++;
    return;
  } else if (percentile() > 95 && HS_SHIP_TYPE != 0) {
    int damage = randomNumber(HS_SHIELD + HS_HULL + 10 * HS_SHIP_TYPE *
    (EN_SHIP_TYPE - 5));
    if (damage != 0) {
      printfLinesAt(COM_BELOW_MAP_START, "%s\n%i damage was done",
      "Boarding the vessel tripped a security protocol.", damage);
      damageReport(damage, 2);
    } else {
      printLinesAt(COM_BELOW_MAP_START,
      "Boarding the vessel tripped a security protocol.\n"
      "Fortunately the system was damaged and malfunctioned.");
    }
    Debug.log(DEBUG_DEFAULT, "enemy tile: %d, enemy pos: %d", EN_TILE, EN_POS);
    currentSystem->map[EN_POS] = EN_TILE;
    Player::unload(currentSystem->enemy);
    currentSystem->enemy = Player::newEnemy(0);
    return;
  }

  // Credits
  if (percentile() > 50) {
    lootCount = 20 + randomNumber(100);
    if (EN_SHIP_TYPE == 7) {
      lootCount += randomNumber(500);
    }
    if (EN_SHIP_TYPE == 8) {
      lootCount += 500 + randomNumber(1000);
    }
    if (EN_SHIP_TYPE == 9) {
      lootCount += 1000 + randomNumber(5000);
    }
    if (EN_SHIP_TYPE == 10) {
      lootCount += 2000 + randomNumber(10000);
    }
    msgLen += snprintf(msg + msgLen, BUFFER_MEDIUM - msgLen,
    "%i Credits, ", lootCount);
    HS_CREDITS += lootCount;
    foundLoot = 1;
  }

  if (HS_SHIP_TYPE != 0) {
    // Commodities
    if (percentile() > 50) {
      int lootType = randomNumber(4);
      lootCount =
      Math.min((EN_SHIP_TYPE - 5) * randomNumber(5), cargoLeft());

      if (lootCount) {
        msgLen += snprintf(msg + msgLen, BUFFER_MEDIUM - msgLen,
        "%d %s, ", lootCount, hudstatToCommodity(lootType));
        currentPlayer->ship->commodities[lootType] += lootCount;
        foundLoot = 1;
      }
    }

    // Energy
    if (percentile() > 65 && HS_SHIP_TYPE != 1 && EN_SHIP_TYPE != 6) {
      lootCount = Math.min((EN_SHIP_TYPE - 5) * randomNumber(100),
      hudstatToCapacity(COMMODITY_ENERGY) - HS_ENERGY);

      if (lootCount) {
        msgLen += snprintf(msg + msgLen, BUFFER_MEDIUM - msgLen,
        "%d Power cells, ", lootCount);
        HS_ENERGY += lootCount;
        foundLoot = 1;
      }
    }

    // Hull
    if (percentile() > 70) {
      lootCount = Math.min((EN_SHIP_TYPE - 5) * (randomNumber(30)),
      hudstatToCapacity(COMMODITY_HULL) - HS_HULL);

      if (lootCount) {
        msgLen += snprintf(msg + msgLen, BUFFER_MEDIUM - msgLen,
        "%d Tritanium plating, ",lootCount);
        HS_HULL += lootCount;
        foundLoot = 1;
      }
    }

    // Torpedos
    if (percentile() > 85 && HS_SHIP_TYPE != 1 && HS_SHIP_TYPE != 0) {
      lootCount = Math.min(randomNumber(EN_SHIP_TYPE),
      hudstatToCapacity(COMMODITY_TORPEDOS) - HS_TORPEDOS);

      if (lootCount) {
        msgLen += snprintf(msg + msgLen, BUFFER_MEDIUM - msgLen,
        "%d Torpedos, ", lootCount);
        HS_TORPEDOS += lootCount;
        foundLoot = 1;
      }
    }
  }

  if (!foundLoot) {
    printf("No loot found.");
  } else {
    printfLinesAt(COM_BELOW_MAP_START, "%s\n%.*s",
    "You looted the vessel and found:", msgLen - 2, str);
  }

  Debug.log(DEBUG_DEFAULT, "enemy tile: %d, enemy pos: %d", EN_TILE, EN_POS);
  currentSystem->map[EN_POS] = EN_TILE;
  Player::unload(currentSystem->enemy);
  currentSystem->enemy = Player::newEnemy(0);
}



/**            ACHIEVEMENTS
0: distance travelled
1: systems visited
2: planets visited
3: ships commanded
4: weapons obtained
5: kills - leads to combat status
**/

