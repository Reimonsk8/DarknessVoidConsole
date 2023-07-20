#include "Character.h"
#include "common.h"
#include "Equipment.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <Windows.h>

Character::Character()
{
	mCurrentEnemy = "";
	enemyDamaged = false;
};
Character::~Character()
{};

void Character::setBaseValues(int baseAP, int baseMaxHP)
{
	mBaseAP = baseAP;
	mBaseMaxHP = baseMaxHP;
	mAP = mBaseAP;
	mMaxHP = mBaseMaxHP;
	mHP = mMaxHP;
};

void Character::setName(std::string name)
{
	mName = name;
};
std::string Character::getName()
{
	return mName;
};

void Character::setHP(int hp)
{
	mHP = mHP + hp;
};
int Character::getHP()
{
	return mHP;
};

void Character::setMaxHP(int maxhp)
{
	mMaxHP = maxhp;
};
int Character::getMaxHP()
{
	return mMaxHP;
};

void Character::setAP(int ap)
{
	mAP = ap;
};
int Character::getAP()
{
	return mAP;
};

int Character::inventorySize()
{
	return mInventory.size();
}

void Character::addToInventory(Equipment &item)
{
	if (item.getType() == T_Head)
	{
		if (mInventory.empty())
			mInventory.push_back(item);
		else
			mInventory[T_Head] = item;
		std::cout << "item: " << item.getName() << " added to inventory" << std::endl;
		PlaySound(TEXT("./Sounds/item.wav"), NULL, SND_SYNC);
		item.posX = OUT_OF_RANGE;
		item.posY = OUT_OF_RANGE;
	}
	else if (item.getType() == T_Chest)
	{
		if (mInventory.size() <= T_Chest)
			mInventory.push_back(item);
		else
			mInventory[T_Chest] = item;
		std::cout << "item: " << item.getName() << " added to inventory" << std::endl;
		PlaySound(TEXT("./Sounds/item.wav"), NULL, SND_SYNC);
		item.posX = OUT_OF_RANGE;
		item.posY = OUT_OF_RANGE;
	}
	else if (item.getType() == T_Weapon)
	{
		if (mInventory.size() <= T_Weapon)
			mInventory.push_back(item);
		else
			mInventory[T_Weapon] = item;
		std::cout << "item: " << item.getName() << " added to inventory" << std::endl;
		PlaySound(TEXT("./Sounds/item.wav"), NULL, SND_SYNC);
		item.posX = OUT_OF_RANGE;
		item.posY = OUT_OF_RANGE;
	}
	else if (item.getType() == T_Consumable)
	{
		if (mInventory.size() >= MAX_INVENTORY_SIZE)
			mInventory.pop_back();
		std::cout << "item: " << item.getName() << " added to inventory" << std::endl;
		PlaySound(TEXT("./Sounds/item.wav"), NULL, SND_SYNC);
		item.posX = OUT_OF_RANGE;
		item.posY = OUT_OF_RANGE;
		mInventory.push_back(item);
		if (mInventory.size() >= POTION_START_INDEX)
			std::sort(mInventory.begin() + POTION_START_INDEX, mInventory.end(), greater);
	}
};

void Character::calculateStats()
{
	int sumMaxHP=0;
	int sumAP=0;
	for (int item = 0; item <= T_Weapon; ++item)
	{
		sumMaxHP = sumMaxHP + mInventory[item].getMaxHP();
		sumAP = sumAP + mInventory[item].getAP();
	}
	mMaxHP = mBaseMaxHP + sumMaxHP;
	mAP = mBaseAP + sumAP ;

	if (mHP > mMaxHP)//if hp over maxhp
		mHP = mMaxHP;
};

Equipment Character::selectItem(int index)
{
	return mInventory[index];
};

void Character::removeFromInventory(int index)
{
	mInventory.erase(mInventory.begin() + index);
	if (mInventory.size()>4)
		std::sort(mInventory.begin() + 3, mInventory.end(), greater);
};

void Character::walk(int y, int x)
{

	if (((heroRow + y) < gHeight) && ((heroCol + x) < gWidth) && ((heroRow + y) >= 0) && ((heroCol + x) >= 0))//if inside limits
	{
		if (lvl.grid[heroRow+y][heroCol+x] != L_L){
			PlaySound(TEXT("./Sounds/step.wav"), NULL, SND_ASYNC);
			heroRow = (heroRow + y);
			heroCol = (heroCol + x);
		}
		
	}
	/*ilumante adyacent places mark visited*/
	for (int vRow = (heroRow - 1); vRow <= (heroRow + 1); ++vRow)
	{
		for (int vCol = (heroCol - 1); vCol <= (heroCol + 1); ++vCol)
		{
			if (vRow < gHeight && vCol < gWidth && vRow >= 0 && vCol >= 0)//if inside limits
				gExplored.grid[vRow][vCol] = 1;
		}
	}
};

void Character::setCurrentEnemy(std::string current)
{
	mCurrentEnemy = current;
}

std::string Character::getCurrentEnemy()
{
	return mCurrentEnemy;
};
