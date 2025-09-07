#include "Equipment.h"
#include "Constants.h"
//#include "common.h"
#include <iostream>
#include <string>
#include <time.h>

Equipment::Equipment(std::string name, int ap, int maxhp, bool fixed)
	:mAP(ap), mMaxHP(maxhp)
{
	if (fixed)
	{
		mName = "          " + name;
		mRarity = "common";
		mRandomValue = 1;
	}
	else //generate random rarity
	{
		mRandomValue = rand() % Random::RARITY_CHANCE_MAX + 1;
		if (mRandomValue >= Random::RARITY_LEGENDARY_THRESHOLD) {
			mRarity = "legendary ";
		}
		else if (mRandomValue >= Random::RARITY_RARE_THRESHOLD) {
			mRarity = "rare      ";
		}
		else {
			mRarity = "common";
		}

		if (!(mRarity == "common"))
			mName = mRarity + name;
		else
			mName = "          " + name;
	}

};

Equipment::~Equipment()
{}

int Equipment::getType()
{
	return mType;
};

std::string Equipment::getName()
{
	return mName;
}
void Equipment::setAP(int ap)
{
	mAP = ap;
}

int Equipment::getAP()
{
	return mAP;
}

void Equipment::setMaxHP(int maxhp)
{
	mMaxHP = maxhp;
};

int Equipment::getMaxHP()
{
	return mMaxHP;
}

