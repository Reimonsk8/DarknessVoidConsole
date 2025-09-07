#include <conio.h>
#include <fstream>
#include "GameManager.h"
#include "common.h"
#include "GameState.h"
#include "Constants.h"
#include "graphics.h"
#include "resource.h"
#include <windows.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

HMODULE GCM() {
	HMODULE hModule = NULL;
	GetModuleHandleEx(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
		(LPCTSTR)GCM,
		&hModule);
	return hModule;
}

GameManager::GameManager()
{
	// Constructor - initialize any instance variables if needed
}

GameManager& GameManager::getInstance()
{
	static GameManager instance;
	return instance;
}

void GameManager::readFiles()
{

	//HRSRC hRes = FindResource(GCM(), MAKEINTRESOURCE(TEXT_FILE), MAKEINTRESOURCE(TEXTFILE));
	//HGLOBAL hData = LoadResource(GCM(), hRes);
	//DWORD hSize = SizeofResource(GCM(), hRes);
	//char* hFinal = (char*)LockResource(hData);
	//std::string result;
	//result.assign(hFinal, hSize);

	//std::istringstream iss(static_cast<const char*>(hData), hSize);

	//// Read dimensions
	//iss >> gMaxHP >> gAP;
	//iss >> gHeight >> gWidth;

	// Read grid contents
	//for (int row = 0; row < gHeight; ++row) 
	//{
	//	for (int col = 0; col < gWidth; ++col) 
	//	{
	//		char cell;
	//		iss >> cell;
	//		lvl.grid[row][col] = cell;
	//		if (cell == L_O) lvl.grid[row][col] = 'X';
	//		
	//	}
	//}


	std::ifstream reader("input2.txt");
	if (!reader)
	{
		std::cerr << "Error: Could not open input2.txt file!" << std::endl;
		std::cerr << "Please ensure the file exists in the game directory." << std::endl;
		exit(EXIT_FAILURE);
	}
	
	// Read game configuration with error checking
	GameState& gameState = GameState::getInstance();
	if (!(reader >> gMaxHP >> gAP >> gHeight >> gWidth)) {
		std::cerr << "Error: Invalid format in input2.txt file!" << std::endl;
		std::cerr << "Expected format: maxHP AP height width" << std::endl;
		reader.close();
		exit(EXIT_FAILURE);
	}
	
	// Set GameState values
	gameState.setMaxHP(gMaxHP);
	gameState.setAP(gAP);
	gameState.setHeight(gHeight);
	gameState.setWidth(gWidth);
	
	// Validate configuration values
	if (gMaxHP <= 0 || gAP <= 0 || gHeight <= 0 || gWidth <= 0 || 
		gHeight > Game::MAX_MAP_SIZE || gWidth > Game::MAX_MAP_SIZE) {
		std::cerr << "Error: Invalid configuration values!" << std::endl;
		std::cerr << "Values must be positive and map size <= " << Game::MAX_MAP_SIZE << "x" << Game::MAX_MAP_SIZE << std::endl;
		reader.close();
		exit(EXIT_FAILURE);
	}
	
	/* assign file info to map */
	for (int row = 0; row < gHeight; ++row)
	{
		for (int col = 0; col < gWidth; ++col)
		{
			if (!(reader >> lvl.grid[row][col])) {
				std::cerr << "Error: Incomplete map data in input2.txt!" << std::endl;
				std::cerr << "Expected " << (gHeight * gWidth) << " characters for map." << std::endl;
				reader.close();
				exit(EXIT_FAILURE);
			}
			if (lvl.grid[row][col] == L_O)
			{
				lvl.grid[row][col] = 'X';
			}
			// Also set in GameState
			gameState.setMapCell(row, col, lvl.grid[row][col]);
		}
	}
	
	reader.close();

}

void GameManager::startMenu(Character &hero){ // only one time call at the beggining//only one time call at the beggining

	printTitle();
	std::string input;
	bool validate = false;
	while (!validate)
	{
		std::cout << "Welcome, enter your character name:" << std::endl;
		std::cin >> input;
		Sleep(Timing::UI_DELAY_MS);
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
	Equipment weapon = Weapon("knife             \t ~\\=>     ", 1, 0, true);
	hero.addToInventory(weapon);
	hero.calculateStats();
	std::cout << "Good, " << hero.getName() << " are you ready ? " << std::endl;
	PlaySound(TEXT("./Sounds/begin.wav"), NULL, SND_ASYNC);
	Sleep(Timing::UI_DELAY_MS);
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
	else
		return -1; // Invalid key
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
		markScreenForUpdate(); // Mark screen for update instead of immediate redraw
		drawScreen(hero);
		break;

	default:

		if (code == KEY_RIGHT || code == KEY_DOWN || code == KEY_LEFT || code == KEY_UP)
		{
			const int decoded = decode(code);
			if (decoded >= 0 && decoded < DIRECTIONS) {
				hero.walk(deltaRow[decoded], deltaCol[decoded]);
				markScreenForUpdate(); // Mark screen for update after movement
				handleEvent(hero, generated, code);
			}
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
	//case(L_O) ://look event
	//{
	//	if (gFlag)
	//		drawScreen(hero, gFlag);
	//	gFlag = false;
	//	if (code == ACTION)
	//	{
	//		std::string value;
	//		if (gInstructions)
	//			std::cout << "input (1 or right) (2 or down)  (3 or left) (4 or up)" << std::endl;
	//		std::cout << "enter inspect direction: " << std::endl;
	//		std::cin >> value;
	//		if (value == "1" || value == "right")
	//			generated.InfoPlace(hero.heroRow, hero.heroCol + 1);
	//		else if (value == "2" || value == "down")
	//			generated.InfoPlace(hero.heroRow + 1, hero.heroCol);
	//		else if (value == "3" || value == "right")
	//			generated.InfoPlace(hero.heroRow, hero.heroCol - 1);
	//		else if (value == "4" || value == "up")
	//			generated.InfoPlace(hero.heroRow - 1, hero.heroCol);
	//		else
	//			std::cout << "wrong direction try input again (press i fron instructions)" << std::endl;
	//	}
	//	else if (code >= P_1 && code <= P_5)
	//		Potion::usePotion(hero, code);

	//}break;


	case(L_E) :// enemy fight event
	{
		Enemy *current = generated.returnEnemy(hero.heroRow, hero.heroCol);
		std::cout << "Enemy encountered " << current->getName() << " Health " << current->getHP() << " Power " << current->getAP();
		std::cout << "FIGHT!!!" << std::endl;
		PlaySound(TEXT("./Sounds/battle.wav"), NULL, SND_ASYNC);
		std::string value;
		hero.setCurrentEnemy(current->getName());// set hero current enemy for graphic porpouses
		gFlee = false; // battle on event flee set to false, switch map screen to battle screen
		markScreenForUpdate(); // Force screen update for battle graphics
		while (!gFlee)
		{
			Sleep(Timing::BATTLE_PAUSE_MS);//system(pause)
			drawScreen(hero);
			
			// Get and validate battle input
			bool validInput = false;
			while (!validInput) {
				if (gInstructions)
					std::cout << "input (1 or attack) (2 or potion)  (3 or flee)" << std::endl;
				std::cout << "Your turn, select & enter action: " << std::endl;
				std::cin >> value;

				if (isValidBattleInput(value)) {
					validInput = true;
				} else {
					std::cout << "Invalid input! Please enter 1 (attack), 2 (potion), or 3 (flee)" << std::endl;
				}
			}

			if (value == "2" || value == "potion")// potion option doesn't consume turn
			{
				Potion::usePotion(hero);
				
				// Get second input for attack or flee
				validInput = false;
				while (!validInput) {
					std::cout << "input (1 or attack) (3 or flee)" << std::endl;
					std::cout << "Your turn, select & enter action: " << std::endl;
					std::cin >> value;
					
					if (value == "1" || value == "attack" || value == "atack" ||
						value == "3" || value == "flee") {
						validInput = true;
					} else {
						std::cout << "Invalid input! Please enter 1 (attack) or 3 (flee)" << std::endl;
					}
				}
			}

			if (value == "1" || value == "attack" || value == "atack")// attack enemy
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
					Sleep(Timing::SOUND_DELAY_MS); //system("pause");//exit
					gFlee = true;
					//delete current;
				}
				else
					Enemy::heroAtacked(hero, current);
			}
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

bool GameManager::isValidBattleInput(const std::string& input)
{
	return (input == "1" || input == "attack" || input == "atack" ||
			input == "2" || input == "potion" ||
			input == "3" || input == "flee");
}

bool GameManager::isValidPotionInput(int inputNum, const Character& hero)
{
	return (inputNum >= 1 && inputNum <= 5 && 
			(inputNum + 2) < hero.inventorySize());
}

