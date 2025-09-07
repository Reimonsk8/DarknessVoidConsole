
#include "Generator.h"
#include "GameManager.h"
#include "graphics.h"

#include <iostream>
#include <time.h>
#include <windows.h>

#pragma comment(lib, "winmm.lib")

#define PAUSE system("pause")
int main()
{
	HMODULE hModule = NULL;
	GetModuleHandleEx(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
		(LPCTSTR)main,
		&hModule);


	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions

	MoveWindow(console, r.left, r.top, 900, 900, TRUE); // 800 width, 100 height
	
	srand(static_cast<unsigned int>(time(NULL)));
	GameManager::getInstance().readFiles();

	/*initialize hero & generate items and enemies*/
	Character hero;
	Generator generated;
	hero.setBaseValues(gAP, gMaxHP);

	GameManager::getInstance().startMenu(hero);
	generated.SpawnInMap();

	/*maing game loop*/
	while (hero.getHP() > 0)
	{
		GameManager::getInstance().handleInput(hero, generated);
	}
	// If hero dies outside of battle, show game over animation
	gameOver();
	
	PAUSE;
	return 0;
}


