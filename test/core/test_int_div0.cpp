#include <iostream>
#include <stdexcept>
#include "core/int_div0.h"
#include "core/test.h"


void test_simple(void)
{
    JLE_TEST_INIT


    JLE_TEST_EXCEPTION(
        JLE_HANDLE_DIV0_INIT

            int    izero = 0;
            std::cout << 7 / izero << std::endl;

        JLE_HANDLE_DIV0_END
    )
}

void test_double(void)
{
    JLE_TEST_INIT


    JLE_TEST_EXCEPTION(
        JLE_HANDLE_DIV0_INIT

            int    izero = 0;
            std::cout << 7 / izero << std::endl;

        JLE_HANDLE_DIV0_END
    )

    JLE_TEST_EXCEPTION(
        JLE_HANDLE_DIV0_INIT

            int    izero = 0;
            std::cout << 7 / izero << std::endl;

        JLE_HANDLE_DIV0_END
    )
}

void test_nested(void)
{
    JLE_TEST_INIT


    JLE_TEST_EXCEPTION(
        JLE_HANDLE_DIV0_INIT

                try
                {
                    JLE_HANDLE_DIV0_INIT_A(A)
                        int    izero = 0;
                        std::cout << 7 / izero << std::endl;
                    JLE_HANDLE_DIV0_END_A(A)
                }
                catch(const std::exception& error)
                {
                    std::cout << " . ";
                }

            int    izero = 0;
            std::cout << 8 / izero;

        JLE_HANDLE_DIV0_END
    )
}




int main(void)
{

    JLE_TEST_FILE

        test_simple();
        test_double();
        test_nested();

    JLE_TEST_REPORT
}
