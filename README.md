A simple SDL3 implementation of Conway's game of life.

Pre-requisites:
1.C++20 Compiler
2.SDL3
3.SDL3 TTF
4.SDL4 Image

Features:
-Seed indexing of initial configurations based on MxNxD
 Where M and N are the grid width and height respectively and D is the decimal representation of
 2^(MN)-bit unsigned integer.
-Max 8x8 grid size. --> Change in constants.h
-Black and White themes. --> Change in constants.h --> theme to "black" or "white"

Controls:
1.Enter to select a menu item, start/stop simulation
2.UP/DOWN arrow keys to move through menu items
3.LEFT/RIGHT arrow keys to select seeds
4.Press 'R' to reset current seed to original configuration.

Build using (g++ example):
g++ -std=c++20 main.cpp -o main.exe -L<path to libraries> -lSDL3 -lSDL3_image -lSDL3_ttf -mwindows

In my case:
g++ -std=c++20 main.cpp -o main.exe -LC:/msys64/ucrt64/lib/ -lSDL3 -lSDL3_image -lSDL3_ttf -mwindows