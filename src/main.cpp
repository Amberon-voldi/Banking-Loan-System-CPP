#include "gui.h"
#include "bank.h"
#include <vector>
#include <memory>

int main(){
    std::vector<std::unique_ptr<Customer>> customers;
    GUI gui;
    if(!gui.init()) return 1;

    loadCustomers(customers, "customers.txt");
    gui.run(customers);

    saveCustomers(customers, "customers.txt");
    return 0;
}
