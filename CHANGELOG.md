V 4.6
-Bugfixes, generation text now properly starts at 0 as is the convention. Lifetime for oscillators
is now correctly taken as generation number when periodicity starts instead of when 1st period ends. (lifetime = lifetime in previous version - period) now basically. For transients, lifetime is the generation number on which all cells die. Physically, it represents the number of generations the cells were alive. For example, if a config starts at generation 0 and completely dies on generation 5, the lifetime is 5 and physically the cells DID live for 5 generations (0, 1, 2, 3 and 4).
-Now supports 20x and 40x simulation speed.
-Updated README.md

V 4.5
-Changed epoch to generation in on-screen text
-Updated Jupyter Notebook

V 4.4
-Changed some terminologies, making them more consistent with convention

V 4.3
-Mouse optimisations
-Changing seed now requires simulation to be off first

V 4.0
-Fixed some functions to be less-clashing and more tuned to one specific task
-Added functionality to change simulation speed using numbers keys
-Simulation now shows simulation speed
-Added mouse functionality, ability to toggle cells by mouse click
-Updated README.md

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