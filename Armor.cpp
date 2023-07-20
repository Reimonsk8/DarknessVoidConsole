#include "Armor.h"
#include "Equipment.h"
#include "graphics.h"
#include "common.h"
#include <iostream>
#include <string>
#include <time.h>
#include "Character.h"

Armor::Armor(std::string name, int ap, int maxhp, bool fixed) : Equipment(name, ap, maxhp, fixed)
{
	mType = T_Chest;
	if (!fixed)	//increase AP & maxHP based on type and rarity
	{
		mMaxHP = mMaxHP + ((randomValue * 2) + (10 * (randomValue / 4)));
		if (!(mRarity == "common"))
			mAP = mAP + (3 * randomValue / 6);
	}
	
};

Armor::~Armor()
{}

void Armor::pickArmor(Armor *current, Character &hero, int code)
{
	if (gInstructions)
		std::cout << "(press enter key to confirm pick up) " << std::endl;
	std::cout << "found " << current->getName() << " +maxHP: " << current->getMaxHP() << ", ";
	std::cout << " replace Armor ??" << std::endl;
	if (code == ACTION)
	{
		std::cout << "Armor taken" << std::endl;
		hero.addToInventory(*current);
		lvl.grid[hero.heroRow][hero.heroCol] = 'O';
		Sleep(500);
		drawScreen(hero, true);
	}
	//delete current;
}

