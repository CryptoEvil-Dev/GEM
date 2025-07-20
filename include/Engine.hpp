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

namespace gem
{

class DOM{
public:
    DOM();
    DOM(const DOM&) = delete;
    DOM(DOM&&) = delete;

    void append(const Node& el); // Добавить элемент
    void remove(const Node& el); // Удалить элемент

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Event event;

    std::vector<Node&> tree;
};

class Node{
public:
    Node(const DOM&);
    Node(const Node&) noexcept;
    Node(const Node&&) noexcept;

    virtual ~Node() = default;

    using EventCallback = std::function<void(SDL_Event&)>;
    virtual void handleEvent(SDL_Event& event);
    void setMouseDownCallback(EventCallback cb) { mouseDownCallback = std::move(cb); };
    void setMouseUpCallback(EventCallback cb) { mouseUpCallback = std::move(cb); }
    void setMouseEnterCallback(EventCallback cb) { mouseEnterCallback = std::move(cb); }
    void setMouseLeaveCallback(EventCallback cb) { mouseLeaveCallback = std::move(cb); }
    void setMouseMoveCallback(EventCallback cb) { mouseMoveCallback = std::move(cb); }

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

protected:
    EventCallback mouseDownCallback;
    EventCallback mouseUpCallback;
    EventCallback mouseEnterCallback;
    EventCallback mouseLeaveCallback;
    EventCallback mouseMoveCallback;
    bool mouseHovered = false;

private:
    const DOM& dom;
    Node* rootNode = nullptr; // Родительский элемент, от которого будет браться смещение (виртуальные координаты)
    Uint8 background_color[4];
    Uint8 border_color[4];
    std::string content;
    TTF_Font* font = nullptr;
    int font_size;
    int coord[2];
    int size[2];

    SDL_Rect* rect = nullptr; // На всякий случай!
};

class div : public Node{
public:
    div();
    div(const div& other) : Node(other){};
    div(div&& other) noexcept : Node(std::move(other)){};
};


} // namespace gem



#endif