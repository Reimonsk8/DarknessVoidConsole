#include "Equipment.h"
#include "Helmet.h"
#include "common.h"
#include "graphics.h"
#include <iostream>
#include <string>
#include <time.h>


Helmet::Helmet(std::string name, int ap, int maxhp, bool fixed) : Equipment(name, ap, maxhp, fixed)
{
	mType = T_Head;
	if (!fixed)	//increase AP & maxHP based on type and rarity
	{
		mMaxHP = mMaxHP + ((randomValue * 1) + (5 * (randomValue / 4)));
		if (!(mRarity == "common"))
			mAP = mAP + (2 * randomValue / 6);
	}
};

Helmet::~Helmet()
{}

void Helmet::pickHelmet(Helmet *current, Character &hero, int code)
{
	if (gInstructions)
		std::cout << "(press enter key to confirm pick up)" << std::endl;
	std::cout << "found " << current->getName() << " +maxHP: " << current->getMaxHP() << ", ";
	std::cout << "replace Helmet ??" << std::endl;
	if (code == ACTION)
	{
		std::cout << "Helmet taken" << std::endl;
		hero.addToInventory(*current);
		lvl.grid[hero.heroRow][hero.heroCol] = 'O';
		Sleep(500);
		drawScreen(hero, true);
	}
}