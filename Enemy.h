#ifndef ENEMY_H
#define ENEMY_H
#include "Character.h"
#include <string>

class Enemy
{
public:

	int posY;
	int posX;

	Enemy(std::string name, int ap, int hp, int flee);//constructor sets all enemy values on creation
	~Enemy();
	
	std::string getName();

	int getHP();
	void setHP(int damage);//enemy recibes damage by this method
	
	int getAP();//enemy damages player by this method
	
	bool tryFlee();

	static void enemyAtacked(Character &hero, Enemy *current);
	static void heroAtacked(Character &hero, Enemy *current);

private:

	std::string mName;
	int mHP;
	int mAP;
	int mFlee;// to flee value should be higher than this max value should be equal to 11
	
};

#endif //ENEMY_H