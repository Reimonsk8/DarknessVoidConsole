#ifndef EQUIPMENT_H
#define EQUIPMENT_H
#include <string>

class Equipment
{
public:
	
	int posY;
	int posX;

	Equipment(std::string name, int ap, int maxhp, bool fixed );
	~Equipment();

	std::string getName();
	int getAP();
	void setAP(int ap);

	int getMaxHP();
	void setMaxHP(int maxhp);

	int getType();

protected:

	std::string mName;
	int mType; //enum weapon, wearable or consumable
	std::string mRarity; //common, rare or legendary

    //REVIEW [VAR][NAME][Karla]: 'm' prefix for private/protected member variables.
	int randomValue;

    //REVIEW [VAR][NAME][Karla]: Variable should be named mGainedAttackPoints instead of commenting what it is.
	int mAP;//atack points gained by equipment
	int mMaxHP;//max health gained by equipment
	
};

static bool greater(Equipment &a, Equipment &b){
	return a.getMaxHP() > b.getMaxHP();
}

#endif //EQUIPMENT_H