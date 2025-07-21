#include "include/Engine.hpp"


gem::DOM::DOM(int v_width, int v_height){
    setenv("SDL_VIDEODRIVER", "kmsdrm", 1);

    this->virtual_width = v_width;
    this->virtual_height = v_height;

    if(getuid() != 0){
        std::cerr << "Error: Must be run as root! Use: sudo ./GEM" << std::endl;
        this->~DOM();
    }

    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
        this->~DOM();
    }

    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
        std::cerr << "SDL_image init error: " << IMG_GetError() << std::endl;
        SDL_Quit();
        this->~DOM();
    }

    if(TTF_Init() == -1){
        std::cerr << "TTF_Init error: " << TTF_GetError() << std::endl;
        this->~DOM();
    }

    this->window = SDL_CreateWindow(
        "GEM",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        0, 0,
        SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI
    );

    if(!window){
        std::cerr << "Window creation error: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        this->~DOM();
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    this->renderer = SDL_CreateRenderer(
        this->window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE
    );

    if(!renderer){
        std::cerr << "Renderer creation error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(this->window);
        IMG_Quit();
        SDL_Quit();
        this->~DOM();
    }

    this->render_target = SDL_CreateTexture(
        this->renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        this->virtual_width,
        this->virtual_height
    );

    if(!this->render_target){
        std::cerr << "Render target creation error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(this->renderer);
        SDL_DestroyWindow(this->window);
        IMG_Quit();
        SDL_Quit();
        this->~DOM();
    }
}

gem::DOM::~DOM(){
    if(this->render_target) SDL_DestroyTexture(this->render_target);
    if(this->renderer) SDL_DestroyRenderer(this->renderer);
    if(this->window) SDL_DestroyWindow(this->window);

    IMG_Quit();
    SDL_Quit();
}