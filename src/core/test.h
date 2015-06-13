#ifndef JLECPPTK_TEST_H
#define JLECPPTK_TEST_H


/**
 *     \example  ./core/ex_test.cpp
 */


#include <iostream>

#include "core/string.h"
#include "core/misc.h"


/** \brief  It writes and recolect information about a file starting a test
 *
 */
#define JLE_TEST_FILE std::cout << std::endl << std::endl << std::endl   \
            << "_________________________________________________" << std::endl << std::endl   \
            << "TESTING on  FILE ->   " << __FILE__ << std::endl;


/** \brief  It writes and recolect about specific test
 *
 */
#define JLE_TEST_INIT std::cout << std::endl \
            << std::endl << "INIT:>  << " << __FUNCTION__ << " >>" << std::endl;

/** \brief  It writes a dot con cout in order to show test progression
 *
 */
#define JLE_TEST_DOT std::cout << "." << std::flush;


/** \brief  It checks a expression truth (no writting progress)
 *
 */
#define JLE_TEST_ASSERT_NO_DOT(__EXPRESION__) \
    if((__EXPRESION__) == false)   \
    {   \
        std::cerr << std::endl << "ERROR... in " << JLE_HERE << std::endl;   \
        std::cerr << std::endl << (#__EXPRESION__) << std::endl;   \
        exit(-1);   \
    }

/** \brief  It checks a expression truth (writting progress)
 *
 */
#define JLE_TEST_ASSERT(__EXPRESION__) \
    JLE_TEST_ASSERT_NO_DOT(__EXPRESION__)  \
    else {    \
        JLE_TEST_DOT    \
    }



/** \brief  Gets an write recolected test info
 *
 */
#define JLE_TEST_REPORT   std::cout << std::endl << "FINISHED TEST  OK." << std::endl;  return 0;


#define JLE_TEST_EXCEPTION(__EXPRESION__)  \
    {bool except = false; try { (void) (__EXPRESION__);  } catch(...) { except=true; };  JLE_TEST_ASSERT(except);}



namespace jle {




};



#endif // JLECPPTK_TEST_H
