#include <iostream>
#include "include/Engine.hpp"

#include <thread>
#include <chrono>

int main() {
    gem::DOM dom(800, 600);

    std::this_thread::sleep_for(std::chrono::seconds(3));

    return 0;
}