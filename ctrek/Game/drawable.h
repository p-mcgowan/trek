/**
 *
 */
void drawWindow(int x, int y, int w, int h) {
  Debug.log(DEBUG_DEFAULT, "void drawWindow(%d, %d, %d, %d)", x, y, w, h);
  int i;
  char line[BUFFER_MEDIUM];
  memset(line, '-', BUFFER_MEDIUM * sizeof(char));

  gotoXY(x, y);
  printf("+%*.*s+", w - 2, w - 2, line);

  for (i = y + 1; i < y + h - 1; i++) {
    gotoXY(x, i);
    printf("|%*s|", w - 2, " ");
    //printf("%d%*s|", i, w - 2, " ");
  }

  gotoXY(x, y + h - 1);
  printf("+%*.*s+", w - 2, w - 2, line);
}


/**
 * Print a formatted string at x, y.
 * Returns next y coord.
 */
int printAt(int x, int y, const char *fmt, ...) {
  va_list args, dlog;
  va_start(args, fmt);
  if (Debug.logLevel <= DEBUG_LOG_LOW) {
    __va_copy(dlog, args);
    char buffer[BUFFER_MEDIUM] = { "int printAt(%d, %d, " };
    snprintf(buffer, BUFFER_MEDIUM, "int printAt(%d, %d, ", x, y);
    strcat(buffer, fmt);
    strcat(buffer, ")\n");
    vfprintf(stderr, buffer, dlog);
    va_end(dlog);
  }
  gotoXY(x, y);
  vprintf(fmt, args);
  va_end(args);
  return ++y;
}


/**
 * Split a string with newlines print tokens on separate, indented lines.
 * Returns next y coord.
 */
int printLinesAt(int x, int y, const char *str) {
  char buffer[BUFFER_MEDIUM] = {0};
  int i;
  if (Debug.logLevel <= DEBUG_LOG_LOW) {
    snprintf(buffer, BUFFER_MEDIUM - 1, "printLinesAt: %s", str);
    Debug.log(DEBUG_DEFAULT, buffer);
    memset(buffer, 0, BUFFER_MEDIUM * sizeof(char));
  }
  snprintf(buffer, BUFFER_MEDIUM - 1, "%s", str);

  gotoXY(x, y);
  for (i = 0; i < BUFFER_MEDIUM; i++) {
    if (buffer[i] == '\n') {
      gotoXY(x, ++y);
    } else if (buffer[i] == '\0') {
      break;
    } else {
      printf("%c", buffer[i]);
    }
  }

  return ++y;
}


int printfLinesAt(int x, int y, const char *fmt, ...) {
  va_list args;
  char buffer[BUFFER_MEDIUM] = {0};
  va_start(args, fmt);
  vsnprintf(buffer, BUFFER_MEDIUM - 1, fmt, args);
  printLinesAt(x, y, (const char*)buffer);
  va_end(args);
  return ++y;
}


void rangeMap(int scanner) {
  Debug.log(DEBUG_DEFAULT, "int rangeMap(%d)", scanner);
  int xDifference, yDifference, i;
  int upgrades, maxRange = 0, sx, sy, x, y;
  int cellWidth = 4;
  int cellHeight = 2;
  float distance;

  int *weaponArray;
  if (scanner == ENEMY) {
    weaponArray = currentPlayer->ship->weapons;
    x = HS_SYS_X;
    y = HS_SYS_Y;
    upgrades = 0;
  } else {
    weaponArray = currentSystem->enemy->ship->weapons;
    x = EN_SYS_X;
    y = EN_SYS_Y;
    upgrades = HS_UP_RANGE;
  }

  for (i = 0; i < 5; i++) {
    if (weaponArray[i] != 0) {
      maxRange = Math.max(weaponArray[i], maxRange);
    }
  }

  if (maxRange > 0) {
    int fallingEdge;
    int size = sqrt(SYSTEM_SIZE);
    for (sy = 0; sy < size; sy++) {
      fallingEdge = 0;
      for (sx = 0; sx < size; sx++) {
        xDifference = abs(x - sx) - upgrades;
        yDifference = abs(y - sy) - upgrades;
        distance = Math.round(sqrt(pow(xDifference, 2) + pow(yDifference, 2)));

        if (distance <= 3 + (maxRange - 1) * 2) {
          fallingEdge = 1;
          for (i = 0; i < cellHeight; i++) {
            printAt(3 + Math.max(1, cellWidth * sx), 3 + Math.max(1, cellHeight * sy + i),
            "%s%-*s%s", STR_COL_CYAN_BG, cellWidth, " ", STR_COL_RESET);
          }
        } else if (fallingEdge == 1) {
          fallingEdge = 0;
          break;
        }
      }
    }
  }
}


/**
 * Draws system map containing planets, players, and enemies
 */
void printMap(int scanTarget) {
  Debug.log(DEBUG_DEFAULT, "void printMap(%d)", scanTarget);
  int i;
  int cellWidth = 4;
  int cellWCenter = 2;
  int cellHeight = 2;
  int cellHCenter = 1;

  drawWindow(3, 3, 10 * cellWidth + 2, 10 * cellHeight + 1);

  if (scanTarget != 0) {
    rangeMap(scanTarget);
  }
  printAt(3 + (cellWidth * currentPlayer->systemX) + cellWCenter,
  3 + (cellHeight * currentPlayer->systemY) + cellHCenter,
  "%s", charColour((int)'P', scanTarget, currentPlayer->pos()));

  for (i = 0; i < currentSystem->nPlanets; i++) {
    int planetX = currentSystem->planets[i]->position % 10;
    int planetY = currentSystem->planets[i]->position / 10;
    printAt(3 + cellWidth * planetX + cellWCenter, 3 + cellHeight * planetY + cellHCenter,
    "%s", charColour((int)'O', scanTarget, currentSystem->planets[i]->position));
  }

  if (EN_SHIP_TYPE > SHIP_ESCAPE_POD) {
    printAt(3 + cellWidth * EN_SYS_X + cellWCenter, 3 + cellHeight * EN_SYS_Y + cellHCenter,
    "%s", charColour(EN_SHIP_TYPE, scanTarget, EN_POS));
  }

  healthBars();
}


char* charColour(int mapTile, int scanTarget, int mapIndex) {
  Debug.log(DEBUG_DEFAULT, "char* charColour(%d, %d, %d)", mapTile, scanTarget, mapIndex);
  static char out[BUFFER_MEDIUM] = {0};
  static char tile[32] = {0};
  int colourForeground = COL_WHITE;
  int colourBackground = COL_BLACK_BG;

  if (scanTarget != 0 && inRange(scanTarget, mapIndex)) {
    colourBackground = COL_CYAN_BG;
    colourForeground = COL_WHITE;
  }
  if (mapTile >= 1 && mapTile <= 5) {
    colourForeground = COL_RED;
    snprintf(tile, 31, "%s", printShip(mapTile));
  }
  if (mapTile >= 6 && mapTile <= 10) {
    colourForeground = COL_GREY;
    snprintf(tile, 31, "%s", printShip(mapTile));
  }
  if (mapTile == TILE_PLANET) {
    colourForeground = COL_GREEN;
    snprintf(tile, 31, "%c", TILE_PLANET);
  }
  if (mapTile == TILE_PLAYER) {
    colourForeground = COL_YELLOW;
    snprintf(tile, 31, "%s", printShip(HS_SHIP_TYPE));
  }
  if (mapTile == TILE_EMPTY) {
    colourForeground = COL_WHITE;
    snprintf(tile, 31, " ");
  }

  // Format and print out colours and tiles
  snprintf(out, BUFFER_MEDIUM - 1, "\033[%d;%dm%s" STR_COL_RESET, colourForeground, colourBackground, tile);

  return out;
}


const char* printShip(int ship) {
  Debug.log(DEBUG_DEFAULT, "const char* printShip(%d)", ship);
  switch (ship) {
    case 0:
      return ".";
    case 1:
    case 6:
      return "1";
    case 2:
    case 7:
      return "2";
    case 3:
    case 8:
      return "3";
    case 4:
    case 9:
      return "4";
    case 5:
    case 10:
      return "5";
    default:
      return "S";
  }
}


void printProgressBar(int skillSlot) {
  Debug.log(DEBUG_DEFAULT, "void printProgressBar(%d)", skillSlot);
  int i;
  printf("  ");
  for (i = 1; i <= 10; i++) {
    if (HS_SKILLS[skillSlot] % 10 >= i) {
      setCB(COL_WHITE, COL_GREEN);
    } else {
      setCB(COL_WHITE, COL_BLACK);
      printf("  ");
      setCB(COL_WHITE, COL_BLACK);
      printf("|");
    }
  }
}


// TODO
void skillsHUD(int x, int page) {
  Debug.log(DEBUG_DEFAULT, "void skillsHUD(%d, %d)", x, page);
  int i, j, p = 9;
  gotoXY(x, 2);
  printf(" Skills:");
  gotoXY(x + 48, 2);
  printf("Page %i", page);
  gotoXY(x, 3);
  printf("%s", HUD_LINE);

  for (j = 4; j <= 22; j += 2) {
    gotoXY(x, j);
    printf("  Skill %i", 10 * page - p);
    gotoXY(x, j + 1);
    printProgressBar(10 * page - p);
    printf("\tLevel %i (%i %c)\t", HS_SKILLS[10 * page - p] / 10 + 1,
    (HS_SKILLS[10 * page - p] % 10) * 10, 37);
    p--;
  }

  for (i = 4; i < 32; i++) {
    gotoXY(x, i);
    printf("|");
    gotoXY(x + HUD_WIDTH, i);
    printf("|");
  }
}


void minimalHUD(int startX, int startY) {
  Debug.log(DEBUG_DEFAULT, "void minimalHUD(%d, %d)", startX, startY);
  gotoXY(startX, startY++);
  clrLine();
  printf("Shields: %s  ", hudColours(HS_SHIELD, CAP_SHIELD));
  printf("Hull: %s  ", hudColours(HS_HULL, CAP_HULL));
  printf("Energy: %-10s", hudColours(HS_ENERGY, CAP_ENERGY));
  gotoXY(startX, startY++);
  clrLine();
  printf("Security: %s  Armaments: %s  ", securityStatus(), weaponStatus());
  printf("Torpedos: %-10s", hudColours(HS_TORPEDOS, CAP_TORP));
  gotoXY(startX, startY++);
  clrLine();
  printf("Enemies: %d  Planets: %d  ISO: %d  Credits: %-10s",
  EN_SHIP_TYPE > SHIP_ESCAPE_POD && EN_SHIP_TYPE <= SHIP_DESTROYER, HS_PLANETS, HS_STELLAR, prefix(HS_CREDITS, 'n'));
  gotoXY(startX, startY);
  clrLine();
  printf("System: (%d, %d)  ", HS_SYS_X, HS_SYS_Y);
  printf("Galaxy: (%d, %d)", HS_GAL_X, HS_GAL_Y);
}


void healthBars() {
  Debug.log(DEBUG_DEFAULT, "void healthBars()");
  int i, x = 47, y = 22;
  int height = 19;
  if (HS_SHIP_TYPE == SHIP_ESCAPE_POD) {
    return;
  }
  int pctShield = (CAP_SHIELD == 0 || HS_SHIELD == 0) ? 0 : Math.max(100 * HS_SHIELD / CAP_SHIELD, 100 / height + 1);
  int pctHull = (CAP_HULL == 0 || HS_HULL == 0) ? 0 : Math.max(100 * HS_HULL / CAP_HULL, 100 / height + 1);
  int pctEnergy = (CAP_ENERGY == 0 || HS_ENERGY == 0) ? 0 : Math.max(100 * HS_ENERGY / CAP_ENERGY, 100 / height + 1);

  for (i = 0; i < height; i++) {
    printAt(x, y - i, "      ");
  }
  const char* s = (pctHull <= 20) ? STR_COL_RED_BG : STR_COL_GREEN_BG;
  for (i = 0; i < height; i++) {
    if ((pctEnergy * height) / 100 > i) {
      printAt(x, y - i, "%s ", STR_COL_YELLOW_BG);
    } else {
      break;
    }
  }
  for (i = 0; i < height; i++) {
    if ((pctHull * height) / 100 > i) {
      printAt(x + 2, y - i, "%s ", s);
    } else {
      break;
    }
  }
  for (i = 0; i < height; i++) {
    if ((pctShield * height) / 100 > i) {
      printAt(x + 4, y - i, "%s ", STR_COL_CYAN_BG);
    } else {
      break;
    }
  }
  printf(STR_COL_RESET);
}


char* weaponStatus() {
  Debug.log(DEBUG_DEFAULT, "static char* weaponStatus()");
  static char buf[BUFFER_MEDIUM] = {0};

  if (HS_SHIP_TYPE == 0) {
    snprintf(buf, BUFFER_MEDIUM - 1, "---");
    return buf;
  } else {
    weaponsCheck(PLAYER);
    if (EN_SHIP_TYPE > SHIP_ESCAPE_POD && EN_SHIP_TYPE <= SHIP_DESTROYER) {
      snprintf(buf, BUFFER_MEDIUM - 1, "%i/%i (%s%i%s in range)", HS_NWEAPONS, CAP_HARDPOINT,
      gunRangeColour(HS_NWEAPONS, HS_IN_RANGE), HS_IN_RANGE, STR_COL_WHITE);
    } else {
      snprintf(buf, BUFFER_MEDIUM - 1, "%i/%i", HS_NWEAPONS, CAP_HARDPOINT);
    }
  }

  return buf;
}


// Clear, redraw, and set cursor at top left spot
void comLog(int x, int y) {
  Debug.log(DEBUG_DEFAULT, "void comLog(%d, %d)", x, y);
  drawWindow(x, y, COM_WIDTH, COM_HEIGHT);
  gotoXY(x + 1, y + 1);
}


void HUD(int startX, int startY) {
  Debug.log(DEBUG_DEFAULT, "void HUD(%d, %d)", startX, startY);
  securityStatus();
  startX++;
  printAt(startX, startY, "Helm Console:");
  drawWindow(startX - 1, ++startY, HUD_WIDTH, HUD_HELM_HEIGHT);
  printAt(startX, ++startY, " Stardate           : %i.%i", HS_DATE / 10, HS_DATE % 10);
  printAt(startX, ++startY, " Ship Class         : %s", Ship::shipName(HS_SHIP_TYPE));
  printAt(startX, ++startY, " System Coordinates : (%i, %i)  ", HS_SYS_X, HS_SYS_Y);
  printAt(startX, ++startY, " Galaxy Coordinates : (%i, %i)  ", HS_GAL_X, HS_GAL_Y);
  startY++;
  printAt(startX, ++startY, "Tactical Ship Status:");
  drawWindow(startX - 1, startY, HUD_WIDTH, HUD_STATUS_HEIGHT);
  printAt(startX, ++startY, " Shields         : %s", hudColours(HS_SHIELD, CAP_SHIELD));
  printAt(startX, ++startY, " Hull Integrity  : %s", hudColours(HS_HULL, CAP_HULL));
  printAt(startX, ++startY, " Security        : %s", securityStatus());
  printAt(startX, ++startY, " Energy Reserves : %s", hudColours(HS_ENERGY, CAP_ENERGY));
  printAt(startX, ++startY, " Torpedos        : %s", hudColours(HS_TORPEDOS, CAP_TORP));
  printAt(startX, ++startY, " Armaments       : %s", weaponStatus());

  startY++;
  printAt(startX, ++startY, "Cargo: ");
  if (HS_SHIP_TYPE == 0) {
    printf("---");
  } else {
    printf("(%i/%i units)", HS_CARGO, CAP_CARGO + 100 * HS_UP_CARGO);
  }
  drawWindow(startX - 1, startY, HUD_WIDTH, HUD_CARGO_HEIGHT);
  printAt(startX, ++startY, " Credits         : %s", prefix(HS_CREDITS, 'n'));
  if (HS_SHIP_TYPE != SHIP_ESCAPE_POD) {
    printAt(startX, ++startY, " Warp Plasma     : %s", prefix(HS_PLASMA, 'L'));
    printAt(startX, ++startY, " Duterium        : %s", prefix(HS_DUTERIUM, 'g'));
    printAt(startX, ++startY, " Latinum         : %s", prefix(HS_LATINUM, 'g'));
    printAt(startX, ++startY, " Water           : %s", prefix(HS_WATER, 'L'));
    printAt(startX, ++startY, " Uranium Ore     : %s", prefix(HS_URANIUM, 'g'));
  } else {
    printAt(startX, ++startY, " Warp Plasma     : ---");
    printAt(startX, ++startY, " Duterium        : ---");
    printAt(startX, ++startY, " Latinum         : ---");
    printAt(startX, ++startY, " Water           : ---");
    printAt(startX, ++startY, " Uranium Ore     : ---");
  }

  ++startY;
  printAt(startX, ++startY, "System Peripherals:");
  drawWindow(startX - 1, ++startY, HUD_WIDTH, HUD_SYSTEM_HEIGHT);
  printAt(startX, ++startY, " Planets: %i ", HS_PLANETS);
  printAt(startX, ++startY, " Enemies: %i", EN_SHIP_TYPE > SHIP_ESCAPE_POD && EN_SHIP_TYPE <= SHIP_DESTROYER);
  printAt(startX, ++startY, " Inactive Stellar Objects: %i  ", HS_STELLAR);
}


const char* gunRangeColour(int totalGuns, int gunsInRange) {
  Debug.log(DEBUG_DEFAULT, "const char* gunRangeColour(%d, %d)", totalGuns, gunsInRange);
  if (gunsInRange == 0) {
    // red no guns in range
    return STR_COL_RED;
  } else if (gunsInRange == totalGuns) {
    // green, all guns in range
    return STR_COL_GREEN;
  }
  // yellow, not all guns in range
  return STR_COL_YELLOW;
}


// 3, 5, 9
char* hudColours(int currentHud, int maxHud) {
  Debug.log(DEBUG_DEFAULT, "static char* hudColours(%d, %d)", currentHud, maxHud);
  const char *fmt = "%s%i" STR_COL_WHITE "/%i";
  static char msg[BUFFER_MEDIUM] = {0};

  if (HS_SHIP_TYPE == SHIP_ESCAPE_POD) {
    return (char*)"---";
  }

  if (maxHud != 0) {
    int percent = currentHud * 100 / maxHud;
    if (percent == 100) {
      snprintf(msg, BUFFER_MEDIUM - 1, fmt, STR_COL_WHITE, currentHud, maxHud);
    } else if (percent > 75) {
      snprintf(msg, BUFFER_MEDIUM - 1, fmt, STR_COL_GREEN, currentHud, maxHud);
    } else if (percent > 50) {
      snprintf(msg, BUFFER_MEDIUM - 1, fmt, STR_COL_YELLOW, currentHud, maxHud);
    } else {
      snprintf(msg, BUFFER_MEDIUM - 1, fmt, STR_COL_RED, currentHud, maxHud);
    }
  } else {
    snprintf(msg, BUFFER_MEDIUM - 1, fmt, STR_COL_WHITE, currentHud, maxHud);
  }

  return msg;
}


void achievementsMenu() {
  Debug.log(DEBUG_DEFAULT, "void achievementsMenu()");
  int page = 1, choice, slot, value;
  do {
    //planetHUD(); // dis = 0, 1 = syst, 2 = plan, 3 = ships, 4 = weps, 5 = kills
    if (page == 1) {
      gotoXY(3, 5);
      printf("Basic Achievements:");
      gotoXY(5, 7);
      if (ACHIEVE_DISTANCE >= 20) {
        setC(COL_WHITE);
      } else {
        setC(COL_GREY);
      }
      printf("The Traveler");
      printf("\t\t%i/20 km travelled", ACHIEVE_DISTANCE);
      gotoXY(5, 8);
      if (ACHIEVE_SYSTEMS >= 10) {
        setC(COL_WHITE);
      } else {
        setC(COL_GREY);
      }
      printf("Discoverer");
      printf("\t\t\t%i/10 systems visited", ACHIEVE_SYSTEMS);
      gotoXY(5, 9);
      if (ACHIEVE_PLANETS >= 15) {
        setC(COL_WHITE);
      } else {
        setC(COL_GREY);
      }
      printf("Land Harr");
      printf("\t\t\t%i/15 planets landed on", ACHIEVE_PLANETS);
      gotoXY(5, 10);
      if (ACHIEVE_SHIPS >= 2) {
        setC(COL_WHITE);
      } else {
        setC(COL_GREY);
      }
      printf("Able Captain");
      printf("\t\t%i/2 ships owned", ACHIEVE_SHIPS);
      gotoXY(5, 11);
      if (ACHIEVE_WEAPONS >= 5) {
        setC(COL_WHITE);
      } else {
        setC(COL_GREY);
      }
      printf("Rifleman");
      printf("\t\t\t%i/5 weapons obtained", ACHIEVE_WEAPONS);
      gotoXY(5, 12);
      if (ACHIEVE_KILLS >= 10) {
        setC(COL_WHITE);
      } else {
        setC(COL_GREY);
      }
      printf("Brute");
      printf("\t\t\t%i/10 enemies killed", ACHIEVE_KILLS);
    }
    if (page == 2) {
      gotoXY(3, 5);
      printf("Intermediate Achievements:");
      gotoXY(5, 7);
      if (ACHIEVE_DISTANCE >= 50) {
        setC(COL_WHITE);
      } else {
        setC(COL_GREY);
      }
      printf("Well Travelled");
      printf("\t\t%i/50 km travelled", ACHIEVE_DISTANCE);
      gotoXY(5, 8);
      if (ACHIEVE_SYSTEMS >= 25) {
        setC(COL_WHITE);
      } else {
        setC(COL_GREY);
      }
      printf("Explorer");
      printf("\t\t\t%i/25 systems visited", ACHIEVE_SYSTEMS);
      gotoXY(5, 9);
      if (ACHIEVE_PLANETS >= 35) {
        setC(COL_WHITE);
      } else {
        setC(COL_GREY);
      }
      printf("Sphere Hunter");
      printf("\t\t%i/35 planets landed on", ACHIEVE_PLANETS);
      gotoXY(5, 10);
      if (ACHIEVE_SHIPS >= 4) {
        setC(COL_WHITE);
      } else {
        setC(COL_GREY);
      }
      printf("Admiral");
      printf("\t\t\t%i/4 ships owned", ACHIEVE_SHIPS);
      gotoXY(5, 11);
      if (ACHIEVE_WEAPONS >= 15) {
        setC(COL_WHITE);
      } else {
        setC(COL_GREY);
      }
      printf("Arms Dealer");
      printf("\t\t%i/15 weapons obtained", ACHIEVE_WEAPONS);
      gotoXY(5, 12);
      if (ACHIEVE_KILLS >= 25) {
        setC(COL_WHITE);
      } else {
        setC(COL_GREY);
      }
      printf("Soldier");
      printf("\t\t\t%i/25 enemiies killed", ACHIEVE_KILLS);
    }
    if (page == 3) {
      gotoXY(3, 5);
      printf("Advanced Achievements:");
      gotoXY(5, 7);
      if (ACHIEVE_DISTANCE >= 150) {
        setC(COL_WHITE);
      } else {
        setC(COL_GREY);
      }
      printf("Not Lost In Space");
      printf("\t\t%i/150 km travelled", ACHIEVE_DISTANCE);
      gotoXY(5, 8);
      if (ACHIEVE_SYSTEMS >= 50) {
        setC(COL_WHITE);
      } else {
        setC(COL_GREY);
      }
      printf("Found Your Hemingway");
      printf("\t%i/50 system visited", ACHIEVE_SYSTEMS);
      gotoXY(5, 9);
      if (ACHIEVE_PLANETS >= 75) {
        setC(COL_WHITE);
      } else {
        setC(COL_GREY);
      }
      printf("Diplomat");
      printf("\t\t\t%i/75 planets landed on", ACHIEVE_PLANETS);
      gotoXY(5, 10);
      if (ACHIEVE_SHIPS >= 5) {
        setC(COL_WHITE);
      } else {
        setC(COL_GREY);
      }
      printf("Enshiplopedia");
      printf("\t\t%i/5 ships owned", ACHIEVE_SHIPS);
      gotoXY(5, 11);
      if (ACHIEVE_WEAPONS >= 30) {
        setC(COL_WHITE);
      } else {
        setC(COL_GREY);
      }
      printf("Gun Fettish");
      printf("\t\t%i/30 weapons obtained", ACHIEVE_WEAPONS);
      gotoXY(5, 12);
      if (ACHIEVE_KILLS >= 50) {
        setC(COL_WHITE);
      } else {
        setC(COL_GREY);
      }
      printf("Warren Piece");
      printf("\t\t%i/50 enemies killed", ACHIEVE_KILLS);
// silver tounge
    }
    setC(COL_WHITE);
    gotoXY(3, 27);
    if (page == 1) {
      printf("0: Back\t\t2: Next");
    } else if (page == 3) {
      printf("1: Previous\t\t0: Back");
    } else {
      printf("1: Previous\t\t0: Back\t\t2: Next");
    }
    gotoXY(3, 29);
    printf("5: achievements[x] = y");
    gotoXY(3, 30);
    choice = getch_arrow() - '0';
    comLog(COM_BELOW_MAP); //66
    if (choice == 5) {
      gotoXY(3, 28);
      printf("achievements[");
      scanf("%i", &slot);
      if (slot > 9) {
        gotoXY(18, 28) ;
      } else {
        gotoXY(17, 28);
      }
      printf("] = ");
      scanf("%i", &value);
      if (slot > 50 || value > 200) {
        gotoXY(67, 27);
        printf("Invalid slot/value");
      } else {
        HS_ACHIEV[slot] = value;
      }
    }
    if (choice == 1) {
      if (page == 1) {
        printf("Cannot go back a page");
      } else {
        page--;
      }
    }
    if (choice == 2) {
      if (page == 3) {
        printf("Cannot go forward a page");
      } else {
        page++;
      }
    }
    if (choice != 1 && choice != 2 && choice != 0 && choice != 5) {
      printf("Invalid selection");
    }
  } while (choice != 0);
}

