#include "Weapon.cpp"

int main() {
  cursor();
  clrsc(WHITE, BLUE);
  //std::cout << "\033[0;31m";
  //std::cout << "\033[0;44m";
  // textbackground(BLUE);
  // textcolor(WHITE);
  // clrscr();
  std::cout << "hello world";
  getch();
  while (true) {
    CWeapon wep;
    std::cout
    /*<< "type " << wep.getType() << std::endl
    << "sType " << wep.getSType() << std::endl
    << "prefix " << wep.getPrefix() << std::endl
    << "suffix " << wep.getSuffix() << std::endl
    << "name " << wep.getName() << std::endl
    << "desc " << wep.getDesc() << std::endl
    //<< "lvl " << wep.getLvl() << std::endl
    << "tier " << wep.getTier() << std::endl
    << "crit " << wep.getCrit() << std::endl
    << "shots " << wep.getShots() << std::endl
    << "maxdmg " << wep.getMaxdmg() << std::endl
    << "dmg " << wep.getDmg() << std::endl
    //<< "ammoType " << wep.getAmmoType() << std::endl
    << "mupgrades " << wep.getMupgrades() << std::endl
    << "nupgrades " << wep.getNupgrades() << std::endl
    //<< "*upgrades " << wep.*upgrades << std::endl
    << "rarity " << wep.getRarity() << std::endl
    << "rlvl " << wep.getRlvl() << std::endl
    << "rclout " << wep.getRclout() << std::endl
    << "rsocial " << wep.getRsocial() << std::endl
    << "mods: " << std::endl;
    for (int i = 0; i < 20; i++) {
      if (wep.getStats(i) != 0)
        std::cout << "  Slot " << i << ", name " << indexToStat[i] << " , value " << wep.getStats(i) << std::endl;
    }*/
    << "+=========+=========+=========+=========+=========+=========+=========+========+"
    << wep.getName() << std::endl
    << wep.getRarity() << " tier " << wep.getTier() << " " << wep.getSType() 
    << " <" << wep.getType() << ">" << std::endl
    << wep.getDesc() << std::endl << std::endl
    << "dmg:       " << wep.getDmg() << std::endl
    << "maxdmg:    " << wep.getMaxdmg() << std::endl
    << "ammoType:  " << wep.getAmmoType() << std::endl
    << "shots:     " << wep.getShots() << std::endl
    << "crit:      " << wep.getCrit() << std::endl
    << "mupgrades: " << wep.getMupgrades() << std::endl
    << "nupgrades: " << wep.getNupgrades() << std::endl
    << "rlvl:      " << wep.getRlvl() << std::endl
    << "rclout:    " << wep.getRclout() << std::endl
    << "rsocial:   " << wep.getRsocial() << std::endl
    << std::endl;
    //<< "*upgrades " << wep.*upgrades << std::endl
    for (int i = 0; i < 20; i++) {
      if (wep.getStats(i) != 0) {
        if (wep.getStats(i) > 0)
          std::cout << "+";
        std::cout << wep.getStats(i) << " " << indexToStat[i] << std::endl;
      }
    }
    std::cout << "+=========+=========+=========+=========+=========+=========+=========+========+" << std::endl << std::endl;
    if(getch() == 'q')
        return 1;
  }
        //<< "id " << wep.getId() << std::endl;
  return 1;
}
