#include <iostream>
#include "core/optional.hpp"
#include "core/dbl.h"
#include "core/misc.h"



namespace jle {

};


struct pr_t {
    int         a;
    jle::dbl    b;

    bool operator==(const pr_t& pr) const {
        return this->a == pr.a  &&  this->b == pr.b;
    }
};


int main (void)
{

    {
        jle::optional<pr_t>  oi = pr_t{1, 3.1415926535};
        oi = pr_t{1, 3.1415926535};

        //  checking if has a value
        std::cout << oi.has_value() << std::endl;
        std::cout << int(bool(oi)) << std::endl;

        //  accessing element
        std::cout << oi->a << std::endl;
        oi->a = 55;
        std::cout << oi->a << std::endl;

        //  clear value
        oi ={};
        try {
            std::cout << int(bool(oi)) << std::endl;
            std::cout << oi->a << std::endl;
            std::cout << oi->b << std::endl;
        } catch(...) {
            std::cout << "exception" << std::endl;
        }
    }

    {
        jle::optional<std::string>  maybe_s;
        maybe_s = std::string{"Hi there"};
        std::cout << maybe_s.value_or("Hi here") << std::endl;
    }

    {
        jle::optional<std::string>  maybe_s;
        std::cout << maybe_s.value_or("Hi here") << std::endl;
    }

    {
        jle::optional<std::string>  maybe_s;
        maybe_s = std::string{"Hi there"};
        std::cout << maybe_s.value() << std::endl;
    }

    {
        jle::optional<std::string>  maybe_s;
        try {
            std::cout << maybe_s.value() << std::endl;
        } catch(...) {
            std::cout << "exception" << std::endl;
        }
    }

    {
        jle::optional<std::string>  maybe_s;
        maybe_s = std::string{"Hi there"};
        std::cout << *maybe_s << std::endl;
    }

    {
        jle::optional<std::string>  maybe_s;
        try {
            std::cout << *maybe_s << std::endl;
        } catch(...) {
            std::cout << "exception" << std::endl;
        }
    }

    // comparison operators
    {
        jle::optional<pr_t>  oi = pr_t{1, 3.1415926535};
        jle::optional<pr_t>  oi2;
        oi2 = pr_t{1, 3.1415926535};
        auto oi3 = jle::make_optional(pr_t{10, 1.});


        JLE_COUT_TRACE(int(oi == oi2));
        JLE_COUT_TRACE(int(oi != oi2));
        JLE_COUT_TRACE(int(oi == oi3));
        JLE_COUT_TRACE(int(oi != oi3));
    }
    {
        auto o1 = jle::optional<std::string>{"hi"};
        jle::optional<std::string> o2;
        o2 = std::string{"hi"};

        JLE_COUT_TRACE(int(o1 == o2));
        JLE_COUT_TRACE(int(o1 <  o2));
        JLE_COUT_TRACE(int(o1 <=  o2));
    }
}

