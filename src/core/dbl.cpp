#include "dbl.h"

#include <cmath>
#include <ostream>


namespace jle {


dbl pow (const dbl& base,    const dbl& exp )
{
    return std::pow(double(base), double(exp));
}


dbl log (const dbl& d                       )
{
    return std::log(double(d));
}


dbl exp (const dbl& d                       )
{
    return std::exp(double(d));
}


int get_decimals10(const jle::dbl& d)
{
    int sign=  d<0. ? -1  : 1;

    auto pow10= jle::dbl{1};
    for(int i=0; i<15; ++i, pow10*=10.)
    {
        if(static_cast<double>(
                    static_cast<int>(
                        static_cast<double>(d*pow10  + 0.49999*sign)))
                    / pow10
                    == d)
            return i;
    }
    throw "error ...";
}


};      //  namespace jle {




std::ostream& operator<< (std::ostream& os, const jle::dbl& d)
{
    os << double(d);
    return os;
}

jle::dbl operator+ (const jle::dbl& d1, const jle::dbl& d2)
{
    return double(d1) + double(d2);
}

jle::dbl operator- (const jle::dbl& d1, const jle::dbl& d2)
{
    return double(d1) - double(d2);
}

jle::dbl operator* (const jle::dbl& d1, const jle::dbl& d2)
{
    return double(d1) * double(d2);
}

jle::dbl operator/ (const jle::dbl& d1, const jle::dbl& d2)
{
    return double(d1) / double(d2);
}

jle::dbl& operator+= (jle::dbl& d1, const jle::dbl& d2)
{
    d1 = double(d1) + double(d2);
    return d1;
}

jle::dbl& operator-= (jle::dbl& d1, const jle::dbl& d2)
{
    d1 = double(d1) - double(d2);
    return d1;
}

jle::dbl& operator*= (jle::dbl& d1, const jle::dbl& d2)
{
    d1 = double(d1) * double(d2);
    return d1;
}

jle::dbl& operator/= (jle::dbl& d1, const jle::dbl& d2)
{
    d1 = double(d1) / double(d2);
    return d1;
}

bool     operator    ==  (const jle::dbl&  _d1, const jle::dbl&  _d2)
{
    auto d1 = double(_d1);
    auto d2 = double(_d2);

    //  managing zero in a symetric way
    if(d2<1e-300  &&  d2>-1e-300)        //  it is interpreted as exact zero
    {       //  here we don't consider the exponent
        if( d1 > -1e-13  &&  d1 < 1e-13)
            return true;
        else
            return false;

    }
    if(d1<1e-300  &&  d1>-1e-300)        //  it is interpreted as exact zero
    {       //  here we don't consider the exponent
        if( d2 > -1e-13  &&  d2 < 1e-13)
            return true;
        else
            return false;
    }

    //  checking for not zeros
    double div = d1 / d2;
    if( div > 1-1e-13  &&  div < 1+1e-13)
        return true;
    else
        return false;
}

bool     operator    !=  (const jle::dbl&  d1, const jle::dbl&  d2)
{
    return !(d1==d2);
}

bool     operator    <   (const jle::dbl&  d1, const jle::dbl&  d2)
{
    if(d1==d2)  return false;
    else        return double(d1)<double(d2);
}

bool     operator    <=  (const jle::dbl&  d1, const jle::dbl&  d2)
{
    if(d1==d2)  return true;
    else        return double(d1)<double(d2);
}

bool     operator    >   (const jle::dbl&  d1, const jle::dbl&  d2)
{
    if(d1==d2)  return false;
    else        return double(d1)>double(d2);
}

bool     operator    >=  (const jle::dbl&  d1, const jle::dbl&  d2)
{
    if(d1==d2)  return true;
    else        return double(d1)>double(d2);
}

