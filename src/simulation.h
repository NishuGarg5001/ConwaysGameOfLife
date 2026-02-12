#ifndef SIMULATION_H
#define SIMULATION_H

#include "menu.h"

//static_assert(cells>0, "Number of cells should be greater than 0!");

std::string addHexNumbers(std::string_view s1, std::string_view s2)
{
    int carry = 0;
    int i = s1.size() - 1;
    int j = s2.size() - 1;
    std::string res = "";
    while(carry != 0 || i >= 0 || j >= 0)
    {
        int sum = carry;

        if(i >= 0)
            sum += hex_to_decimal_map.at(s1[i--]);

        if(j >= 0)
            sum += hex_to_decimal_map.at(s2[j--]);
        res.push_back(decimal_to_hex_map.at(sum % 16));
        carry = sum / 16;
    }
    std::reverse(res.begin(), res.end());
    return res;
}

std::string subtractHexNumbers(std::string s1, std::string_view s2)
{
    int i = static_cast<int>(s1.size()) - 1;
    int j = static_cast<int>(s2.size()) - 1;
    int u;
    while(j >= 0)
    {
        if((hex_to_decimal_map.at(s1[i])) < (hex_to_decimal_map.at(s2[j])))
        {
            u = i - 1;
            while(u >= 0 && s1[u] == '0')
                u--;
            s1[u] = decimal_to_hex_map.at(hex_to_decimal_map.at(s1[u]) - 1);
            while(u!=(i-1))
                s1[++u] = 'F';
            s1[i] = decimal_to_hex_map.at(hex_to_decimal_map.at(s1[i]) + 16 - hex_to_decimal_map.at(s2[j--]));
        }
        else
            s1[i] = decimal_to_hex_map.at(hex_to_decimal_map.at(s1[i]) - hex_to_decimal_map.at(s2[j--]));
        i--;
    }
    while(s1.size() >= 2 && s1[0] == '0')
        s1.erase(0, 1);
    return s1;
}

class Simulation
{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;
    std::string command = "";
    int fps = 60;
    int frame_time = 1000 / fps;
    size_t tick = 1000;
    size_t grid_width = 8;
    size_t grid_height = 8;
    size_t cells = grid_width * grid_height;
    size_t generation = 1;
    std::string lifetime = "N/A";
    std::string period = "N/A";
    std::string main_class = "N/A";
    std::string sub_class = "N/A";
    std::string seed_counter = "0";
    std::string seed_counter_binary = "0";
    bool simulation_state = false;
    bool is_black_theme = false;
    std::vector<std::vector<int>> life_grid;
    std::vector<std::vector<int>> state_buffer;
    SimulationState code_state = SimulationState::MAIN;
    Menu main_menu = Menu({"Simulate", "Quit"}, MAIN_MENU_BOX_WIDTH, MAIN_MENU_BOX_HEIGHT);
    Menu pause_menu =  Menu({"Continue", "Seed", "Switch Theme", "Quit to Main Menu"}, PAUSE_MENU_BOX_WIDTH, PAUSE_MENU_BOX_HEIGHT);
    Menu seed_menu = Menu({"Seed"}, SEED_MENU_BOX_WIDTH, SEED_MENU_BOX_HEIGHT);
    public:
        Simulation() :
        life_grid(grid_height, std::vector<int>(grid_width, 0)),
        state_buffer(grid_height, std::vector<int>(grid_width, 0))
        {
            seed_counter_binary.reserve(cells);
            seed_counter_binary = std::string(cells, '0');
        }

        void handleInput()
        {
            SDL_Event event;
            while(SDL_PollEvent(&event))
            {
                switch(code_state)
                {
                    case SimulationState::MAIN:
                    {
                        if(event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat)
                        {
                            switch(event.key.key)
                            {
                                case SDLK_UP:
                                {
                                    main_menu.moveUp();
                                    break;
                                }
                                case SDLK_DOWN:
                                {
                                    main_menu.moveDown();
                                    break;
                                }
                                case SDLK_RETURN:
                                {
                                    std::string_view main_menu_item_name = main_menu.currentItem();
                                    if(main_menu_item_name == "Simulate")
                                    {
                                        code_state = SimulationState::RUNNING;
                                        newSim();
                                    }
                                    else
                                        code_state = SimulationState::QUIT;
                                    break;
                                }
                            }
                        }
                        break;
                    }
                    case SimulationState::PAUSE:
                    {
                        if(event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat)
                        {
                            switch(event.key.key)
                            {
                                case SDLK_UP:
                                {
                                    pause_menu.moveUp();
                                    break;
                                }
                                case SDLK_DOWN:
                                {
                                    pause_menu.moveDown();
                                    break;
                                }
                                case SDLK_RETURN:
                                {
                                    std::string_view pause_menu_item_name = pause_menu.currentItem();
                                    if(pause_menu_item_name == "Continue")
                                        code_state = SimulationState::RUNNING;
                                    else if(pause_menu_item_name == "Seed")
                                    {
                                        code_state = SimulationState::SEED;
                                        SDL_StartTextInput(window);
                                    }
                                    else if(pause_menu_item_name == "Switch Theme")
                                        is_black_theme = !is_black_theme;
                                    else
                                        code_state = SimulationState::MAIN;
                                    break;
                                }
                                case SDLK_ESCAPE:
                                {
                                    code_state = SimulationState::RUNNING;
                                    break;
                                }
                            }
                        }
                        break;
                    }
                    case SimulationState::SEED:
                    {
                        if(event.type == SDL_EVENT_TEXT_INPUT)
                        {
                            if(getTextLen(command) + getTextLen(event.text.text) <= SEED_MENU_BOX_WIDTH)
                                command += event.text.text;
                        }
                        else if(event.type == SDL_EVENT_KEY_DOWN)
                        {
                            if(!event.key.repeat)
                            {
                                switch(event.key.key)
                                {
                                    case SDLK_ESCAPE:
                                    {
                                        code_state = SimulationState::PAUSE;
                                        break;
                                    }
                                    case SDLK_RETURN:
                                    {
                                        if(!command.empty() && isHex(command))
                                        {
                                            if(hexToBinary(command).size() <= cells)
                                            {
                                                setSeed(command);
                                                command.clear();
                                                SDL_StopTextInput(window);
                                                code_state = SimulationState::RUNNING;
                                            }
                                        }
                                        break;
                                    }
                                    default:
                                    {
                                        break;
                                    }
                                }
                            }

                            if(event.key.key == SDLK_BACKSPACE)
                            {
                                if(!command.empty())
                                    command.pop_back();
                            }
                        }
                        break;
                    }
                    case SimulationState::RUNNING:
                    {
                        if(event.type == SDL_EVENT_KEY_DOWN)
                        {
                            if(event.key.key == SDLK_LEFT || event.key.key == SDLK_RIGHT ||
                               event.key.key == SDLK_UP || event.key.key == SDLK_DOWN ||
                               event.key.key == SDLK_W || event.key.key == SDLK_A ||
                               event.key.key == SDLK_S || event.key.key == SDLK_D)
                            {
                                if(event.key.key == SDLK_LEFT)
                                {
                                    if(!simulation_state && seed_counter != "0")
                                        setSeed(subtractHexNumbers(seed_counter, "1"));
                                }
                                else if(event.key.key == SDLK_RIGHT)
                                {
                                    if(!simulation_state && hexToBinary(addHexNumbers(seed_counter, "1")).size() <= cells)
                                        setSeed(addHexNumbers(seed_counter, "1"));
                                }
                                else if(event.key.key == SDLK_UP)
                                {
                                    if(!simulation_state && hexToBinary(addHexNumbers(seed_counter, "FFFF")).size() <= cells)
                                        setSeed(addHexNumbers(seed_counter, "FFFF"));
                                }
                                else if(event.key.key == SDLK_DOWN)
                                {
                                    if(!simulation_state && (seed_counter.size() > 4 || (seed_counter.size() == 4 && hex_to_decimal_map.at(seed_counter[0]) == 15 && hex_to_decimal_map.at(seed_counter[1]) == 15 && hex_to_decimal_map.at(seed_counter[2]) == 15 && hex_to_decimal_map.at(seed_counter[3]) == 15)))
                                        setSeed(subtractHexNumbers(seed_counter, "FFFF"));
                                }
                                else if(event.key.key == SDLK_W)
                                {
                                    if(grid_height > 1)
                                        changeGrid(grid_width, grid_height - 1);
                                }
                                else if(event.key.key == SDLK_A)
                                {
                                    if(grid_width > 1)
                                        changeGrid(grid_width - 1, grid_height);
                                }
                                else if(event.key.key == SDLK_S)
                                    changeGrid(grid_width, grid_height + 1);
                                else
                                    changeGrid(grid_width + 1, grid_height);
                            }
                            else if(!event.key.repeat)
                            {
                                switch(event.key.key)
                                {
                                    case SDLK_ESCAPE:
                                    {
                                        code_state = SimulationState::PAUSE;
                                        break;
                                    }
                                    case SDLK_RETURN:
                                    {
                                        simulation_state = !simulation_state;
                                        break;
                                    }
                                    case SDLK_R:
                                    {
                                        resetSeed();
                                        break;
                                    }
                                    case SDLK_P:
                                    {
                                        evaluateParams();
                                        break;
                                    }
                                    case SDLK_1:
                                    {
                                        tick = 1000;
                                        break;
                                    }
                                    case SDLK_2:
                                    {
                                        tick = 500;
                                        break;
                                    }
                                    case SDLK_3:
                                    {
                                        tick = 200;
                                        break;
                                    }
                                    case SDLK_4:
                                    {
                                        tick = 100;
                                        break;
                                    }
                                    default:
                                    break;
                                }
                            }
                        }
                        else if(event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
                        {
                            if(!simulation_state && event.button.button == SDL_BUTTON_LEFT)
                                setSeedFromMouse(event.button.x, event.button.y);
                        }
                        break;
                    }
                }
                if(event.type == SDL_EVENT_QUIT)
                    code_state = SimulationState::QUIT;
            }
        }

        bool isHex(std::string_view s)
        {
            for (char c : s)
                if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F')))
                    return false;
            return true;
        }

        std::string hexToBinary(std::string_view hex)
        {
            std::string res = "";

            for(const char s : hex)
                res = res + hex_to_binary_map.at(s);

            while(res.size() >= 2 && res[0] == '0')
                res.erase(0, 1);

            return res;
        }

        std::string binaryToHex(std::string bin)
        {
            std::string res = "";
            while(bin.size() % 4 != 0)
                bin = "0" + bin;
            for(size_t i=0; i<bin.size()/4; i++)
                res.push_back(binary_to_hex_map.at(bin.substr(i*4, 4)));
            while(res.size() >= 2 && res[0] == '0')
                res.erase(0, 1);
            return res;
        }

        void resetParams()
        {
            lifetime = "N/A";
            period = "N/A";
            main_class = "N/A";
            sub_class = "N/A";
        }

        void newSim()
        {
            seed_counter = "0";
            seed_counter_binary = std::string(cells, '0');
            generation = 1;
            resetParams();
            simulation_state = false;
            life_grid = std::vector(grid_height, std::vector<int>(grid_width, 0));
            state_buffer = std::vector(grid_height, std::vector<int>(grid_width, 0));
        }

        void setSeedToGrid(std::vector<std::vector<int>>& grid)
        {
            for(size_t i = cells; i-- > 0;)
            {
                size_t row = i / grid_width;
                size_t col = i % grid_width;
                grid[row][col] = (seed_counter_binary[i] == '1');
            }
        }

        void resetSeed()
        {
            generation = 1;
            setSeedToGrid(life_grid);
            state_buffer = std::vector(grid_height, std::vector<int>(grid_width, 0));
        }

        void setSeed(std::string x)
        {
            generation = 1;
            resetParams();

            seed_counter = x;
            seed_counter_binary = hexToBinary(seed_counter);

            if(seed_counter_binary.size() < cells)
                seed_counter_binary = std::string(cells - seed_counter_binary.size(), '0') + seed_counter_binary;

            setSeedToGrid(life_grid);
            state_buffer = std::vector(grid_height, std::vector<int>(grid_width, 0));
        }

        void setSeedFromMouse(int x, int y)
        {
            size_t col = x / PIXEL_SIZE;
            size_t row = y / PIXEL_SIZE;
            if(row >= grid_height || col >= grid_width)
                return;
            life_grid[row][col] ^= 1;
            size_t idx = row*grid_width + col;
            seed_counter_binary[idx] = (seed_counter_binary[idx] == '1') ? '0' : '1';
            seed_counter = binaryToHex(seed_counter_binary);
            generation = 1;
            resetParams();
            state_buffer = std::vector(grid_height, std::vector<int>(grid_width, 0));
        }

        void changeGrid(size_t x, size_t y)
        {
            grid_width = x;
            grid_height = y;
            cells = grid_width * grid_height;
            newSim();
        }

        void evaluateParams()
        {
            std::vector<std::vector<int>> virtual_life_grid = std::vector(grid_height, std::vector<int>(grid_width, 0));
            setSeedToGrid(virtual_life_grid);

            bool pre_check = false;
            for(const auto& row : virtual_life_grid)
                for(int v : row)
                    if(v == 1)
                    {
                        pre_check = true;
                        break;
                    }

            if(!pre_check)
            {
                lifetime = "0";
                main_class = "N/A";
                sub_class = "N/A";
                period = "N/A";
                return;
            }

            std::vector<std::vector<int>> virtual_state_buffer = std::vector(grid_height, std::vector<int>(grid_width, 0));
            std::map<std::vector<std::vector<int>>, size_t> seen_map;
            
            size_t counter = 0;
            seen_map[virtual_life_grid] = 0;
            while(true)
            {
                evaluateState(virtual_life_grid, virtual_state_buffer);
                counter++;

                bool is_persistent = false;
                for(const auto& row : virtual_state_buffer)
                    for(int v : row)
                        if(v == 1)
                        {
                            is_persistent = true;
                            break;
                        }

                if(!is_persistent)
                {
                    lifetime = std::to_string(counter);
                    period = "N/A";
                    main_class = "Transient";
                    sub_class = "N/A";
                    break;
                }

                if (seen_map.count(virtual_state_buffer))
                {
                    lifetime = "Oscillating from Generation " + std::to_string(counter) + " onwards";
                    main_class = "Oscillator";
                    int x = counter - seen_map[virtual_state_buffer];
                    if(x == 1)
                        sub_class = "Still Life";
                    else
                        sub_class = "Oscillator";
                    period = std::to_string(x);
                    break;
                }

                seen_map[virtual_state_buffer] = counter;
                virtual_life_grid = virtual_state_buffer;
            }
        }

        void evaluateState(const std::vector<std::vector<int>>& x, std::vector<std::vector<int>>& y)
        {
            for (size_t i = 0; i < grid_height; ++i)
            {
                for (size_t j = 0; j < grid_width; ++j)
                {
                    int counter = 0;
                    for (int di = -1; di <= 1; ++di)
                    {
                        for (int dj = -1; dj <= 1; ++dj)
                        {
                            if (di == 0 && dj == 0)
                                continue;

                            int ni = static_cast<int>(i) + di;
                            int nj = static_cast<int>(j) + dj;

                            if (ni >= 0 && ni < static_cast<int>(grid_height) &&
                                nj >= 0 && nj < static_cast<int>(grid_width))
                                    counter += x[ni][nj];
                        }
                    }

                    if (x[i][j])
                        y[i][j] = (counter == 2 || counter == 3);
                    else
                        y[i][j] = (counter == 3);
                }
            }
        }

        void updateState()
        {
            evaluateState(life_grid, state_buffer);
            life_grid = state_buffer;
            generation++;
        }

        size_t getTextLen(const std::string& text) const noexcept
        {
            int w, h;
            w = h = 0;
            TTF_GetStringSize(font, text.c_str(), text.size(), &w, &h);
            return static_cast<size_t>(w);
        }

        void drawText(const std::string& text, float x, float y, SDL_Color color)
        {
            SDL_Surface* text_surface;
            text_surface = TTF_RenderText_Blended(font, (text).c_str(), strlen(text.c_str()), color);
            SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
            SDL_DestroySurface(text_surface);
            float w = static_cast<float>(getTextLen(text));
            SDL_FRect dst = {x, y, w, FONT_SIZE};
            SDL_RenderTexture(renderer, text_texture, nullptr, &dst);
            SDL_DestroyTexture(text_texture);
        }

        void renderFrame()
        {
            if(is_black_theme)
                SDL_SetRenderDrawColor(renderer, WHITE.r, WHITE.g, WHITE.b, WHITE.a);
            else
                SDL_SetRenderDrawColor(renderer, BLACK.r, BLACK.g, BLACK.b, BLACK.a);
            SDL_RenderClear(renderer);
            switch(code_state)
            {
                case SimulationState::MAIN:
                {
                    main_menu.renderMenu(renderer, font);
                    break;
                }
                case SimulationState::PAUSE:
                {
                    pause_menu.renderMenu(renderer, font);
                    break;
                }
                case SimulationState::SEED:
                {
                    seed_menu.renderMenu(renderer, font);
                    drawText(command, seed_menu.getMenuPosX(), seed_menu.getMenuPosY() + FONT_SIZE, WHITE);
                    break;
                }
                case SimulationState::RUNNING:
                {
                    if(is_black_theme)
                        SDL_SetRenderDrawColor(renderer, BLACK.r, BLACK.g, BLACK.b, BLACK.a);
                    else
                        SDL_SetRenderDrawColor(renderer, WHITE.r, WHITE.g, WHITE.b, WHITE.a);

                    std::string text = "Seed: " + std::to_string(grid_height) + "x" + std::to_string(grid_width) + "x" + seed_counter;
                    size_t w1 = getTextLen(text);
                        
                    std::string text2;
                    if(simulation_state)
                        text2 = "Simulation: ON";
                    else
                        text2 = "Simulation: OFF";
                    size_t w2 = getTextLen(text2);

                    std::string text3 = "Simulation Speed: ";
                    if(!simulation_state)
                        text3 += "0";
                    else
                        text3 += std::to_string(1000.0f/static_cast<float>(tick)).substr(0, 4);
                    text3 += " updates/sec";
                    size_t w3 = getTextLen(text3);
                    
                    std::string text4 = "Generation: " + std::to_string(generation);
                    size_t w4 = getTextLen(text4);

                    std::string text5 = "Lifetime: " + lifetime;
                    size_t w5 = getTextLen(text5);
                    
                    std::string text6 = "Class: " + main_class;
                    size_t w6 = getTextLen(text6);

                    std::string text7 = "Sub Class: " + sub_class;
                    size_t w7 = getTextLen(text7);

                    std::string text8 = "Period: " + period;
                    size_t w8 = getTextLen(text8);

                    if(is_black_theme)
                    {
                        drawText(text, static_cast<float>(SCREEN_WIDTH - 1 - w1), 0.0f, BLACK);
                        drawText(text2, static_cast<float>(SCREEN_WIDTH - 1 - w2), FONT_SIZE, BLACK);
                        drawText(text3, static_cast<float>(SCREEN_WIDTH - 1 - w3), FONT_SIZE*2, BLACK);
                        drawText(text4, static_cast<float>(SCREEN_WIDTH - 1 - w4), FONT_SIZE*3, BLACK);
                        drawText(text5, static_cast<float>(SCREEN_WIDTH - 1 - w5), FONT_SIZE*4, BLACK);
                        drawText(text6, static_cast<float>(SCREEN_WIDTH - 1 - w6), FONT_SIZE*5, BLACK);
                        drawText(text7, static_cast<float>(SCREEN_WIDTH - 1 - w7), FONT_SIZE*6, BLACK);
                        drawText(text8, static_cast<float>(SCREEN_WIDTH - 1 - w8), FONT_SIZE*7, BLACK);
                    }
                    else
                    {
                        drawText(text, static_cast<float>(SCREEN_WIDTH - 1 - w1), 0.0f, WHITE);
                        drawText(text2, static_cast<float>(SCREEN_WIDTH - 1 - w2), FONT_SIZE, WHITE);
                        drawText(text3, static_cast<float>(SCREEN_WIDTH - 1 - w3), FONT_SIZE*2, WHITE);
                        drawText(text4, static_cast<float>(SCREEN_WIDTH - 1 - w4), FONT_SIZE*3, WHITE);
                        drawText(text5, static_cast<float>(SCREEN_WIDTH - 1 - w5), FONT_SIZE*4, WHITE);
                        drawText(text6, static_cast<float>(SCREEN_WIDTH - 1 - w6), FONT_SIZE*5, WHITE);
                        drawText(text7, static_cast<float>(SCREEN_WIDTH - 1 - w7), FONT_SIZE*6, WHITE);
                        drawText(text8, static_cast<float>(SCREEN_WIDTH - 1 - w8), FONT_SIZE*7, WHITE);
                    }

                    SDL_FRect dst;
                    for(size_t i=0; i<=grid_height; i++)
                    {
                        SDL_RenderLine(renderer, 0, static_cast<float>(i*PIXEL_SIZE), (grid_width)*PIXEL_SIZE, static_cast<float>(i*PIXEL_SIZE));
                        for(size_t j=0; j<=grid_width; j++)
                            SDL_RenderLine(renderer, static_cast<float>(j*PIXEL_SIZE), 0, static_cast<float>(j*PIXEL_SIZE), (grid_height)*PIXEL_SIZE);
                    }
                    for(size_t i=0; i<grid_height; i++)
                        for(size_t j=0; j<grid_width; j++)
                            if(life_grid[i][j])
                            {
                                dst = {static_cast<float>(j*PIXEL_SIZE), static_cast<float>(i*PIXEL_SIZE), PIXEL_SIZE, PIXEL_SIZE};
                                SDL_RenderFillRect(renderer, &dst);
                            }
                    break;
                }
                default:
                    break;
            }
            SDL_RenderPresent(renderer);
        }

        int run()
        {
            if(!SDL_Init(SDL_INIT_VIDEO))
            {
                std::cerr<<"Failed to initialize SDL: "<<SDL_GetError()<<"\n";
                return 1;
            }

            window = SDL_CreateWindow("Start", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
            if(!window)
            {
                std::cerr<<"Failed to create window: "<<SDL_GetError()<< "\n";
                SDL_Quit();
                return 2;
            }

            renderer = SDL_CreateRenderer(window, nullptr);
            if (!renderer) 
            {
                std::cerr << "Failed to create renderer: " << SDL_GetError() << "\n";
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 3;
            }

            if (!TTF_Init())
            {
                std::cerr << "Failed to initialize TTF: " << SDL_GetError() << "\n";
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 4;
            }

            font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
            if (!font) 
            {
                std::cerr << "Failed to open font: " << SDL_GetError() << "\n";
                TTF_Quit();
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 5;
            }

            Uint64 last = SDL_GetTicks();
            Uint64 accumulator = 0;

            while(code_state != SimulationState::QUIT)
            {
                Uint64 current = SDL_GetTicks();
                Uint64 delta = current - last;
                last = current;
                accumulator += delta;

                handleInput();
                while(code_state == SimulationState::RUNNING && accumulator >= tick)
                {
                    if(simulation_state)
                        updateState();
                    accumulator -= tick;
                }

                renderFrame();

                Uint64 frame_time_elapsed = SDL_GetTicks() - current;
                if(frame_time_elapsed < frame_time)
                    SDL_Delay(frame_time - frame_time_elapsed);
            }

            TTF_CloseFont(font);
            TTF_Quit();
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 0;
        }
};

#endif