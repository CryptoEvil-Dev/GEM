#include <iostream>
#include "include/sdl_graphic.hpp"

int main() {
    gem::GDriver driver;
    
    // Инициализация с высоким разрешением
    if (!driver.init(2560, 1440)) {
        std::cerr << "Failed to initialize graphics driver!" << std::endl;
        return 1;
    }
    
    // Добавление графических оболочек
    driver.add_shell("GNOME", "startx /usr/bin/gnome-session", "assets/gnome.png");
    driver.add_shell("KDE", "startx /usr/bin/startplasma-x11", "assets/kde.png");
    driver.add_shell("XFCE", "startx /usr/bin/startxfce4", "assets/xfce.png");
    driver.add_shell("LXQt", "startx /usr/bin/startlxqt", "assets/lxqt.png");
    driver.add_shell("LXQt", "startx /usr/bin/startlxqt", "assets/lxqt.png");
    driver.add_shell("LXQt", "startx /usr/bin/startlxqt", "assets/lxqt.png");
    driver.add_shell("LXQt", "startx /usr/bin/startlxqt", "assets/lxqt.png");
    
    // Обработчик клика
    driver.set_click_handler([](const std::string& command) {
        std::cout << "Launching: " << command << std::endl;
        // Для реального использования раскомментируйте:
        // driver.cleanup();
        // system(command.c_str());
        // exit(0);
    });
    
    // Запуск основного цикла
    driver.run();
    
    return 0;
}