#ifndef _GEM_ENGINE_
#define _GEM_ENGINE_
#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <string>
#include <vector>
#include <memory>
#include <functional>

#include <iostream>
#include <unistd.h>
#include <cmath>

namespace gem
{

enum class EventType {MOUSE_DOWN, MOUSE_UP, MOUSE_ENTER, MOUSE_LEAVE, MOUSE_MOVE, FOCUS, CLICK};

typedef struct{
    Uint8 background_color[4]; // rgba
    Uint8 color[4]; // rgba
    SDL_Texture* background_image = nullptr;
    TTF_Font* font_family = nullptr;
    int font_size;
    int position[4]; // left, top, right, bottom
    int size[2]; // wigth, height
} style;

class DOM{
public:
    DOM(int v_width, int v_height);
    DOM(const DOM&) = delete;
    DOM(DOM&&) = delete;

    ~DOM();

    void append(Node* el); // Добавить элемент
    void remove(Node* el); // Удалить элемент

    std::vector<Node*> getAllNodes() const;
    void addToFlatList(Node* node);
    void removeFromFlatList(Node* node);

// protected:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* render_target;
    SDL_Event event;
    std::vector<Node*> tree;
    std::vector<Node*> all_nodes;
    bool running;

private:
    int virtual_width;
    int virtual_height;


};

class Node{
public:
    Node(DOM*);
    Node(const Node&) noexcept;
    Node(const Node&&) noexcept;

    virtual ~Node() = default;

    virtual void geometry(int x, int y, int w, int h);

    using EventCallback = std::function<void(SDL_Event&)>;
    virtual void handleEvent(SDL_Event& event);
    // virtual void setMouseDownCallback(EventCallback cb) { mouseDownCallback = std::move(cb); };
    // virtual void setMouseUpCallback(EventCallback cb) { mouseUpCallback = std::move(cb); }
    // virtual void setMouseEnterCallback(EventCallback cb) { mouseEnterCallback = std::move(cb); }
    // virtual void setMouseLeaveCallback(EventCallback cb) { mouseLeaveCallback = std::move(cb); }
    // virtual void setMouseMoveCallback(EventCallback cb) { mouseMoveCallback = std::move(cb); }

    virtual void addEventListeneer(gem::EventType event_type, EventCallback cb);
    virtual void removeEventListeneer(gem::EventType event_type);

    virtual void get_coord(int& x, int& y) const;
    virtual void get_size(int& w, int& h) const;

    virtual void left(int px);
    virtual void top(int px);
    virtual void right(int px);
    virtual void bottom(int px);

    virtual void width(int px);
    virtual void height(int px);

    virtual void background_image(SDL_Texture* image);
    virtual void content(std::string content);

    virtual void append_child(Node* child);
    virtual void remove_child(Node* child);
    virtual void style(const gem::style& style);


    Node* parent = nullptr;
    std::vector<Node*> childs; // Дети элемента
    bool contains(int x, int y);

// protected:
    EventCallback mouseDownCallback;
    EventCallback mouseUpCallback;
    EventCallback mouseEnterCallback;
    EventCallback mouseLeaveCallback;
    EventCallback mouseMoveCallback;
    
    Node* rootNode = nullptr; // Родительский элемент, от которого будет браться смещение (виртуальные координаты)
    
    bool mouseHovered = false;
    bool mousePressed = false;
    bool focused = false;

    SDL_Rect rect;

    std::unordered_map<EventType, EventCallback> callbacks;
private:


    DOM* dom = nullptr;
    
    Uint8 background_color[4];
    Uint8 border_color[4];
    std::string content;
    TTF_Font* font = nullptr;
    int font_size;
    int coord[2];
    int size[2];

};

class EventManager {
    public:
        EventManager(gem::DOM* dom) : dom(dom) {};
        EventManager(const EventManager&) = delete;
        EventManager(EventManager&&) = delete;

        void handleEvent(SDL_Event& event);

    private:
        gem::DOM* dom = nullptr;
        Node* lastHoveredNode = nullptr;
        Node* pressedNode = nullptr;

        Node* findTopNodeAtPosition(int x, int y) const;
    };


} // namespace gem



#endif