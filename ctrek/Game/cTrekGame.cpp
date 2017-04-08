#include "cTrekGame.h"

/** TODO
 * - on enter planet, switch hud option
 * - rebalance Commodities
 * - multiple enemies per system
 * - universe map
 * - gamble menu
 * - commerce.h class for buy/sell menus
 */

void test() {
  int cellWidth = 4;
  //int cellWCenter = 2;
  int cellHeight = 2;
  //int cellHCenter = 1;

  drawWindow(3, 3, 10 * cellWidth + 2, 10 * cellHeight + 1);
  // start at player centered, arrow keys move
  
}

void debuggingMenu(char *s, int value) {
  if (strstr(s, "hull") != NULL) {
    HS_HULL = value;
  } else if (strstr(s, "shield") != NULL) {
    HS_SHIELD = value;
  } else if (strstr(s, "energy") != NULL) {
    HS_ENERGY = value;
  } else if (strstr(s, "eweapon1") != NULL) {
    EN_WEAPON1= value;
  } else if (strstr(s, "eweapon2") != NULL) {
    EN_WEAPON2 = value;
  } else if (strstr(s, "eweapon3") != NULL) {
    EN_WEAPON3 = value;
  } else if (strstr(s, "eweapon4") != NULL) {
    EN_WEAPON4 = value;
  } else if (strstr(s, "eweapon5") != NULL) {
    EN_WEAPON5 = value;
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
      LASTBUY_PLASMA   = CAP_AVGPLASMA;
      LASTBUY_DUTERIUM = CAP_AVGDUTERIUM;
      LASTBUY_LATINUM  = CAP_AVGLATINUM;
      LASTBUY_WATER    = CAP_AVGWATER;
      LASTBUY_URANIUM  = CAP_AVGURANIUM;

      if (debug == -1) {
        currentPlayer->ship->setType(5);
        HS_CREDITS = 10000000;
        HS_WEAPON1 = 1;
        //HS_WEAPONS[0] = 3;
        //HS_WEAPONS[1] = 3;
        //HS_WEAPONS[2] = 3;

        EN_SHIP->setType(6 + randomNumber(4));
        currentSystem->enemy->setPos(46);
        currentSystem->map[46] = EN_SHIP_TYPE;
        EN_SHIP->clearWeapons();
        //EN_WEAPON1 = 2;
      } else {
        currentPlayer->ship->setType(1);
        HS_WEAPONS[0] = 1;
      }
      currentPlayer->save();
    } else if (choice == '2') {
      Debug.log(DEBUG_DEFAULT, "loading system");
      // load player then load system
      currentPlayer = Player::load();
      if (currentPlayer == NULL) {
        Debug.log(DEBUG_DEFAULT, "Error loading player file");
        return 0;
      }
      currentSystem = System::load(HS_GAL_X, HS_GAL_Y);
      if (currentSystem == NULL) {
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
  Debug.enabled = 1;
  Debug.write(__FILE__, __LINE__, "\n\n=====================NEW LOG=========================\n\n\n");

  test();

  while (1) {
    clrsCB(15, 0);
    int debug = preGameMenu();
    if (debug == 0) {
      return 0;
    }
    test();
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
  printMap(NONE);
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
          printMap(NONE);
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
        fire(ENEMY);
        printMap(NONE);
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
        printMap(NONE);
        continue;
        break;
      case KEY_TEST:
        test();
        continue;
        break;
      }
    }
    switch (choice) {
    case '0':
      comLog(COM_BELOW_MAP); //52
      printf("Are you sure you want to end the program?");
      exit = getch_arrow();
      exit = toupper(exit);
      if (exit == 'Y') {
        currentSystem->save();
        currentPlayer->save();
        System::unload(currentSystem);
        Player::unload(currentPlayer);
        return;
      }
    break;
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
      printMap(NONE);
      minimalHUD(HUD_MINIMAL_START);
      break;
    case KEY_STATS:
      clrs();
      HUD(1, 1);
      getch_arrow();
      clrs();
      printMap(NONE);
      comLog(COM_BELOW_MAP);
      break;
    case KEY_SCANS:
      if (scans == 1) {
        printMap(NONE);
      } else {
        printMap(NONE);
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
      printMap(NONE);
      break;
    case KEY_SKILLS:
      HS_SKILLS[0] = (HS_SKILLS[0] != 5) ? HS_SKILLS[0] + 1 : 0;
      break;
    case KEY_RANGE:
      scan(ENEMY);
      break;
    case KEY_SCAN:
      scan(PLAYER);
      break;
    case KEY_ATTACK:
      fire(PLAYER);
      printMap(NONE);
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
      printMap(NONE);
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
  } while (1);

  return;
}


void move(char toWhere) { // move to com log
  Debug.log(DEBUG_DEFAULT, "void move(%c)", toWhere);
  int systemPos, systemEdge;
  int nextSystemIndex, systemWrap;
  int nextGalaxyX = HS_GAL_X, nextGalaxyY = HS_GAL_Y;

  systemPos = 10 * HS_SYS_Y + HS_SYS_X ;
  comLog(COM_BELOW_MAP);

  switch(toWhere) {
    case KEY_UP:
    case KEY_WARP_UP:
      systemEdge = (HS_SYS_Y == 0);
      nextSystemIndex = -10;
      nextGalaxyY += 1;
    break;
    case KEY_RIGHT:
    case KEY_WARP_RIGHT:
      systemEdge = (HS_SYS_X == 9);
      nextSystemIndex = 1;
      nextGalaxyX += 1;
    break;
    case KEY_DOWN:
    case KEY_WARP_DOWN:
      systemEdge = (HS_SYS_Y == 9);
      nextSystemIndex = 10;
      nextGalaxyY -= 1;
    break;
    case KEY_LEFT:
    case KEY_WARP_LEFT:
    default:
      systemEdge = (HS_SYS_X == 0);
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
      currentPlayer->setPos(systemPos + nextSystemIndex);
      ACHIEVE_DISTANCE++;
    } else {
      comLog(COM_BELOW_MAP);
      fprintf(stderr, "Unknown map tile %d", getMapTile(systemPos + nextSystemIndex));
      exit(1);
    }
  } else {
    currentPlayer->setPos(systemPos + systemWrap);
    currentPlayer->setGalaxyPos(nextGalaxyX, nextGalaxyY);
    currentSystem->map[systemPos] = TILE_EMPTY;
    currentSystem->save();
    System::unload(currentSystem);
    currentSystem = System::load(nextGalaxyX, nextGalaxyY);
    if (currentSystem == NULL) {
      currentSystem = System::newSystem(nextGalaxyX, nextGalaxyY);
    }
    currentSystem->initialize();
    ACHIEVE_DISTANCE++;
    ACHIEVE_SYSTEMS++;
  }

  currentSystem->save();
  currentPlayer->save();
}


/*
 * Returns true if the target is in range of a map cell
 */
int inRange(int playerScanniing, int mapIndex) {
  Debug.log(DEBUG_DEFAULT, "int inRange(%d, %d)", playerScanniing, mapIndex);
  int *weaponArray;
  int i;
  int x;
  int y;
  int xDifference;
  int yDifference;
  int upgrades = 0;
  int maxRange = 0;
  int distance;

  if (playerScanniing == ENEMY) {
    weaponArray = currentPlayer->ship->weapons;
    x = HS_SYS_X;
    y = HS_SYS_Y;
    upgrades = HS_UP_RANGE;
  } else if (playerScanniing == PLAYER) {
    weaponArray = currentSystem->enemy->ship->weapons;
    x = EN_SYS_X;
    y = EN_SYS_Y;
  } else {
    Debug.log(DEBUG_DEFAULT, "Error: Unknown scan type '%d'", playerScanniing);
    return -1;
  }

  for (i = 0; i < 5; i++) {
    if (weaponArray[i] != 0) {
      maxRange = Math.max(maxRange, weaponArray[i]);
    }
  }

  xDifference = abs(x - mapIndex % 10) - upgrades;
  yDifference = abs(y - mapIndex / 10) - upgrades;
  distance = Math.round(sqrt(pow(xDifference, 2) + pow(yDifference, 2)));

  if (distance <= 3 + (maxRange - 1) * 2) {
    return 1;
  }

  return 0;
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


/*
 *
 */
void scan(int scanTarget) {
  Debug.log(DEBUG_DEFAULT, "void scan(%d)", scanTarget);
  int i;
  int damage = 0;

  if (scanTarget == PLAYER) {
    damage = weaponsCheck(ENEMY);
    comLog(COM_BELOW_MAP);
    gotoXY(1 + COM_BELOW_MAP_START);
    if (HS_NWEAPONS == 0) {
      printf("You have no weapons.");
      return;
    } else if (HS_SECURITY == SECURITY_SAFE) {
      printf("No enemies nearby.");
      return;
    } else {
      if (HS_IN_RANGE == 0) {
        printf("Hostile Out of Range.");
      } else {
        printf("Hostile In Range.");
      }
      gotoXY(1 + COM_BELOW_MAP_START + 1);
      printf("Potential Damage Output: %i", damage);

      int totalDepletion = 0;

      for (i = 0; i < 5; i++) {
        if (HS_WEAPONS[i] != 0) {
          totalDepletion += WEAPONS[HS_WEAPONS[i] - 1].dep;
        }
      }

      gotoXY(1 + COM_BELOW_MAP_START + 2);
      if (HS_DEPLETION < HS_ENERGY) {
        printf("Sufficient energy available.");
      } else if (HS_DEPLETION == 0 && totalDepletion != 0) {
        printf("Insufficient energy available.");
      } else if (HS_DEPLETION < totalDepletion) {
        printf("Limited energy available.");
      } else {
        printf("read cTrekGame.c:618 log");
        Debug.log(DEBUG_DEFAULT, "other case: totalDepletion: %d, HS_DEPLETION: %d, HS_ENERGY: %d, damage: %d", totalDepletion, HS_DEPLETION, HS_ENERGY, damage);
      }
    }
  } else {
    damage = weaponsCheck(PLAYER);
    comLog(COM_BELOW_MAP);
    gotoXY(1 + COM_BELOW_MAP_START);
    if (EN_SHIP_TYPE > SHIP_ESCAPE_POD && EN_SHIP_TYPE <= SHIP_DESTROYER) {
      if (HS_SHIP_TYPE < 2) {
        printf("No scanning capabilities in this ship.");
        return;
      } else {
        printf("Shield Power: %i  Hull Strength: %i  ", EN_SHIELD, EN_HULL);

        gotoXY(1 + COM_BELOW_MAP_START + 1);
        if (HS_SHIP_TYPE > 3) {
          printf("Gun Types: ( ");
          for (i = 0; i < 5; i++) {
            if (EN_WEAPONS[i] != 0) {
              printf("%i ", EN_WEAPONS[i]);
            }
          }
          printf(")  ");
        }
        printf("Potential Damage Output: %i  ", damage);
        gotoXY(1 + COM_BELOW_MAP_START + 2);
        if (damage == 0) {
          printf("Out of Range.");
        } else {
          printf("In Range."); // 8, 9
        }
      }
    } else {
      printf("No hostiles to scan.");
    }
  }
  printMap(scanTarget);
}


/**
 * TODO armour piercing etc
 */
int applyPlayerDamageReduction(int damage) {
  Debug.log(DEBUG_DEFAULT, "int applyPlayerDamageReduction(%d)", damage);
  int hullReduction = HS_UP_HULL * UPGRADES[UPGRADE_HULL].value;
  int shieldReduction = HS_UP_SHIELD * UPGRADES[UPGRADE_SHIELD].value;

  if (damage - shieldReduction > HS_SHIELD) {
    damage -= HS_SHIELD;
    if (damage - hullReduction > HS_HULL) {
      return HS_HULL;
    } else {
      return damage - hullReduction;
    }
  } else {
    return damage - shieldReduction;
  }
}


/*
 * Get potential damage, weapons in range, and depletion when firing
 * returns damage
 */
int weaponsCheck(int playerFiring) {
  Debug.log(DEBUG_DEFAULT, "int weaponsCheck(%d)", playerFiring);
  int i;
  int weapType;
  int weapCount;
  int xDifference;
  int yDifference;
  int pDamage = 0;
  int eDamage = 0;
  int totalDepletion = 0;
  int pGunCount[3] = {0};
  int eGunCount[3] = {0};
  float distance;

  HS_DEPLETION = 0;
  HS_IN_RANGE = 0;
  EN_IN_RANGE = 0;

  xDifference = pow(EN_SYS_X - HS_SYS_X, 2);
  yDifference = pow(EN_SYS_Y - HS_SYS_Y, 2);
  distance = Math.round(sqrt(xDifference + yDifference));

  for (i = 0; i < 5; i++) {
    if (HS_WEAPONS[i] != 0) {
      pGunCount[HS_WEAPONS[i] - 1]++;
    }
    if (EN_WEAPONS[i] != 0) {
      eGunCount[EN_WEAPONS[i] - 1]++;
    }
  }

  for (i = 0; i < 3; i++) {
    if (distance <= WEAPONS[i].range + HS_UP_RANGE) {
      pDamage += WEAPONS[i].dmg * pGunCount[i];
      HS_IN_RANGE += pGunCount[i];
      totalDepletion += WEAPONS[i].dep * pGunCount[i];
    }
    if (distance <= WEAPONS[i].range) {
      eDamage += WEAPONS[i].dmg * eGunCount[i];
      EN_IN_RANGE += pGunCount[i];
    }
  }
  HS_DEPLETION = totalDepletion;

  // Calculate max dmg with remaining energy
  if (totalDepletion > HS_ENERGY) {
    HS_DEPLETION = 0;
    totalDepletion = HS_ENERGY;
    pDamage = 0;

    for (weapType = 2; weapType >= 0; weapType--) {
      for (weapCount = 0; weapCount < pGunCount[weapType]; weapCount++) {
        if (totalDepletion >= WEAPONS[weapType].dep * (pGunCount[weapType] - weapCount)) {
          pDamage += (pGunCount[weapType] - weapCount) * WEAPONS[weapType].dmg;
          totalDepletion -= (pGunCount[weapType] - weapCount) * WEAPONS[weapType].dep;
          HS_DEPLETION += (pGunCount[weapType] - weapCount) * WEAPONS[weapType].dep;
          break;
        }
      }
    }

    if (pDamage != 0) {
      pDamage += HS_UP_DMG * 10;
    }
  }

  if (playerFiring == PLAYER) {
    return pDamage;
  } else {
    return applyPlayerDamageReduction(eDamage);
  }
}


/*
 * Fire or take damage
 */
void fire(int playerFiring) {
  Debug.log(DEBUG_DEFAULT, "void fire(%d)", playerFiring);
  int damage;
  int crit;
  int target;
  comLog(COM_BELOW_MAP);
  if (playerFiring == PLAYER) {
    target = ENEMY;
    if (HS_NWEAPONS == 0) {
      printf("You have no weapons.");
      return;
    } else if (HS_SECURITY == 1) {
      printf("There are no hostiles to shoot at.");
      return;
    }
  } else {
    target = PLAYER;
  }
  damage = weaponsCheck(playerFiring);
  if (damage != 0) {
    if (percentile() < 10) {
      if (playerFiring == PLAYER) {
        printf("You miss. ");
      } else {
        printf("Enemy misses.");
      }
      return;
    }
    damage *= (playerFiring == ENEMY) ? 0.9 : 1;
    damage -= damage / 10 + randomNumber(damage / 6);
    crit = percentile();
    if (crit >= 95) {
      damage *= 2;
      if (playerFiring == PLAYER) {
        printf("You hit a critical system! ");
      } else {
        printf("Enemy hits a critical system! ");
      }
    } else {
      if (playerFiring == PLAYER) {
        printf("You hit. ");
      } else {
        printf("Enemy hits. ");
      }
      //hitAnim(playerFiring);
    }
    printfLinesAt(COM_BELOW_MAP_START, "\n%i damage was done.\n", damage);
    if (playerFiring == PLAYER) {
      HS_ENERGY -= HS_DEPLETION;
    }
    damageReport(damage, target);
  } else {
    //dmg==0
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
    printf("Enemy takes %d damage.", damage);
    if (distance < 2) {
      printf(" You are hit by shrapnel for %d damage.", damage / 2);
      damageReport(damage / 2, 2);
    }
    damageReport(damage, 1);
  }

  HS_TORPEDOS--;
}


/*
 * Deal damage. Upgrade reduction already applied
 */
void damageReport(int damage, int target) {
  Debug.log(DEBUG_DEFAULT, "void damageReport(%d, %d)", damage, target);
  Ship *damageReceiver;
  int shieldReduction = 0;
  int hullReduction = 0;
  if (target == PLAYER) {
    damageReceiver = HS_SHIP;
  } else {
    damageReceiver = EN_SHIP;
    HS_ENERGY -= HS_DEPLETION;
  }
  Debug.log(DEBUG_DEFAULT, "target:%d, dmg: %d, shield:%d, hull:%d",
  target, damage, damageReceiver->shield, damageReceiver->hull);

  if (damage > damageReceiver->shield) {
    // shields down - hull damage
    if (damage - damageReceiver->shield >= damageReceiver->hull) {
      // death
      if (target == PLAYER) {
        printf("Your ship was destroyed.");
        shipDestroyed();
      } else {
        printf("The enemy ship was destroyed.");
        currentSystem->enemy = System::destroyEnemy(currentSystem->enemy);
        HS_SECURITY = 1;
      }
    } else {
      // shield and hull damage
      if (damageReceiver->shield == 0) {
        damageReceiver->hull -= Math.max(0, damage - 10 * hullReduction);
      } else {
        damageReceiver->hull -= Math.max(0, damage - 10 * hullReduction - 10 * shieldReduction);
        damageReceiver->shield = 0;
      }
    }
  } else {
    // shield damaged
    damageReceiver->shield -= damage;
  }
  Debug.log(DEBUG_DEFAULT, "target:%d, dmg: %d, shield:%d, hull:%d",
  target, damage, damageReceiver->shield, damageReceiver->hull);
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
    int damage = randomNumber(HS_SHIELD + HS_HULL + 10 * HS_SHIP_TYPE * (EN_SHIP_TYPE - 5));
    if (damage != 0) {
      printfLinesAt(COM_BELOW_MAP_START, "%s\n%i damage was done",
      "Boarding the vessel tripped a security protocol.", damage);
      damageReport(damage, 2);
    } else {
      printLinesAt(COM_BELOW_MAP_START,
      "Boarding the vessel tripped a security protocol.\n"
      "Fortunately the system was damaged and malfunctioned.");
    }
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
    msgLen += snprintf(msg + msgLen, BUFFER_MEDIUM - msgLen, "%d Credits, ", lootCount);
    HS_CREDITS += lootCount;
    foundLoot = 1;
  }

  if (HS_SHIP_TYPE != 0) {
    // Commodities
    if (percentile() > 50) {
      int lootType = COMMODITY_PLASMA + randomNumber(4);
      lootCount = Math.min((EN_SHIP_TYPE - 5) * randomNumber(5), cargoLeft());

      if (lootCount) {
        msgLen += snprintf(msg + msgLen, BUFFER_MEDIUM - msgLen, "%d %s, ", lootCount, hudstatToCommodity(lootType));
        currentPlayer->ship->commodities[lootType - COMMODITY_PLASMA] += lootCount;
        foundLoot = 1;
      }
    }

    // Energy
    if (percentile() > 65 && HS_SHIP_TYPE != 1 && EN_SHIP_TYPE != 6) {
      lootCount = Math.min((EN_SHIP_TYPE - 5) * randomNumber(100), hudstatToCapacity(COMMODITY_ENERGY) - HS_ENERGY);

      if (lootCount) {
        msgLen += snprintf(msg + msgLen, BUFFER_MEDIUM - msgLen,
        "%d Power cells, ", lootCount);
        HS_ENERGY += lootCount;
        foundLoot = 1;
      }
    }

    // Hull
    if (percentile() > 70) {
      lootCount = Math.min((EN_SHIP_TYPE - 5) * (randomNumber(30)), hudstatToCapacity(COMMODITY_HULL) - HS_HULL);

      if (lootCount) {
        msgLen += snprintf(msg + msgLen, BUFFER_MEDIUM - msgLen,
        "%d Tritanium plating, ",lootCount);
        HS_HULL += lootCount;
        foundLoot = 1;
      }
    }

    // Torpedos
    if (percentile() > 85 && HS_SHIP_TYPE > 1) {
      lootCount = Math.min(randomNumber(EN_SHIP_TYPE), hudstatToCapacity(COMMODITY_TORPEDOS) - HS_TORPEDOS);

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

  currentSystem->map[EN_POS] = EN_TILE;
  Player::unload(currentSystem->enemy);
  currentSystem->enemy = Player::newEnemy(0);
}

