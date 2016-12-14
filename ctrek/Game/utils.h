/**
 * Map hudstat to player object
 * Returns a count of the current player hudstat
 */
int *hudstatToQuantity(int hudstat) {
  Debug.log(DEBUG_DEFAULT, "int hudstatToCapacity(%d)", hudstat);
  switch(hudstat) {
    case COMMODITY_TORPEDOS:
       return &HS_TORPEDOS;
    case COMMODITY_PLASMA:
    case COMMODITY_DUTERIUM:
    case COMMODITY_LATINUM:
    case COMMODITY_WATER:
    case COMMODITY_URANIUM:
       return &(currentPlayer->ship->commodities[hudstat - COMMODITY_PLASMA]);
//    case COMMODITY_WEAPON1:
//    case COMMODITY_WEAPON2:
//    case COMMODITY_WEAPON3:
//    case COMMODITY_WEAPON4:
//    case COMMODITY_WEAPON5:
//    case COMMODITY_WEAPONS:
//       return &HS_WEAPONS;
    default:
      Debug.log(DEBUG_DEFAULT, "ERROR: Could not find hudstat %d", hudstat);
      return NULL;
  }
}


/**
 * Map hudstats to capacityArray.
 * Returns capacity of hudstat, or -1 if not found.
 */
int hudstatToCapacity(int hudstat) {
  Debug.log(DEBUG_DEFAULT, "int hudstatToCapacity(%d)", hudstat);
  int ret = -1;
  switch(hudstat) {
    case COMMODITY_SHIELD:
      ret = CAP_SHIELD;  // capacityArray[CAP_I_SHIELD + HS_SHIP_TYPE];
      break;
    case COMMODITY_HULL:
      ret = CAP_HULL;  // capacityArray[CAP_I_HULL + HS_SHIP_TYPE];
      break;
    case COMMODITY_ENERGY:
      ret = CAP_ENERGY;  // capacityArray[CAP_I_ENERGY + HS_SHIP_TYPE];
      break;
    case COMMODITY_TORPEDOS:
      ret = CAP_TORP;  // capacityArray[CAP_I_TORP + HS_SHIP_TYPE];
      break;
    case COMMODITY_PLASMA:
    case COMMODITY_DUTERIUM:
    case COMMODITY_LATINUM:
    case COMMODITY_WATER:
    case COMMODITY_URANIUM:
      ret = CAP_CARGO + HS_UP_CARGO;
      //capacityArray[CAP_I_CARGO + HS_SHIP_TYPE] + HS_UP_CARGO;
      break;
//    case COMMODITY_WEAPON1:
//    case COMMODITY_WEAPON2:
//    case COMMODITY_WEAPON3:
//    case COMMODITY_WEAPON4:
//    case COMMODITY_WEAPON5:
    case COMMODITY_WEAPONS:
      ret = CAP_HARDPOINT;  // capacityArray[CAP_I_HARDPOINT + HS_SHIP_TYPE];
      break;
    default:
      ret = -1;
      break;
  }
  Debug.log(DEBUG_DEFAULT, "%d", ret);
  return ret;
}


/**
 * Map commodities to strings
 */
const char *hudstatToCommodity(int hudstat) {
  Debug.log(DEBUG_DEFAULT, "const char *hudstatToCommodity(%d)", hudstat);
  switch(hudstat) {
    case COMMODITY_PLASMA:
      return "Warp plasma";
    case COMMODITY_DUTERIUM:
      return "Duterium";
    case COMMODITY_LATINUM:
      return "Latinum";
    case COMMODITY_WATER:
      return "Water";
    case COMMODITY_URANIUM:
      return "Uranium ore";
    default:
      return "";
  }
}


void seed() {
  if ((unsigned int)clock() != lastSeed) {
    srand(clock());
    lastSeed = clock();
  }
}


int randomNumber(int max) {
  seed();
  return ((float)rand() / (float)RAND_MAX) * max;
}


int percentile() {
  return randomNumber(100);
}


/**
 * Returns topmost map tile
 */
int getMapTile(int coord) {
  Debug.log(DEBUG_DEFAULT, "int getMapTile(%d)", coord);
  return currentSystem->map[coord];
}


/**
 * Returns true if there is an enemy tile at <coords>
 */
int enemyAtTile(int coord) {
  Debug.log(DEBUG_DEFAULT, "int enemyAtTile(%d)", coord);
  if (coord > 99 || coord < 0) {
    return 0;
  }
  int tile = getMapTile(coord);
  return (tile >= TILE_ENEMY1 && tile <= TILE_ENEMY5);
}


/**
 * Returns true if there is derelict tile at <coords>
 */
int derelictAtTile(int coord) {
  Debug.log(DEBUG_DEFAULT, "int derelictAtTile(%d)", coord);
  int tile = getMapTile(coord);
  return (tile >= TILE_DERELICT1 && tile <= TILE_DERELICT5);
}


char* getCoords() {
  Debug.log(DEBUG_DEFAULT, "static char* getCoords()");
  static char buf[BUFFER_MEDIUM] = {0};
  const char* fmt = "(%d, %d)";

  snprintf(buf, BUFFER_MEDIUM, fmt, HS_SYS_X, HS_SYS_Y);

  return buf;
}


char* prefix(int arrayValue, char unit) {
  Debug.log(DEBUG_DEFAULT, "static char* prefix(%d, %c)", arrayValue, unit);
  int pointValue, decimal, zero = 0;
  char unitPrefix;
  static char buf[BUFFER_MEDIUM] = {0};

  if (arrayValue > 1000) {
    pointValue = arrayValue / 1000;
    decimal = (arrayValue % 1000) / 10;
    zero = (decimal < 10);

    unitPrefix = 'k';
  }

  if (arrayValue > 1000000) {
    zero = 0;
    pointValue = arrayValue / 1000000;
    decimal = (arrayValue % 1000000) / 10000;
    zero = (decimal < 10);
    unitPrefix = 'M';
  }
  const char *suffix = { (unit == 'g') ? " kg" : (unit == 'L') ? " L" : "" };

  if (arrayValue < 1000) {
    snprintf(buf, BUFFER_MEDIUM , "%i%s", arrayValue, suffix);
  } else if (zero == 0) {
    snprintf(buf, BUFFER_MEDIUM, "%i.%i%c%s", pointValue, decimal, unitPrefix, suffix);
  } else {
    snprintf(buf, BUFFER_MEDIUM - 1, "%i.0%i%c%s", pointValue, decimal, unitPrefix, suffix);
  }
  return buf;
}


int cargoLeft() {
  Debug.log(DEBUG_DEFAULT, "int cargoLeft()");
  return CAP_CARGO + 100 * HS_UP_CARGO - HS_CARGO;
}


const char* securityStatus() {
  Debug.log(DEBUG_DEFAULT, "const char* securityStatus()");
  int status = SECURITY_SAFE;

  if (EN_SHIP_TYPE > SHIP_ESCAPE_POD && EN_SHIP_TYPE <= SHIP_DESTROYER) {
    status = SECURITY_UNSAFE;
    if (weaponsCheck(PLAYER)) {
      status = SECURITY_INRANGE;
    }
  }

  HS_SECURITY = status;

  if (HS_SHIP_TYPE == SHIP_ESCAPE_POD) {
    return "---";
  }

  switch (status) {
  case 1:
    return "Safe";
  case 2:
    return "\033[30;43mCaution" STR_COL_RESET;
  case 3:
    return "\033[37;41mDanger" STR_COL_RESET;
  }
  return "";
}


int tryToSell(int quantity, int price, int hudstat) {
  Debug.log(DEBUG_DEFAULT, "int tryToSell(%d, %d, %d)", quantity, price, hudstat);
  int *item = hudstatToQuantity(hudstat);
  if (*item < quantity) {
    comLog(COM_BELOW_PLANET_HUD);
    printf("Insufficient supply. Did you want to sell %d (y/n)?", *item);
    int buymax = getch_arrow();
    if (buymax == 'y') {
      printBuySell('s', hudstat, *item, *item * price, 0);
      HS_CREDITS += *item * price;
      *item = 0;
      currentPlayer->commodityLastBuy[hudstat - COMMODITY_PLASMA] = CAP_AVG[hudstat - COMMODITY_PLASMA];
    } else {
      comLog(COM_BELOW_PLANET_HUD);
    }
  } else {
    printBuySell('s', hudstat, quantity, quantity * price, 0);
    *item -= quantity;
    HS_CREDITS += quantity * price;
    if (*item == 0) {
      currentPlayer->commodityLastBuy[hudstat - COMMODITY_PLASMA] = CAP_AVG[hudstat - COMMODITY_PLASMA];
    }
  }

  return 1;
}


int tryToBuy(int quantity, int price, int hudstat) {
  Debug.log(DEBUG_DEFAULT, "int tryToBuy(%d, %d, %d)", quantity, price, hudstat);
  int capacity = hudstatToCapacity(hudstat);
  int nBought = 0;
  int cost = 0;
  char buymax;
  int *item = hudstatToQuantity(hudstat);

  if (HS_CREDITS >= quantity * price && checkCapacity(hudstat, quantity, HS_SHIP_TYPE)) {
    *item += quantity;
    HS_CREDITS -= quantity * price;
    if (hudstat >= COMMODITY_PLASMA && hudstat <= COMMODITY_URANIUM) {
      HS_LASTBUY[hudstat - COMMODITY_PLASMA] = price;
    }
    printBuySell('b', hudstat, quantity, price, 0);
    return 1;
  } else {
    comLog(COM_BELOW_PLANET_HUD);
    printLinesAt(COM_BELOW_PLANET_HUD_START, "Insufficient credits or space.\n"
    "Do you want to buy the max ammount (y/n)?");
    buymax = getch_arrow();
    if (buymax == 'y') {
      if (HS_CREDITS / price >= capacity - *item) {
        nBought = capacity - *item;
      } else {
        nBought = HS_CREDITS / price;
      }
      cost = price;
      printBuySell('b', hudstat, nBought, cost, 0);
      *item += nBought;
      HS_CREDITS -= cost * nBought;
      if (hudstat >= COMMODITY_PLASMA && hudstat <= COMMODITY_URANIUM) {
        HS_LASTBUY[hudstat - COMMODITY_PLASMA] = price;
      }
      return 1;
    } else {
      comLog(COM_BELOW_PLANET_HUD);
      return 0;
    }
  }//buymax

  return 0;
}


int checkCapacity(int hudstat, int quantity, int ship) {
  Debug.log(DEBUG_DEFAULT, "int checkCapacity(%d, %d, %d)", hudstat, quantity, ship);
  int capacity = hudstatToCapacity(hudstat);

  switch(hudstat) {
    case COMMODITY_SHIELD:
    case COMMODITY_HULL:
    case COMMODITY_ENERGY:
    case COMMODITY_TORPEDOS:
      if (quantity + *hudstatToQuantity(hudstat) < capacity) {
        return 1;
      }
      break;
    case COMMODITY_PLASMA:
    case COMMODITY_DUTERIUM:
    case COMMODITY_LATINUM:
    case COMMODITY_WATER:
    case COMMODITY_URANIUM:
      if (quantity + HS_CARGO <= CAP_CARGO + 100 * HS_UP_CARGO) {
        return 1;
      }
      break;
    default:
      return 0;
  }

  return 0;
}


static char* printBuySell(char buySell, int commodity, int quantity, int costProfit, int shipCost) {
  Debug.log(DEBUG_DEFAULT, "static char* printBuySell(%c, %d, %d, %d, %d)", 
  buySell, commodity, quantity, costProfit, shipCost);
  char unit;
  char desc[128] = {0};
  char item[128] = {0};
  char profit[128] = {0};
  static char buffer[BUFFER_MEDIUM] = {0};

  switch (commodity) {
    case COMMODITY_PLASMA:
    case COMMODITY_WATER:
      unit = 'L';
    break;
    case COMMODITY_DUTERIUM:
    case COMMODITY_LATINUM:
    case COMMODITY_URANIUM:
      unit = 'g';
    break;
    case COMMODITY_HULL:
    case COMMODITY_ENERGY:
    case COMMODITY_TORPEDOS:
    default:
      unit = 'n';
    break;
  }

  if (commodity == COMMODITY_WEAPONS && buySell == 'b') {
    snprintf(buffer, BUFFER_MEDIUM - 1, "You bought a class %i cannon for %s credits.",
    quantity, prefix(costProfit, 'n'));
    return buffer;
  }
  if (commodity == COMMODITY_WEAPONS && buySell == 's') {
    snprintf(buffer, BUFFER_MEDIUM - 1, "You sold your class %i cannon for %s credits.",
    quantity, prefix(costProfit, 'n'));
    return buffer;
  }
  if (commodity == COMMODITY_SHIP_TYPE) {
    snprintf(buffer, BUFFER_MEDIUM, "You sold your %s and bought a %s for %s credits.",
    Ship::shipName(quantity), Ship::shipName(costProfit), prefix(shipCost, 'n'));
    return buffer;
  }

  if (buySell == 's' && quantity != 0) {
    snprintf(desc, 127, "You sold %s", prefix(quantity, unit));
  }
  if (buySell == 's' && quantity == 0) {
    snprintf(desc, 127, "You did not sell any ");
  }
  if (buySell == 'b' && quantity != 0) {
    snprintf(desc, 127, "You bought %s", prefix(quantity, unit));
  }
  if (buySell == 'b' && quantity == 0) {
    snprintf(desc, 127, "You did not purchase any ");
  }
  if (commodity == COMMODITY_PLASMA) {
    if (quantity != 0) {
      snprintf(item, 127, " of Warp Plasma.");
    } else {
      snprintf(item, 127, "Warp Plasma");
    }
  }
  if (commodity == COMMODITY_DUTERIUM) {
    if (quantity != 0) {
      snprintf(item, 127, " of Duterium.");
    } else {
      snprintf(item, 127, "Duterium");
    }
  }
  if (commodity == COMMODITY_LATINUM) {
    if (quantity != 0) {
      snprintf(item, 127, " of Latinum.");
    } else {
      snprintf(item, 127, "Latinum");
    }
  }
  if (commodity == COMMODITY_WATER) {
    if (quantity != 0) {
      snprintf(item, 127, " of Water.");
    } else {
      snprintf(item, 127, "Water");
    }
  }
  if (commodity == COMMODITY_URANIUM) {
    if (quantity != 0) {
      snprintf(item, 127, " of Uranium Ore.");
    } else {
      snprintf(item, 127, "Uranium Ore");
    }
  }
  if (commodity == COMMODITY_HULL) {
    if (quantity != 0) {
      snprintf(item, 127, " hours of hull repairs.");
      HS_DATE += quantity / 24;
    } else {
      snprintf(item, 127, "hull repairs.");
    }
  }
  if (commodity == COMMODITY_ENERGY) {
    if (quantity != 0) {
      snprintf(item, 127, " hours of energy cell replacements.");
      HS_DATE += quantity / 24;
    } else {
      if (commodity == COMMODITY_ENERGY && quantity == 0) {
        snprintf(item, 127, "energy cell replacements.");
      }
    }
  }
  if (commodity == COMMODITY_TORPEDOS) {
    if (quantity != 0) {
      snprintf(item, 127, " torpedos.");
    } else {
      snprintf(item, 127, "torpedos.");
    }
  }

  if (buySell == 's' && quantity != 0) {
    snprintf(profit, 127, "\nYou made %s credits.", prefix(costProfit, 'n'));
  }
  if (buySell == 'b' && quantity != 0) {
    snprintf(profit, 127, "\nYou paid %s credits.",
    prefix(quantity * costProfit, 'n'));
  }
  snprintf(buffer, 1027, "%s%s%s", desc, item, profit);

  comLog(COM_BELOW_PLANET_HUD);
  printLinesAt(COM_BELOW_PLANET_HUD_START, buffer);

  return buffer;
}


inline int fExists(char *f) {
  return (access(f, F_OK) != -1);
}
