#include "bank.h"
#include <cmath>

double monthlyPayment(double P,double rPercent,int y){int n=y*12; if(n==0) return 0; double r=rPercent/100.0/12.0; if(r==0) return P/n; double v=std::pow(1+r,n); return P*r*v/(v-1);}
