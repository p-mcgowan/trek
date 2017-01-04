#define MENU_I_SIZE 100
#define BUY  1
#define SELL 0

typedef struct Planet {
  Menu *mainMenu;
  Menu *commMenu;
  Menu *outfitMenu;
  Menu *repairMenu;
  Menu *shipMenu;
  Menu *supplyMenu;
  Menu *upgradeMenu;
  Menu *buyMenu;
  Menu *sellMenu;
  // faction based
  char name[32];
  int position;
  int initialized;
  // streamline
  int commerceArray[MENU_I_SIZE];
  int shipyardArray[MENU_I_SIZE];
  int outfitArray[MENU_I_SIZE];

  static Planet *newPlanet(int pos) {
    Debug.log(DEBUG_DEFAULT, "Planet Planet::Planet(%d)", pos);
    Planet *p = (Planet*)malloc(sizeof(Planet));
    p->mainMenu = Menu::newMenu(PLANET_MENU_START);
    p->commMenu = Menu::newMenu(PLANET_MENU_START);
    p->outfitMenu = Menu::newMenu(PLANET_MENU_START);
    p->repairMenu = Menu::newMenu(PLANET_MENU_START);
    p->shipMenu = Menu::newMenu(PLANET_MENU_START);
    p->supplyMenu = Menu::newMenu(PLANET_MENU_START);
    p->upgradeMenu = Menu::newMenu(PLANET_MENU_START);
    p->buyMenu = Menu::newMenu(PLANET_MENU_START);
    p->sellMenu = Menu::newMenu(PLANET_MENU_START);
    memset(p->commerceArray, 0, MENU_I_SIZE * sizeof(int));
    memset(p->shipyardArray, 0, MENU_I_SIZE * sizeof(int));
    memset(p->outfitArray, 0, MENU_I_SIZE * sizeof(int));
    snprintf(p->name, 32, "%s", "Hello world");
    p->position = pos;
    p->initialized = 0;
    return p;
  }


  // Called on docking if uninitialized
  //
  void initialize() {
    Debug.log(DEBUG_DEFAULT, "void Planet::initialize()");
    if (initialized) {
      rebuildCommerceMenus();
      return;
    }

    int i = 1;

    // Build menus
    mainMenu->addOption("0: Undock");
    if (initShips()) {
      mainMenu->addOption("%d: Shipyard", i++);
      buildShipMenu();
    }
    if (initOutfit()) {
      mainMenu->addOption("%d: Outfitter", i++);
      buildOutfitterMenu();
    }
    if (initCommerce()) {
      mainMenu->addOption("%d: Commercial Sector", i++);
      buildCommerceMenu();
    }
    mainMenu->addOption("%d: Bar", i++);

    ACHIEVE_PLANETS++;
    initialized = 1;
    Debug.log(DEBUG_DEFAULT, "Planet initialized: %d", initialized);
  }


  static Planet *load(FILE *fp) {
    Debug.log(DEBUG_DEFAULT, "static Planet* Planet::load(%d)", fp);
    Planet *p = newPlanet(0);
    fread(p, sizeof(Planet), 1, fp);
    p->mainMenu = Menu::load(fp);
    p->commMenu = Menu::load(fp);
    p->outfitMenu = Menu::load(fp);
    p->repairMenu = Menu::load(fp);
    p->shipMenu = Menu::load(fp);
    p->supplyMenu = Menu::load(fp);
    p->upgradeMenu = Menu::load(fp);
    p->buyMenu = Menu::load(fp);
    p->sellMenu = Menu::load(fp);
    return p;
  }


  int save(FILE *fp) {
    Debug.log(DEBUG_DEFAULT, "int Planet::save(%d)", fp);
    fwrite(this, sizeof(Planet), 1, fp);
    mainMenu->save(fp);
    commMenu->save(fp);
    outfitMenu->save(fp);
    repairMenu->save(fp);
    shipMenu->save(fp);
    supplyMenu->save(fp);
    upgradeMenu->save(fp);
    buyMenu->save(fp);
    sellMenu->save(fp);
    return 1;
  }

  static void unload(Planet *p) {
    Debug.log(DEBUG_DEFAULT, "static void Planet::unload(%d)", p);
    if (p == NULL) {
      return;
    }
    Menu::unload(p->mainMenu);
    Menu::unload(p->commMenu);
    Menu::unload(p->outfitMenu);
    Menu::unload(p->repairMenu);
    Menu::unload(p->shipMenu);
    Menu::unload(p->supplyMenu);
    Menu::unload(p->upgradeMenu);
    Menu::unload(p->buyMenu);
    Menu::unload(p->sellMenu);
    free(p);
  }

  void planetHUD() {
    Debug.log(DEBUG_DEFAULT, "void Planet::planetHUD()");
    clrs();
    planetWindow();
    minimalHUD(HUD_MINIMAL_START);
    comLog(COM_BELOW_PLANET_HUD);
  }

  void dock() {
    Debug.log(DEBUG_DEFAULT, "void Planet::dock(init = %d)", initialized);
    initialize();

    do {
      planetHUD();
      if (HS_PASSENGERS != 0) {
        int reward = HS_PASSENGERS * (8000 + randomNumber(4000));
        comLog(COM_BELOW_PLANET_HUD); //66
        printf("Your passenger");
        if (HS_PASSENGERS != 1) {
          printf("s");
        }
        printf(" rewarded you with %s credits.", prefix(reward, 'n'));
        HS_CREDITS += reward;
        HS_PASSENGERS = 0;
      }
      printAt(PLANET_MENU_HEADER, "%s", "Planetary services:");
      mainMenu->getSelection();

      if (mainMenu->isHighlighted("Shipyard")) {
        shipyardMenu();
      } else if (mainMenu->isHighlighted("Outfitter")) {
        outfitterMenu();
      } else if (mainMenu->isHighlighted("Commercial")) {
        commerceMenu();
      } else if (mainMenu->isHighlighted("Bar")) {
        barMenus();
      } else {
        clrs();
        HS_DATE++;
        return;
      }
    } while (1);
    currentPlayer->save();
  }

  int initOutfit() {
    Debug.log(DEBUG_DEFAULT, "int Planet::initOutfit()");
    int i = 0;
    if (percentile() > 20) {
      outfitArray[0] = 1;
      i = 1;
    }
    outfitArray[1] = 5 + randomNumber(10);
    outfitArray[2] = 10 + randomNumber(10);

    if (percentile() > 50) {
      outfitArray[3] = 1;
      i = 1;
    }
    outfitArray[4] = 900 + randomNumber(200); // torps
    outfitArray[5] = 80000 + randomNumber(20000); // cannon 1
    outfitArray[6] = 100000 + randomNumber(100000); // cannon 2
    outfitArray[7] = 5000000 + randomNumber(5000000); // cannon 3

    if (percentile() > 70) {
      outfitArray[8] = 1;
      i = 1;
    }

    return i;
  }

  int initShips() {
    Debug.log(DEBUG_DEFAULT, "int Planet::initShips()");
    int i = 0;
    if (percentile() > 20) {
      shipyardArray[0] = 1;
      shipyardArray[1] = 10000 + randomNumber(1000);
      i = 1;
    }
    if (percentile() > 40) {
      shipyardArray[2] = 1;
      shipyardArray[3] = 100000 + randomNumber(10000);
      i = 1;
    }
    if (percentile() > 60) {
      shipyardArray[4] = 1;
      shipyardArray[5] = 1000000 + randomNumber(100000);
      i = 1;
    }
    if (percentile() > 80) {
      shipyardArray[6] = 1;
      shipyardArray[7] = 10000000 + randomNumber(10000000);
      i = 1;
    }
    if (percentile() > 90) {
      shipyardArray[8] = 1;
      shipyardArray[9] = 100000000 + randomNumber(100000000);
      i = 1;
    }

    return i;
  }

  int buildShipMenu() {
    Debug.log(DEBUG_DEFAULT, "int Planet::buildShipMenu()");
    int i = 1;
    shipMenu->addOption("0: Back");
    if (shipyardArray[0] == 1) {
      shipMenu->addOption("%i: NXC-01: P-Class Shuttle", i++);
      shipMenu->indices[i - 1] = 1;
    }
    if (shipyardArray[2] == 1) {
      shipMenu->addOption("%i: Expanse 5060: F-Class Cargo Drone", i++);
      shipMenu->indices[i - 1] = 2;
    }
    if (shipyardArray[4] == 1) {
      shipMenu->addOption("%i: Axess DN-5: F-Class Freighter", i++);
      shipMenu->indices[i - 1] = 3;
    }
    if (shipyardArray[6] == 1) {
      shipMenu->addOption("%i: Green Goose FX-1: C-Class Frigate", i++);
      shipMenu->indices[i - 1] = 4;
    }
    if (shipyardArray[8] == 1) {
      shipMenu->addOption("%i: Imperium Industry FT9: C-Class Destroyer", i++);
      shipMenu->indices[i - 1] = 5;
    }

    return i - 1;
  }


  void shipyardMenu() {
    Debug.log(DEBUG_DEFAULT, "void Planet::shipyardMenu()");
    int choice;

    planetHUD();
    printAt(PLANET_MENU_HEADER, "%s", "Ships avaliable at this location:");
    do {
      choice = shipMenu->getInput();
      planetHUD();
      printAt(PLANET_MENU_HEADER, "%s", "Ships avaliable at this location:");

      if (shipMenu->highlighted > 0 && shipMenu->highlighted <= shipMenu->length - 1) {
        shipInfo(shipMenu->indices[shipMenu->highlighted]); // 1, 3, 5, 7, 9
      }
      if (choice == KEY_RETURN && !shipMenu->isHighlighted("Back")) {
        comLog(COM_BELOW_PLANET_HUD); //66
        printf("You entered %d", shipMenu->highlighted);
        /*
        comLog(COM_BELOW_PLANET_HUD); //66
        if (submenu == 1) {
          if (HS_CREDITS + resaleValue() < shipyardArray[ship * 2 - 1]) {
            comLog(COM_BELOW_PLANET_HUD); //66
            printf("Insufficient credits.");
          } else {
            cost = (shipyardArray[ship * 2 - 1] - resaleValue() > 0) ?
            shipyardArray[ship * 2 - 1] - resaleValue() : 0;

            printBuySell('b', 's', HS_SHIP_TYPE, ship, cost);
            HS_CREDITS -= cost;
            HS_SHIP_TYPE = ship;
            HS_SHIELD = capacityArray[10 + HS_SHIP_TYPE];
            HS_HULL = capacityArray[20 + HS_SHIP_TYPE];
            HS_ENERGY = capacityArray[30 + HS_SHIP_TYPE];
            HS_TORPEDOS = 0;
            for (i = HS_I_WEAPON1; i <= HS_I_UP_TORP; i++) {
              hudstats[i] = 0;
            }
            ACHIEVE_SHIPS++;
          }
        }*/
      }
    } while (!((choice == KEY_RETURN && shipMenu->isHighlighted("Back")) ||
    choice == 0));
  }


  int buildOutfitterMenu() {
    Debug.log(DEBUG_DEFAULT, "int Planet::buildOutfitterMenu(menus)");
    int i = 1;
    int n = outfitArray[0] + outfitArray[3] + outfitArray[8];
    outfitMenu->addOption("0: Back");

    if (n > 0) {
      if (outfitArray[0] == 1) {
        outfitMenu->addOption("%d: Repairs", i);
        i++;
        repairMenu->addOption("0: Back");
        repairMenu->addOption("1: Hull Repairs (%i/hour)",
        outfitArray[1] * HS_SHIP_TYPE);
        repairMenu->addOption("2: Power Cell Replacement (%i/hour)",
        outfitArray[2] * HS_SHIP_TYPE);
      }
      if (outfitArray[3] == 1) {
        outfitMenu->addOption("%d: Supplies", i);
        i++;
        supplyMenu->addOption("0: Back");
        supplyMenu->addOption("1: Buy Torpedos (%i/unit)", outfitArray[4]);
        supplyMenu->addOption("2: Sell Torpedos (%i/unit)", outfitArray[4]);
        supplyMenu->addOption("3: Buy Weapons");
        supplyMenu->addOption("4: Sell Weapons");
      }
      if (outfitArray[8] == 1) {
        outfitMenu->addOption("%d: Upgrades", i);
        upgradeMenu->addOption("0: Back");
        upgradeMenu->addOption("1: Ammunition Upgrade");
        upgradeMenu->addOption("2: Cannon Barrel Upgrade");
        upgradeMenu->addOption("3: Advanced Hull Plating Upgrade");
        upgradeMenu->addOption("4: Oscillating Shield Upgrade");
        upgradeMenu->addOption("5: Cargo Refit");
        upgradeMenu->addOption("6: Advanced Torpedo Upgrade");
        upgradeMenu->addOption("7: Port and Polish Engine Upgrade");
        i++;
      }
    }

    return n;
  }


  void outfitterMenu() {
    Debug.log(DEBUG_DEFAULT, "void Planet::outfitterMenu()");
    int quantity = 0, choice;

    do {
      planetHUD();
      printAt(PLANET_MENU_HEADER, "%s", "Oufitter services at this location:");
      outfitMenu->getSelection();

      if (outfitMenu->isHighlighted("Repair")) {
        do {
          planetHUD();
          printAt(PLANET_MENU_HEADER, "%s", "Repairs:");
          repairMenu->getSelection();

          if (repairMenu->isHighlighted("Hull")) {
            comLog(COM_BELOW_PLANET_HUD);
            printf("Hours: ");
            scanf("%i", &quantity);
            clean();
            tryToBuy(quantity, outfitArray[1] * HS_SHIP_TYPE, COMMODITY_HULL);
          }
          if (repairMenu->isHighlighted("Power")) {
            comLog(COM_BELOW_PLANET_HUD);
            printf("Hours:");
            scanf("%i", &quantity);
            clean();
            tryToBuy(quantity, outfitArray[2] * HS_SHIP_TYPE, COMMODITY_SHIELD);
          }
        } while (!repairMenu->isHighlighted("Back"));
      } else if (outfitMenu->isHighlighted("Supplies")) {
        do {
          planetHUD();
          printAt(PLANET_MENU_HEADER, "%s", "Resupply:");
          supplyMenu->getSelection();

          if (supplyMenu->isHighlighted("Buy Torpedos")) {
            comLog(COM_BELOW_PLANET_HUD);
            printf("Torpedo quantity to buy: ");
            scanf("%i", &quantity);
            clean();
            tryToBuy(quantity, outfitArray[4] * HS_SHIP_TYPE, COMMODITY_TORPEDOS);
          }
          if (supplyMenu->isHighlighted("Sell Torpedos")) {
            gotoXY(3, 27);
            printf("Torpedo quantity to sell: ");
            scanf("%i", &quantity);
            clean();
            tryToSell(quantity, outfitArray[4], COMMODITY_TORPEDOS);
          }
          if (supplyMenu->isHighlighted("Buy Weapons")) {
            /*do {
              planetHUD();
              printf("Weapons for sale:");
              //wepon menu
              printf("1: Pew-Pew SK-337 Lance (%s)",
              prefix(outfitArray[5], 'n'));
              printf("2: N00bzorz ID 10-T Killer (%s)",
              prefix(outfitArray[6], 'n'));
              printf("3: Pwnzorz 1337 Cannon (%s)", prefix(outfitArray[7], 'n'));
              printf("1-3: Weapon info    0: Back");
              gotoXY(3, 29);
              subsubmenu = getch_arrow() - '0';
              comLog(COM_BELOW_PLANET_HUD); //66
              if (subsubmenu == 1) {
                do {
                  planetHUD();
                  gotoXY(3, 5);
                  printf("Pew-Pew SK-337 Lance    %s",
                  prefix(outfitArray[5], 'n'));
                  gotoXY(3, 7);
                  printf("Damage: 30");
                  gotoXY(3, 8);
                  printf("Range: 30km");
                  gotoXY(3, 9);
                  printf("Energy depletion: 10 MJ");
                  gotoXY(3, 10);
                  printf("Compatibility: All standard regulation federation"
                  " ships.");
                  gotoXY(3, 27);
                  printf("1: Buy  0: Back");
                  gotoXY(3, 28);
                  subsubsubmenu = getch_arrow() - '0';
                  gotoXY(3, 28);
                  printf("  ");
                  *
                  comLog(COM_BELOW_PLANET_HUD); //66
                  if (subsubsubmenu == 1) {
                    if (HS_CREDITS < outfitArray[5] ||
                    HS_NWEAPONS == capacityArray[5 + HS_SHIP_TYPE]) {
                      comLog(COM_BELOW_PLANET_HUD); //66
                      printf("Insufficient funds or space.");
                    } else {
                      cost = outfitArray[5];
                      printBuySell('b', 'g', 1, cost, 0);
                      HS_CREDITS -= cost;
                      for (i = HS_I_WEAPON1; i < HS_I_WEAPON5; i++) {
                        if (hudstats[i] == 0) {
                          hudstats[i] = 1;
                          break;
                        }
                      }
                      hudstats[i] = 1;
                      HS_NWEAPONS++;
                      ACHIEVE_WEAPONS++;
                    }
                  }*
                  if (subsubsubmenu != 0 && subsubsubmenu != 1) {
                    comLog(COM_BELOW_PLANET_HUD); //66
                    printf("Invalid selection.");
                  }
                } while (subsubsubmenu != 0);
                comLog(COM_BELOW_PLANET_HUD); //66
              }
              if (subsubmenu == 2) {
                do {
                  planetHUD();
                  gotoXY(3, 5);
                  printf("N00bzorz ID10-T Killer    %s",
                  prefix(outfitArray[6], 'n'));
                  gotoXY(3, 7);
                  printf("Damage: 50");
                  gotoXY(3, 8);
                  printf("Range: 50km");
                  gotoXY(3, 9);
                  printf("Energy depletion: 50 MJ");
                  gotoXY(3, 10);
                  printf("Compatibility: F or C class federation ships.");
                  gotoXY(3, 27);
                  printf("1: Buy  0: Back");
                  gotoXY(3, 28);
                  subsubsubmenu = getch_arrow() - '0';
                  gotoXY(3, 28);
                  printf("  ");
                  *
                  comLog(COM_BELOW_PLANET_HUD); //66
                  if (subsubsubmenu == 1) {
                    // TODO split for better errors
                    if (HS_CREDITS < outfitArray[6] || HS_NWEAPONS ==
                    capacityArray[5 + HS_SHIP_TYPE] || HS_SHIP_TYPE == 1) {
                      comLog(COM_BELOW_PLANET_HUD); //66
                      printf("Insufficient funds, space, or compatibility.");
                    } else {
                      cost = outfitArray[6];
                      printBuySell('b', 'g', 2, cost, 0);
                      HS_CREDITS -= cost;
                      for (i = HS_WEAPON1; i < HS_WEAPON5; i++) {
                        if (hudstats[i] == 0) {
                          hudstats[i] = 2;
                          break;
                        }
                      }
                      HS_NWEAPONS++;
                      ACHIEVE_WEAPONS++;
                    }
                  }*
                  if (subsubsubmenu != 0 && subsubsubmenu != 1) {
                    comLog(COM_BELOW_PLANET_HUD); //66
                    printf("Invalid selection.");
                  }
                } while (subsubsubmenu != 0);
                comLog(COM_BELOW_PLANET_HUD); //66
              }
              if (subsubmenu == 3) {
                do {
                  planetHUD();
                  gotoXY(3, 5);
                  printf("Pwnzorz 1337 Cannon    %s",
                  prefix(outfitArray[7], 'n'));
                  gotoXY(3, 7);
                  printf("Damage: 100");
                  gotoXY(3, 8);
                  printf("Range: 70km");
                  gotoXY(3, 9);
                  printf("Energy depletion: 100 MJ");
                  gotoXY(3, 10);
                  printf("Compatibility: C class federation ships.");
                  gotoXY(3, 27);
                  printf("1: Buy  0: Back");
                  gotoXY(3, 28);
                  subsubsubmenu = getch_arrow() - '0';
                  gotoXY(3, 28);
                  printf("  ");
                  *
                  comLog(COM_BELOW_PLANET_HUD); //66
                  if (subsubsubmenu == 1) {
                    if (HS_CREDITS < outfitArray[7] ||
                    HS_NWEAPONS == capacityArray[5 + HS_SHIP_TYPE] ||
                    HS_SHIP_TYPE < 4) {
                      comLog(COM_BELOW_PLANET_HUD); //66
                      printf("Insufficient funds, space, or compatibility.");
                    } else {
                      cost = outfitArray[7];
                      printBuySell('b', 'g', 3, cost, 0);
                      HS_CREDITS -= cost;
                      for (i = 1; i < 6; i++) {
                        if (hudstats[HS_I_ENEMY_LOC + i] == 0) {
                          hudstats[HS_I_ENEMY_LOC + i] = 3;
                          break;
                        }
                      }
                      HS_NWEAPONS++;
                      ACHIEVE_WEAPONS++;
                    }
                  }*
                  if (subsubsubmenu != 0 && subsubsubmenu != 1) {
                    comLog(COM_BELOW_PLANET_HUD); //66
                    printf("Invalid selection.");
                  }
                } while (subsubsubmenu != 0);
                comLog(COM_BELOW_PLANET_HUD); //66
              }
              if (subsubmenu != 1 && subsubmenu != 2 && subsubmenu != 3) {
                gotoXY(67, 27);
                printf("Invalid selection.");
              }
            } while (subsubmenu != 0);
            */
          }
          if (supplyMenu->isHighlighted("Sell weapons")) {
  //        do {
  //          planetHUD();
  //          gotoXY(3, 5);
  //          printf("Your Weapons:");
  //          gotoXY(3, 7);
  //          if (HS_WEAPON1 != 0) {
  //            printf("1: Sell Type %i Weapon in slot 1 (%s)", HS_WEAPON1,
  //            prefix(outfitArray[4 + HS_WEAPON1] / 2, 'n'));
  //          } else {
  //             printf("No weapon in slot 1");
  //          }
  //          gotoXY(3, 8);
  //          if (HS_WEAPON2 != 0) {
  //            printf("2: Sell Type %i Weapon in slot 2 (%s)", HS_WEAPON2,
  //            prefix(outfitArray[4 + HS_WEAPON2] / 2, 'n'));
  //          } else {
  //             printf("No weapon in slot 2");
  //          }
  //          gotoXY(3, 9);
  //          if (HS_WEAPON3 != 0) {
  //            printf("3: Sell Type %i Weapon in slot 2 (%s)", HS_WEAPON3,
  //            prefix(outfitArray[4 + HS_WEAPON3] / 2, 'n'));
  //          } else {
  //             printf("No weapon in slot 3");
  //          }
  //          gotoXY(3, 10);
  //          if (HS_WEAPON4 != 0) {
  //            printf("4: Sell Type %i Weapon in slot 2 (%s)", HS_WEAPON4,
  //            prefix(outfitArray[4 + HS_WEAPON4] / 2, 'n'));
  //          } else {
  //             printf("No weapon in slot 4");
  //          }
  //          gotoXY(3, 11);
  //          if (HS_WEAPON5 != 0) {
  //            printf("5: Sell Type %i Weapon in slot 2 (%s)", HS_WEAPON5,
  //            prefix(outfitArray[4 + HS_WEAPON5] / 2, 'n'));
  //          } else {
  //             printf("No weapon in slot 5");
  //          }
  //          gotoXY(3, 13);
  //          printf("0: Back");
  //          gotoXY(3, 27);
  //          printf("1-5: Sell weapon in slot    0: Back");
  //          gotoXY(3, 29);
  //          subsubmenu = getch_arrow() - '0';
  //          gotoXY(3, 27);
  //          comLog(COM_BELOW_PLANET_HUD); //66
  //          if (subsubmenu > 0 && subsubmenu < 6 &&
  //          hudstats[HS_I_ENEMY_LOC + subsubmenu] == 0) {
  //            comLog(COM_BELOW_PLANET_HUD); //66
  //            printf("There is no weapon in that slot.");
  //          }
  //          if (subsubmenu > 0 && subsubmenu < 6 &&
  //          hudstats[HS_I_ENEMY_LOC + subsubmenu] != 0) {
  //            cost = outfitArray[4 + hudstats[HS_I_ENEMY_LOC + subsubmenu]] / 2;
  //            printBuySell('s', HS_I_WEAPONS,
  //            hudstats[HS_I_ENEMY_LOC + subsubmenu],
  //            cost, 0);
  //            HS_CREDITS += cost;
  //            hudstats[HS_I_ENEMY_LOC + subsubmenu] = 0;
  //            HS_NWEAPONS--;
  //          }
  //          if (subsubmenu < 0 || subsubmenu > 5) {
  //            comLog(COM_BELOW_PLANET_HUD); //66
  //            printf("Invalid selection.");
  //          }
  //        } while (subsubmenu != 0);
          }
        } while (!supplyMenu->isHighlighted("Back"));
      } else if (outfitMenu->isHighlighted("Upgrade")) {
        planetHUD();
        printAt(PLANET_MENU_HEADER, "Upgrades:");
        do {
          choice = upgradeMenu->getInput();
          planetHUD();
          printAt(PLANET_MENU_HEADER, "Upgrades:");
          if (upgradeMenu->highlighted > 0 && upgradeMenu->highlighted <= 7) {
            Upgrade upgrade = UPGRADES[upgradeMenu->highlighted - 1];
            printLinesAt(3, 14, upgrade.description);
            printf("%i", currentPlayer->ship->upgrades[upgrade.hudstat]);
            if (choice == KEY_RETURN) {
              comLog(COM_BELOW_PLANET_HUD); //66
              if (HS_CREDITS < upgrade.cost) {
                printf("Insufficient credits.");
              } else if (currentPlayer->ship->upgrades[upgrade.hudstat] >= upgrade.max) {
                printf("There is no more room for this upgrade.");
              } else if (HS_SHIP_TYPE < upgrade.compatibility) {
                printf("Upgrade incompatible with ship architecture.");
              } else {
                printf("You bought %s for %s credits.", upgrade.name,
                prefix(upgrade.cost, 'n'));
                HS_CREDITS -= upgrade.cost;
                currentPlayer->ship->upgrades[upgrade.hudstat]++;
                printLinesAt(3, 14, upgrade.description);
                printf("%i", currentPlayer->ship->upgrades[upgrade.hudstat]++);
              }
            }
          }
        } while (!((choice == KEY_RETURN && upgradeMenu->isHighlighted("Back")) ||
        choice == 0));
      }
    } while (!outfitMenu->isHighlighted("Back"));
    planetHUD();
  }


  /**
   * Dynamic commodity preview.
   * Returns last y coord printed at.
   */
  int commodityPreview(int x, int y, int buying) {
    Debug.log(DEBUG_DEFAULT, "int Planet::commodityPreview(%d %d, %d)", x, y, buying);

    if (buying) {
      if (commerceArray[0] == 1) {
        y = printAt(x, y, "Warp Plasma %i/L %s (%d)", commerceArray[1],
        printDealArrow(commerceArray[1], CAP_AVGPLASMA, 1), HS_CREDITS / commerceArray[1]);
      }
      if (commerceArray[2] == 1) {
        y = printAt(x, y, "Duterium %i/kg %s (%d)", commerceArray[3],
        printDealArrow(commerceArray[3], CAP_AVGDUTERIUM, 1), HS_CREDITS / commerceArray[3]);
      }
      if (commerceArray[4] == 1) {
        y = printAt(x, y, "Latinum %i/kg %s (%d)", commerceArray[5],
        printDealArrow(commerceArray[5], CAP_AVGLATINUM, 1), HS_CREDITS / commerceArray[5]);
      }
      if (commerceArray[6] == 1) {
        y = printAt(x, y, "Water %i/L %s (%d)", commerceArray[7],
        printDealArrow(commerceArray[7], CAP_AVGWATER, 1), HS_CREDITS / commerceArray[7]);
      }
      if (commerceArray[8] == 1) {
        y = printAt(x, y, "Uranium Ore %i/kg %s (%d)", commerceArray[9],
        printDealArrow(commerceArray[9], CAP_AVGURANIUM, 1), HS_CREDITS / commerceArray[9]);
      }
    } else {
      if (HS_PLASMA > 0) {
        y = printAt(x, y, "Warp Plasma %i/L %s (%d)", commerceArray[1],
        printDealArrow(commerceArray[1], LASTBUY_PLASMA, 0), HS_PLASMA);
      }
      if (HS_DUTERIUM > 0) {
        y = printAt(x, y, "Duterium %i/kg %s (%d)", commerceArray[3],
        printDealArrow(commerceArray[3], LASTBUY_DUTERIUM, 0), HS_DUTERIUM);
      }
      if (HS_LATINUM > 0) {
        y = printAt(x, y, "Latinum %i/kg %s (%d)", commerceArray[5],
        printDealArrow(commerceArray[5], LASTBUY_LATINUM, 0), HS_LATINUM);
      }
      if (HS_WATER > 0) {
        y = printAt(x, y, "Water %i/L %s (%d)", commerceArray[7],
        printDealArrow(commerceArray[7], LASTBUY_WATER, 0), HS_WATER);
      }
      if (HS_URANIUM > 0) {
        y = printAt(x, y, "Uranium Ore %i/kg %s (%d)", commerceArray[9],
        printDealArrow(commerceArray[9], LASTBUY_URANIUM, 0), HS_URANIUM);
      }
    }

    return y;
  }


  /**
   *
   */
  const char* printDealArrow(int buySellPrice, int lastBuy, int buying) {
    Debug.log(DEBUG_DEFAULT,
    "const char* Planet::printDealArrow(%d, %d, %d)", buySellPrice, lastBuy, buying);
    int cost, comparator;

    if (buying) {
      cost = buySellPrice;
      comparator = lastBuy;
    } else {
      cost = lastBuy;
      comparator = buySellPrice;
    }

    if (cost > comparator) {
      if (buying) {
        return STR_COL_RED ">" STR_COL_WHITE;
      } else {
        return STR_COL_RED "<" STR_COL_WHITE;
      }
    }
    if (cost < comparator) {
      if (buying) {
        return STR_COL_GREEN "<" STR_COL_WHITE;
      } else {
        return STR_COL_GREEN ">" STR_COL_WHITE;
      }
    }
    return STR_COL_YELLOW "=" STR_COL_WHITE;
  }


  void buildCommerceMenu() {
    Debug.log(DEBUG_DEFAULT, "void Planet::buildCommerceMenu(menus)");
    int i, j = 1;
    Menu::unload(commMenu);
    Menu::unload(buyMenu);
    Menu::unload(sellMenu);
    commMenu = Menu::newMenu(PLANET_MENU_START);
    buyMenu = Menu::newMenu(PLANET_MENU_START);
    sellMenu = Menu::newMenu(PLANET_MENU_START);
    commMenu->addOption("0: Back");
    buyMenu->addOption("0: Back");
    sellMenu->addOption("0: Back");

    i = 1;
    if (commerceArray[0] == 1) {
      buyMenu->addOption("%i: Warp Plasma %i/L %s (%d)", i++, commerceArray[1],
      printDealArrow(commerceArray[1], CAP_AVGPLASMA, BUY), HS_CREDITS / commerceArray[1]);
    }
    if (commerceArray[2] == 1) {
      buyMenu->addOption("%i: Duterium %i/kg %s (%d) ", i++, commerceArray[3],
      printDealArrow(commerceArray[3], CAP_AVGDUTERIUM, BUY), HS_CREDITS / commerceArray[3]);
    }
    if (commerceArray[4] == 1) {
      buyMenu->addOption("%i: Latinum %i/kg %s (%d)", i++, commerceArray[5],
      printDealArrow(commerceArray[5], CAP_AVGLATINUM, BUY), HS_CREDITS / commerceArray[5]);
    }
    if (commerceArray[6] == 1) {
      buyMenu->addOption("%i: Water %i/L %s (%d)", i++, commerceArray[7],
      printDealArrow(commerceArray[7], CAP_AVGWATER, BUY), HS_CREDITS / commerceArray[7]);
    }
    if (commerceArray[8] == 1) {
      buyMenu->addOption("%i: Uranium Ore %i/kg (%d)", i++, commerceArray[9],
      printDealArrow(commerceArray[9], CAP_AVGURANIUM, BUY), HS_CREDITS / commerceArray[9]);
    }
    if (i > 1) {
      commMenu->addOption("%d: Buy", j++);
    }

    i = 1;
    if (HS_PLASMA > 0) {
      sellMenu->addOption("%i: Warp Plasma %i/L %s (%d)", i++, commerceArray[1],
      printDealArrow(commerceArray[1], LASTBUY_PLASMA, SELL), HS_PLASMA);
    }
    if (HS_DUTERIUM > 0) {
      sellMenu->addOption("%i: Duterium %i/kg %s (%d)", i++, commerceArray[3],
      printDealArrow(commerceArray[3], LASTBUY_DUTERIUM, SELL), HS_DUTERIUM);
    }
    if (HS_LATINUM > 0) {
      sellMenu->addOption("%i: Latinum %i/kg %s (%d)", i++, commerceArray[5],
      printDealArrow(commerceArray[5], LASTBUY_LATINUM, SELL), HS_LATINUM);
    }
    if (HS_WATER > 0) {
      sellMenu->addOption("%i: Water %i/L %s (%d)", i++, commerceArray[7],
      printDealArrow(commerceArray[7], LASTBUY_WATER, SELL), HS_WATER);
    }
    if (HS_URANIUM > 0) {
      sellMenu->addOption("%i: Uranium Ore %i/kg %s (%d)", i++, commerceArray[9],
      printDealArrow(commerceArray[9], LASTBUY_URANIUM, SELL), HS_URANIUM);
    }
    Debug.log(DEBUG_DEFAULT, "done adding menu items");
    commMenu->addOption("%d: Sell", j++);
  }


  void getMenuCommodity(char *opt, int *price, int *index) {
    Debug.log(DEBUG_DEFAULT, "void Planet::getMenuCommodity(%s, %d, %d)", opt, *price, *index);
    if (strstr(opt, "Plasma")) {
      *price = commerceArray[1];
      *index = COMMODITY_PLASMA;
    } else if (strstr(opt, "Duterium")) {
      *price = commerceArray[3];
      *index = COMMODITY_DUTERIUM;
    } else if (strstr(opt, "Latinum")) {
      *price = commerceArray[5];
      *index = COMMODITY_LATINUM;
    } else if (strstr(opt, "Water")) {
      *price = commerceArray[7];
      *index = COMMODITY_WATER;
    } else if (strstr(opt, "Uranium")) {
      *price = commerceArray[9];
      *index = COMMODITY_URANIUM;
    }
    Debug.log(DEBUG_DEFAULT, "return getMenuCommodity(%s, %d, %d)", opt, *price, *index);

    return;
  }


  void rebuildCommerceMenus() {
    Debug.log(DEBUG_DEFAULT, "void Planet::rebuildCommerceMenus(menus)");
    int m = Math.max(commMenu->highlighted, 0);
    int b = Math.max(buyMenu->highlighted, 0);
    int s = Math.max(sellMenu->highlighted, 0);

    buildCommerceMenu();
    commMenu->highlighted = m;
    buyMenu->highlighted = b;
    sellMenu->highlighted = Math.min(s, sellMenu->length - 1);
  }


  void commerceMenu() {
    Debug.log(DEBUG_DEFAULT, "void Planet::commerceMenu()");
    int choice, price, hudstat;
    double quantity;

    planetHUD();
    printAt(PLANET_MENU_HEADER, "Commercial sector:");
    do {
      choice = commMenu->getInput();
      planetHUD();
      printAt(PLANET_MENU_HEADER, "Commercial sector:");

      if (commMenu->isHighlighted("Buy")) {
        commodityPreview(3, 10, 1);
        if (choice == KEY_RETURN || (choice > -1 &&
        choice == commMenu->optionIndex("Buy"))) {
          planetHUD();
          do {
            planetWindow();
            printAt(PLANET_MENU_HEADER, "Buy commodities:");
            choice = buyMenu->getSelection();

            if (!buyMenu->isHighlighted("Back")) {
              comLog(COM_BELOW_PLANET_HUD);
              printf("Purchase how many units: ");
              scanf("%lf", &quantity);
              clean();
              getMenuCommodity(buyMenu->options[buyMenu->highlighted], &price, &hudstat);
              if (tryToBuy(quantity, price, hudstat)) {
                currentPlayer->commodityLastBuy[hudstat - COMMODITY_PLASMA] = price;
                minimalHUD(HUD_MINIMAL_START);
                rebuildCommerceMenus();
              }
              choice = -1;
            }
          } while (!((buyMenu->isHighlighted("Back") && choice == KEY_RETURN) ||
          choice == 0));
          choice = -1;
          planetHUD();
          printAt(PLANET_MENU_HEADER, "Commercial sector:");
          commodityPreview(3, 10, 1);
        }
      } else if (commMenu->isHighlighted("Sell")) {
        commodityPreview(3, 10, 0);
        if (choice == KEY_RETURN || (choice > -1 &&
        choice == commMenu->optionIndex("Sell"))) {
          planetHUD();
          do {
            planetWindow();
            printAt(PLANET_MENU_HEADER, "Sell commodities:");
            choice = sellMenu->getSelection();

            if (!sellMenu->isHighlighted("Back")) {
              comLog(COM_BELOW_PLANET_HUD);
              printf("Sell how many units: ");
              scanf("%lf", &quantity);
              clean();
              getMenuCommodity(sellMenu->options[sellMenu->highlighted],
              &price, &hudstat);
              if (tryToSell(quantity, price, hudstat)) {
                minimalHUD(HUD_MINIMAL_START);
                rebuildCommerceMenus();
                planetWindow();
                choice = -1;
              }
            }
          } while (!((sellMenu->isHighlighted("Back") && choice == KEY_RETURN) ||
          choice == 0));
          choice = -1;
          planetHUD();
          printAt(PLANET_MENU_HEADER, "Commercial sector:");
          commodityPreview(3, 10, 0);
        }
      }
    } while (!((commMenu->isHighlighted("Back") && choice == KEY_RETURN) || choice == 0));
  }


  // TODO: rebalance - make higher commodities more expensive
  int initCommerce() {
    Debug.log(DEBUG_DEFAULT, "int Planet::initCommerce()");
    int i;
    if (percentile() > 20) {
      commerceArray[0] = 1;
      i = 1;
    }
    commerceArray[1] = 5 + randomNumber(15);
    if (percentile() > 30) {
      commerceArray[2] = 1;
      i = 1;
    }
    commerceArray[3] = 20 + randomNumber(30);
    if (percentile() > 40) {
      commerceArray[4] = 1;
      i = 1;
    }
    commerceArray[5] = 70 + randomNumber(30);
    if (percentile() > 50) {
      commerceArray[6] = 1;
      i = 1;
    }
    commerceArray[7] = 350 + randomNumber(150);
    if (percentile() > 75) {
      commerceArray[8] = 1;
      i = 1;
    }
    commerceArray[9] = 700 + randomNumber(300);

    return i;
  }

  void shipInfo(int ship) {
    Debug.log(DEBUG_DEFAULT, "void Planet::shipInfo(%d)", ship);
    int x = 3, y = 12;
    const char *sfmt = "%-30s%-16s%s";
    const char *ifmt = "%-30s%-16d%d";
    y = printAt(x, y, sfmt, "Ship stats:", Ship::shipName(ship), "Your ship");
    y = printAt(x, y, ifmt, "Shield Power:", Ship::shipCapacity(ship, CAP_I_SHIELD), CAP_SHIELD);
    y = printAt(x, y, ifmt, "Hull Strength:", Ship::shipCapacity(ship, CAP_I_HULL), CAP_HULL);
    y = printAt(x, y, ifmt, "Battery Charge:", Ship::shipCapacity(ship, CAP_I_ENERGY), CAP_ENERGY);
    y = printAt(x, y, ifmt, "Torpedo Bay Capacity:", Ship::shipCapacity(ship, CAP_I_TORP), CAP_TORP);
    y = printAt(x, y, ifmt, "Cargo Hold Capacity:", Ship::shipCapacity(ship, CAP_I_CARGO), CAP_CARGO);
    y = printAt(x, y, ifmt, "Turret Hardpoints:", Ship::shipCapacity(ship, CAP_I_HARDPOINT), CAP_HARDPOINT);
    y = printAt(x, y, "Current Ship Value: %s", prefix(resaleValue(), 'n'));
    y = printAt(x, y, "Cost before resale: %s",
    prefix(shipyardArray[ship * 2 - 1], 'n'));
    y = printAt(x, y, "Total Cost: %s",
    prefix(Math.max(shipyardArray[ship * 2 - 1] - resaleValue(), 0), 'n'));
  }


  int resaleValue() { // torps???????!?!?!?
    Debug.log(DEBUG_DEFAULT, "int Planet::resaleValue()");
    int guns = 0, i, upgrades;
    float shipCondition;
    upgrades = 50000 * (HS_UP_DMG + HS_UP_RANGE) + 125000 *
    (HS_UP_HULL + HS_UP_SHIELD) + 75000 * HS_UP_CARGO + 250000 * HS_UP_TORP;
    shipCondition = (HS_SHIELD + HS_HULL + HS_ENERGY) / (CAP_HULL + CAP_SHIELD + CAP_ENERGY);

    if (ACHIEVE_SHIPS != 1) {
      for (i = 1; i <= 5; i++) {
        guns += (currentPlayer->ship->weapons[i] == 1) ? outfitArray[5] : 0;
        guns += (currentPlayer->ship->weapons[i] == 2) ? outfitArray[6] : 0;
        guns += (currentPlayer->ship->weapons[i] == 3) ? outfitArray[7] : 0;
      }
      return ((shipCondition * shipyardArray[2 * HS_SHIP_TYPE - 1] / 4) +
      shipyardArray[2 * HS_SHIP_TYPE - 1] / 4 + guns / 2 + upgrades +
      (HS_TORPEDOS * outfitArray[4]) / 2);
    }

    return ((shipCondition * shipyardArray[2 * HS_SHIP_TYPE - 1] / 4) +
    shipyardArray[2 * HS_SHIP_TYPE - 1] / 4);
  }


  void barMenus() {
    Debug.log(DEBUG_DEFAULT, "void Planet::barMenus()");
    int choice;
    Menu barMenu(PLANET_MENU_START);
    barMenu.addOption("0: Back");
    barMenu.addOption("1: Ship Logs");
    barMenu.addOption("2: Achievements");
    barMenu.addOption("3: Gamble");

    do {
      planetHUD();
      printAt(PLANET_MENU_HEADER, "Bar:");
      choice = barMenu.getSelection();

      if (barMenu.isHighlighted("Logs")) {
        shipsLog();
      } else if (barMenu.isHighlighted("Achievements")) {
        achievementsMenu();
      } else if (barMenu.isHighlighted("Gamble")) {
        gamblingMenu();
      }
    } while (!((barMenu.isHighlighted("Back") && choice == KEY_RETURN) || choice == 0));
    //gamble, crew?
  }


  void printLogPage(int page) {
    Debug.log(DEBUG_DEFAULT, "void Planet::printLogPage(%d)", page);
    int x = 3;
    int y = 9;
    int i;
    printAt(PLANET_MENU_HEADER, "Ship Logs: Current as of stardate %d.%d", HS_DATE / 10, HS_DATE % 10);
    switch(page) {
      case 1:
        y = printAt(x, y, "General Information:");
        y++;
        y = printAt(x, y, "Exploration Logs:");
        y = printAt(x, y, "  Time in Space: %d days", HS_DATE - 407594);
        y = printAt(x, y, "  Distance Travelled: %d km", ACHIEVE_DISTANCE * 10);
        y = printAt(x, y, "  Systems Visited: %d", ACHIEVE_SYSTEMS);
        y = printAt(x, y, "  Planets Visited: %d", ACHIEVE_PLANETS);
        y = printAt(x, y, "Combat Logs:");
        y = printAt(x, y, "  Ships Commanded: %d", ACHIEVE_SHIPS);
        y = printAt(x, y, "  Weapons Obtained: %d", ACHIEVE_WEAPONS);
        y = printAt(x, y, "  Enemies Ships Destroyed: %d", ACHIEVE_KILLS);
        y = printAt(x, y, "  Combat Status: Legionnaire");
      break;
      case 2:
        y = printAt(x, y, "Outfit Details:");
        y++;
        y = printAt(x, y, "Ship Class: Section %d %s Class", HS_SHIP_TYPE, Ship::shipName(HS_SHIP_TYPE));
        y = printAt(x, y, "Armaments:");
        y = printAt(x, y, "  Ship Hardpoints: %d", CAP_HARDPOINT);
        if (HS_NWEAPONS == 0) {
          y = printAt(x, y, "  No weapon modules currently equipped.");
        } else {
          weaponsCheck(PLAYER);
          y = printAt(x, y, "  Current weapon installations: %d", HS_NWEAPONS);
          int guns[3] = {0};
          int dep[3] = {10, 50, 100};
          int dmg[3] = {20, 50, 100};
          int totalDepletion = 0;
          int totalDamage    = 0;
          for (i = 0; i < 5; i++) {
            if (HS_WEAPONS[i] != 0) {
              guns[HS_WEAPONS[i] - 1]++;
              totalDepletion += dep[HS_WEAPONS[i] - 1];
              totalDamage += dmg[HS_WEAPONS[i] - 1];
            }
          }
          for (i = 0; i < 3; i++) {
            if (guns[i] != 0) {
              y = printAt(x, y, "    %d Class %d type pulse cannon%s", guns[i], i + 1, (guns[i] > 1) ? "s." : ".");
            }
          }
          y = printAt(x, y, "  Maximum energy draw: %d MJ", totalDepletion);
          y = printAt(x, y, "  Potential damage output: %d", totalDamage);
        }
        if (HS_SHIP_TYPE != 1) {
          y = printAt(x, y, "  Standard Sigma Shipyards torpedo bays: %d", CAP_TORP);
        }
      break;
      case 3:
        y = printAt(x, y, "Ship Enhancements:");
        y++;
        if (HS_UP_DMG + HS_UP_RANGE + HS_UP_HULL + HS_UP_SHIELD + HS_UP_CARGO + HS_UP_TORP == 0) {
          y = printAt(x, y, "No upgrades at this time.");
        } else {
          if (HS_UP_DMG != 0) {
            y = printAt(x, y, "%d Pulse cannon armour piercing shell upgrades.", HS_UP_DMG);
            y = printAt(x, y, "  %d extra damage per shot.", HS_UP_DMG * UPGRADES[HS_I_UP_DMG].value);
          }
          if (HS_UP_RANGE != 0) {
            y = printAt(x, y, "%d Pulse cannon carbon alloy barrel upgrades.", HS_UP_RANGE);
            y = printAt(x, y, "  %d km range added.", HS_UP_RANGE * UPGRADES[HS_I_UP_RANGE].value);
          }
          if (HS_UP_HULL != 0) {
            y = printAt(x, y, "%d Tritanium hull strut upgrades.", HS_UP_HULL);
            y = printAt(x, y, "  %d damage dampening on hull per shot.", HS_UP_HULL * UPGRADES[HS_I_UP_HULL].value);
          }
          if (HS_UP_SHIELD != 0) {
            y = printAt(x, y, "%d Shileding reaction protocol reconfigurations.", HS_UP_SHIELD);
            y = printAt(x, y, "  %d damage deflection on shield per shot.", HS_UP_SHIELD * UPGRADES[HS_I_UP_SHIELD].value);
          }
          if (HS_UP_CARGO != 0) {
            y = printAt(x, y, "%d Cargo bay reconfigurations and expansion processes.", HS_UP_CARGO);
            y = printAt(x, y, "  %d extra cargo capacity.", HS_UP_CARGO * UPGRADES[HS_I_UP_CARGO].value);
          }
          if (HS_UP_TORP != 0) {
            y = printAt(x, y, "%d Photonic torpedo upgrades.", HS_UP_TORP);
            y = printAt(x, y, "  %d extra damage per torpedo.", HS_UP_TORP * UPGRADES[HS_I_UP_TORP].value);
          }
          if (HS_UP_ENGINE != 0) {
            y = printAt(x, y, "%d Port and polish upgrades.", HS_UP_ENGINE);
            y = printAt(x, y, "  %d engine speed.", HS_UP_ENGINE * UPGRADES[HS_I_UP_ENGINE].value);
          }
        }
      break;
    }
  }


  void shipsLog() {
    Debug.log(DEBUG_DEFAULT, "void Planet::shipsLog()");
    int page = 0;
    int choice;
    Menu pageMenu(PLANET_MENU_START);
    pageMenu.addOption("0: Back");
    pageMenu.addOption("1: Previous Page");
    pageMenu.addOption("2: Next Page");
    do {
      planetHUD();
      printLogPage(page + 1);
      choice = pageMenu.getSelection();
      if (pageMenu.isHighlighted("Previous")) {
        page = (page + 5) % 3;
      } else if (pageMenu.isHighlighted("Next")) {
        page = (page + 1) % 3;
      }
    } while (!((pageMenu.isHighlighted("Back") && choice == KEY_RETURN) || choice == 0));
  }


  const char *achieveColour(int current, int target) {
    if (current >= target) {
      return STR_COL_CYAN;
    } else {
      return STR_COL_WHITE;
    }
  }


  void printAchievementPage(int page) {
    int x = 3;
    int y = 9;
    switch(page) {
      case 1:
        printAt(PLANET_MENU_HEADER, "Basic Achievements:");
        y = printAt(x, y, "%s%-30s%i%s" STR_COL_RESET,
        achieveColour(ACHIEVE_DISTANCE, 20), "The Traveller", ACHIEVE_DISTANCE, "/20 AU travelled");
        y = printAt(x, y, "%s%-30s%i%s" STR_COL_RESET,
        achieveColour(ACHIEVE_SYSTEMS, 10), "Discoverer", ACHIEVE_SYSTEMS, "/10 systems visited");
        y = printAt(x, y, "%s%-30s%i%s" STR_COL_RESET,
        achieveColour(ACHIEVE_PLANETS, 15), "Land Harr", ACHIEVE_PLANETS, "/15 planets landed on");
        y = printAt(x, y, "%s%-30s%i%s" STR_COL_RESET,
        achieveColour(ACHIEVE_SHIPS, 2), "Able Captain", ACHIEVE_SHIPS, "/2 ships owned");
        y = printAt(x, y, "%s%-30s%i%s" STR_COL_RESET,
        achieveColour(ACHIEVE_WEAPONS, 5), "Rifleman", ACHIEVE_WEAPONS, "/5 weapons obtained");
        y = printAt(x, y, "%s%-30s%i%s" STR_COL_RESET,
        achieveColour(ACHIEVE_KILLS, 10), "Brute", ACHIEVE_KILLS, "/10 enemies killed");
      break;
      case 2:
        printAt(PLANET_MENU_HEADER, "Intermediate Achievements:");
        y = printAt(x, y, "%s%-30s%i%s" STR_COL_RESET,
        achieveColour(ACHIEVE_DISTANCE, 200), "Well Travelled", ACHIEVE_DISTANCE, "/200 AU travelled");
        y = printAt(x, y, "%s%-30s%i%s" STR_COL_RESET,
        achieveColour(ACHIEVE_SYSTEMS, 25), "Explorer", ACHIEVE_SYSTEMS, "/25 systems visited");
        y = printAt(x, y, "%s%-30s%i%s" STR_COL_RESET,
        achieveColour(ACHIEVE_PLANETS, 35), "Sphere Hunter", ACHIEVE_PLANETS, "/35 planets landed on");
        y = printAt(x, y, "%s%-30s%i%s" STR_COL_RESET,
        achieveColour(ACHIEVE_SHIPS, 4), "Admiral", ACHIEVE_SHIPS, "/4 ships owned");
        y = printAt(x, y, "%s%-30s%i%s" STR_COL_RESET,
        achieveColour(ACHIEVE_WEAPONS, 15), "Arms Dealer", ACHIEVE_WEAPONS, "/15 weapons obtained");
        y = printAt(x, y, "%s%-30s%i%s" STR_COL_RESET,
        achieveColour(ACHIEVE_KILLS, 25), "Soldier", ACHIEVE_KILLS, "/25 enemies killed");
      break;
      case 3:
        printAt(PLANET_MENU_HEADER, "Advanced Achievements:");
        y = printAt(x, y, "%s%-30s%i%s" STR_COL_RESET,
        achieveColour(ACHIEVE_DISTANCE, 2000), "Not Lost In Space", ACHIEVE_DISTANCE, "/2000 AU travelled");
        y = printAt(x, y, "%s%-30s%i%s" STR_COL_RESET,
        achieveColour(ACHIEVE_SYSTEMS, 100), "Found Your Hemingway", ACHIEVE_SYSTEMS, "/100 systems visited");
        y = printAt(x, y, "%s%-30s%i%s" STR_COL_RESET,
        achieveColour(ACHIEVE_PLANETS, 150), "Diplomat", ACHIEVE_PLANETS, "/150 planets landed on");
        y = printAt(x, y, "%s%-30s%i%s" STR_COL_RESET,
        achieveColour(ACHIEVE_SHIPS, 5), "Enshiplopedia", ACHIEVE_SHIPS, "/5 ships owned");
        y = printAt(x, y, "%s%-30s%i%s" STR_COL_RESET,
        achieveColour(ACHIEVE_WEAPONS, 30), "Gun Fettish", ACHIEVE_WEAPONS, "/30 weapons obtained");
        y = printAt(x, y, "%s%-30s%i%s" STR_COL_RESET,
        achieveColour(ACHIEVE_KILLS, 75), "Warren Piece", ACHIEVE_KILLS, "/75 enemies killed");
        // silver tounge
      break;
    }
  }


  void achievementsMenu() {
    Debug.log(DEBUG_DEFAULT, "void Planet::achievementsMenu()");
    int page = 0;
    int choice = 1;
    Menu pageMenu(PLANET_MENU_START);
    pageMenu.addOption("0: Back");
    pageMenu.addOption("1: Previous Page");
    pageMenu.addOption("2: Next Page");
    if (Debug.enabled) {
      pageMenu.addOption("3: Set Ach (debug)");
    }
    do {
      planetHUD();
      Debug.log(DEBUG_DEFAULT, "page: %d", page);
      printAchievementPage(page + 1);
      choice = pageMenu.getSelection();
      if (pageMenu.isHighlighted("Previous")) {
        page = (page + 5) % 3;
      } else if (pageMenu.isHighlighted("Next")) {
        page = (page + 1) % 3;
      } else if (pageMenu.isHighlighted("Set Ach")) {
        int slot, value;
        printAt(COM_BELOW_PLANET_HUD_START, "achievement slot: ");
        scanf("%d", &slot);
        printAt(COM_BELOW_PLANET_HUD_START + 1, "achievement value: ");
        scanf("%d", &value);
        if (slot < 0 || slot > ACHIEVE_I_SIZE) {
          comLog(COM_BELOW_PLANET_HUD);
          printf("Invalid slot %d", slot);
        } else {
          HS_ACHIEV[slot] = value;
        }
        clean();
      }
    } while (!((pageMenu.isHighlighted("Back") && choice == KEY_RETURN) || choice == 0));
  }


  void gamblingMenu() {
    Debug.log(DEBUG_DEFAULT, "void Planet::gamblingMenu()");
    int choice;
    do {
      planetHUD();

      gotoXY(3, 27);
      printf("Under Development    0: Back");
      choice = getch_arrow() - '0';
      if (choice != 1 && choice != 2 && choice != 0 && choice != 5) {
        printf("Invalid selection");
      }
    } while (choice != 0);
  }

  void planetWindow() {
    Debug.log(DEBUG_DEFAULT, "void planetWindow()");
    drawWindow(1, 1, 60, 23);
  }


} Planet;

