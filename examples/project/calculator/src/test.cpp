#include <iostream>

#include "core/hp/humbleparser.h"
#include "core/chrono.h"


#include "arithmachine.h"





int main()
{



    //  small example writting directly on memory

    try
    {

        //  creating an instance of the virtual machine
        Arith_machine amachine;

        //  basic functions connections
        Functions_Basic fb(amachine);

        //  partial compilation with humbel_parser
        jle::hp::Humble_parser  hParser;
        bool parseResult;
        std::string parseMessage;
        std::tie(parseResult, parseMessage) =
            hParser.add_rules_from_file("arithm.rul");
        if (parseResult == false){
            std::cout << std::endl << "ERROR parsing  " << parseMessage << std::endl;
            return -1;
        }

        //  instance for second compilation step
        AM_Asembler asembler;
        asembler.signalCompiledProgram.connect(&amachine, &Arith_machine::AddProgram);


        //  --------------------------------------------
        {
            //  first compilation step
            std::string asmCode;
            {
                jle::hp::AST_node_item astRoot("");
                bool result=false;
                std::string resultTest;
                std::tie(result, resultTest, astRoot) =
                        hParser.multi_parse("y=x=-1+2*(j=3)");
                if (result == false)
                {
                    std::cout << std::endl << "FALLO sintáctico " << resultTest << std::endl;
                    return 0;
                }
                asmCode = astRoot.value;
            }

            //  finishing compilation
            asembler.Compile(asmCode);


            //  evaluation
            auto result = amachine.Eval();

            std::cout << std::endl << "RESULTADO... " << result << std::endl;

            std::cout << std::endl << std::endl << "x == " << amachine.GetValueFromHeap("x");
            std::cout << std::endl <<              "y == " << amachine.GetValueFromHeap("y");
            std::cout << std::endl <<              "z == " << amachine.GetValueFromHeap("z");
            std::cout << std::endl <<              "j == " << amachine.GetValueFromHeap("j") << std::endl;

        }


        //  --------------------------------------------

        //  another compilation...
        //  current value 5

        //  first compilation step
        {
            std::string asmCode;
            {
                jle::hp::AST_node_item astRoot("");
                bool result=false;
                std::string resultTest;
                std::tie(result, resultTest, astRoot) =
                hParser.multi_parse("y*2.5");
                if (result == false)
                {
                    std::cout << std::endl << "FALLO sintáctico " << resultTest << std::endl;
                    return 0;
                }
                asmCode = astRoot.value;
            }

            std::cout << asmCode << std::endl;
            //  finishing compilation
            asembler.Compile(asmCode);


            //  evaluation
            auto result = amachine.Eval();

            std::cout << std::endl << "RESULTADO2... " << result << std::endl;
        }


        //  --------------------------------------------

        //  another comiplation... now with a loop and variable modificaition
        {
            std::string asmCode;
            {
                jle::hp::AST_node_item astRoot("");
                bool result=false;
                std::string resultTest;
                std::tie(result, resultTest, astRoot) =
                    hParser.multi_parse(      "-7*9+(7-3*4+(5*x+1)-x/2)/y");
                if (result == false)
                {
                    std::cout << std::endl << "FALLO sintáctico " << resultTest << std::endl;
                    return 0;
                }
                asmCode = astRoot.value;
            }

            //  finishing compilation
            asembler.Compile(asmCode);


            for (int x=0; x<1000; ++x)
            {
                amachine.SetValueInHeap("x", Odbl(x));
                //  evaluation
                auto result = amachine.Eval();
                std::cout << std::endl << "RESULTADO (" << x << ") =  " << result << std::endl;
            }

	    const int iterations = 1000000;
	    auto start = jle::chrono::now();
	    std::cout << start << std::endl << "Empezamos iteraciones... " << iterations << std::endl;
            for (int x=0; x<iterations; ++x)
            {
                amachine.SetValueInHeap("x", Odbl(x));
                //  evaluationf
                /*jle::dbl result = */amachine.Eval();
            }
	    auto stop = jle::chrono::now();
	    std::cout << stop << std::endl;

	    auto interval = stop - start;

	    std::cout << std::endl << "time: "<< interval
                << "  iterations/milliseconds:  "
                <<  iterations / std::chrono::duration_cast<std::chrono::milliseconds>(interval).count()
                << std::endl << std::endl;
        }


    }
    catch(const std::string& error)
    {
        std::cout << std::endl << error << std::endl;

    }

    return 0;
}

void jle::alarm_msg(const jle::alarm& al)
{
    std::cout << "ouch...: "  << al << std::endl;

}
