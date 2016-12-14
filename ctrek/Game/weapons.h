typedef struct Weapon {
  int dmg;
  int dep;
  int range;
  int type;
  int compatibility;
  const char* name;
  const char* desc;
  const char* compDesc;

  Weapon(int pDmg, int pDep, int pRange, int pType, int pCompatibility,
  const char* pName, const char* pDesc, const char* pCompDesc) {
    dmg = pDmg;
    dep = pDep;
    range = pRange;
    type = pType;
    compatibility = pCompatibility;
    name = pName;
    desc = pDesc;
    compDesc = pCompDesc;
  }
} Weapon;

static const Weapon WEAPONS[3] = {
  Weapon(
    20,
    10,
    3,
    1,
    1,
    "Pew-Pew SK-337 Lance",
    "",
    "All standard regulation fedration vessels."
  ), Weapon(
    50,
    50,
    5,
    2,
    2,
    "N00bzorz ID 10-T Killer",
    "",
    "C or F class standard regulation fedration vessels."
  ), Weapon(
    100,
    100,
    7,
    3,
    4,
    "Pwnzorz 1337 Cannon",
    "",
    "C class standard regulation fedration vessels."
  )
};

