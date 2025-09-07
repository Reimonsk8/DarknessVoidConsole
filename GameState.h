#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "common.h"

class GameState
{
public:
	// Singleton pattern
	static GameState& getInstance();
	
	// Game configuration
	short getHeight() const { return mHeight; }
	short getWidth() const { return mWidth; }
	short getMaxHP() const { return mMaxHP; }
	short getAP() const { return mAP; }
	
	void setHeight(short height) { mHeight = height; }
	void setWidth(short width) { mWidth = width; }
	void setMaxHP(short maxHP) { mMaxHP = maxHP; }
	void setAP(short ap) { mAP = ap; }
	
	// Game state flags
	bool getInstructions() const { return mInstructions; }
	bool getFlee() const { return mFlee; }
	bool getFlag() const { return mFlag; }
	bool getScreenNeedsUpdate() const { return mScreenNeedsUpdate; }
	
	void setInstructions(bool instructions) { mInstructions = instructions; }
	void setFlee(bool flee) { mFlee = flee; }
	void setFlag(bool flag) { mFlag = flag; }
	void setScreenNeedsUpdate(bool needsUpdate) { mScreenNeedsUpdate = needsUpdate; }
	
	// Map access
	char getMapCell(int row, int col) const;
	void setMapCell(int row, int col, char value);
	bool isExplored(int row, int col) const;
	void setExplored(int row, int col, bool explored);
	
	// Map management
	void initializeMap();
	void clearExploredMap();
	
private:
	GameState() = default;
	~GameState() = default;
	GameState(const GameState&) = delete;
	GameState& operator=(const GameState&) = delete;
	
	// Game configuration
	short mHeight = 30;
	short mWidth = 30;
	short mMaxHP = 100;
	short mAP = 10;
	
	// Game state flags
	bool mInstructions = true;
	bool mFlee = true;
	bool mFlag = false;
	bool mScreenNeedsUpdate = true;
	
	// Maps
	Map mLevel;
	Map mExplored;
};

#endif //GAMESTATE_H
