#include <iostream>

#include "support/chrono.h"
#include "support/misc.h"
#include "support/test.h"


void test_sum_duration(void)
{
    JLE_TEST_INIT

    {
        auto now = jle::chrono::now();
        auto later = now + 1s+333ms;
        JLE_TEST_ASSERT(now + 1s+333ms == later);
        JLE_TEST_ASSERT(now == later -1s-333ms);
        JLE_TEST_ASSERT(now +1s == later -333ms);
    }
}

void test_comparission(void)
{
    JLE_TEST_INIT

    auto now = jle::chrono::now();
    auto later = now + 10min;
    JLE_TEST_ASSERT(now < later);
    JLE_TEST_ASSERT(now <= later);
    JLE_TEST_ASSERT(later > now);
    JLE_TEST_ASSERT(later >= now);

    JLE_TEST_ASSERT(now+15min > later);
    JLE_TEST_ASSERT(now+15min >= later);
}

void test_ostream(void)
{
    JLE_TEST_INIT
    {
        auto tp = jle::chrono::make_from_date(jle::chrono::year{2015}, jle::chrono::month{4}, jle::chrono::day{18}) + 12h + 26min + 59s + 384ms;
        JLE_TEST_ASSERT(JLE_SS(tp) == "2015-04-18 12:27:00.383")
    }
    {
        auto dur = std::chrono::hours(24*5) + 12h + 26min + 59s + 384ms;
        JLE_TEST_ASSERT(JLE_SS(dur) == "5d 12h 26m 59s 384ms")
    }

}


int main(void)
{

    JLE_TEST_FILE

        test_sum_duration();
        test_comparission();
        test_ostream();

    JLE_TEST_REPORT
}
