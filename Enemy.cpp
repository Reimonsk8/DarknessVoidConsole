#include "Enemy.h"
#include "common.h"
#include "Constants.h"
#include "graphics.h"
#include <string>
#include <iostream>
#include <Windows.h>

Enemy::Enemy(std::string name, int ap, int hp, int flee)
	:mName(name), mAP(ap), mHP(hp), mFlee(flee)
{};
Enemy::~Enemy()
{};
std::string Enemy::getName()
{
	return mName;
};

int Enemy::getHP()
{
	return mHP;
};

void Enemy::setHP(int damage)
{
	mHP = mHP - damage;
};

int Enemy::getAP()
{
	return mAP;
};

bool Enemy::tryFlee()
{
	int random_value = rand() % Random::FLEE_CHANCE_MAX + 1;
	if (random_value > mFlee)
		return true;
	else return false;
}

void Enemy::heroAtacked(Character &hero, Enemy *current)
{
		hero.enemyDamaged = false;
		PlaySound(TEXT("./Sounds/claw.wav"), NULL, SND_SYNC);
		std::cout << "Enemy " << current->getName() << " atacked you dealing ";
		SetConsoleTextAttribute(hConsole, C_Red);
		std::cout << current->getAP();
		hero.setHP(-current->getAP());//deal damage to player (subtract HP) 
		SetConsoleTextAttribute(hConsole, C_White);
		std::cout << " damage, now you have ";
		SetConsoleTextAttribute(hConsole, C_Green);
		std::cout << hero.getHP();
		SetConsoleTextAttribute(hConsole, C_White);
		std::cout << " Health left" << std::endl;
		std::cout << std::endl;
		if (hero.getHP() <= 0)// game over event
		{
			PlaySound(TEXT("./Sounds/death.wav"), NULL, SND_SYNC);
			std::cout << "YOU DIED!!! \n your body lies in a pool of blood while the enemy eat's your bones and flesh...";
			Sleep(Timing::SOUND_DELAY_MS);
			gameOver();
		}
}

void Enemy::enemyAtacked(Character &hero, Enemy *current)
{
	hero.enemyDamaged = true;
	markScreenForUpdate(); // Force screen update to show damaged enemy graphic
	drawScreen(hero);
	std::cout << "you atacked " << current->getName() << " using your" << std::endl;
	SetConsoleTextAttribute(hConsole, C_Yellow);
	std::cout << hero.selectItem(2).getName() << std::endl;
	SetConsoleTextAttribute(hConsole, C_Red);
	PlaySound(TEXT("./Sounds/hit.wav"), NULL, SND_SYNC);
	std::cout << "dealing " << hero.getAP() << " damage ";
	current->setHP(hero.getAP());//enemy hp - hero AP damage
	SetConsoleTextAttribute(hConsole, C_White);
	std::cout << "enemy now has ";
	SetConsoleTextAttribute(hConsole, C_BGreen);
	std::cout << current->getHP();
	SetConsoleTextAttribute(hConsole, C_White);
	std::cout << " Health left" << std::endl;
	Sleep(Timing::SOUND_DELAY_MS);
	if (current->getHP() <= 0)
	{
		PlaySound(TEXT("./Sounds/win.wav"), NULL, SND_SYNC);
		std::cout << "Congratulations " << current->getName() << " defeated" << std::endl;
		lvl.grid[hero.heroRow][hero.heroCol] = 'O';//remove enemy
		current->posX = OUT_OF_RANGE;
		current->posY = OUT_OF_RANGE;
		Sleep(Timing::SOUND_DELAY_MS);//system("pause");
		hero.enemyDamaged = false;
		gFlee = true;
		markScreenForUpdate(); // Force screen update to return to map view
		//delete current;
	}
	else
		heroAtacked(hero, current);
};