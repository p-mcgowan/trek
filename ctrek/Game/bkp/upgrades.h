#define HS_I_UP_DMG     0
#define HS_I_UP_RANGE   1
#define HS_I_UP_HULL    2
#define HS_I_UP_SHIELD  3
#define HS_I_UP_CARGO   4
#define HS_I_UP_TORP    5
#define HS_I_UP_ENGINE  6
#define UPGRADE_DMG    0
#define UPGRADE_RANGE  1
#define UPGRADE_HULL   2
#define UPGRADE_SHIELD 3
#define UPGRADE_CARGO  4
#define UPGRADE_TORP   5
#define UPGRADE_ENGINE 6

typedef struct Upgrade {
  const char* name;
  const char* description;
  int cost;
  int hudstat;
  int max;
  int compatibility;
  int value;

  Upgrade(
  const char* pName, const char* pDescription, int pCost, int pHudstat, int pMax, int pCompatibility, int pValue) {
    name = pName;
    description = pDescription;
    cost = pCost;
    hudstat = pHudstat;
    max = pMax;
    compatibility = pCompatibility;
    value = pValue;
  }

} Upgrade;

static const Upgrade UPGRADES[7] = {
  Upgrade(
    "Ammunition upgrade",
    "Replaces the standard issue impact casings with armour \n"
    "piercing ones.\n"
    "Effect: +10 damage per upgrade\n"
    "Max: 5 upgrades\n"
    "Cost: 100,000\n"
    "Compatibility: C or F class ships\n"
    "Currently equipped: ",
    100000,
    HS_I_UP_DMG,
    5,
    2,
    10
  ), Upgrade(
    "Cannon Barrel upgrade",
    "Replaces the stock steel barrels on weapons with a high \n"
    "efficiency carbon alloy.\n"
    "Effect: +10 km range per upgrade\n"
    "Max: 2 upgrades\n"
    "Cost: 100,000\n"
    "Compatibility: C or F class ships\n"
    "Currently equipped: ",
    100000,
    HS_I_UP_RANGE,
    2,
    2,
    10
  ), Upgrade(
    "Advanced Hull Plating",
    "Replaces stock hull struts with a high tensile strength \n"
    "tritanium alloy.\n"
    "Effect: +10 hull damage dampening per hit\n"
    "Max: 3 upgrades\n"
    "Cost: 250,000\n"
    "Compatibility: C or F class ships\n"
    "Currently equipped: ",
    250000,
    HS_I_UP_HULL,
    3,
    2,
    10
  ), Upgrade(
    "Oscillating Shield upgrade",
    "Reconfigures shielding protocols to react quicker to \n"
    "phase fluctuations.\n"
    "Effect: +10 shield damage dampening per hit\n"
    "Max: 3 upgrades\n"
    "Cost: 250,000\n"
    "Compatibility: C or F class ships\n"
    "Currently equipped: ",
    250000,
    HS_I_UP_SHIELD,
    3,
    2,
    10
  ), Upgrade(
    "Cargo Refit",
    "Reconfigures cargo bays and consolidates surrounding \n"
    "areas of unused space.\n"
    "Effect: +100 cargo space\n"
    "Max: 5 upgrades\n"
    "Cost: 150,000\n"
    "Compatibility: C or F class ships\n"
    "Currently equipped: ",
    150000,
    HS_I_UP_CARGO,
    5,
    2,
    100
  ), Upgrade(
    "Advanced Torpedos",
    "Exchanges standard javelin torpedos for photonic ones\n"
    "and recalibrates torpedo bays for optimum speed.\n"
    "Effect: +20 torpedo damage\n"
    "Max: 3 upgrades\n"
    "Cost: 500,000\n"
    "Compatibility: C class ships\n"
    "Currently equipped: ",
    500000,
    HS_I_UP_TORP,
    3,
    4,
    20
  ), Upgrade(
    "Port and Polish",
    "Standard cleaning and optimization of all engine and \n"
    "propulsion systems. After upgrade your ship will fly \n"
    "like she's just come out of ATLAS Shipyards.\n"
    "Effect: +1 move per turn\n"
    "Max: 2 upgrades\n"
    "Cost: 750,000\n"
    "Compatibility: C class ships\n"
    "Currently equipped: ",
    750000,
    HS_I_UP_ENGINE,
    2,
    4,
    1
  )
};
