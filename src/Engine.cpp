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



gem::Node::Node(DOM* _DOM){
    this->dom = _DOM;
    this->dom->append(this);
}
gem::Node::Node(const Node& other){

    this->dom = other.dom;
    this->dom->append(this);

    this->background_color[0] = other.background_color[0];
    this->background_color[1] = other.background_color[1];
    this->background_color[2] = other.background_color[2];
    this->background_color[3] = other.background_color[3];

    this->border_color[0] = other.border_color[0];
    this->border_color[1] = other.border_color[1];
    this->border_color[2] = other.border_color[2];
    this->border_color[3] = other.border_color[3];

    this->content = other.content;
    this->font = other.font;
    this->font_size = other.font_size;

    this->coord[0] = other.coord[0];
    this->coord[1] = other.coord[1];

    this->size[0] = other.size[0];
    this->size[1] = other.size[1];

    this->rect->x = this->coord[0];
    this->rect->y = this->coord[1];

    this->rect->w = this->size[0];
    this->rect->h = this->size[1];


    this->mouseDownCallback = other.mouseDownCallback;
    this->mouseUpCallback = other.mouseUpCallback;
    this->mouseEnterCallback = other.mouseEnterCallback;
    this->mouseLeaveCallback = other.mouseLeaveCallback;
    this->mouseMoveCallback = other.mouseMoveCallback;

    this->rootNode = other.rootNode;
    if(other.childs.size() != 0){
        for(int i = 0; i < other.childs.size(); i++) this->childs.push_back(other.childs[i]);
    }
}




bool gem::Node::contains(int x, int y){
    return (x >= this->coord[0] && x < this->coord[0] + this->size[0] && y >= this->coord[1] + this->size[1]);
}