#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "common.h"
#include "Character.h"
#include <iostream>
#include <Windows.h>

enum BackgroundColors { B_Hero = 26, B_Enemy = 44, B_Floor = 120, B_Potion = 116, B_Gear = 126, B_Walls = 135 };
static const int notExplored = C_Black;//should be black
static const int defaultTextColor = C_White;

static const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// Double buffering for smooth screen updates
struct ScreenBuffer {
    CHAR_INFO* buffer;
    COORD bufferSize;
    COORD bufferCoord;
    SMALL_RECT writeRegion;
    bool initialized;
};

// Initialize double buffering system
void initScreenBuffer();

// Clean up screen buffer
void cleanupScreenBuffer();

// Write to buffer instead of directly to console
void writeToBuffer(int x, int y, char character, WORD attributes);

// Swap buffers (display buffer content)
void swapBuffers();

// Clear buffer without flashing
void clearBuffer();

void cls(bool invCLS = false);

// Efficient screen update functions
void clearScreenArea(int startX, int startY, int width, int height);
void setCursorPosition(int x, int y);
void hideCursor();
void markScreenForUpdate(); // Mark that screen needs to be redrawn

void drawScreen(Character &hero, bool inventory = false);

void printTitle();

void gameOver();

void victory();
#endif //GRAPHICS_H