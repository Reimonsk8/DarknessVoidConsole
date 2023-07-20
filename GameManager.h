#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "Generator.h"
#include  "Character.h"

class GameManager
{
public:

	static void readFiles();

	static void startMenu(Character &hero);

	static int decode(int code);

	static void handleInput(Character &hero, Generator &generated);

	static void handleEvent(Character &hero, Generator &generated, const int code);

};
#endif //GAMEMANAGER_H