#include <iostream>
#include "support/string.h"


int main()
{
    std::string s {"lkjlkj"};
    double d {3.1416926535};
    int i {42};

    std::string  result = JLE_SS("string " << s << "  num " << d << "  " << i << 888 << std::endl);
    std::cout  << result;

    std::string result_recurs = JLE_SS("string " << s << JLE_SS("  num " << d << "  ") << i << 888 << std::endl);
    std::cout << result_recurs;
}
