#include "Weapon.cpp"
//#include "CMultiContainer.cpp"

int main() {
  while (true) {
    CWeapon *wep = new CWeapon();
    int t;
    for (int i = 0; i < rndm(0, 4); i++) {
      t = (rndm(0,3) == 0)? -1 : 1;
      wep->applyUpgrade({indexToStat[rndm(0,23)], t*rndm(1, 10)});
    }
    LOGD("+=========+=========+=========+=========+=========+=========+=========+========+");
    LOGD(wep->getName());
    LOGDN(wep->getRarity() << " tier " << wep->getTier() << " " << wep->getSType());
    LOGD(" <" << wep->getType() << ">");
    LOGD(wep->getDesc());
    LOGD("dmg:       " << wep->getDmg());
    LOGD("maxdmg:    " << wep->getMaxdmg());
    LOGD("ammoType:  " << wep->getAmmoType());
    LOGD("shots:     " << wep->getShots());
    LOGD("crit:      " << wep->getCrit());
    LOGD("mupgrades: " << wep->getMupgrades());
    LOGD("nupgrades: " << wep->getNupgrades());
    LOGD("rlvl:      " << wep->getRlvl());
    LOGD("rclout:    " << wep->getRclout());
    LOGD("rsocial:   " << wep->getRsocial());
    LOGD(std::endl << "Upgrades:");
    std::vector<std::pair<std::string,int>> test = wep->getUpgrades();
    for(std::vector<std::pair<std::string, int>>::iterator it = test.begin(); it != test.end(); ++it)
      LOGD(it->first << " " << it->second);
    LOGD(std::endl << "Stats:");
    test = wep->getStats();
    for(std::vector<std::pair<std::string, int>>::iterator it = test.begin(); it != test.end(); ++it)
      LOGD(it->first << " " << it->second);
    std::cout << "+=========+=========+=========+=========+=========+=========+=========+========+" << std::endl << std::endl;
    if(getch() == 'q')
      return 1;
  }
  return 1;
}
