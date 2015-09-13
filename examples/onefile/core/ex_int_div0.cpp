#include <iostream>
#include <stdexcept>
#include "core/int_div0.h"
#include "core/alarm.h"






int main()
{

    //  simple case
    try
    {
        JLE_HANDLE_DIV0_INIT

            std::cout << "one " << std::endl;
            int    izero = 0;
            std::cout << 7 / izero << std::endl;

        JLE_HANDLE_DIV0_END

    }
    catch(const std::exception& error)
    {
        std::cout << "captured in 1 " << error.what() << std::endl;
    }




    //  two consecutives

    try
    {
        JLE_HANDLE_DIV0_INIT
        {
            std::cout << "two " << std::endl;
            int    izero = 0;
            std::cout << 7 / izero << std::endl;
        }
        JLE_HANDLE_DIV0_END



        JLE_HANDLE_DIV0_INIT
            std::cout << "three " << std::endl;
            int    izero = 0;
            std::cout << 8 / izero;
        JLE_HANDLE_DIV0_END


    }
    catch(const std::exception& error)
    {
        std::cout << "captured in 2-3 " << error.what() << std::endl;
    }



    //  nested
    try
    {
        JLE_HANDLE_DIV0_INIT

                try
                {
                    JLE_HANDLE_DIV0_INIT_A(A)
                        std::cout << "four " << std::endl;
                        int    izero = 0;
                        std::cout << 7 / izero << std::endl;
                    JLE_HANDLE_DIV0_END_A(A)
                }
                catch(const std::exception& error)
                {
                    std::cout << "captured in 4 " << error.what() << std::endl;
                }

            std::cout << "five " << std::endl;
            std::cout << "we try AGAIN" << std::endl;
            int    izero = 0;
            std::cout << 8 / izero;

        JLE_HANDLE_DIV0_END

    }
    catch(const std::exception& error)
    {
        std::cout << "captured in 5 " << error.what() << std::endl;
    }

    std::cout << "End of program..." << std::endl;

    return 0;
}



//---------------------------------------------------------------------------

//  RECEPTOR ALARMAS SALIDA GENERAL

//---------------------------------------------------------------------------

void jle::alarm_msg(const jle::alarm& alarma)
{
        std::cout << "\n\r";
        std::cout << std::endl << "ALARMA SALIDA..." << alarma <<  std::endl ;
        std::cout << alarma << std::endl ;
}

