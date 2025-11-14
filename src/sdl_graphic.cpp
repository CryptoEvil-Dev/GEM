/*
    DEPRECATED
*/

#include "include/sdl_graphic.hpp"
#include <iostream>
#include <unistd.h>
#include <chrono>
#include <cmath>

gem::GDriver::GDriver() {
    setenv("SDL_VIDEODRIVER", "kmsdrm", 1);
}

gem::GDriver::~GDriver() {
    cleanup();
}

bool gem::GDriver::init(int virtual_width, int virtual_height) {
    this->virtual_width = virtual_width;
    this->virtual_height = virtual_height;
    
    if (getuid() != 0) {
        std::cerr << "Error: Must be run as root! Use: sudo ./app" << std::endl;
        return false;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image init error: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init error: " << TTF_GetError() << std::endl;
        return false;
    }
    font = TTF_OpenFont("assets/font.ttf", 24);  // 24 - размер шрифта
    if (!font) {
        std::cerr << "TTF_OpenFont error: " << TTF_GetError() << std::endl;
        return false;
    }

    // Создание окна с поддержкой HighDPI
    window = SDL_CreateWindow(
        "Display Manager",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        0, 0,
        SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI
    );

    if (!window) {
        std::cerr << "Window creation error: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    // Создание рендерера с поддержкой сглаживания
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");  // Анизотропное сглаживание
    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE
    );

    if (!renderer) {
        std::cerr << "Renderer creation error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    // Создаем цель рендеринга для виртуального разрешения
    render_target = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        virtual_width,
        virtual_height
    );
    
    if (!render_target) {
        std::cerr << "Render target creation error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    // Загрузка курсора (опционально)
    if (!load_texture(&cursor_texture, "assets/cursor.png")) {
        std::cout << "Using default cursor" << std::endl;
    }

    return true;
}

bool gem::GDriver::load_texture(SDL_Texture** texture, const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "IMG_Load error: " << IMG_GetError() << std::endl;
        return false;
    }
    
    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    if (!*texture) {
        std::cerr << "Texture creation error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    return true;
}

void gem::GDriver::add_shell(const std::string& name, 
                            const std::string& command, 
                            const std::string& icon_path) {
    ShellTile tile;
    tile.name = name;
    tile.command = command;
    
    // Автоматическое расположение плиток
    int tile_size = virtual_width / 6;
    int margin = tile_size / 4;
    tile.rect = {
        margin + (tile_size + margin) * static_cast<int>(shells.size()),
        virtual_height / 2 - tile_size / 2,
        tile_size,
        tile_size
    };
    
    if (!icon_path.empty()) {
        load_texture(&tile.texture, icon_path);
    }
    
    shells.push_back(tile);
}

void gem::GDriver::handle_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) quit = true;
        
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_q || event.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            }
        }
        
        if (event.type == SDL_MOUSEMOTION) {
            // Масштабирование координат мыши
            int window_w, window_h;
            SDL_GetWindowSize(window, &window_w, &window_h);
            
            mouse_x = static_cast<int>((event.motion.x / static_cast<float>(window_w)) * virtual_width);
            mouse_y = static_cast<int>((event.motion.y / static_cast<float>(window_h)) * virtual_height);
        }
        
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                mouse_down = true;
            }
        }
        
        if (event.type == SDL_MOUSEBUTTONUP) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                mouse_down = false;
                
                // Проверка клика по плиткам
                for (auto& tile : shells) {
                    if (mouse_x >= tile.rect.x && 
                        mouse_x <= tile.rect.x + tile.rect.w &&
                        mouse_y >= tile.rect.y && 
                        mouse_y <= tile.rect.y + tile.rect.h) {
                        
                        if (click_handler) {
                            click_handler(tile.command);
                        }
                    }
                }
            }
        }
    }
}

void gem::GDriver::update() {
    // Обновление состояния плиток
    for (auto& tile : shells) {
        tile.hovered = (mouse_x >= tile.rect.x && 
                       mouse_x <= tile.rect.x + tile.rect.w &&
                       mouse_y >= tile.rect.y && 
                       mouse_y <= tile.rect.y + tile.rect.h);
    }
}

void gem::GDriver::render() {
    // Устанавливаем виртуальную цель рендеринга
    SDL_SetRenderTarget(renderer, render_target);
    
    // Очистка экрана
    SDL_SetRenderDrawColor(renderer, 
                          background_color.r, 
                          background_color.g, 
                          background_color.b, 
                          background_color.a);
    SDL_RenderClear(renderer);
    
    // Отрисовка плиток
    for (const auto& tile : shells) {
        // Фон плитки
        if (tile.hovered) {
            roundedRectangleRGBA(renderer, 
                                tile.rect.x - 5, 
                                tile.rect.y - 5, 
                                tile.rect.x + tile.rect.w + 5, 
                                tile.rect.y + tile.rect.h + 5, 
                                15, 
                                100, 200, 255, 180);
        }
        
        // Иконка
        if (tile.texture) {
            SDL_RenderCopy(renderer, tile.texture, nullptr, &tile.rect);
        } else {
            // Заглушка если нет иконки
            roundedRectangleRGBA(renderer, 
                                tile.rect.x, 
                                tile.rect.y, 
                                tile.rect.x + tile.rect.w, 
                                tile.rect.y + tile.rect.h, 
                                10, 
                                70, 130, 180, 255);
        }
        
        // Название
        const char* text = tile.name.c_str();
        int text_width = 8 * strlen(text); // Примерное вычисление
        // stringRGBA(renderer, 
        //           tile.rect.x + (tile.rect.w - text_width) / 2,
        //           tile.rect.y + tile.rect.h + 20,
        //           text,
        //           255, 255, 255, 255);
        SDL_Color text_color = {255,255,255,255};
        SDL_Surface* text_surface = TTF_RenderText_Blended(font, text, text_color);
        if (text_surface) {
        SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        if (text_texture) {
            SDL_Rect text_rect = {
                tile.rect.x, 
                tile.rect.y,
                text_surface->w,
                text_surface->h
            };
            SDL_RenderCopy(renderer, text_texture, nullptr, &text_rect);
            SDL_DestroyTexture(text_texture);
        }
        SDL_FreeSurface(text_surface);
        }

    }
    
    // Отрисовка курсора мыши
    if (cursor_texture) {
        SDL_Rect cursor_rect = {mouse_x - 16, mouse_y - 16, 32, 32};
        SDL_RenderCopy(renderer, cursor_texture, nullptr, &cursor_rect);
    } else {
        // Кастомный курсор: белый круг с черной обводкой
        filledCircleRGBA(renderer, mouse_x, mouse_y, 15, 255, 255, 255, 255);
        aacircleRGBA(renderer, mouse_x, mouse_y, 15, 0, 0, 0, 255);
        filledCircleRGBA(renderer, mouse_x, mouse_y, 3, 50, 50, 50, 255);
    }
    
    // Возвращаем рендер на экран и копируем виртуальное изображение
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderCopy(renderer, render_target, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void gem::GDriver::run() {
    const Uint32 frame_delay = 1000 / 60; // 60 FPS
    
    while (!quit) {
        Uint32 frame_start = SDL_GetTicks();
        
        handle_events();
        update();
        render();
        
        Uint32 frame_time = SDL_GetTicks() - frame_start;
        if (frame_delay > frame_time) {
            SDL_Delay(frame_delay - frame_time);
        }
    }
}

void gem::GDriver::cleanup() {
    for (auto& tile : shells) {
        if (tile.texture) {
            SDL_DestroyTexture(tile.texture);
        }
    }
    
    if (cursor_texture) SDL_DestroyTexture(cursor_texture);
    if (background_texture) SDL_DestroyTexture(background_texture);
    if (render_target) SDL_DestroyTexture(render_target);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    
    IMG_Quit();
    SDL_Quit();
}

void gem::GDriver::set_background(const std::string& image_path) {
    load_texture(&background_texture, image_path);
}

void gem::GDriver::set_background_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    background_color = {r, g, b, a};
}

void gem::GDriver::set_click_handler(std::function<void(const std::string&)> handler) {
    click_handler = handler;
}