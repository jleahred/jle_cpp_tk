#include <iostream>

#include "test_forward.h"
#include "test.h"
#include "core/alarm.h"
#include "core/tuple.hpp"




int main()
{
    {   //  record
        auto simple = idl::pr::t_simple{ 1, "one", "two" };

        std::cout << simple << std::endl;
    }


    {   //  tuple
        idl::pr::t_tuple  t{1, "one" };
        std::cout << t << std::endl;

        idl::pr::t_tuple2  t2{2, "two" };
        std::cout << t2 << std::endl;
    }

    {   //  enum
        using namespace  idl::pr;
        auto color = t_color{t_color::red};
        std::cout << color << std::endl;
    }

    {   //  record with type modifiers
        auto type = idl::pr::t_type_modifiers {
                    32,
                    "string32",
                    jle::optional<std::string>{},
                    jle::optional<jle::dbl>{},
                    "default1",
                    "new mandatory",
                    jle::list<std::string>{},
                    jle::list<int32_t>{}
        };
        std::cout << type << std::endl;
    }

    {   //  record composed simple
        auto type = t_composed {
                    32,
                    idl::pr::t_simple{ 1, "one", "two" }
        };
        std::cout << type << std::endl;
    }

    {   //  union
        using idl::pr::t_union;
        auto type = t_union {32};
        switch(type.field) {
            case  t_union::en_field::s :
                    std::cout << *type.s;
                    break;
            case  t_union::en_field::i :
                    std::cout << *type.i;
                    break;
            case  t_union::en_field::d :
                    std::cout << *type.d;
                    break;
            default:    ;
        }
        std::cout << type << std::endl;
    }


    return 0;
}


void jle::alarm_msg(const jle::alarm&  al)
{
    using ::operator<<;
    std::cerr << al << std::endl;
}
