V 2.0
-Switched to hex representation for seed number
-Program is now independent of number of possible states, limits overflow by checking size of seed number with cells,
ensuring that seed number is a cell-bit hex number.
-Dynamic grid, scale with WASD.
-Dynamic theme, switch from pause menu.
-Updated README.md
-Added CHANGELOG.md
-Max seed value not tested

V 1.0
-Conway's Game of Life simulator
-Static grid, change in constants.h and re-build
-Static theme, change in constants.h and re-build
-Decimal representation for seed number
-Max grid size limited to 8x8 due 64-bit unsigned int limit for representing number of possible states 2^(cells)