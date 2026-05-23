#include "gui.h"
#include "bank.h"
#include <vector>

int main(){
    std::vector<Customer> customers;
    GUI gui;
    if(!gui.init()) return 1;
    gui.run(customers);
    return 0;
}
