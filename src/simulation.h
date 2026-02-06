#ifndef SIMULATION_H
#define SIMULATION_H

#include "menu.h"

static_assert(CELLS>0, "Number of cells should be greater than 0!");

class Simulation
{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;
    std::string command = "";
    int fps = 60;
    int frame_time = 1000 / fps;
    unsigned long long int seed_counter = 0;
    std::string seed_counter_binary_str = std::string(CELLS, '0');
    bool simulation_state = false;
    std::array<std::array<int, GRID_WIDTH>, GRID_HEIGHT> life_grid = {0};
    std::array<std::array<int, GRID_WIDTH>, GRID_HEIGHT> state_buffer = {0};
    SimulationState code_state = SimulationState::MAIN;
    Menu main_menu = Menu({"Simulate", "Quit"}, MAIN_MENU_BOX_WIDTH, MAIN_MENU_BOX_HEIGHT);
    Menu pause_menu =  Menu({"Continue", "Seed", "Quit to Main Menu"}, PAUSE_MENU_BOX_WIDTH, PAUSE_MENU_BOX_HEIGHT);
    Menu seed_menu = Menu({"Seed"}, SEED_MENU_BOX_WIDTH, SEED_MENU_BOX_HEIGHT);
    public:
        Simulation(){}

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
                                        if (main_menu_item_name == "Simulate")
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
                                        if(!command.empty() && isDigit(command))
                                        {
                                            auto x = std::stoull(command);
                                            if(x <= LAST_STATE_INDEX)
                                            {
                                                setSeed(x);
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
                            if(event.key.key == SDLK_LEFT || event.key.key == SDLK_RIGHT)
                            {
                                if(event.key.key == SDLK_LEFT)
                                {
                                    if(seed_counter > 0)
                                        setSeed(seed_counter - 1);
                                }
                                else
                                {
                                    if(seed_counter < LAST_STATE_INDEX)
                                        setSeed(seed_counter + 1);
                                }
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
                                        setSeed(seed_counter);
                                        break;
                                    }
                                    default:
                                    break;
                                }
                            }
                        }
                        break;
                    }
                }
                if(event.type == SDL_EVENT_QUIT)
                    code_state = SimulationState::QUIT;
            }
        }

        void binarize(unsigned long long int k, std::string& x)
        {
            x.assign(CELLS, '0');
            size_t count = CELLS - 1;
            while(k > 0)
            {
                x[count] = k % 2 + '0';
                k = k/2;
                count--;
            }
        }

        bool isDigit(const std::string& s)
        {
            for (char c : s)
                if (c < '0' || c > '9')
                    return false;
            return true;
        }

        void newSim()
        {
            seed_counter = 0;
            seed_counter_binary_str.assign(CELLS, '0');
            simulation_state = false;
            life_grid = {0};
            state_buffer = {0};
        }

        void setSeed(unsigned long long int x)
        {
            seed_counter = x;
            binarize(seed_counter, seed_counter_binary_str);
            for(size_t i=0; i<CELLS; i++)
            {
                size_t row = i / GRID_WIDTH;
                size_t col = i % GRID_WIDTH;
                life_grid[row][col] = (seed_counter_binary_str[i] == '1');
            }
        }

        void updateState()
        {
            for(size_t i=0; i<GRID_HEIGHT; i++)
                for(size_t j=0; j<GRID_WIDTH; j++)
                {
                    size_t counter = 0;
                    if(i > 0 && i < (GRID_HEIGHT - 1) && j > 0 && j < (GRID_WIDTH - 1)) //middle section 8 neighbours
                    {
                        if(life_grid[i-1][j-1] == 1)
                            counter++;
                        if(life_grid[i-1][j] == 1)
                            counter++;
                        if(life_grid[i-1][j+1] == 1)
                            counter++;
                        if(life_grid[i][j-1] == 1)
                            counter++;
                        if(life_grid[i][j+1] == 1)
                            counter++;
                        if(life_grid[i+1][j-1] == 1)
                            counter++;
                        if(life_grid[i+1][j] == 1)
                            counter++;
                        if(life_grid[i+1][j+1] == 1)
                            counter++;
                        if(life_grid[i][j] == 1)
                        {
                            if(counter < 2 || counter > 3)
                                state_buffer[i][j] = 0;
                            else
                                state_buffer[i][j] = 1;
                        }
                        else if(counter == 3)
                            state_buffer[i][j] = 1;
                        else
                            state_buffer[i][j] = 0;
                    }
                    else if(i == 0 && !(j == 0 || j == (GRID_WIDTH - 1))) //top row 5 neighbours
                    {
                        if(life_grid[i][j-1] == 1)
                            counter++;
                        if(life_grid[i][j+1] == 1)
                            counter++;
                        if(life_grid[i+1][j-1] == 1)
                            counter++;
                        if(life_grid[i+1][j] == 1)
                            counter++;
                        if(life_grid[i+1][j+1] == 1)
                            counter++;
                        if(life_grid[i][j] == 1)
                        {
                            if(counter < 2 || counter > 3)
                                state_buffer[i][j] = 0;
                            else
                                state_buffer[i][j] = 1;
                        }
                        else if(counter == 3)
                            state_buffer[i][j] = 1;
                        else
                            state_buffer[i][j] = 0;
                    }

                    else if(i == (GRID_HEIGHT - 1) && !(j == 0 || j == (GRID_WIDTH - 1))) //bottom row 5 neighbours
                    {
                        if(life_grid[i][j-1] == 1)
                            counter++;
                        if(life_grid[i][j+1] == 1)
                            counter++;
                        if(life_grid[i-1][j-1] == 1)
                            counter++;
                        if(life_grid[i-1][j] == 1)
                            counter++;
                        if(life_grid[i-1][j+1] == 1)
                            counter++;
                        if(life_grid[i][j] == 1)
                        {
                            if(counter < 2 || counter > 3)
                                state_buffer[i][j] = 0;
                            else
                                state_buffer[i][j] = 1;
                        }
                        else if(counter == 3)
                            state_buffer[i][j] = 1;
                        else
                            state_buffer[i][j] = 0;
                    }

                    else if(j == 0 && !(i == 0 || i == (GRID_HEIGHT - 1))) //left column 5 neighbours
                    {
                        if(life_grid[i+1][j] == 1)
                            counter++;
                        if(life_grid[i-1][j] == 1)
                            counter++;
                        if(life_grid[i-1][j+1] == 1)
                            counter++;
                        if(life_grid[i][j+1] == 1)
                            counter++;
                        if(life_grid[i+1][j+1] == 1)
                            counter++;
                        if(life_grid[i][j] == 1)
                        {
                            if(counter < 2 || counter > 3)
                                state_buffer[i][j] = 0;
                            else
                                state_buffer[i][j] = 1;
                        }
                        else if(counter == 3)
                            state_buffer[i][j] = 1;
                        else
                            state_buffer[i][j] = 0;
                    }

                    else if(j == (GRID_WIDTH - 1) && !(i == 0 || i == (GRID_HEIGHT - 1))) //right column 5 neighbours
                    {
                        if(life_grid[i+1][j] == 1)
                            counter++;
                        if(life_grid[i-1][j] == 1)
                            counter++;
                        if(life_grid[i-1][j-1] == 1)
                            counter++;
                        if(life_grid[i][j-1] == 1)
                            counter++;
                        if(life_grid[i+1][j-1] == 1)
                            counter++;
                        if(life_grid[i][j] == 1)
                        {
                            if(counter < 2 || counter > 3)
                                state_buffer[i][j] = 0;
                            else
                                state_buffer[i][j] = 1;
                        }
                        else if(counter == 3)
                            state_buffer[i][j] = 1;
                        else
                            state_buffer[i][j] = 0;
                    }

                    else if(i == 0 && j==0) //top left corner 3 neighbours
                    {
                        if(life_grid[i+1][j+1] == 1)
                            counter++;
                        if(life_grid[i+1][j] == 1)
                            counter++;
                        if(life_grid[i][j+1] == 1)
                            counter++;
                        if(life_grid[i][j] == 1)
                        {
                            if(counter < 2 || counter > 3)
                                state_buffer[i][j] = 0;
                            else
                                state_buffer[i][j] = 1;
                        }
                        else if(counter == 3)
                            state_buffer[i][j] = 1;
                        else
                            state_buffer[i][j] = 0;
                    }

                    else if(i == 0 && j == (GRID_WIDTH - 1)) //top right corner 3 neighbours
                    {
                        if(life_grid[i][j-1] == 1)
                            counter++;
                        if(life_grid[i+1][j] == 1)
                            counter++;
                        if(life_grid[i+1][j-1] == 1)
                            counter++;
                        if(life_grid[i][j] == 1)
                        {
                            if(counter < 2 || counter > 3)
                                state_buffer[i][j] = 0;
                            else
                                state_buffer[i][j] = 1;
                        }
                        else if(counter == 3)
                            state_buffer[i][j] = 1;
                        else
                            state_buffer[i][j] = 0;
                    }

                    else if(i == (GRID_HEIGHT - 1) && j == 0) //bottom left corner 3 neighbours
                    {
                        if(life_grid[i-1][j] == 1)
                            counter++;
                        if(life_grid[i][j+1] == 1)
                            counter++;
                        if(life_grid[i-1][j+1] == 1)
                            counter++;
                        if(life_grid[i][j] == 1)
                        {
                            if(counter < 2 || counter > 3)
                                state_buffer[i][j] = 0;
                            else
                                state_buffer[i][j] = 1;
                        }
                        else if(counter == 3)
                            state_buffer[i][j] = 1;
                        else
                            state_buffer[i][j] = 0;
                    }

                    else if(i == (GRID_HEIGHT - 1) && j == (GRID_WIDTH - 1)) //bottom right corner 3 neighbours
                    {
                        if(life_grid[i-1][j] == 1)
                            counter++;
                        if(life_grid[i][j-1] == 1)
                            counter++;
                        if(life_grid[i-1][j-1] == 1)
                            counter++;
                        if(life_grid[i][j] == 1)
                        {
                            if(counter < 2 || counter > 3)
                                state_buffer[i][j] = 0;
                            else
                                state_buffer[i][j] = 1;
                        }
                        else if(counter == 3)
                            state_buffer[i][j] = 1;
                        else
                            state_buffer[i][j] = 0;
                    }
                }
            life_grid = state_buffer;
        }

        size_t getTextLen(const std::string& text) const noexcept
        {
            int w, h;
            w = h = 0;
            TTF_GetStringSize(font, text.c_str(), text.size(), &w, &h);
            return static_cast<size_t>(w);
        }

        void drawText(const std::string& text, float x, float y)
        {
            SDL_Surface* text_surface;
            if(theme == "black")
                text_surface = TTF_RenderText_Blended(font, (text).c_str(), strlen(text.c_str()), BLACK);
            else
                text_surface = TTF_RenderText_Blended(font, (text).c_str(), strlen(text.c_str()), WHITE);
            SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
            SDL_DestroySurface(text_surface);
            float w = static_cast<float>(getTextLen(text));
            SDL_FRect dst = {x, y, w, FONT_SIZE};
            SDL_RenderTexture(renderer, text_texture, nullptr, &dst);
            SDL_DestroyTexture(text_texture);
        }

        void renderFrame()
        {
            if(theme == "black")
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
                    drawText(command, seed_menu.getMenuPosX(), seed_menu.getMenuPosY() + FONT_SIZE);
                    break;
                }
                case SimulationState::RUNNING:
                {
                    if(theme == "black")
                        SDL_SetRenderDrawColor(renderer, BLACK.r, BLACK.g, BLACK.b, BLACK.a);
                    else
                        SDL_SetRenderDrawColor(renderer, WHITE.r, WHITE.g, WHITE.b, WHITE.a);

                    std::string text = "Seed: " + std::to_string(GRID_WIDTH) + "x" + std::to_string(GRID_HEIGHT) + "x" + std::to_string(seed_counter);
                    size_t w = getTextLen(text);
                    drawText(text, static_cast<float>(SCREEN_WIDTH - 1 - w), 0.0f);

                    std::string text2;
                    if(simulation_state)
                        text2 = "Simulation: ON";
                    else
                        text2 = "Simulation: OFF";
                    w = getTextLen(text2);
                    drawText(text2, static_cast<float>(SCREEN_WIDTH - 1 - w), FONT_SIZE);

                    SDL_FRect dst;
                    for(size_t i=0; i<=GRID_HEIGHT; i++)
                    {
                        SDL_RenderLine(renderer, 0, static_cast<float>(i*PIXEL_SIZE), (GRID_WIDTH)*PIXEL_SIZE, static_cast<float>(i*PIXEL_SIZE));
                        for(size_t j=0; j<=GRID_WIDTH; j++)
                            SDL_RenderLine(renderer, static_cast<float>(j*PIXEL_SIZE), 0, static_cast<float>(j*PIXEL_SIZE), (GRID_HEIGHT)*PIXEL_SIZE);
                    }
                    for(size_t i=0; i<GRID_HEIGHT; i++)
                        for(size_t j=0; j<GRID_WIDTH; j++)
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
                while(code_state == SimulationState::RUNNING && accumulator >= TICK)
                {
                    if(simulation_state)
                        updateState();
                    accumulator -= TICK;
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