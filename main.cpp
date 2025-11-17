#include <iostream>
#include "include/DOM.hpp"
#include "include/Node.hpp"

#include <thread>
#include <chrono>

int main() {
    GEM::DOM dom(800, 600);
    std::vector<GEM::Node> main_page;
    GEM::Node header("Hello World!");
    main_page.push_back(header);
    dom.SetPage(main_page);

    std::this_thread::sleep_for(std::chrono::seconds(3));

    return 0;
}