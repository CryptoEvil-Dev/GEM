#ifndef _DOM_HPP_
#define _DOM_HPP_
#pragma once

#include "include/Node.hpp"
#include <iostream>
#include <unistd.h>

namespace GEM{
class DOM{
public:
    DOM(int v_width, int v_height);
    DOM(DOM&) = delete;
    DOM(DOM&&) = delete;
    ~DOM();
    
    void SetPage(std::vector<Node> page);
    const std::vector<Node>& GetPage() const;

private:
    std::vector<Node> VDOM;

    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window{nullptr, SDL_DestroyWindow};
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer{nullptr, SDL_DestroyRenderer};
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> render_target{nullptr, SDL_DestroyTexture};
    SDL_Event event;

    int v_width;
    int v_height;
    };

}



#endif