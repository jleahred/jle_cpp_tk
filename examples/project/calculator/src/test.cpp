#include <iostream>

#include "support/hp/humbleparser.h"
#include "support/date_time.h"


#include "arithmachine.h"



//#include "support/debug/checkscopetime.h"



int main()
{



    //  ejemplo escribiendo un programa directamente en la memoria

    try
    {

        //  creamos una instancia de la máquina virtual
        ArithMachine amachine;

        //  creamos y conectamos una instancia de funciones basicorras
        Functions_Basic fb(amachine);

        //  instancia de humbleParser para verificación sintáctica y compilación parcial
        mtk::HumbleParser  hParser;
        bool parseResult;
        std::string parseMessage;
        hParser.LoadRulesFromFile("arithm.rul").assign(parseResult, parseMessage);
        if (parseResult == false){
            std::cout << std::endl << "ERROR parsing  " << parseMessage << std::endl;
            return -1;
        }

        //  instancia del segundo paso de compilación
        AM_Asembler asembler;
        asembler.signalCompiledProgram.connect(&amachine, &ArithMachine::AddProgram);


        //  --------------------------------------------
        {
            //  primer paso de compilación
            std::string asmCode;
            {
                mtk::AST_Node_Item astRoot("");
                bool result=false;
                std::string resultTest;
                hParser.MultiParse(      "y=x=-1+2*(j=3)"
                                        ).assign(result, resultTest, astRoot);
                if (result == false)
                {
                    std::cout << std::endl << "FALLO sintáctico " << resultTest;
                    return 0;
                }
                asmCode = astRoot.value;
            }

            //  terminamos la compilación
            asembler.Compile(asmCode);


            //  evaluación
            mtk::Double result = amachine.Eval();

            std::cout << std::endl << "RESULTADO... " << result;

            std::cout << std::endl << std::endl << "x == " << amachine.GetValueFromHeap("x");
            std::cout << std::endl <<              "y == " << amachine.GetValueFromHeap("y");
            std::cout << std::endl <<              "z == " << amachine.GetValueFromHeap("z");
            std::cout << std::endl <<              "j == " << amachine.GetValueFromHeap("j");

        }



        //  --------------------------------------------

        //  otra compilación...
        //  ahora y vale 5

        //  primer paso de compilación
        {
            std::string asmCode;
            {
                mtk::AST_Node_Item astRoot("");
                bool result=false;
                std::string resultTest;
                hParser.MultiParse(      "y*2.5"
                                        ).assign(result, resultTest, astRoot);
                if (result == false)
                {
                    std::cout << std::endl << "FALLO sintáctico " << resultTest;
                    return 0;
                }
                asmCode = astRoot.value;
            }

            //  terminamos la compilación
            asembler.Compile(asmCode);


            //  evaluación
            mtk::Double result = amachine.Eval();

            std::cout << std::endl << "RESULTADO2... " << result;
        }



        //  --------------------------------------------

        //  otra compilación...  ahora con un bucle y cambio de variable
        {
            std::string asmCode;
            {
                mtk::AST_Node_Item astRoot("");
                bool result=false;
                std::string resultTest;
                hParser.MultiParse(      "-7*9+(7-3*4+(5*x+1)-x/2)/y"
                                        ).assign(result, resultTest, astRoot);
                if (result == false)
                {
                    std::cout << std::endl << "FALLO sintáctico " << resultTest;
                    return 0;
                }
                asmCode = astRoot.value;
            }

            //  terminamos la compilación
            asembler.Compile(asmCode);


            for (int x=0; x<1000; ++x)
            {
                amachine.SetValueInHeap("x", x);
                //  evaluación
                mtk::Double result = amachine.Eval();
                std::cout << std::endl << "RESULTADO (" << x << ") =  " << result << std::endl;
            }

            //mtk::CheckScopeTime csBucle("10000 iteraciones");
            //  ahora hacemos lo mismo para contar el tiempo transcurrido
	    const int iterations = 1000000; 
	    mtk::DateTime start = mtk::dtNowLocal();
	    std::cout << "__NR__:  " << start << std::endl << "Empezamos iteraciones... " << iterations << std::endl;
            for (int x=0; x<iterations; ++x)
            {
                amachine.SetValueInHeap("x", x);
                //  evaluación
                /*mtk::Double result = */amachine.Eval();
            }
	    mtk::DateTime stop = mtk::dtNowLocal();
	    std::cout << "__NR__:  " << stop << std::endl;

	    mtk::dtTimeQuantity interval = stop - start;

	    std::cout << std::endl << "__NR__:  "  << "time: "<< interval  << "  iterations/milliseconds:  " <<  iterations / mtk::ARE_YOU_SURE_YOU_WANT_GetMillisecsDay(interval) << std::endl << std::endl;
            //csBucle.Stop();
            //csBucle.PrintTimes();
        }


    }
    catch(const std::string& error)
    {
        std::cout << std::endl << error << std::endl;

    }

    #include "support/release_on_exit.hpp"
    return 0;
}

void mtk::AlarmMsg(const mtk::Alarm& al)
{
    std::cout << "ouch...: "  << al << std::endl;

}
