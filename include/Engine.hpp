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

enum class EventType {MOUSE_DOWN, MOUSE_UP, MOUSE_ENTER, MOUSE_LEAVE, MOUSE_MOVE, FOCUS};

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
    void remove(const Node& el); // Удалить элемент

    std::vector<Node&> tree;
protected:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* render_target;
    SDL_Event event;

private:
    int virtual_width;
    int virtual_height;

    bool running;
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
    virtual void setMouseDownCallback(EventCallback cb) { mouseDownCallback = std::move(cb); };
    virtual void setMouseUpCallback(EventCallback cb) { mouseUpCallback = std::move(cb); }
    virtual void setMouseEnterCallback(EventCallback cb) { mouseEnterCallback = std::move(cb); }
    virtual void setMouseLeaveCallback(EventCallback cb) { mouseLeaveCallback = std::move(cb); }
    virtual void setMouseMoveCallback(EventCallback cb) { mouseMoveCallback = std::move(cb); }

    virtual void addEventListeneer(gem::EventType event_type, EventCallback cb);

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

    virtual void append_child(Node& child);
    virtual void style(const gem::style&);

protected:

    bool contains(int x, int y);

    EventCallback mouseDownCallback;
    EventCallback mouseUpCallback;
    EventCallback mouseEnterCallback;
    EventCallback mouseLeaveCallback;
    EventCallback mouseMoveCallback;
    std::vector<Node&> childs; // Дети элемента
    Node* rootNode = nullptr; // Родительский элемент, от которого будет браться смещение (виртуальные координаты)
    
    bool mouseHovered = false;
    bool focused = false;

private:
    DOM* dom = nullptr;
    
    Uint8 background_color[4];
    Uint8 border_color[4];
    std::string content;
    TTF_Font* font = nullptr;
    int font_size;
    int coord[2];
    int size[2];

    SDL_Rect* rect = nullptr; // На всякий случай!
};

class EventManager{
public:
    EventManager(gem::DOM* DOM) : root(DOM){};
    EventManager(const EventManager&) = delete;
    EventManager(const EventManager&&) = delete;

    void HandleAllEvents();

private:
    gem::DOM* root = nullptr;
};

} // namespace gem



#endif