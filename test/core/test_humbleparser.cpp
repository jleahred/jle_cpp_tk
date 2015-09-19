#include <iostream>

#include "core/hp/humbleparser.h"
#include "core/test.h"



void test_expresion_ok(void)
{
    JLE_TEST_INIT

    jle::Humble_parser  hparser;

    hparser.load_rules_from_string(
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
    jle::AST_node_item ast_root("");

    std::tie(result, result_test, ast_root) =
                hparser.parse(      "+1+2-53*(-sin(x))+(5-(-(-a)+cos(56))+4)+5",
                                    "EXPR"
                        );


    JLE_TEST_ASSERT(result = true);
}


void test_expresion_error(void)
{
    JLE_TEST_INIT

    jle::Humble_parser  hparser;

    hparser.load_rules_from_string(
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
    jle::AST_node_item ast_root("");

    std::tie(result, result_test, ast_root) =
                hparser.parse(      "+1+2-53*(-sin(x x))+(5-(-(-a)+cos(56))+4)+5",
                                    "EXPR"
                        );


    JLE_TEST_ASSERT(result = false);
}



int main(void)
{

    JLE_TEST_FILE

        test_expresion_ok();
        test_expresion_error();

    JLE_TEST_REPORT
}
