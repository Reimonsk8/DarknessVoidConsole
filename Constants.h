#ifndef CONSTANTS_H
#define CONSTANTS_H

// Animation and timing constants
namespace Timing {
	const int BATTLE_PAUSE_MS = 2500;
	const int UI_DELAY_MS = 500;
	const int ANIMATION_DELAY_MS = 500;
	const int ANIMATION_FRAMES = 6;
	const int SOUND_DELAY_MS = 1000;
	const int STEP_DELAY_MS = 20;
	const int POTION_DELAY_MS = 2000;
}

// Random number generation constants
namespace Random {
	const int FLEE_CHANCE_MAX = 13;
	const int RARITY_CHANCE_MAX = 13;
	const int RARITY_LEGENDARY_THRESHOLD = 12;
	const int RARITY_RARE_THRESHOLD = 8;
}

// Map and game constants
namespace Game {
	const short MAX_MAP_SIZE = 30;
	const short DEFAULT_MAP_SIZE = 30;
	const short DEFAULT_MAX_HP = 100;
	const short DEFAULT_AP = 10;
}

// Inventory constants
namespace Inventory {
	const short MAX_INVENTORY_SIZE = 8;
	const short POTION_START_INDEX = 3;
	const short OUT_OF_RANGE = -1;
}

#endif //CONSTANTS_H
