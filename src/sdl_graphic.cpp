#include "include/sdl_graphic.hpp"

gem::GDriver::GDriver(){
    setenv("SDL_VIDEODRIVER", "kmsdrm", 1);

    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
        this->err_code = gem::rc_code::init_err;
        this->~GDriver();
    }

    this->window = SDL_CreateWindow(
        "Graphic Environment Manager v0.1",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        0, 0,
        SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL
    );
    if(!this->window){
        std::cerr << "Window creation error: " << SDL_GetError() << std::endl;
        this->err_code = gem::rc_code::window_error;
        this->~GDriver();
    };

    this->renderer = SDL_CreateRenderer(
        this->window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if(!this->renderer){
        std::cerr << "Renderer creation error: " << SDL_GetError() << std::endl;
        this->err_code = gem::rc_code::renderer_error;
        this->~GDriver();
    }
}

void gem::GDriver::clear(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha){
    SDL_SetRenderDrawColor(this->renderer, red, green, blue, alpha);
    SDL_RenderClear(this->renderer);
}

void gem::GDriver::rect(int x1, int y1, int x2, int y2, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha){
    SDL_Rect tile = {x1, y1, x2, y2};
    SDL_SetRenderDrawColor(this->renderer, red, green, blue, alpha);
    SDL_RenderFillRect(this->renderer, &tile);

    SDL_RenderPresent(this->renderer);
}


SDL_Window* gem::GDriver::getWindow(){
    return this->window;
}
SDL_Renderer* gem::GDriver::getRenderer(){
    return this->renderer;
}
SDL_Event* gem::GDriver::getEvent(){
    return this->event;
}

gem::GDriver::~GDriver(){
    switch (this->err_code)
    {
    case gem::rc_code::init_err:
        break;
    case gem::rc_code::window_error:
        SDL_Quit();
        break;
    case gem::rc_code::renderer_error:
        SDL_DestroyWindow(this->window);
        SDL_Quit();
        break;
    
    default:
        SDL_DestroyRenderer(this->renderer);
        SDL_DestroyWindow(this->window);
        SDL_Quit();
        break;
    }
}