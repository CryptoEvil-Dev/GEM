#include <iostream>
#include "include/DOM.hpp"
#include "include/GReport.hpp"

#include <thread>
#include <chrono>

int main() {
    GTEST::GReport rep(800, 600);
    rep.RunAll();
    rep.Print();
    rep.Save("./GReport.txt");

    return 0;
}