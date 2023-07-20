#include <time.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include "common.h"
#include "Generator.h"

Generator::Generator()
	:weapons(0), helmets(0), armors(0), potions(0), enemies(0)
{
	srand(time(0));

	for (int i = 0; i < 16; i++)
		listEnemies.push_back(Enemy("skeleton", 10, 50, 6));//name,AP,maxHP, flee
	for (int i = 0; i < 14; i++)
		listEnemies.push_back(Enemy("ghost", 15, 100, 9));
	for (int i = 0; i < 12; i++)
		listEnemies.push_back(Enemy("demon", 26, 200, 6));
	for (int i = 0; i < 10; i++)
		listEnemies.push_back(Enemy("troll", 20, 300, 3));
	for (int i = 0; i < 9; i++)
		listEnemies.push_back(Enemy("centaur", 20, 150, 8));
	for (int i = 0; i < 8; i++)
		listEnemies.push_back(Enemy("bat", 7, 30, 10));
	for (int i = 0; i < 7; i++)
		listEnemies.push_back(Enemy("minotaur", 25, 200, 4));
	for (int i = 0; i < 6; i++)
		listEnemies.push_back(Enemy("gryphon", 35, 180, 9));
	for (int i = 0; i < 4; i++)
		listEnemies.push_back(Enemy("reaper", 50, 250, 9));
	for (int i = 0; i < 2; i++)
		listEnemies.push_back(Enemy("dragon", 100, 700, 10));
	std::random_shuffle(listEnemies.begin(), listEnemies.end());

	for (int i = 0; i < 2; i++)//generate helms
	{
		listHelmets.push_back(Helmet("hat             \t  _[]_    ", 0, 1));//name,AP,maxHP
		listHelmets.push_back(Helmet("thief hood      \t  /o_o\\   ", 0, 2));
		listHelmets.push_back(Helmet("soldier helm    \t  (-_-)   ", 0, 3));
		listHelmets.push_back(Helmet("sage cap        \t  _/^\\_   ", 0, 4));
		listHelmets.push_back(Helmet("templar helmet  \t  [-+-]   ", 0, 5));
		listHelmets.push_back(Helmet("straight helmet \t  |---|   ", 0, 6));
		listHelmets.push_back(Helmet("dragon  mask    \t VowoV    ", 0, 7));
		char h2 = 209; // special char ╤

		//REVIEW [VAR][CONVENTION][Karla]: One variable per line.
		std::string h1 = "|", h3 = "T", h4 = "|   ", helm = h1 + h2 + h3 + h2 + h4;
		listHelmets.push_back(Helmet("barbute helmet  \t  " + helm, 0, 8));
		h1 = 247; // special char §
		h3 = 157; // special char ¥
		h2 = 215; // special char ¤
		h4 = "   ", helm = h1 + h2 + h3 + h2 + h1 + h4;
		listHelmets.push_back(Helmet("evil dark helm  \t  " + helm, -15, 55));

	}
	std::random_shuffle(listHelmets.begin(), listHelmets.end());

	for (int i = 0; i < 2; i++)//generate armors
	{
		listArmors.push_back(Armor("cloth shirt      \t /|__|\\   ", 0, 1));
		listArmors.push_back(Armor("stripped armor   \t /\|||\\   ", 0, 2));
		listArmors.push_back(Armor("chainmail        \t /{###}\\  ", 0, 3));
		listArmors.push_back(Armor("magician robe    \t /%~&~%\\  ", 0, 4));
		listArmors.push_back(Armor("mithrill armor   \t /\\ | /\\  ", 0, 6));
		listArmors.push_back(Armor("brigadine armor  \t /\\~T~/\\  ", 0, 8));
		listArmors.push_back(Armor("dragon slayer set\t />>V<<\\  ", 0, 10));
		char a3 = 232; // special char Φ
		char a2 = 178; // special char ▓
		std::string a1 = "/\\", a4 = "/\\  ", armor = a1 + a2 + a3 + a2 + a4;
		listArmors.push_back(Armor("evil cursed armor\t " + armor, 100, -100));
	}
	std::random_shuffle(listArmors.begin(), listArmors.end());

	for (int i = 0; i < 2; i++)//generate weapons
	{
		listWeapons.push_back(Weapon("pickaxe       \t ---)     ", 1, 0));
		listWeapons.push_back(Weapon("sword         \t -/==>    ", 2, 0));
		listWeapons.push_back(Weapon("axe           \t ---(H)   ", 3, 0));
		listWeapons.push_back(Weapon("trident       \t ------E  ", 4, 0));
		listWeapons.push_back(Weapon("wand          \t --~o~    ", 5, 0));
		listWeapons.push_back(Weapon("long sword    \t -/===>   ", 6, 0));
		listWeapons.push_back(Weapon("spear         \t -------> ", 7, 0));
		listWeapons.push_back(Weapon("javelin       \t ------>> ", 8, 0));
		listWeapons.push_back(Weapon("claymore      \t +-|<===> ", 9, 0));
		char s2 = 240; // special char ≡
		std::string s1 = "-|", s3 = "> ", sword = s1 + s2 + s2 + s2 + s2 + s3;
		listWeapons.push_back(Weapon("greatsword    \t  " + sword, 12, 0));
	}
std:random_shuffle(listWeapons.begin(), listWeapons.end());

	const int TOTAL_POTIONS = 50;
	for (int i = 0; i < TOTAL_POTIONS; i++)//generate potions
		listPotions.push_back(Potion("health potion \t (<3)     ", 0, 14));
};

Generator::~Generator()
{};

void Generator::InfoPlace(int y, int x)//function to look info
{
	if (((y) < gHeight) && ((x) < gWidth) && ((y) >= 0) && ((x) >= 0))//if inside limits
	{
		for (int i = 0; i < listEnemies.size(); ++i)
		{
			if (y == listEnemies[i].posY && x == listEnemies[i].posX)
			{
				std::cout << "Enemy ahead " << listEnemies[i].getName() << " Health " << listEnemies[i].getHP() << " Power " << listEnemies[i].getAP();
				return;
			}
		}
		for (int i = 0; i < listHelmets.size(); ++i)
		{
			if (y == listHelmets[i].posY && x == listHelmets[i].posX)
			{
				std::cout << "Helmet ahead " << listHelmets[i].getName() << " Defense " << listHelmets[i].getMaxHP();
				return;
			}
		}
		for (int i = 0; i < listArmors.size(); ++i)
		{
			if (y == listArmors[i].posY && x == listArmors[i].posX)
			{
				std::cout << "Armor ahead " << listArmors[i].getName() << " Defense " << listArmors[i].getMaxHP();
				return;
			}
		}
		for (int i = 0; i < listWeapons.size(); ++i)
		{
			if (y == listWeapons[i].posY && x == listWeapons[i].posX)
			{
				std::cout << "Weapon ahead " << listWeapons[i].getName() << " Power " << listWeapons[i].getAP();
				return;
			}
		}
		for (int i = 0; i < listPotions.size(); ++i)
		{
			if (y == listPotions[i].posY && x == listPotions[i].posX)
			{
				std::cout << "Potion ahead " << listPotions[i].getName() << " Health " << listPotions[i].getMaxHP();
				return;
			}
		}
	}
	std::cout << "there's nothing to look" << std::endl;
	return;
}

void Generator::SpawnInMap()
{
	int mapValue;
	for (int row = 0; row < gHeight; ++row)//count items to generate
	{
		for (int col = 0; col < gHeight; ++col)
		{

			mapValue = lvl.grid[row][col];
			switch (mapValue)
			{
			case(L_E) :
			{
				if (enemies < listEnemies.size())
				{
					listEnemies[enemies].posY = row;
					listEnemies[enemies].posX = col;
					++enemies;
				}
				else
					std::cout << "out of enemies" << std::endl;
			}break;

			case(L_W) :
			{
				if (weapons < listWeapons.size())
				{
					listWeapons[weapons].posY = row;
					listWeapons[weapons].posX = col;
					++weapons;
				}
				else
					std::cout << "out of weapons" << std::endl;
			}break;

			case(L_A) :
			{
				if (armors < listArmors.size())
				{
					listArmors[armors].posY = row;
					listArmors[armors].posX = col;
					++armors;
				}
				else
					std::cout << "out of armors" << std::endl;
			}break;

			case(L_H) :
			{
				if (helmets < listHelmets.size())
				{
					listHelmets[helmets].posY = row;
					listHelmets[helmets].posX = col;
					++helmets;
				}
				else
					std::cout << "out of helmets" << std::endl;
			}break;

			case(L_P) :
			{
				if (potions < listPotions.size())
				{
					listPotions[potions].posY = row;
					listPotions[potions].posX = col;
					++potions;
				}
				else
					std::cout << "out of potions" << std::endl;
			}break;

			default:
				break;
			}

		}
	}
};

Enemy *Generator::returnEnemy(int heroRow, int heroCol)
{
	for (int x = 0; x < listEnemies.size(); ++x)
	{
		if (heroRow == listEnemies[x].posY && heroCol == listEnemies[x].posX)
		{
			return &listEnemies[x];
			break;
		}
	}
	return nullptr;
}

Weapon *Generator::returnWeapon(int heroRow, int heroCol)
{
	for (int x = 0; x < listWeapons.size(); ++x)
	{
		if (heroRow == listWeapons[x].posY && heroCol == listWeapons[x].posX)
		{
			return &listWeapons[x];
			break;
		}
	}
	return nullptr;
}

Helmet *Generator::returnHelmet(int heroRow, int heroCol)
{
	for (int x = 0; x < listHelmets.size(); ++x)
	{
		if (heroRow == listHelmets[x].posY && heroCol == listHelmets[x].posX)
		{
			return &listHelmets[x];
			break;
		}
	}
	return nullptr;
}

Armor *Generator::returnArmor(int heroRow, int heroCol)
{
	for (int x = 0; x < listArmors.size(); ++x)
	{
		if (heroRow == listArmors[x].posY && heroCol == listArmors[x].posX)
		{
			return &listArmors[x];
			break;
		}
	}
	return nullptr;
}

Potion *Generator::returnPotion(int heroRow, int heroCol)
{
	for (int x = 0; x < listPotions.size(); ++x)
	{
		if (heroRow == listPotions[x].posY && heroCol == listPotions[x].posX)
		{
			return &listPotions[x];
		break;
		}
	}
	return nullptr;
}