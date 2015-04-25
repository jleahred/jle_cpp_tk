#include "support/test.h"


#include <tuple>

#include "support/string.h"
#include "support/chrono.h"




#define TEST_TRIM(__INPUT__, __RESULT__, __TRIM_CRITERIA__) \
    {  \
        std::string input(__INPUT__), result(__RESULT__);  \
        JLE_TEST_ASSERT(result == jle::s_trim(input, __TRIM_CRITERIA__))    \
    }






void test_trim(void)
{
    JLE_TEST_INIT


    TEST_TRIM("    a    ",       "a",    ' ')
    TEST_TRIM("    a"    ,       "a",    ' ')
    TEST_TRIM("a    "    ,       "a",    ' ')
    TEST_TRIM(" a "      ,       "a",    ' ')
    TEST_TRIM("a"        ,       "a",    ' ')

    TEST_TRIM("         ",       "",    ' ')
    TEST_TRIM("     "    ,       "",    ' ')
    TEST_TRIM("  "       ,       "",    ' ')
    TEST_TRIM(" "        ,       "",    ' ')
    TEST_TRIM(""         ,       "",    ' ')

    TEST_TRIM("         ",       "",    " ")
    TEST_TRIM("     "    ,       "",    " ")
    TEST_TRIM("  "       ,       "",    " ")
    TEST_TRIM(" "        ,       "",    " ")
    TEST_TRIM(""         ,       "",    " ")

    TEST_TRIM("    a    ",       "a",    " ")
    TEST_TRIM("    a"    ,       "a",    " ")
    TEST_TRIM("a    "    ,       "a",    " ")
    TEST_TRIM(" a "      ,       "a",    " ")
    TEST_TRIM("a"        ,       "a",    " ")

    TEST_TRIM("    a    ",       "a",    "  ")
    TEST_TRIM("    a"    ,       "a",    "  ")
    TEST_TRIM("a    "    ,       "a",    "  ")
    TEST_TRIM(" a "      ,       "a",    "  ")
    TEST_TRIM("a"        ,       "a",    "  ")

    TEST_TRIM("    a    ",       "a",    "b ")
    TEST_TRIM("    a"    ,       "a",    "b ")
    TEST_TRIM("a    "    ,       "a",    "b ")
    TEST_TRIM(" a "      ,       "a",    "b ")
    TEST_TRIM("a"        ,       "a",    "b ")

    TEST_TRIM("    a    ",       "a",    " b ")
    TEST_TRIM("    a"    ,       "a",    " b ")
    TEST_TRIM("a    "    ,       "a",    " b ")
    TEST_TRIM(" a "      ,       "a",    " b ")
    TEST_TRIM("a"        ,       "a",    " b ")

    TEST_TRIM("    a    ",       "a",    " b")
    TEST_TRIM("    a"    ,       "a",    " b")
    TEST_TRIM("a    "    ,       "a",    " b")
    TEST_TRIM(" a "      ,       "a",    " b")
    TEST_TRIM("a"        ,       "a",    " b")

    TEST_TRIM("    a    ",       "",    "a ")
    TEST_TRIM("    a"    ,       "",    "a ")
    TEST_TRIM("a    "    ,       "",    "a ")
    TEST_TRIM(" a "      ,       "",    "a ")
    TEST_TRIM("a"        ,       "",    "a ")

    TEST_TRIM("    a    ",       "",    " a ")
    TEST_TRIM("    a"    ,       "",    " a ")
    TEST_TRIM("a    "    ,       "",    " a ")
    TEST_TRIM(" a "      ,       "",    " a ")
    TEST_TRIM("a"        ,       "",    " a ")

    TEST_TRIM("    a    ",       "",    " a")
    TEST_TRIM("    a"    ,       "",    " a")
    TEST_TRIM("a    "    ,       "",    " a")
    TEST_TRIM(" a "      ,       "",    " a")
    TEST_TRIM("a"        ,       "",    " a")

    TEST_TRIM("    a    ",       "",    "ab ")
    TEST_TRIM("    a"    ,       "",    "ab ")
    TEST_TRIM("a    "    ,       "",    "ab ")
    TEST_TRIM(" a "      ,       "",    "ab ")
    TEST_TRIM("a"        ,       "",    "ab ")

    TEST_TRIM("    a    ",       "",    " ab ")
    TEST_TRIM("    a"    ,       "",    " ab ")
    TEST_TRIM("a    "    ,       "",    " ab ")
    TEST_TRIM(" a "      ,       "",    " ab ")
    TEST_TRIM("a"        ,       "",    " ab ")

    TEST_TRIM("    a    ",       "",    " ab")
    TEST_TRIM("    a"    ,       "",    " ab")
    TEST_TRIM("a    "    ,       "",    " ab")
    TEST_TRIM(" a "      ,       "",    " ab")
    TEST_TRIM("a"        ,       "",    " ab")

    TEST_TRIM("    a    ",       "",    "ba ")
    TEST_TRIM("    a"    ,       "",    "ba ")
    TEST_TRIM("a    "    ,       "",    "ba ")
    TEST_TRIM(" a "      ,       "",    "ba ")
    TEST_TRIM("a"        ,       "",    "ba ")

    TEST_TRIM("         ",       "",    ' ')
    TEST_TRIM("     "    ,       "",    ' ')
    TEST_TRIM("  "       ,       "",    ' ')
    TEST_TRIM(" "        ,       "",    ' ')
    TEST_TRIM(""         ,       "",    ' ')

    TEST_TRIM("         ",       "",    " ")
    TEST_TRIM("     "    ,       "",    " ")
    TEST_TRIM("  "       ,       "",    " ")
    TEST_TRIM(" "        ,       "",    " ")
    TEST_TRIM(""         ,       "",    " ")

    TEST_TRIM("         ",       "",    "  ")
    TEST_TRIM("     "    ,       "",    "  ")
    TEST_TRIM("  "       ,       "",    "  ")
    TEST_TRIM(" "        ,       "",    "  ")
    TEST_TRIM(""         ,       "",    "  ")

    TEST_TRIM("         ",       "",    "a ")
    TEST_TRIM("     "    ,       "",    "a ")
    TEST_TRIM("  "       ,       "",    "a ")
    TEST_TRIM(" "        ,       "",    "a ")
    TEST_TRIM(""         ,       "",    "a ")

    TEST_TRIM("         ",       "",    " a ")
    TEST_TRIM("     "    ,       "",    " a ")
    TEST_TRIM("  "       ,       "",    " a ")
    TEST_TRIM(" "        ,       "",    " a ")
    TEST_TRIM(""         ,       "",    " a ")

    TEST_TRIM("         ",       "",    " a")
    TEST_TRIM("     "    ,       "",    " a")
    TEST_TRIM("  "       ,       "",    " a")
    TEST_TRIM(" "        ,       "",    " a")
    TEST_TRIM(""         ,       "",    " a")


    TEST_TRIM("    ab    ",       "a",    "b ")
    TEST_TRIM("    ab"    ,       "a",    "b ")
    TEST_TRIM("ab    "    ,       "a",    "b ")
    TEST_TRIM(" ab "      ,       "a",    "b ")
    TEST_TRIM("ab"        ,       "a",    "b ")

    TEST_TRIM("    ab    ",       "a",    " b ")
    TEST_TRIM("    ab"    ,       "a",    " b ")
    TEST_TRIM("ab    "    ,       "a",    " b ")
    TEST_TRIM(" ab "      ,       "a",    " b ")
    TEST_TRIM("ab"        ,       "a",    " b ")

    TEST_TRIM("    ab    ",       "a",    " b")
    TEST_TRIM("    ab"    ,       "a",    " b")
    TEST_TRIM("ab    "    ,       "a",    " b")
    TEST_TRIM(" ab "      ,       "a",    " b")
    TEST_TRIM("ab"        ,       "a",    " b")

    TEST_TRIM("    ab    ",       "a",    " bb")
    TEST_TRIM("    ab"    ,       "a",    " bb")
    TEST_TRIM("ab    "    ,       "a",    " bb")
    TEST_TRIM(" ab "      ,       "a",    " bb")
    TEST_TRIM("ab"        ,       "a",    " bb")

    TEST_TRIM("    ab    ",       "a",    " b b")
    TEST_TRIM("    ab"    ,       "a",    " b b")
    TEST_TRIM("ab    "    ,       "a",    " b b")
    TEST_TRIM(" ab "      ,       "a",    " b b")
    TEST_TRIM("ab"        ,       "a",    " b b")

    TEST_TRIM("    a a    ",       "a a",    " ")
    TEST_TRIM("    a a"    ,       "a a",    " ")
    TEST_TRIM("a a    "    ,       "a a",    " ")
    TEST_TRIM(" a a "      ,       "a a",    " ")
    TEST_TRIM("a a"        ,       "a a",    " ")

    TEST_TRIM("    a a    ",       "a a",    "  ")
    TEST_TRIM("    a a"    ,       "a a",    "  ")
    TEST_TRIM("a a    "    ,       "a a",    "  ")
    TEST_TRIM(" a a "      ,       "a a",    "  ")
    TEST_TRIM("a a"        ,       "a a",    "  ")

    TEST_TRIM("    a a    ",       "a a",    "b ")
    TEST_TRIM("    a a"    ,       "a a",    "b ")
    TEST_TRIM("a a    "    ,       "a a",    "b ")
    TEST_TRIM(" a a "      ,       "a a",    "b ")
    TEST_TRIM("a a"        ,       "a a",    "b ")

    TEST_TRIM("    a a    ",       "a a",    " b ")
    TEST_TRIM("    a a"    ,       "a a",    " b ")
    TEST_TRIM("a a    "    ,       "a a",    " b ")
    TEST_TRIM(" a a "      ,       "a a",    " b ")
    TEST_TRIM("a a"        ,       "a a",    " b ")

    TEST_TRIM("    a a    ",       "a a",    " b")
    TEST_TRIM("    a a"    ,       "a a",    " b")
    TEST_TRIM("a a    "    ,       "a a",    " b")
    TEST_TRIM(" a a "      ,       "a a",    " b")
    TEST_TRIM("a a"        ,       "a a",    " b")

    TEST_TRIM("    a a    ",       "a a",    " b b ")
    TEST_TRIM("    a a"    ,       "a a",    " b b ")
    TEST_TRIM("a a    "    ,       "a a",    " b b ")
    TEST_TRIM(" a a "      ,       "a a",    " b b ")
    TEST_TRIM("a a"        ,       "a a",    " b b ")
}


#define  TEST_CONVERSION(__TYPE__, __ORIGIN__, __DEFAULT__, __FUNCTION__)   \
    {    \
        __TYPE__  origin = __ORIGIN__;   \
        JLE_TEST_ASSERT(JLE_SS(origin) == JLE_SS(std::get<0>(__FUNCTION__(JLE_SS(origin), __DEFAULT__))));   \
    }

void test_conversion()
{
    JLE_TEST_INIT

    TEST_CONVERSION(int,                        123456,             0,      jle::s_try_s2i);
    TEST_CONVERSION(int,                        -123456,            0,      jle::s_try_s2i);
    TEST_CONVERSION(jle::chrono::duration,      1s,                 0s,     jle::s_try_s2dur);
    TEST_CONVERSION(jle::chrono::duration,      1s+231ms,           0s,     jle::s_try_s2dur);
    TEST_CONVERSION(jle::chrono::duration,      3h+7min+1s+231ms,   0s,     jle::s_try_s2dur);
    TEST_CONVERSION(jle::chrono::time_point,    jle::chrono::make_from_date(
                                                        jle::chrono::year   {2015},
                                                        jle::chrono::month  {2},
                                                        jle::chrono::day    {25}
                                                        ),
                                                                    jle::chrono::now(),     jle::s_try_s2tp);
}


int main(void)
{

    JLE_TEST_FILE

        test_trim();
        test_conversion();

    JLE_TEST_REPORT
}


