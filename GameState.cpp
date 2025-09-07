#include "GameState.h"
#include <iostream>
#include <cstring>

GameState& GameState::getInstance()
{
	static GameState instance;
	return instance;
}

char GameState::getMapCell(int row, int col) const
{
	if (row >= 0 && row < mHeight && col >= 0 && col < mWidth) {
		return mLevel.grid[row][col];
	}
	return 'X'; // Return wall for out of bounds
}

void GameState::setMapCell(int row, int col, char value)
{
	if (row >= 0 && row < mHeight && col >= 0 && col < mWidth) {
		mLevel.grid[row][col] = value;
	}
}

bool GameState::isExplored(int row, int col) const
{
	if (row >= 0 && row < mHeight && col >= 0 && col < mWidth) {
		return mExplored.grid[row][col] == 1;
	}
	return false;
}

void GameState::setExplored(int row, int col, bool explored)
{
	if (row >= 0 && row < mHeight && col >= 0 && col < mWidth) {
		mExplored.grid[row][col] = explored ? 1 : 0;
	}
}

void GameState::initializeMap()
{
	// Initialize level map with walls
	for (int row = 0; row < mHeight; ++row) {
		for (int col = 0; col < mWidth; ++col) {
			mLevel.grid[row][col] = 'X'; // Unexplored
		}
	}
	
	// Initialize explored map
	clearExploredMap();
}

void GameState::clearExploredMap()
{
	// Clear explored map
	for (int row = 0; row < mHeight; ++row) {
		for (int col = 0; col < mWidth; ++col) {
			mExplored.grid[row][col] = 0;
		}
	}
}
