#ifndef _NODE_HPP_
#define _NODE_HPP_
#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <string>
#include <vector>
#include <memory>

namespace GEM{

struct Style{
    unsigned char color[4]; // RGBA 0-255
    unsigned char background[4]; // RGBA 0-255
    SDL_Texture* image; // background-image
    TTF_Font* font;
    int font_size;
    int padding[4]; // left, top, right, bottom
    int width;
    int height;

    int left;
    int top;
    int right;
    int bottom;
};


class Node{
public:
    Node();
    Node(std::string content);
    Node(Style style);
    Node(std::string content, Style style);
    Node(const Node&) noexcept;
    Node(Node&&) noexcept;

    Node& operator = (const Node&& other) noexcept;
    Node& operator = (const Node& other) noexcept;

    ~Node() = default;

    std::vector<Node>& GetChilds();
    Node* GetChild(size_t index);
    void AppendChild(Node&);
    void RemoveChild(size_t index);

    void SetContent(const std::string& content);
    const std::string& GetContent() const;


private:
    std::unique_ptr<Style> style;
    std::vector<Node> tree;
    std::string content;

};


}
#endif