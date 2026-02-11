#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <array>
#include <set>
#include <fstream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>


//screen dimensions
constexpr size_t SCREEN_WIDTH  = 1366;
constexpr size_t SCREEN_HEIGHT = 768;

//font
constexpr const char* FONT_PATH = "assets/VT323-Regular.ttf";
constexpr float FONT_SIZE = 24;

//time constants
constexpr Uint64 TICK = 100;

//menu dimensions
constexpr size_t MAIN_MENU_BOX_WIDTH = 120;
constexpr size_t MAIN_MENU_BOX_HEIGHT = static_cast<size_t>(FONT_SIZE * 2.0f + 0.2f * FONT_SIZE);
constexpr size_t PAUSE_MENU_BOX_WIDTH = 205;
constexpr size_t PAUSE_MENU_BOX_HEIGHT = static_cast<size_t>(FONT_SIZE * 4.0f + 0.2f * FONT_SIZE);
constexpr size_t SEED_MENU_BOX_WIDTH = 700;
constexpr size_t SEED_MENU_BOX_HEIGHT = static_cast<size_t>(FONT_SIZE * 2.0f + 0.2f * FONT_SIZE);

//pixel size
constexpr size_t PIXEL_SIZE = 16;

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

const std::unordered_map<char, std::string> hex_to_binary_map
{
    {'0', "0000"},
    {'1', "0001"},
    {'2', "0010"},
    {'3', "0011"},
    {'4', "0100"},
    {'5', "0101"},
    {'6', "0110"},
    {'7', "0111"},
    {'8', "1000"},
    {'9', "1001"},
    {'A', "1010"},
    {'B', "1011"},
    {'C', "1100"},
    {'D', "1101"},
    {'E', "1110"},
    {'F', "1111"},
};

const std::unordered_map<char, int> hex_to_decimal_map
{
    {'0', 0},
    {'1', 1},
    {'2', 2},
    {'3', 3},
    {'4', 4},
    {'5', 5},
    {'6', 6},
    {'7', 7},
    {'8', 8},
    {'9', 9},
    {'A', 10},
    {'B', 11},
    {'C', 12},
    {'D', 13},
    {'E', 14},
    {'F', 15},
};

const std::unordered_map<int, char> decimal_to_hex_map
{
    {0, '0'},
    {1, '1'},
    {2, '2'},
    {3, '3'},
    {4, '4'},
    {5, '5'},
    {6, '6'},
    {7, '7'},
    {8, '8'},
    {9, '9'},
    {10, 'A'},
    {11, 'B'},
    {12, 'C'},
    {13, 'D'},
    {14, 'E'},
    {15, 'F'},
};

#endif