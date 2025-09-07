#ifndef COMMON_H
#define COMMON_H

#include "Constants.h"

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define INSTRUCTIONS 105
#define ACTION 13
#define DIRECTIONS 4

enum Colors {C_Black, C_Blue, C_Green, C_Cyan, C_Red, C_Pink, C_Gray = 8, C_BGreen = 10, C_Yellow = 14, C_White};
enum Letter {L_A = 65, L_E = 69, L_F, L_H = 72, L_L = 76, L_O = 79, L_P, L_S = 83, L_W = 87, L_X, L__ = 95};
enum Type {T_Head, T_Chest, T_Weapon, T_Consumable};
enum PotionCode { P_1 = 49, P_2, P_3, P_4, P_5 };

static const short OUT_OF_RANGE = Inventory::OUT_OF_RANGE;
static const short MAX_INVENTORY_SIZE = Inventory::MAX_INVENTORY_SIZE;
static const short POTION_START_INDEX = Inventory::POTION_START_INDEX;

extern bool gInstructions;

// Forward declaration for GameState singleton
class GameState;

// Legacy global variables - will be replaced by GameState
// These are kept for backward compatibility during transition
extern short gHeight;
extern short gWidth;
extern short gMaxHP;
extern short gAP;
extern bool gFlee;
extern bool gFlag;
extern bool gScreenNeedsUpdate; // Flag to control screen redraw frequency

struct Map
{
	char grid[30][30];
};

extern Map lvl;
extern Map gExplored;

#endif //COMMON_H