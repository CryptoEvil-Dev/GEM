/*
    DEPRECATED
*/

#ifndef _GEM_SDL_GRAPHIC_
#define _GEM_SDL_GRAPHIC_

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>  // Для сглаженных примитивов
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <functional>
#include <string>

namespace gem {

enum class rc_code { success, null, init_err, window_error, renderer_error };

struct ShellTile {
    SDL_Rect rect;
    std::string name;
    std::string command;
    SDL_Texture* texture = nullptr;
    bool hovered = false;
};

class GDriver {
public:
    GDriver();
    GDriver(const GDriver&) = delete;
    GDriver& operator=(const GDriver&) = delete;
    ~GDriver();
    
    bool init(int virtual_width = 1920, int virtual_height = 1080);
    void run();
    
    // Добавляем оболочку
    void add_shell(const std::string& name, 
                   const std::string& command, 
                   const std::string& icon_path = "");
    
    // Установка фона
    void set_background(const std::string& image_path);
    void set_background_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
    
    // Обработчики событий
    void set_click_handler(std::function<void(const std::string&)> handler);
    
private:
    void handle_events();
    void update();
    void render();
    void cleanup();
    bool load_texture(SDL_Texture** texture, const std::string& path);
    
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* cursor_texture = nullptr;
    SDL_Texture* background_texture = nullptr;
    SDL_Texture* render_target = nullptr;  // Для виртуального разрешения
    SDL_Color background_color = {30, 30, 45, 255};

    TTF_Font* font = nullptr;  
    
    std::vector<ShellTile> shells;
    std::function<void(const std::string&)> click_handler;
    
    bool quit = false;
    int mouse_x = 0, mouse_y = 0;
    int virtual_width = 1920, virtual_height = 1080;
    bool mouse_down = false;
    Uint32 last_frame_time = 0;
};

} // namespace gem

#endif