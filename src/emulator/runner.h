#pragma once
#include <unordered_map>
#include "SDL.h"
#include "display.h"
#include "processor.h"
#include "common.h"
#include "SDL_ttf.h"

#define CPU_RATE_HZ 600
#define CLOCK_RATE_HZ 60
#define POLL_RATE_COUNT 15
#ifdef DEBUG
    #undef CPU_RATE_HZ
    #define CPU_RATE_HZ 60
    #undef CLOCK_RATE_HZ
    #define CLOCK_RATE_HZ 30
    #undef POLL_RATE_HZ
    #define POLL_RATE_COUNT 5
#endif

namespace emulator
{
class Runner
{
public:
    static constexpr int WINDOW_HEIGHT = 320;
    static constexpr int WINDOW_WIDTH = 640;
    static constexpr int DEBUG_WINDOW_HEIGHT = 500;
    static constexpr int DEBUG_WINDOW_WIDTH = 240;
    static constexpr int unsigned POLL_RATE = POLL_RATE_COUNT;
    static constexpr unsigned int CPU_RATE = CPU_RATE_HZ;
    static constexpr unsigned int CLOCK_RATE = CLOCK_RATE_HZ;

    Runner(const std::string &rom_file_name);
    ~Runner();

    void start();

private:
    Processor processor;
    SDL_Window *window;
    SDL_Surface *surface;
    SDL_Window *debug_window;
    SDL_Texture *debug_texture;
    SDL_Renderer *debug_renderer;
    TTF_Font *font;
    std::unordered_map<SDL_Keycode, u8> key_mapping;
    unsigned int x_scale;
    unsigned int y_scale;

    void draw_display(const Display &display);
    void modify_rectangle(SDL_Rect &rect, Display::Position position);
    void update_debug_window();
    void render_debug_text(const std::string &text, Display::Position position);
    void create_window();
    void create_debug_window();
    void create_key_mapping();
}; 
}