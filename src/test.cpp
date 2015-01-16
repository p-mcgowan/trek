#include "Weapon.cpp"

int main() {
  while (true) {
    CWeapon *wep = new CWeapon();
    int t;
    for (int i = 0; i < rndm(0, 4); i++) {
      t = (rndm(0,5) == 0)? -1 : 1;
      std::pair<std::string, std::string> uGrade = {indexToStat[rndm(0,23)], std::to_string(t*rndm(1, 10))};
      if (std::get<0>(uGrade) == "CRIT")
        std::get<1>(uGrade) = std::to_string(std::stof(std::get<1>(uGrade)) / 100);
      wep->applyUpgrade(uGrade);
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

    LOGD(std::endl << "Stats:");
    std::vector<std::pair<std::string, std::string>> test = wep->getStats();
    for(std::vector<std::pair<std::string, std::string>>::iterator it = test.begin(); it != test.end(); ++it)
      LOGD(it->first << " " << it->second);

    LOGD(std::endl << "Upgrades:");
    test = wep->getUpgrades();
    for(std::vector<std::pair<std::string, std::string>>::iterator it = test.begin(); it != test.end(); ++it)
      LOGD(it->first << " " << it->second);
    
    char s = getch();
    if (s == 'l') {
      std::string name = "", val = "";
      std::cout << "Enter upgrade to remove: "; std::cin >> name >> val;
      LOGD(std::endl << "Upgrades:");
      wep->removeUpgrade({name, val});
      test = wep->getUpgrades();
      for(std::vector<std::pair<std::string, std::string>>::iterator it = test.begin(); it != test.end(); ++it)
        LOGD(it->first << " " << it->second);
      getch();
    }
    
    LOGD(std::endl << "Stats:");
    test = wep->getStats();
    for(std::vector<std::pair<std::string, std::string>>::iterator it = test.begin(); it != test.end(); ++it)
      LOGD(it->first << " " << it->second);
    std::cout << "+=========+=========+=========+=========+=========+=========+=========+========+" << std::endl << std::endl;
    if(s == 'l' && getch() == 'q')
      return 1;
  }
  return 1;
}
