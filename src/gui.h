#pragma once
#include "bank.h"
#include <vector>
#include <memory>

class GUI{
public:
    GUI();
    ~GUI();
    bool init();
    void run(std::vector<std::unique_ptr<Customer>>& customers);
private:
    struct Impl;
    Impl* p;
};
