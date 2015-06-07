#include <iostream>
#include "core/optional.hpp"
#include "core/dbl.h"
#include "core/misc.h"
#include "core/test.h"

namespace jle {

};


struct pr_t {
    int         a;
    jle::dbl    b;

    bool operator==(const pr_t& pr) const {
        return this->a == pr.a  &&  this->b == pr.b;
    }
};




void test_initialization_assignament(void)
{
    JLE_TEST_INIT

    {
        auto oi = jle::optional<pr_t>{pr_t{1, 3.1415926535}};
        JLE_TEST_ASSERT(oi == (pr_t{1, 3.1415926535}));
    }
    {
        jle::optional<pr_t>  oi;
        JLE_TEST_ASSERT(oi.has_value() == false);
        JLE_TEST_ASSERT(bool(oi) == false);
    }
    {
        auto oi = jle::make_optional(pr_t{1, 3.1415926535});
        oi = {};
        JLE_TEST_ASSERT(oi.has_value() == false);
        JLE_TEST_ASSERT(bool(oi) == false);
    }
    {
        auto oi = jle::make_optional(pr_t{1, 3.1415926535});
        oi = jle::nullopt;
        JLE_TEST_ASSERT(oi.has_value() == false);
        JLE_TEST_ASSERT(bool(oi) == false);
    }
    {
        jle::optional<pr_t>  oi;
        oi = jle::make_optional(pr_t{1, 3.1415926535});
        JLE_TEST_ASSERT(oi.has_value() == true);
        JLE_TEST_ASSERT(bool(oi) == true);
        JLE_TEST_ASSERT(oi == (pr_t{1, 3.1415926535}));
    }
}

void test_access(void)
{
    JLE_TEST_INIT

    {
        auto oi = jle::make_optional(pr_t{1, 3.1415926535});
        JLE_TEST_ASSERT(oi.has_value() == true);
        JLE_TEST_ASSERT(oi->a == 1);
        JLE_TEST_ASSERT(oi->b == 3.1415926535);
        JLE_TEST_ASSERT(oi.value() == (pr_t{1, 3.1415926535}));
        JLE_TEST_ASSERT(*oi == (pr_t{1, 3.1415926535}));

        oi->a = 2;
        oi->b = 1.5;
        JLE_TEST_ASSERT(oi.has_value() == true);
        JLE_TEST_ASSERT(oi->a == 2);
        JLE_TEST_ASSERT(oi->b == 1.5);
        JLE_TEST_ASSERT(oi.value() == (pr_t{2, 1.5}));
        JLE_TEST_ASSERT(*oi == (pr_t{2, 1.5}));
    }
}

void test_no_und_beh(void)
{
    JLE_TEST_INIT

    {
        jle::optional<pr_t>  oi;

        JLE_TEST_EXCEPTION(oi->a == 1);
        JLE_TEST_EXCEPTION(oi->b == 3.1415926535);
        JLE_TEST_EXCEPTION(oi.value() == (pr_t{1, 3.1415926535}));
        JLE_TEST_EXCEPTION(*oi == (pr_t{1, 3.1415926535}));
    }
}


int main(void)
{

    JLE_TEST_FILE

        test_initialization_assignament();
        test_access();
        test_no_und_beh();

    JLE_TEST_REPORT
}
