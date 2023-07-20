#ifndef GENERATOR_H
#define GENERATOR_H

#include "Enemy.h"
#include "Weapon.h"
#include "Helmet.h"
#include "Armor.h"
#include "Potion.h"

class Generator
{
public:
	Generator();
	~Generator();

	void InfoPlace(int y, int x);
	void SpawnInMap();

	Enemy *returnEnemy(int heroRow, int heroCol);
	Weapon *returnWeapon(int heroRow, int heroCol);
	Helmet *returnHelmet(int heroRow, int heroCol);
	Armor *returnArmor(int heroRow, int heroCol);
	Potion *returnPotion(int heroRow, int heroCol);


private: 
	
	int enemies;
	int weapons;
	int helmets;
	int armors;
	int potions;

	std::vector <Enemy> listEnemies;
	std::vector <Weapon> listWeapons;
	std::vector <Helmet> listHelmets;
	std::vector <Armor> listArmors;
	std::vector <Potion> listPotions;
};

#endif //GENERATOR_H