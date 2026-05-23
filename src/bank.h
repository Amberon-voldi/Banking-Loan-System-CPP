#pragma once
#include <string>
#include <vector>

struct Customer{std::string name;double principal=0;double rate=0;int years=0;bool hasLoan=false;};

double monthlyPayment(double P,double rPercent,int y);
