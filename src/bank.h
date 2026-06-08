#pragma once
#include <string>
#include <vector>
#include <memory>

class Customer
{
public:
    virtual ~Customer() = default;
    virtual const std::string &name() const = 0;
    virtual void setName(const std::string &n) = 0;
    virtual double principal() const = 0;
    virtual void setPrincipal(double p) = 0;
    virtual double rate() const = 0;
    virtual void setRate(double r) = 0;
    virtual int years() const = 0;
    virtual void setYears(int y) = 0;
    virtual bool hasLoan() const = 0;
    virtual void setHasLoan(bool h) = 0;
    // runtime type tag for persistence
    virtual std::string typeName() const = 0;
};

// Simple concrete customer implementation (replaces previous POD)
class SimpleCustomer : public Customer
{
public:
    std::string name_;
    double principal_ = 0;
    double rate_ = 0;
    int years_ = 0;
    bool hasLoan_ = false;

    SimpleCustomer() = default;
    explicit SimpleCustomer(const std::string &n) : name_(n) {}

    const std::string &name() const override { return name_; }
    void setName(const std::string &n) override { name_ = n; }
    double principal() const override { return principal_; }
    void setPrincipal(double p) override { principal_ = p; }
    double rate() const override { return rate_; }
    void setRate(double r) override { rate_ = r; }
    int years() const override { return years_; }
    void setYears(int y) override { years_ = y; }
    bool hasLoan() const override { return hasLoan_; }
    void setHasLoan(bool h) override { hasLoan_ = h; }
    std::string typeName() const override { return "Simple"; }
};

double monthlyPayment(double P, double rPercent, int y);

// Persistence helpers work with polymorphic customers stored as unique_ptrs
bool saveCustomers(const std::vector<std::unique_ptr<Customer>> &customers, const std::string &path);
bool loadCustomers(std::vector<std::unique_ptr<Customer>> &customers, const std::string &path);
