#include <iostream>

#include "core/hp/humbleparser.h"



int main(int /*argc*/, char* /*argv*/[])
{
    jle::hp::Humble_parser  hparser;

    hparser.add_rules_from_string(
    R"(
        EXPR    ::=    _ unaryoperator _ ADDS _
        EXPR    ::=    ADDS
        ADDS    ::=    FACTS _ add_operator _ ADDS
        ADDS    ::=    FACTS
        FACTS   ::=    VAL _ mult_operator _ ADDS
        FACTS   ::=    VAL

        VAL     ::=    FUNC _ ( _ EXPR _ )
        VAL     ::=    _ num
        VAL     ::=    VAR
        VAR     ::=    _ id
        FUNC    ::=    _ id

        FACTS   ::=    _ ( _ EXPR _ )


        num           ::=    ([0-9]+)
        id            ::=    ([a-z|A-Z][0-9|a-z|A-Z|_]*)
        id            ::=    (_+[0-9|a-z|A-Z]+[0-9|a-z|A-Z|_]*)
        mult_operator ::=    ([\*|\\])
        add_operator  ::=    ([\+|\-])
        unaryoperator ::=    ([\+|\-])
        _             ::=    ([ |\t]*)
        (             ::=    (\()
        )             ::=    (\))
    )");

    bool result;
    std::string result_test;
    jle::hp::AST_node_item ast_root("");

    std::tie(result, result_test, ast_root) =
                hparser.parse(      "+1+2-53*(-sin(x))+(5-(-(-a)+cos(56))+4)+5",
                                    "EXPR"
                        );

    std::cout << std::endl << result <<  "  "  <<   result_test;

    return result == true ? 0 : -1;
}




//---------------------------------------------------------------------------

//  RECEPTOR ALARMAS SALIDA GENERAL

//---------------------------------------------------------------------------

void jle::alarm_msg (const jle::alarm& alarm)
{
    std::cout << "\n\r";
    std::cout << std::endl << "ERROR..." << std::endl ;
    std::cout << alarm << std::endl ;
}

