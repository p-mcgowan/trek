#define BUFFER_MEDIUM 1024

#define HUD_LINE          "+--------------------------------------------------+"
#define HUD_WIDTH         51
#define HUD_HELM_HEIGHT   6
#define HUD_STATUS_HEIGHT 8
#define HUD_CARGO_HEIGHT  8
#define HUD_SYSTEM_HEIGHT 5
#define HUD_COM_HEIGHT    5

#define COM_WIDTH                  60
#define COM_HEIGHT                 5
#define COM_BELOW_MAP              1, 30
#define COM_BELOW_MAP_START        2, 31
#define COM_BELOW_PLANET_HUD       1, 30
#define COM_BELOW_PLANET_HUD_START 2, 31
#define HUD_MINIMAL_START          2, 25
#define PLANET_MENU_HEADER         3, 3
#define PLANET_MENU_START          3, 5

#define SCAN_TARGET_NONE   0
#define SCAN_TARGET_PLAYER 1
#define SCAN_TARGET_ENEMY  2

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>  // va_arg
#include <dirent.h>  // directory

unsigned int lastSeed;

#include "getch.h" // getch (conio.h, unistd.h)
#include "con_c_nix.h"
#include "externs.h"
#include "libs.h"
#include "upgrades.h"
#include "weapons.h"

#include "ship.h"
#include "player.h"

#include "menu.h"

#include "planet.h"
#include "system.h"

#include "drawable.h"
#include "utils.h"

