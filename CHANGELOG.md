V 3.1
-Fixed seed notation to correctly show row x column x seed hex instead of wrongly showing column x row x seed hex
-Switched the controls of W and S keys, W now decreases grid height and S increases grid height, thus aligning the controls
to visual placement of the grid
-Added Jupyter Notebook to analyze seed params easily and in an interactive environment, and dump them into CSV files

V 3.0
-Added various params, accessed by pressing P.
-Added functionality to skip seeds in chunks of 0xFFFF by pressing up/down key.
-Expanded seed menu to enter larger seeds

V 2.5
-Bug fixes

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