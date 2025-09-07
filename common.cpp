#include <string>
#include "common.h"
#include "GameState.h"

short gValue;
//std::string gInput = "";
bool gFlag = false; // for clearing screen porpouses
bool gInstructions = true;
bool gScreenNeedsUpdate = true; // Flag to control screen redraw frequency
short gHeight = 30;
short gWidth = 30;
short gMaxHP = 100;
short gAP = 10;
bool gFlee = true;
Map lvl;
Map gExplored;