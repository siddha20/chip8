#include <assert.h>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <vector>
#include "runner.h"
#include "SDL.h"
#include "SDL_ttf.h"

namespace emulator
{
Runner::Runner(const std::string &rom_file_name) 
    : y_scale(WINDOW_HEIGHT/processor.display.CHIP8_HEIGHT)
    , x_scale(WINDOW_WIDTH/processor.display.CHIP8_LENGTH)
{
    // Load ROM into memory.
    std::ifstream in(rom_file_name, std::ios::binary);
    std::vector<char> ROM((std::istreambuf_iterator<char>(in))
                        , std::istreambuf_iterator<char>());

    std::cout << "ROM byte size: " << ROM.size() << std::endl;
    processor.memory.load_rom_to_mem(ROM);

    TTF_Init();
    font = TTF_OpenFont("arial.ttf", 18);
    assert(font);
    if (SDL_Init(SDL_INIT_VIDEO) < 0) std::cout << "Failed to initialize the SDL2 library" << std::endl;
    create_window();
    #ifdef DEBUG
        create_debug_window();
    #endif
    create_key_mapping();
}

Runner::~Runner()
{ 
    SDL_FreeSurface(surface);
    SDL_DestroyWindow(window); 
    #ifdef DEBUG
        SDL_DestroyTexture(debug_texture);
        SDL_DestroyWindow(debug_window);
        SDL_DestroyRenderer(debug_renderer);
    #endif
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}

void Runner::start()
{
    SDL_Event event;
    bool quit = false;
    bool pause = false;
    #ifdef DEBUG
        pause = true;
    #endif
    double clock_accumulator = 0, clock_delta = 0;
    uint64_t start_tick = 0, end_tick = 0, delta_tick, end_frame_tick = 0, count = 0;
    int clock_rate_modifier = 0, step = 5;
    while (!quit) 
    {
        start_tick = SDL_GetTicks64();
        if (!(++count % POLL_RATE))
        {
            while (SDL_PollEvent(&event))
            {
                /* For some reason a KEYDOWN and KEYUP is registered on the same
                frame for the same key. key_down bool is meant to fix that. */
                bool key_down = false;
                switch (event.type)
                {
                    case SDL_WINDOWEVENT:
                        if (event.window.event == SDL_WINDOWEVENT_CLOSE) quit = true;
                    case SDL_KEYDOWN:
                        if (event.key.repeat == 0 && key_mapping.contains(event.key.keysym.sym))
                        {
                            processor.keyboard[key_mapping.at(event.key.keysym.sym)] = 1;
                            key_down = true;
                        }
                        #ifdef DEBUG
                            if (event.key.repeat == 0 && event.key.keysym.sym == SDLK_RIGHTBRACKET) clock_rate_modifier += step;
                            if (event.key.repeat == 0 && event.key.keysym.sym == SDLK_LEFTBRACKET && CPU_RATE + clock_rate_modifier > step) clock_rate_modifier -= step;
                        #endif
                        if (event.key.repeat == 0 && event.key.keysym.sym == SDLK_p) pause = !pause;
                    case SDL_KEYUP:
                        if (event.key.repeat == 0 &&
                            !key_down &&
                            key_mapping.contains(event.key.keysym.sym)) processor.keyboard[key_mapping.at(event.key.keysym.sym)] = 0;
                    default:
                        break;
                }
            }
        }
        if (!pause) processor.step();
        clock_delta = static_cast<double>(start_tick) - static_cast<double>(end_tick);
        clock_delta = clock_delta > 100.0f ? 100.0f : clock_delta;
        clock_accumulator += clock_delta;
        while (clock_accumulator >= 1000.0f/static_cast<double>(CLOCK_RATE))
        {
            processor.decrement_timers();
            draw_display(processor.display);
            clock_accumulator -= 1000.0f/static_cast<double>(CLOCK_RATE);
        }
        #ifdef DEBUG
            // Slows down loop a bit. Debug use only.
            update_debug_window();
            std::string hz = std::to_string(1000 / ( start_tick > end_tick ? start_tick - end_tick : 1000));
            render_debug_text("CPU rate: " + hz + "(" + std::to_string(CPU_RATE + clock_rate_modifier) + ")", {20, 110});
            SDL_RenderPresent(debug_renderer);
        #endif
        end_tick = SDL_GetTicks64();
        delta_tick = end_tick - start_tick;
        if(delta_tick < (1000 / (CPU_RATE + clock_rate_modifier))) SDL_Delay((1000 / (CPU_RATE + clock_rate_modifier)) - delta_tick);
    }
}

void Runner::draw_display(const Display &display)
{
    SDL_FillRect(surface, NULL, 0);
    for (size_t i = 0; i < display.CHIP8_HEIGHT; i++)
    {
        for (size_t j = 0; j < display.CHIP8_LENGTH; j++)
        {
            SDL_Rect rect;
            if (display[{i, j}])
            {
                modify_rectangle(rect, {i, j});
                SDL_FillRect(surface, &rect, 0xFFFFFFFF);
            }
        }
    }
    SDL_UpdateWindowSurface(window);
}

void Runner::modify_rectangle(SDL_Rect &rect, Display::Position position)
{
    rect.w = y_scale;
    rect.h = x_scale;
    rect.y = position.row * x_scale;
    rect.x = position.col * y_scale;
}

void Runner::update_debug_window()
{
    SDL_RenderClear(debug_renderer);
    std::stringstream register_info;
    register_info << std::uppercase << std::hex;
    int PC = processor.get_program_counter();
    register_info << "PC: 0x" << static_cast<int>(PC);
    register_info << " --> 0x" << processor.get_last_instruction();
    render_debug_text(register_info.str(), {20, 20});
    register_info.str("");
    register_info.clear();

    register_info << std::uppercase << std::hex;
    int SP = processor.get_stack_pointer();
    register_info << "SP: 0x" << static_cast<int>(SP);
    register_info << " --> 0x" << static_cast<int>(processor.memory.get_stack(static_cast<size_t>(SP)));
    render_debug_text(register_info.str(), {20, 50});
    register_info.str("");
    register_info.clear();

    register_info << std::uppercase << std::hex;
    register_info << "Last instr: " << processor.get_last_instruction_name();
    render_debug_text(register_info.str(), {20, 80});
    register_info.str("");
    register_info.clear();

    for (size_t i = 0; i < processor.memory.REGISTER_COUNT; i++)
    {
        register_info << std::uppercase << std::hex << "V" << i << std::setw(3);
        register_info << std::dec << "0d" << static_cast<int>(processor.memory.get_registers(i));
        render_debug_text(register_info.str(), {20, 150 + (i * 20)});
        register_info.str("");
        register_info.clear();
    }

    register_info << std::uppercase << std::hex << std::setw(4) << "I" << std::setw(4);
    register_info << "0x" << static_cast<int>(processor.memory.get_special_registers().I);
    render_debug_text(register_info.str(), {120, 150});
    register_info.str("");
    register_info.clear();

    register_info << std::uppercase << std::dec << "DT" << std::setw(3);
    register_info << "0d" << static_cast<int>(processor.memory.get_special_registers().DT);
    render_debug_text(register_info.str(), {120, 170});
    register_info.str("");
    register_info.clear();

    register_info << std::uppercase << std::dec << "ST" << std::setw(3);
    register_info << "0d" << static_cast<int>(processor.memory.get_special_registers().ST);
    render_debug_text(register_info.str(), {120, 190});
    register_info.str("");
    register_info.clear();
    // SDL_RenderPresent(debug_renderer);
}

void Runner::render_debug_text(const std::string &text, Display::Position position)
{
    int w, h;
    TTF_SizeText(font, text.c_str(), &w, &h);

    SDL_Color color = {0xFF, 0xFF, 0xFF}, bgcolor={0, 0, 0};

    SDL_Surface *surface = TTF_RenderText_Shaded(font, text.c_str(), color, bgcolor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(debug_renderer, surface);
    SDL_Rect dstrect = {static_cast<int>(position.row), static_cast<int>(position.col), w, h};
    SDL_Rect bgrect = {static_cast<int>(position.row), static_cast<int>(position.col), w+60, h+10};
    SDL_RenderCopy(debug_renderer, texture, NULL, &dstrect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void Runner::create_window()
{
    window = SDL_CreateWindow( "chip8 emulator"
                             , SDL_WINDOWPOS_CENTERED
                             , SDL_WINDOWPOS_CENTERED
                             , WINDOW_WIDTH
                             , WINDOW_HEIGHT
                             , 0 );
    if (!window) std::cout << "Failed to create window" << std::endl;

    surface = SDL_GetWindowSurface(window);
    if (!surface) std::cout << "Failed to get the surface from the window" << std::endl;
}

void Runner::create_debug_window()
{
    int x, y;
    SDL_GetWindowPosition(window, &x, &y);
    debug_window = SDL_CreateWindow( "c8 debug"
                                    , x + WINDOW_WIDTH + 50
                                    , y
                                    , DEBUG_WINDOW_WIDTH
                                    , DEBUG_WINDOW_HEIGHT
                                    , 0 );
    if (!debug_window) std::cout << "Failed to create window" << std::endl;

    debug_renderer = SDL_CreateRenderer(debug_window, -1, 0);
    if (!debug_renderer) std::cout << "Failed to get the renderer from the window" << std::endl;
}

void Runner::create_key_mapping()
{
    key_mapping[SDLK_1] = 0x1;
    key_mapping[SDLK_2] = 0x2;
    key_mapping[SDLK_3] = 0x3;
    key_mapping[SDLK_4] = 0xC;
    key_mapping[SDLK_q] = 0x4;
    key_mapping[SDLK_w] = 0x5;
    key_mapping[SDLK_e] = 0x6;
    key_mapping[SDLK_r] = 0xD;
    key_mapping[SDLK_a] = 0x7;
    key_mapping[SDLK_s] = 0x8;
    key_mapping[SDLK_d] = 0x9;
    key_mapping[SDLK_f] = 0xE;
    key_mapping[SDLK_z] = 0xA;
    key_mapping[SDLK_x] = 0x0;
    key_mapping[SDLK_c] = 0xB;
    key_mapping[SDLK_v] = 0xF;
}
}