/*
    DEPRECATED
*/

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

gem::DOM::~DOM(){
    if(this->render_target) SDL_DestroyTexture(this->render_target);
    if(this->renderer) SDL_DestroyRenderer(this->renderer);
    if(this->window) SDL_DestroyWindow(this->window);

    IMG_Quit();
    SDL_Quit();
}

void gem::DOM::append(Node* el){
    this->tree.push_back(el);
    addToFlatList(el);
}

void gem::DOM::remove(Node* el){
    auto it = std::find(tree.begin(), tree.end(), el);
    if(it != tree.end()){
        tree.erase(it);
    }

    removeFromFlatList(el);

    if(el->parent){
        el->parent->remove_child(el);
    }
}

void gem::DOM::addToFlatList(Node* node){
    all_nodes.push_back(node);
    for(Node* child : node->childs){
        addToFlatList(child);
    }
}

void gem::DOM::removeFromFlatList(Node* node){
    auto it = std::find(all_nodes.begin(), all_nodes.end(), node);
    if(it != all_nodes.end()){
        all_nodes.erase(it);
    }

    for(Node* child : node->childs){
        removeFromFlatList(child);
    }
}

std::vector<gem::Node*> gem::DOM::getAllNodes() const{
    return all_nodes;
}



gem::Node::Node(DOM* _DOM) : dom(_DOM){
    this->coord[0] = 0;
    this->coord[1] = 0;
    this->size[0] = 10;
    this->size[1] = 10;
    this->rect = SDL_Rect{coord[0],coord[1],size[0],size[1]};

    this->dom->append(this);
}
gem::Node::Node(const Node& other){
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

    this->rect.x = this->coord[0];
    this->rect.y = this->coord[1];

    this->rect.w = this->size[0];
    this->rect.h = this->size[1];


    this->mouseDownCallback = other.mouseDownCallback;
    this->mouseUpCallback = other.mouseUpCallback;
    this->mouseEnterCallback = other.mouseEnterCallback;
    this->mouseLeaveCallback = other.mouseLeaveCallback;
    this->mouseMoveCallback = other.mouseMoveCallback;

    this->rootNode = other.rootNode;
    if(other.childs.size() != 0){
        for(int i = 0; i < other.childs.size(); i++) this->childs.push_back(other.childs[i]);
    }

    this->dom = other.dom;
    this->dom->append(this);
}

gem::Node::~Node(){
    if(dom) dom->remove(this);

    for(Node* child : childs){
        delete child;
    }
    childs.clear();
}



bool gem::Node::contains(int x, int y){
    return (x >= rect.x) && (x < rect.x + rect.w) &&
           (y >= rect.y) && (y < rect.y + rect.h);
}




// Other Modification

void gem::Node::addEventListeneer(EventType type, EventCallback cb) {
    callbacks[type] = std::move(cb);
}

void gem::Node::removeEventListeneer(EventType type) {
    callbacks.erase(type);
}

void gem::Node::append_child(Node* child) {
    child->parent = this;
    childs.push_back(child);
    if(dom) dom->addToFlatList(child);
}

void gem::Node::remove_child(Node* child) {
    auto it = std::find(childs.begin(), childs.end(), child);
    if(it != childs.end()) {
        childs.erase(it);
        if(dom) dom->removeFromFlatList(child);
    }
}

// Реализация EventManager
void gem::EventManager::handleEvent(SDL_Event& event) {
    switch(event.type) {
        case SDL_MOUSEMOTION: {
            int x = event.motion.x;
            int y = event.motion.y;
            
            // Находим верхний элемент под курсором
            Node* topNode = findTopNodeAtPosition(x, y);
            
            // Обработка изменения состояния hover
            if(topNode != lastHoveredNode) {
                if(lastHoveredNode) {
                    lastHoveredNode->mouseHovered = false;
                    if(lastHoveredNode->callbacks.count(EventType::MOUSE_LEAVE)) {
                        lastHoveredNode->callbacks[EventType::MOUSE_LEAVE](event);
                    }
                }
                
                if(topNode) {
                    topNode->mouseHovered = true;
                    if(topNode->callbacks.count(EventType::MOUSE_ENTER)) {
                        topNode->callbacks[EventType::MOUSE_ENTER](event);
                    }
                }
                
                lastHoveredNode = topNode;
            }
            
            // Вызов события перемещения мыши
            if(lastHoveredNode && lastHoveredNode->callbacks.count(EventType::MOUSE_MOVE)) {
                lastHoveredNode->callbacks[EventType::MOUSE_MOVE](event);
            }
            break;
        }
        
        case SDL_MOUSEBUTTONDOWN: {
            if(event.button.button == SDL_BUTTON_LEFT) {
                pressedNode = lastHoveredNode;
                if(pressedNode) {
                    pressedNode->mousePressed = true;
                    if(pressedNode->callbacks.count(EventType::MOUSE_DOWN)) {
                        pressedNode->callbacks[EventType::MOUSE_DOWN](event);
                    }
                }
            }
            break;
        }
        
        case SDL_MOUSEBUTTONUP: {
            if(event.button.button == SDL_BUTTON_LEFT && pressedNode) {
                pressedNode->mousePressed = false;
                if(pressedNode->callbacks.count(gem::EventType::MOUSE_UP)) {
                    pressedNode->callbacks[gem::EventType::MOUSE_UP](event);
                }
                
                // Если кнопка отпущена над тем же элементом - это клик
                if(lastHoveredNode == pressedNode) {
                    if(pressedNode->callbacks.count(gem::EventType::CLICK)) {
                        pressedNode->callbacks[gem::EventType::CLICK](event);
                    }
                }
                
                pressedNode = nullptr;
            }
            break;
        }
    }
}

gem::Node* gem::EventManager::findTopNodeAtPosition(int x, int y) const {
    // Ищем в обратном порядке (от верхних к нижним)
    for(auto it = dom->all_nodes.rbegin(); it != dom->all_nodes.rend(); ++it) {
        Node* node = *it;
        if(node->contains(x, y)) {
            return node;
        }
    }
    return nullptr;
}
