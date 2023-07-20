#include "Equipment.h"
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
		randomValue = 1;
	}
	else //generate random rarity
	{
        //REVIEW [CTRLS][CONVENTION][Karla]: if-else statements' body must be enclosed by curly braces, even if they are one-liners.
		randomValue = rand() % 13 + 1;
		if (randomValue >= 12)
			mRarity = "legendary ";
		else if (randomValue >= 8)
			mRarity = "rare      ";
		else
			mRarity = "common";

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

