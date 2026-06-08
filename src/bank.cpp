#include "bank.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <memory>

double monthlyPayment(double P, double rPercent, int y)
{
    int n = y * 12;
    if (n == 0)
        return 0;
    double r = rPercent / 100.0 / 12.0;
    if (r == 0)
        return P / n;
    double v = std::pow(1 + r, n);
    return P * r * v / (v - 1);
}

bool saveCustomers(const std::vector<std::unique_ptr<Customer>>& customers, const std::string& path)
{
    std::ofstream ofs(path);
    if (!ofs)
        return false;
    for (const auto &cptr : customers)
    {
        const Customer &c = *cptr;
        // format: type TAB "name" TAB principal TAB rate TAB years TAB hasLoan\n
        ofs << c.typeName() << '\t' << std::quoted(c.name()) << '\t' << c.principal() << '\t' << c.rate() << '\t' << c.years() << '\t' << (c.hasLoan() ? 1 : 0) << '\n';
        if (!ofs)
            return false;
    }
    return true;
}

bool loadCustomers(std::vector<std::unique_ptr<Customer>> &customers, const std::string &path)
{
    std::ifstream ifs(path);
    if (!ifs)
        return false;
    customers.clear();
    std::string type;
    while (ifs >> type)
    {
        std::string name;
        if (!(ifs >> std::quoted(name)))
            break;
        double principal = 0.0;
        double rate = 0.0;
        int years = 0;
        int hasLoanInt = 0;
        if (!(ifs >> principal >> rate >> years >> hasLoanInt))
            break;
        if (type == "Simple")
        {
            auto c = std::make_unique<SimpleCustomer>();
            c->setName(name);
            c->setPrincipal(principal);
            c->setRate(rate);
            c->setYears(years);
            c->setHasLoan(hasLoanInt != 0);
            customers.push_back(std::move(c));
        }
        else
        {
            // unknown type: skip
            continue;
        }
    }
    return true;
}
