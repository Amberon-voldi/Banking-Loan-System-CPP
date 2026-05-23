#pragma once
#include "bank.h"
#include <vector>

class GUI{
public:
    GUI();
    ~GUI();
    bool init();
    void run(std::vector<Customer>& customers);
private:
    struct Impl;
    Impl* p;
};
