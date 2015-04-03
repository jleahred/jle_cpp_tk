#ifndef JLECPPTK_TEST_H
#define JLECPPTK_TEST_H


#include <iostream>

#include "support/string.h"



#define JLE_TEST_FILE std::cout << std::endl << std::endl << "TESTING on  FILE ->   " << __FILE__ << std::endl;

#define JLE_TEST_INIT std::cout << std::endl \
            << "_________________________________________________" << std::endl << std::endl  \
            << "INIT:>  << " << __FUNCTION__ << " >>" << std::endl;

#define JLE_TEST_DOT std::cout << "." << std::flush;

#define JLE_TEST_ASSERT_NO_DOT(__EXPRESION__) \
    if((__EXPRESION__) == false)   \
    {   \
        std::cerr << std::endl << "ERROR... in " << JLE_HERE << std::endl;   \
        exit(-1);   \
    }

#define JLE_TEST_ASSERT(__EXPRESION__) \
    JLE_TEST_ASSERT_NO_DOT(__EXPRESION__)  \
    else {    \
        JLE_TEST_DOT    \
    }



#define JLE_TEST_REPORT   std::cout << std::endl << "FINISHED TEST  OK." << std::endl;

namespace jle {




};



#endif // JLECPPTK_TEST_H
