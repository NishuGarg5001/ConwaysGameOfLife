A simple SDL3 implementation of Conway's game of life.

Pre-requisites:
1.C++20 Compiler
2.SDL3
3.SDL3 TTF
4.SDL4 Image

Features:
-Seed indexing of initial configurations based on MxNxD
 Where M and N are the grid width and height respectively and D is the hex representation of
 2^(MN)-bit unsigned integer.
-Custom seed from seed menu.
-Choose between Black and White themes from pause menu.
-Various parameters for a particular seed.

Controls:
1.Enter to select a menu item, start/stop simulation.
2.UP/DOWN arrow keys to move through menu items or increase/decrease seeds in chunks of 0xFFFF.
3.LEFT/RIGHT arrow keys to decrease/increase seeds.
4.WASD keys to adjust grid size.
5.Press 'R' to reset current seed to original configuration.
6.Press 'P' to evaluate parameters for the current seed.

Note:
Best suited for a maximum of 15x15 grid. The seeds in hexadecimal starting becoming too lengthy after that.
May shift to base32 or base64 in future for more efficient seed packing and to support even larger grid.

Build using (g++ example):
g++ -std=c++20 main.cpp -o main.exe -L<path to libraries> -lSDL3 -lSDL3_image -lSDL3_ttf -mwindows

In my case:
g++ -std=c++20 main.cpp -o main.exe -LC:/msys64/ucrt64/lib/ -lSDL3 -lSDL3_image -lSDL3_ttf -mwindows