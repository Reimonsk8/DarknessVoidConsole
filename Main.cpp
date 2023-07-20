
#include "Generator.h"
#include "GameManager.h"

#include <iostream>
#include <time.h>
#include <windows.h>


int main()
{

	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions

	MoveWindow(console, r.left, r.top, 900, 900, TRUE); // 800 width, 100 height
	srand(time(NULL));
	GameManager::readFiles();

	/*initialize hero & generate items and enemies*/
	Character hero;
	Generator generated;
	hero.setBaseValues(gAP, gMaxHP);

	GameManager::startMenu(hero);
	generated.SpawnInMap();

	/*maing game loop*/
	while (hero.getHP() > 0)
	{
		GameManager::handleInput(hero, generated);
	}
	//gameOver();

	return 0;
}


