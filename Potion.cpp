#include "common.h"
#include "Potion.h"
#include "Graphics.h"
#include "Equipment.h"
#include <iostream>
#include <string>
#include <time.h>
#include <Windows.h>

Potion::Potion(std::string name, int ap, int maxhp, bool fixed) : Equipment(name, ap, maxhp, fixed)
{
	mType = T_Consumable;
	if (!fixed)	//increase AP & maxHP based on type and rarity
	{
		mMaxHP = mMaxHP + (2 * randomValue)+ (5 * (randomValue / 3));
		if (!(mRarity == "common"))
			mAP = mAP + (1 * randomValue / 4);
	}
};

Potion::~Potion()
{}

void Potion::pickPotion(Potion *current, Character &hero, int code)
{
	if (gInstructions)
		std::cout << "(press enter key to confirm pick up)" << std::endl;
	std::cout << "found " << current->getName() << " +HP: " << current->getMaxHP() << ", ";
	if (hero.inventorySize() >= 8) // inventory full?
		std::cout << "inventory full, replace Potion ??" << std::endl;
	else
		std::cout << "take Potion ??" << std::endl;
	if (code == ACTION)
	{
		std::cout << "Potion taken" << std::endl;
		hero.addToInventory(*current);
		lvl.grid[hero.heroRow][hero.heroCol] = 'O';
		Sleep(500);
		drawScreen(hero, true);
	}
}


void Potion::usePotion(Character &hero, int code)
{
	drawScreen(hero);
	if (hero.inventorySize() <= 3)// if no potions
	{
		std::cout << "so dumb, you forgot that you dont have any potions left" << std::endl;
		Sleep(1000);
	}
	else
	{
		bool potionUsed = false;
		int inputNum;
		while (!potionUsed)
		{
			drawScreen(hero);//refresh to delete previous commands
			if (code >= P_1 && code <= P_5)//if accesesd by keys
				inputNum = (code - 48);
			else //input by text
			{
				if (gInstructions)
					std::cout << "input the number of potions you want to use" << std::endl;
				std::cout << "select & enter potion number: " << std::endl;
				std::cin >> inputNum;
			}
			if ((inputNum + 3) <= hero.inventorySize() && (inputNum + 3) >= 4)//validate potion number
			{
				int potionHP = hero.selectItem(inputNum + 2).getMaxHP();//potion healing points
				int maxHP = hero.getMaxHP();//maxhp hero can heal
				int HP = hero.getHP();// initial hero hp
				hero.setAP(hero.getAP() + hero.selectItem(inputNum + 2).getAP());//increase ap if rare item
				/*heal 1 point until hero hp = inital hp  + potion hp or max hp reached*/
				for (int heal = HP; (hero.getHP() < (HP + potionHP)) && (hero.getHP() < maxHP); ++heal)
					hero.setHP(1);
				std::cout << hero.selectItem(inputNum + 2).getName() << " used!! hero hp now is ";
				PlaySound(TEXT("./Sounds/powerup.wav"), NULL, SND_ASYNC);
				SetConsoleTextAttribute(hConsole, C_BGreen);
				std::cout << hero.getHP() << std::endl;
				SetConsoleTextAttribute(hConsole, C_White);
				hero.removeFromInventory(inputNum + 2);
				potionUsed = true;
				Sleep(2000);
			}
			else
			{
				std::cout << "wrong input try again (press i fron instructions)" << std::endl;
				Sleep(500);
			}
		}
	}
	drawScreen(hero, true);//refresh inventory info
};