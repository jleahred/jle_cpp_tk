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
        Functions_basic fb(amachine);

        //  partial compilation with humbel_parser
        jle::hp::Humble_parser  h_parser;
        bool parseResult;
        std::string parseMessage;
        std::tie(parseResult, parseMessage) =
            h_parser.add_rules_from_file("arithm.rul");
        if (parseResult == false){
            std::cout << std::endl << "ERROR parsing  " << parseMessage << std::endl;
            return -1;
        }

        //  instance for second compilation step
        AM_asembler asembler;
        asembler.signal_compiled_program.connect(&amachine, &Arith_machine::add_program);


        //  --------------------------------------------
        {
            //  first compilation step
            std::string asm_code;
            {
                jle::hp::AST_node_item ast_root("");
                bool result=false;
                std::string resultTest;
                std::tie(result, resultTest, ast_root) =
                        h_parser.multi_parse("y=x=-1+2*(j=3)");
                if (result == false)
                {
                    std::cout << std::endl << "SYNTAX ERROR " << resultTest << std::endl;
                    return 0;
                }
                asm_code = ast_root.value;
            }

            //  finishing compilation
            asembler.compile(asm_code);


            //  evaluation
            auto result = amachine.eval();

            std::cout << std::endl << "RESULT... " << result << std::endl;

            std::cout << std::endl << std::endl << "x == " << amachine.get_value_from_heap("x");
            std::cout << std::endl <<              "y == " << amachine.get_value_from_heap("y");
            std::cout << std::endl <<              "z == " << amachine.get_value_from_heap("z");
            std::cout << std::endl <<              "j == " << amachine.get_value_from_heap("j") << std::endl;

        }


        //  --------------------------------------------

        //  another compilation...
        //  current value 5

        //  first compilation step
        {
            std::string asm_code;
            {
                jle::hp::AST_node_item ast_root("");
                bool result=false;
                std::string resultTest;
                std::tie(result, resultTest, ast_root) =
                h_parser.multi_parse("y*2.5");
                if (result == false)
                {
                    std::cout << std::endl << "SYNTAX ERROR " << resultTest << std::endl;
                    return 0;
                }
                asm_code = ast_root.value;
            }

            std::cout << asm_code << std::endl;
            //  finishing compilation
            asembler.compile(asm_code);


            //  evaluation
            auto result = amachine.eval();

            std::cout << std::endl << "RESULT2... " << result << std::endl;
        }


        //  --------------------------------------------

        //  another comiplation... now with a loop and variable modificaition
        {
            std::string asm_code;
            {
                jle::hp::AST_node_item ast_root("");
                bool result=false;
                std::string resultTest;
                std::tie(result, resultTest, ast_root) =
                    h_parser.multi_parse(      "-7*9+(7-3*4+(5*x+1)-x/2)/y");
                if (result == false)
                {
                    std::cout << std::endl << "SYNTAX ERROR " << resultTest << std::endl;
                    return 0;
                }
                asm_code = ast_root.value;
            }

            //  finishing compilation
            asembler.compile(asm_code);


            for (int x=0; x<1000; ++x)
            {
                amachine.set_value_in_heap("x", Odbl(x));
                //  evaluation
                auto result = amachine.eval();
                std::cout << std::endl << "RESULT (" << x << ") =  " << result << std::endl;
            }

	    const int iterations = 1000000;
	    auto start = jle::chrono::now();
	    std::cout << start << std::endl << "Starting iterations... " << iterations << std::endl;
            for (int x=0; x<iterations; ++x)
            {
                amachine.set_value_in_heap("x", Odbl(x));
                //  evaluationf
                /*jle::dbl result = */amachine.eval();
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
