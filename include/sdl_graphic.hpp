#ifndef _GEM_SDL_GRAPHIC_
#define _GEM_SDL_GRAPHIC_

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <iostream>

namespace gem{

enum class rc_code{success, null, init_err, window_error, renderer_error};

class GDriver{
public:
    GDriver();
    GDriver(const GDriver&) = delete;
    GDriver(const GDriver&&) = delete;
    
    void clear(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);
    void rect(int x1, int y1, int x2, int y2, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);

    SDL_Window *getWindow();
    SDL_Renderer *getRenderer();
    SDL_Event *getEvent();

    ~GDriver();

private:
    rc_code err_code = gem::rc_code::null;

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event *event;
};


}

#endif