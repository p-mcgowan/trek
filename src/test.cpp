#include "Weapon.cpp"

int main() {
  while (true) {
    CWeapon *wep = new CWeapon();
    for (int i = 0; i < rndm(0, 4); i++) {
        wep->applyUpgrade({indexToStat[rndm(0,23)], rndm(1, 10)});
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
    std::cout << getStats("");
    std::cout << "+=========+=========+=========+=========+=========+=========+=========+========+" << std::endl << std::endl;
    if(getch() == 'q')
        return 1;
  }
  return 1;
}
