#include "Weapon.cpp"

int main() {
	CWeapon wep;
	std::cout	<< "type " << wep.getType() << std::endl
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
				<< "mods: " <<std::endl;
				for (int i = 0; i < 20; i++) {
					std::cout << "  " << i << wep.getMods(i) << std::endl;
				}
				//<< "id " << wep.getId() << std::endl;
	return 1;
}
