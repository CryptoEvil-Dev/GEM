#include <iostream>
#include "include/DOM.hpp"
#include "include/GReport.hpp"

#include <thread>
#include <chrono>

int main() {
    // gem::DOM dom(800, 600);
    // GEM::DOM* dom = new GEM::DOM(800, 600);

    // std::this_thread::sleep_for(std::chrono::seconds(3));
    // delete dom;
    GTEST::GReport rep(800, 600);
    rep.RunAll();
    rep.Print();
    rep.Save("./benchmark.txt");

    return 0;
}