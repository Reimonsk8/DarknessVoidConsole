#include "Weapon.h"
#include "Equipment.h"
#include "Common.h"
#include "Constants.h"
#include "graphics.h"
#include <string>
#include <time.h>

Weapon::Weapon(std::string name, int ap, int maxhp, bool fixed) : Equipment(name, ap, maxhp, fixed)
{
	mType = T_Weapon;
	if (!fixed)	//increase AP & maxHP based on type and rarity
	{
		mAP = mAP + (5 * (mRandomValue / 4));
		if (!(mRarity == "common"))
			mMaxHP = mMaxHP + (5 * mRandomValue / 6);
	}
};

Weapon::~Weapon()
{}

void Weapon::pickWeapon(Weapon *current, Character &hero, int code)
{
	if (gInstructions)
		std::cout << "(press enter key to confirm pick up)" << std::endl;
		std::cout << "found " << current->getName() << " +maxAP: " << current->getAP() << ",";
		std::cout << " replace Weapon ??" << std::endl;
	if (code == ACTION)
	{
		std::cout << "Weapon taken" << std::endl;
		hero.addToInventory(*current);
		lvl.grid[hero.heroRow][hero.heroCol] = 'O';
		Sleep(Timing::UI_DELAY_MS);
		drawScreen(hero, true);
	}
};

