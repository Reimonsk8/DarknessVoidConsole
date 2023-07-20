#ifndef CHARACTER_H
#define CHARACTER_H
#include <string>
#include <vector>
#include "Equipment.h"
#include "common.h"

class Character
{
public:
	int heroRow;
	int heroCol;
	bool enemyDamaged;

	Character(); //int gHeight, int gWidht,Map &gMap
	~Character();

	void setName(std::string name);
	std::string getName();

	void setBaseValues(int baseAP, int baseMaxHP);

	void setHP(int hp);
	int getHP();

	void setMaxHP(int maxhp);
	int getMaxHP();

	void setAP(int ap);
	int getAP();

	int inventorySize();
	virtual void addToInventory(Equipment &item);
	Equipment selectItem(int index);
	void removeFromInventory(int index);
	
	void walk(int y, int x);
	void calculateStats();

	void setCurrentEnemy(std::string current);
	std::string getCurrentEnemy();

private:

	std::vector<Equipment> mInventory;
	std::string mName;
	int mMaxHP;
	int mHP;
	int mAP;

	int mBaseAP;
	int mBaseMaxHP;
	std::string mCurrentEnemy;
};

#endif //CHARACTER_H