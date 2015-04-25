#include <iostream>
#include "support/alarm.h"


int main()
{

    {
        auto alarm = jle::alarm{JLE_HERE, "sub", JLE_SS("Description " << jle::chrono::now()), jle::al::priority::critic, jle::al::type::logic_error};
        std::cout << alarm << std::endl;
    }

    {
        try {
            throw jle::alarm{JLE_HERE, "sub", JLE_SS("Description " << jle::chrono::now()), jle::al::priority::critic, jle::al::type::logic_error};
        } catch(const jle::alarm& alarm) {
            std::cout << alarm << std::endl;
        }
    }

    {
        try {
            throw jle::alarm{JLE_HERE, "sub", JLE_SS("Description " << jle::chrono::now()), jle::al::priority::critic, jle::al::type::unknown};
        } JLE_CATCH_CALLFUNCION(std::cout << , "sub_test", "description")
    }

    {
        try {
            try {
                throw "this is a test";
            } JLE_CATCH_RETHROW("RT", "TESTING RETHROW")
        } JLE_CATCH_CALLFUNCION(std::cout << , "rt", "rethrow test")
    }


    return 0;
}
