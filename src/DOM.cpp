#include "include/DOM.hpp"


GEM::DOM::DOM(int v_width, int v_height){
    if(isatty(STDIN_FILENO))
        setenv("SDL_VIDEODRIVER", "kmsdrm", 1);
    else
        unsetenv("SDL_VIDEODRIVER");
    this->v_width = v_width;
    this->v_height = v_height;

    if(getuid() != 0)
        throw std::runtime_error("Error: Must be run as root!\nUse: sudo ./GEM");

    if(SDL_Init(SDL_INIT_VIDEO) != 0)
        throw std::runtime_error("SDL_Init error: " + std::string(SDL_GetError()));
    
    if(!IMG_Init((IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
        throw std::runtime_error("SDL_Image init err: " + std::string(IMG_GetError()));


    if(TTF_Init() == -1)
        throw std::runtime_error("TTF_Init error: " + std::string(TTF_GetError()));
    

    this->window = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>(
        SDL_CreateWindow(
            "GEM",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            0, 0,
            SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI
        ), SDL_DestroyWindow
    );

    if(!window)
        throw std::runtime_error("Window creation error: " + std::string(SDL_GetError()));

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    this->renderer = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>(
        SDL_CreateRenderer(
            this->window.get(),
            -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE
        ), SDL_DestroyRenderer
    );

    if(!renderer)
        throw std::runtime_error("Renderer creation error: " + std::string(SDL_GetError()));

    this->render_target = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>(
        SDL_CreateTexture(
            this->renderer.get(),
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            this->v_width,
            this->v_height
        ), SDL_DestroyTexture
    );
    

    if(!this->render_target){
        throw std::runtime_error("Render target creation error: " + std::string(SDL_GetError()));
    }

}

GEM::DOM::~DOM(){
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void GEM::DOM::SetPage(std::vector<Node> page){
    this->VDOM = page;
}

const std::vector<GEM::Node>& GEM::DOM::GetPage() const{
    return this->VDOM;
}