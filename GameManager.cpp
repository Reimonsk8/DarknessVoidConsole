#include <conio.h>
#include <fstream>
#include "GameManager.h"
#include "common.h"
#include "graphics.h"


void GameManager::readFiles()
{
	std::ifstream reader("input2.txt");
	if (!reader)
	{
		std::cout << "While opening a file an error is encountered" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		reader >> gMaxHP;
		reader >> gAP;
		reader >> gHeight;
		reader >> gWidth;
		/* assign file info to map */
		for (int row = 0; row < gHeight; ++row)
		{
			for (int col = 0; col < gWidth; ++col)
			{
				reader >> lvl.grid[row][col];
				if (lvl.grid[row][col] == L_O)
				{
					lvl.grid[row][col] = 'X';
				}
			}
		}
	}
}

void GameManager::startMenu(Character &hero){ // only one time call at the beggining//only one time call at the beggining

	printTitle();
	std::string input;
	bool validate = false;
	while (!validate)
	{
		std::cout << "Welcome, enter your character name:" << std::endl;
		std::cin >> input;
		Sleep(500);
		std::string x(input);
		if ((x.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") != std::string::npos) || x.length()>20)
			std::cerr << "Error name not valid,\nplease input no more than 20 chars \n& no special characters only letters\n";
		else
		{
			hero.setName(input);
			validate = true;
		}
	}

	/*beginning gear should be set to low stats*/
	std::cout << "Hi " << hero.getName() << " now begin your journey to safety, take this gear " << std::endl;
	Equipment helmet = Helmet("hat             \t  _[]_    ", 0, 1, true);
	hero.addToInventory(helmet);
	Equipment armor = Armor("cloth shirt      \t /|__|\\   ", 0, 1, true);
	hero.addToInventory(armor);
	Equipment weapon = Weapon("knife             \t ~\=>     ", 1, 0, true);
	hero.addToInventory(weapon);
	hero.calculateStats();
	std::cout << "Good, " << hero.getName() << " are you ready ? " << std::endl;
	PlaySound(TEXT("./Sounds/begin.wav"), NULL, SND_ASYNC);
	Sleep(500);
	drawScreen(hero);
}

int GameManager::decode(int code)
{
	if (code == KEY_RIGHT)
		return 0;
	else if (code == KEY_DOWN)
		return 1;
	else if (code == KEY_LEFT)
		return 2;
	else if (code == KEY_UP)
		return 3;
}

void GameManager::handleInput(Character &hero, Generator &generated)
{
	const short deltaRow[DIRECTIONS] = { 0, 1, 0, -1 }; // right, down, left, up
	const short deltaCol[DIRECTIONS] = { 1, 0, -1, 0 };
	int code;

	//drawScreen(hero);

	switch (code = _getch())
	{
	case ACTION:
		handleEvent(hero, generated, code);
		break;
	case INSTRUCTIONS:
		gInstructions = !gInstructions;
		drawScreen(hero);
		break;

	default:

		if (code == KEY_RIGHT || code == KEY_DOWN || code == KEY_LEFT || code == KEY_UP)
		{
			const int decoded = decode(code);
			hero.walk(deltaRow[decoded], deltaCol[decoded]);
			handleEvent(hero, generated, code);
		}
		else if (code >= P_1 && code <= P_5)//use potions in map
				handleEvent(hero, generated, code);
		break;
	}
};

void GameManager::handleEvent(Character &hero, Generator &generated, const int code)
{
	hero.calculateStats();
	drawScreen(hero, gFlag);
	int currentChar = lvl.grid[hero.heroRow][hero.heroCol];//casting char to int 
	switch (currentChar)
	{
	case(L_O) ://look event
	{
		if (gFlag)
			drawScreen(hero, gFlag);
		gFlag = false;
		if (code == ACTION)
		{
			std::string value;
			if (gInstructions)
				std::cout << "input (1 or right) (2 or down)  (3 or left) (4 or up)" << std::endl;
			std::cout << "enter inspect direction: " << std::endl;
			std::cin >> value;
			if (value == "1" || value == "right")
				generated.InfoPlace(hero.heroRow, hero.heroCol + 1);
			else if (value == "2" || value == "down")
				generated.InfoPlace(hero.heroRow + 1, hero.heroCol);
			else if (value == "3" || value == "right")
				generated.InfoPlace(hero.heroRow, hero.heroCol - 1);
			else if (value == "4" || value == "up")
				generated.InfoPlace(hero.heroRow - 1, hero.heroCol);
			else
				std::cout << "wrong direction try input again (press i fron instructions)" << std::endl;
		}
		else if (code >= P_1 && code <= P_5)
			Potion::usePotion(hero, code);

	}break;


	case(L_E) :// enemy fight event
	{
		Enemy *current = generated.returnEnemy(hero.heroRow, hero.heroCol);
		std::cout << "Enemy encountered " << current->getName() << " Health " << current->getHP() << " Power " << current->getAP();
		std::cout << "FIGHT!!!" << std::endl;
		PlaySound(TEXT("./Sounds/battle.wav"), NULL, SND_ASYNC);
		std::string value;
		hero.setCurrentEnemy(current->getName());// set hero current enemy for graphic porpouses
		gFlee = false; // battle on event flee set to false, switch map screen to battle screen
		while (!gFlee)
		{
			Sleep(2500);//system(pause)
			drawScreen(hero);
			if (gInstructions)
				std::cout << "input (1 or atack) (2 or potion)  (3 or flee)" << std::endl;
			std::cout << "Your turn, select & enter action: " << std::endl;
			std::cin >> value;

			if (value == "2" || value == "potion")// potion option doesn't consume turn
			{
				Potion::usePotion(hero);
				std::cout << "input (1 or atack) (3 or flee)" << std::endl;
				std::cout << "Your turn, select & enter action: " << std::endl;
				std::cin >> value;
			}


			if (value == "1" || value == "atack")// atack enemy
			{
				Enemy::enemyAtacked(hero, current);
			}

			else if (value == "3" || value == "flee")//flee option
			{
				std::cout << "you try to flee" << std::endl;
				if (current->tryFlee())
				{
					PlaySound(TEXT("./Sounds/flee.wav"), NULL, SND_SYNC);
					std::cout << "you fleed sucesfully from battle !!!" << std::endl;
					Sleep(1000); //system("pause");//exit
					gFlee = true;
					//delete current;
				}
				else
					Enemy::heroAtacked(hero, current);
			}
			else
				std::cout << "wrong input try again (press i fron instructions)" << std::endl;
		}
		//redraw screen
		drawScreen(hero);
		gFlag = true;
	}break;


	case(L_P) ://potion event
	{
		Potion *current = generated.returnPotion(hero.heroRow, hero.heroCol);
		Potion::pickPotion(current, hero, code);
		//delete current;
		gFlag = true;
	}break;

	case(L_W) ://weapon event
	{
		Weapon *current = generated.returnWeapon(hero.heroRow, hero.heroCol);
		Weapon::pickWeapon(current, hero, code);
		//delete current;
		gFlag = true;
	}break;


	case(L_A) ://armor event
	{
		Armor *current = generated.returnArmor(hero.heroRow, hero.heroCol);
		Armor::pickArmor(current, hero, code);
		gFlag = true;
	}break;


	case(L_H) ://helmet event
	{
		Helmet *current = generated.returnHelmet(hero.heroRow, hero.heroCol);
		Helmet::pickHelmet(current, hero, code);
		//delete current;
		gFlag = true;
	}break;


	case(L_F) ://End game event
		PlaySound(TEXT("./Sounds/win.wav"), NULL, SND_SYNC);
		victory();
		break;

	default:
	{}break;
	}
}//global event handler

