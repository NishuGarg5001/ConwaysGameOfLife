#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <iostream>
#include <vector>
#include <deque>
#include <unordered_map>
#include <algorithm>
#include <optional>
#include <array>
#include <cmath>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>


//screen dimensions
constexpr size_t SCREEN_WIDTH  = 1366;
constexpr size_t SCREEN_HEIGHT = 768;

//font
constexpr const char* FONT_PATH = "assets/VT323-Regular.ttf";
constexpr float FONT_SIZE = 24;

//time constants
constexpr Uint64 TICK = 1000;

//menu dimensions
constexpr size_t MAIN_MENU_BOX_WIDTH = 120;
constexpr size_t MAIN_MENU_BOX_HEIGHT = static_cast<size_t>(FONT_SIZE * 2.0f + 0.2f * FONT_SIZE);
constexpr size_t PAUSE_MENU_BOX_WIDTH = 205;
constexpr size_t PAUSE_MENU_BOX_HEIGHT = static_cast<size_t>(FONT_SIZE * 3.0f + 0.2f * FONT_SIZE);
constexpr size_t SEED_MENU_BOX_WIDTH = 205;
constexpr size_t SEED_MENU_BOX_HEIGHT = static_cast<size_t>(FONT_SIZE * 2.0f + 0.2f * FONT_SIZE);

//pixel size
constexpr size_t PIXEL_SIZE = 16;

//grid size
constexpr size_t GRID_HEIGHT = 8;
constexpr size_t GRID_WIDTH = 8;
constexpr size_t CELLS = GRID_HEIGHT * GRID_WIDTH;
static_assert(CELLS<=64, "Grid cannot be larger than 8x8");
constexpr size_t LAST_STATE_INDEX = CELLS < 64 ? (size_t(1) << CELLS) - 1 : 18446744073709551615;
//18446744073709551615 = 2^64 - 1

//probability
constexpr float p = 0.1;

//colors
constexpr SDL_Color WHITE = {255, 255, 255, 255};
constexpr SDL_Color BLACK = {0, 0, 0, 255};
constexpr SDL_Color BROWN = {165, 42, 42, 255};
constexpr SDL_Color YELLOW = {255, 255, 0, 255};
constexpr SDL_Color ORANGE = {255, 165, 0, 255};
constexpr SDL_Color RED = {255, 0, 0, 255};
constexpr SDL_Color INVENTORY_BOX_COLOR = {187, 117, 71, 255};
constexpr SDL_Color INVENTORY_LINE_COLOR = {91, 49, 56, 255};

//theme
constexpr std::string theme = "white";

enum class SimulationState : int
{
    QUIT,
    RUNNING,
    MAIN,
    PAUSE,
    SEED
};

#endif