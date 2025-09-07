#include <Windows.h>
#include <fstream>
#include "graphics.h"
#include "Character.h"
#include "common.h"
#include "Constants.h"

// Global screen buffer for double buffering
static ScreenBuffer screenBuffer = { nullptr, {0, 0}, {0, 0}, {0, 0, 0, 0}, false };

void initScreenBuffer()
{
    if (screenBuffer.initialized) return;
    
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    
    screenBuffer.bufferSize.X = csbi.dwSize.X;
    screenBuffer.bufferSize.Y = csbi.dwSize.Y;
    screenBuffer.bufferCoord.X = 0;
    screenBuffer.bufferCoord.Y = 0;
    screenBuffer.writeRegion.Left = 0;
    screenBuffer.writeRegion.Top = 0;
    screenBuffer.writeRegion.Right = csbi.dwSize.X - 1;
    screenBuffer.writeRegion.Bottom = csbi.dwSize.Y - 1;
    
    // Allocate buffer memory
    screenBuffer.buffer = new CHAR_INFO[screenBuffer.bufferSize.X * screenBuffer.bufferSize.Y];
    
    // Initialize buffer with spaces
    clearBuffer();
    
    screenBuffer.initialized = true;
}

void cleanupScreenBuffer()
{
    if (screenBuffer.buffer) {
        delete[] screenBuffer.buffer;
        screenBuffer.buffer = nullptr;
    }
    screenBuffer.initialized = false;
}

void writeToBuffer(int x, int y, char character, WORD attributes)
{
    if (!screenBuffer.initialized || x < 0 || y < 0 || 
        x >= screenBuffer.bufferSize.X || y >= screenBuffer.bufferSize.Y) {
        return;
    }
    
    int index = y * screenBuffer.bufferSize.X + x;
    screenBuffer.buffer[index].Char.AsciiChar = character;
    screenBuffer.buffer[index].Attributes = attributes;
}

void swapBuffers()
{
    if (!screenBuffer.initialized) return;
    
    WriteConsoleOutput(hConsole, screenBuffer.buffer, screenBuffer.bufferSize, 
                      screenBuffer.bufferCoord, &screenBuffer.writeRegion);
}

void clearBuffer()
{
    if (!screenBuffer.initialized) return;
    
    for (int i = 0; i < screenBuffer.bufferSize.X * screenBuffer.bufferSize.Y; i++) {
        screenBuffer.buffer[i].Char.AsciiChar = ' ';
        screenBuffer.buffer[i].Attributes = 0;
    }
}

// Helper function to write string to buffer
void writeStringToBuffer(int x, int y, const std::string& str, WORD attributes)
{
    for (size_t i = 0; i < str.length(); i++) {
        writeToBuffer(x + i, y, str[i], attributes);
    }
}

void cls(bool invCLS)
{
	// Use efficient console clearing instead of system("cls") to reduce flashing
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	
	// Fill the entire screen buffer with spaces
	DWORD charsWritten;
	COORD coord = {0, 0};
	FillConsoleOutputCharacter(hConsole, ' ', csbi.dwSize.X * csbi.dwSize.Y, coord, &charsWritten);
	
	// Reset cursor to top-left
	SetConsoleCursorPosition(hConsole, coord);
}

void clearScreenArea(int startX, int startY, int width, int height)
{
	// Clear only a specific area of the screen for more efficient updates
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	
	DWORD charsWritten;
	COORD coord = {startX, startY};
	FillConsoleOutputCharacter(hConsole, ' ', width * height, coord, &charsWritten);
}

void setCursorPosition(int x, int y)
{
	// Set cursor position for efficient text output
	COORD coord = {x, y};
	SetConsoleCursorPosition(hConsole, coord);
}

void hideCursor()
{
	// Hide the cursor to reduce visual noise during updates
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void markScreenForUpdate()
{
	// Mark that the screen needs to be redrawn
	gScreenNeedsUpdate = true;
}

void redraw(int y, int x)
{
	// This function appears to be unused or legacy code
	// The double buffer system handles all screen updates now
	// If this function is still needed, it should be updated to use the buffer system
}



void drawScreen(Character &hero, bool inventory)
{
	// Only redraw if screen needs update (reduces flashing)
	// But always redraw during battle (!gFlee) or inventory mode
	if (!gScreenNeedsUpdate && !inventory && gFlee) {
		return;
	}
	
	// Hide cursor during update to reduce visual noise
	hideCursor();
	
	// Use efficient clearing
	cls(inventory);
	
	// Reset the update flag
	gScreenNeedsUpdate = false;
	
	// Draw instructions
	if (gInstructions)
	{
		SetConsoleTextAttribute(hConsole, defaultTextColor);
		std::cout << "############################### 'I' key Toogle  Instructions: ###################" << std::endl;
		std::cout << "##           arrow keys to move # enter key to interact 'F' key to inspect     ##" << std::endl;
		std::cout << "##           C=Character, E=Enemy, P=Potions, W=Weapon, A=Armor.               ##" << std::endl;
		std::cout << "#######################################################################" << std::endl;
	}
	/*draw map*/
	if (gFlee)//if not in battle print map
	{
		for (int row = 0; row < gHeight; ++row)
		{
			for (int col = 0; col < gWidth; ++col)
			{
				if (row == hero.heroRow && col == hero.heroCol)//draw Hero position
				{
					SetConsoleTextAttribute(hConsole, B_Hero);
					std::cout << "[C]";
				}
				else
				{
					WORD color = defaultTextColor;
					std::string display = "";
					
					switch (lvl.grid[row][col])
					{
					case(L_L) ://Walls
					{
						if (gExplored.grid[row][col] == 1)
							color = B_Walls;
						else
							color = notExplored;
						display = "[" + std::string(1, lvl.grid[row][col]) + "]";
					}break;

					case(L_E) ://enemys
					{
						if (gExplored.grid[row][col] == 1)
							color = B_Enemy;
						else
							color = notExplored;
						display = "[" + std::string(1, lvl.grid[row][col]) + "]";
					}break;

					case(L_W) ://item
					{
						if (gExplored.grid[row][col] == 1)
							color = 126;
						else
							color = notExplored;
						display = " " + std::string(1, lvl.grid[row][col]) + " ";
					}break;

					case(L_H) ://helmet
					{
						if (gExplored.grid[row][col] == 1)
							color = B_Gear;
						else
							color = notExplored;
						display = " " + std::string(1, lvl.grid[row][col]) + " ";
					}break;

					case(L_A) ://armor
					{
						if (gExplored.grid[row][col] == 1)
							color = B_Gear;
						else
							color = notExplored;
						display = " " + std::string(1, lvl.grid[row][col]) + " ";
					}break;

					case(L_P) ://potions
					{
						if (gExplored.grid[row][col] == 1)
							color = B_Potion;
						else
							color = notExplored;
						display = " " + std::string(1, lvl.grid[row][col]) + " ";
					}break;

					case(L_X) ://unexplored
					{
						if (gExplored.grid[row][col] == 1)
						{
							lvl.grid[row][col] = 'O';
							color = B_Floor;
						}
						else
							color = notExplored;
						display = " " + std::string(1, lvl.grid[row][col]) + " ";
					}break;

					case(L_O) ://explored
					{
						if (gExplored.grid[row][col] == 1)
							color = B_Floor;
						else
							color = notExplored;
						display = " " + std::string(1, lvl.grid[row][col]) + " ";
					}break;

					case(L_S) ://start point
					{
						if (gExplored.grid[row][col] == 1)
							color = 112;
						else
							color = notExplored;
						display = " " + std::string(1, lvl.grid[row][col]) + " ";
					}break;

					case(L_F) ://finish point
					{
						if (gExplored.grid[row][col] == 1)
							color = 125;
						else
							color = notExplored;
						display = " " + std::string(1, lvl.grid[row][col]) + " ";
					}break;

					default:
					{
						if (gExplored.grid[row][col] == 1)
							color = defaultTextColor;
						else
							color = notExplored;
						display = "[" + std::string(1, lvl.grid[row][col]) + "]";
					}break;
					}
					
					SetConsoleTextAttribute(hConsole, color);
					std::cout << display;
				}
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	else if (!gFlee)// if battle event going on print current enemy 
	{
		std::string fileName = "./Graphics/"+hero.getCurrentEnemy()+".txt";
		std::ifstream file(fileName);
		if (!file.is_open()) {
			std::cerr << "Error: Could not open enemy graphics file: " << fileName << std::endl;
			std::cout << "Enemy: " << hero.getCurrentEnemy() << std::endl;
		} else {
			std::string str;
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			GetConsoleScreenBufferInfo(hConsole, &csbi);
			int screenWidth = csbi.dwSize.X;
			
			if (hero.enemyDamaged)
			{
				SetConsoleTextAttribute(hConsole, C_Red);
				while (std::getline(file, str)) {
					// Pad the line to full screen width to ensure complete coverage
					str.resize(screenWidth, ' ');
					std::cout << str << std::endl;
				}
			}
			else
			{
				SetConsoleTextAttribute(hConsole, defaultTextColor);
				while (std::getline(file, str)) {
					// Pad the line to full screen width to ensure complete coverage
					str.resize(screenWidth, ' ');
					std::cout << str << std::endl;
				}
			}
			file.close();
		}
	}

	// Draw status and inventory
	SetConsoleTextAttribute(hConsole, defaultTextColor);
	
	// Get screen width for proper padding
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	int screenWidth = csbi.dwSize.X;
	
	// Create properly padded status line
	std::string statusLine = "############################### Status ##############################################";
	statusLine.resize(screenWidth, '#');
	std::cout << statusLine << std::endl;
	
	SetConsoleTextAttribute(hConsole, C_Blue);
	std::cout << "N:" << hero.getName();
	SetConsoleTextAttribute(hConsole, C_BGreen);
	std::cout << " Hp: " << hero.getHP() << "/" << hero.getMaxHP();
	SetConsoleTextAttribute(hConsole, C_Red);
	std::cout << " Ap: " << hero.getAP();
	// Pad the rest of the line with spaces
	std::string padding(screenWidth - 20, ' '); // Approximate padding needed
	std::cout << padding << std::endl;
	
	SetConsoleTextAttribute(hConsole, defaultTextColor);
	// Create properly padded inventory line
	std::string inventoryLine = "############################## Inventory ############################################";
	inventoryLine.resize(screenWidth, '#');
	std::cout << inventoryLine << std::endl;
	
	if (hero.inventorySize() > 0)
	{
		for (int item = 0; item < hero.inventorySize(); ++item)
		{
			if (hero.selectItem(item).getType() != T_Consumable)//color weapons yellow and print info
			{
				SetConsoleTextAttribute(hConsole, C_Yellow);
				std::string itemLine = hero.selectItem(item).getName() + " ";
				if (hero.selectItem(item).getAP() > 0)
				{
					SetConsoleTextAttribute(hConsole, C_Red);
					itemLine += "+AP: " + std::to_string(hero.selectItem(item).getAP()) + " ";
				}
				if (hero.selectItem(item).getMaxHP() > 0)
				{
					SetConsoleTextAttribute(hConsole, C_Green);
					itemLine += "+maxHP: " + std::to_string(hero.selectItem(item).getMaxHP()) + " ";
				}
				// Pad to full screen width
				itemLine.resize(screenWidth, ' ');
				std::cout << itemLine << std::endl;
			}
			else if (hero.selectItem(item).getType() == T_Consumable)//color potions and print info
			{
				SetConsoleTextAttribute(hConsole, C_Pink);
				std::string itemLine = "(" + std::to_string(item - 2) + ") " + hero.selectItem(item).getName() + " ";
				if (hero.selectItem(item).getAP() > 0)
				{
					SetConsoleTextAttribute(hConsole, C_Red);
					itemLine += "+AP: " + std::to_string(hero.selectItem(item).getAP()) + " ";
				}
				if (hero.selectItem(item).getMaxHP() > 0)
				{
					SetConsoleTextAttribute(hConsole, C_BGreen);
					itemLine += "+HP: " + std::to_string(hero.selectItem(item).getMaxHP()) + " ";
				}
				// Pad to full screen width
				itemLine.resize(screenWidth, ' ');
				std::cout << itemLine << std::endl;
			}
		}
	}
	else
	{
		SetConsoleTextAttribute(hConsole, defaultTextColor);
		std::string emptyLine = "###                                                                               ###";
		emptyLine.resize(screenWidth, '#');
		std::cout << emptyLine << std::endl;
	}
	
	SetConsoleTextAttribute(hConsole, defaultTextColor);
	std::string finalLine = "#####################################################################################";
	finalLine.resize(screenWidth, '#');
	std::cout << finalLine << std::endl;
};

#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>

void printTitle() {
	gFlee = false;
	const int MAX_LINES = 20;
	PlaySound(TEXT("./Sounds/intro.wav"), NULL, SND_ASYNC);

	std::ifstream titleFile("./Graphics/title.txt");
	if (!titleFile.is_open()) {
		std::cerr << "Error: Could not open title graphics file!" << std::endl;
		std::cerr << "Please ensure ./Graphics/title.txt exists." << std::endl;
		return;
	}

	// Get screen width for proper padding
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	int screenWidth = csbi.dwSize.X;

	for (int linesRemaining = MAX_LINES; linesRemaining > 0; --linesRemaining) {
		cls();

		for (int i = 0; i < linesRemaining; ++i) {
			std::cout << std::endl;
		}

		std::string line;
		while (std::getline(titleFile, line)) {
			// Pad the line to full screen width to ensure complete coverage
			line.resize(screenWidth, ' ');
			std::cout << line << std::endl;
		}

		titleFile.clear();
		titleFile.seekg(0); // Reset file pointer to beginning

		Sleep(Timing::STEP_DELAY_MS);
	}

	gFlee = true;
}


void gameOver()//animation of game over
{
	gFlee = false;
	int times = 0;
	bool end = false;
	SetConsoleTextAttribute(hConsole, C_Red);
	PlaySound(TEXT("./Sounds/gameover.wav"), NULL, SND_SYNC);
	
	// Get screen width for proper padding
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	int screenWidth = csbi.dwSize.X;
	
	while (!end)
	{
		cls();
		std::cout << "YOU DIED!!! \n your body lies in a pool of blood while the enemy eat's your bones and flesh...";
		for (int x = 0; x <= times;++x)
			std::cout << std::endl;
		std::ifstream file("./Graphics/gameover.txt");
		if (!file.is_open()) {
			std::cerr << "Error: Could not open game over graphics file!" << std::endl;
			std::cout << "GAME OVER" << std::endl;
		} else {
			std::string str;
			while (std::getline(file, str)) {
				// Pad the line to full screen width to ensure complete coverage
				str.resize(screenWidth, ' ');
				std::cout << str << std::endl;
			}
			file.close();
		}
		Sleep(Timing::ANIMATION_DELAY_MS);
		++times;
		if (times >= Timing::ANIMATION_FRAMES)
			end = true;
	}
	exit(EXIT_SUCCESS);
}

void victory()//animation of victory
{
	gFlee = false;
	int times = 0;
	bool end = false;
	SetConsoleTextAttribute(hConsole, C_Yellow);
	PlaySound(TEXT("./Sounds/victory.wav"), NULL, SND_ASYNC);
	
	// Get screen width for proper padding
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	int screenWidth = csbi.dwSize.X;
	
	while (!end)
	{
		cls();
		std::cout << "You made it safe back home,\n victory achieved!!\n now u can rest...";
		for (int x = 0; x <= times; ++x)
			std::cout << std::endl;
		std::ifstream file("./Graphics/victory.txt");
		if (!file.is_open()) {
			std::cerr << "Error: Could not open victory graphics file!" << std::endl;
			std::cout << "VICTORY!" << std::endl;
		} else {
			std::string str;
			while (std::getline(file, str)) {
				// Pad the line to full screen width to ensure complete coverage
				str.resize(screenWidth, ' ');
				std::cout << str << std::endl;
			}
			file.close();
		}
		Sleep(Timing::ANIMATION_DELAY_MS);
		++times;
		if (times >= Timing::ANIMATION_FRAMES)
			end = true;
	}
	exit(EXIT_SUCCESS);
}
