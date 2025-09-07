#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "Generator.h"
#include  "Character.h"

class GameManager
{
public:
	// Constructor
	GameManager();
	
	// Instance methods
	void readFiles();
	void startMenu(Character &hero);
	int decode(int code);
	void handleInput(Character &hero, Generator &generated);
	void handleEvent(Character &hero, Generator &generated, const int code);

	// Input validation helper functions
	bool isValidBattleInput(const std::string& input);
	bool isValidPotionInput(int inputNum, const Character& hero);
	
	// Singleton access
	static GameManager& getInstance();

private:
	// Private constructor for singleton
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;
};

#endif //GAMEMANAGER_H