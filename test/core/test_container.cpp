#include "core/vector.hpp"
#include "core/list.hpp"
#include "core/map.hpp"

#include <iostream>


#include "core/test.h"
#include "core/alarm.h"



void test_vector(void)
{
    JLE_TEST_INIT

    jle::vector<int> vint;

    vint.push_back(5);
    vint.push_back(4);
    vint.push_back(3);
    vint.push_back(2);
    vint.push_back(1);

    JLE_TEST_ASSERT(vint.size() == 5);
    JLE_TEST_ASSERT(vint[0] == 5);
    JLE_TEST_ASSERT(vint[4] == 1);
    JLE_TEST_ASSERT(vint.at(1) == 4);

    vint.pop_back();
    vint.push_back(33);
    JLE_TEST_ASSERT(vint.front() == 5);
    JLE_TEST_ASSERT(vint.back() == 33);


    {
        int counter = 0;
        for(auto itvector=vint.begin(); itvector!=vint.end(); ++itvector)
            ++counter;
        JLE_TEST_ASSERT(counter == 5)
    }
    {
        int sum = 0;
        for(auto itvector : vint)
            sum += itvector;
        JLE_TEST_ASSERT(sum == 47)
    }

    JLE_TEST_EXCEPTION(vint[100]);
    JLE_TEST_EXCEPTION(vint[5]);

    vint.clear();
    JLE_TEST_ASSERT(vint.size() == 0);
}


void  test_map()
{
    JLE_TEST_INIT

    jle::map<int, std::string>  mis;
    mis[55] = "555555";
    mis[33] = "333333";
    mis[44] = "444444";

    JLE_TEST_ASSERT(mis[33] == "333333");
    JLE_TEST_ASSERT(mis[55] == "555555");
    JLE_TEST_ASSERT(mis[44] == "444444");

    mis[44] = "lkjlkjlkj";
    JLE_TEST_ASSERT(mis[44] == "lkjlkjlkj");


    JLE_TEST_ASSERT(mis.size() == 3);

    int counter=0;
    for(auto _ : mis)
    {
        ++counter;
    }

    JLE_TEST_ASSERT(counter==3);


    {
        auto itf = mis.find(33);
        JLE_TEST_ASSERT(itf != mis.end())
    }

    {
        auto itf = mis.find(123);
        JLE_TEST_ASSERT(itf == mis.end())
    }

    mis.clear();
    JLE_TEST_ASSERT(mis.size() == 0);
}

void  test_list()
{
    JLE_TEST_INIT

    jle::list<std::string>  li;
    li.push_back("00000000");
    li.push_back("1111111");
    li.push_back("22222222");


    JLE_TEST_ASSERT(li.size() == 3);

    int counter=0;
    for(auto _ : li)
        ++counter;

    JLE_TEST_ASSERT(counter==3);


    li.clear();
    JLE_TEST_ASSERT(li.size() == 0);
}

void  test_set()
{
    // todo:
}


void  test_fail_iterators_map()
{
    JLE_TEST_INIT


    //  getting info from not initialized container
    {
        jle::map<int, int>::iterator  it;
        JLE_TEST_EXCEPTION(it->first);
    }

    //  advancing not initialized iterator
    {
        jle::map<int, int>::iterator  it;
        JLE_TEST_EXCEPTION(++it);
    }

    //  empty map
    {
        jle::map<int, int>  mii;
        jle::map<int, int>::iterator  it = mii.begin();
        JLE_TEST_EXCEPTION(it->first);
    }


    //  iterator to cleaned container
    {
        jle::map<int, int>::iterator  it;
        {
            jle::map<int, int>  mii;
            mii[0]=0;
            it = mii.begin();
            JLE_TEST_ASSERT(it->first == 0);
        }
        JLE_TEST_EXCEPTION(it->first);
    }

    //  advancing iterator, removed container
    {
        jle::map<int, int>::iterator  it;
        {
            jle::map<int, int>  mii;
            mii[0]=0;
            it = mii.begin();
            JLE_TEST_ASSERT(it->second == 0);
        }
        JLE_TEST_EXCEPTION(++it);
    }


    //  advance iterator removed content on container
    {
        jle::map<int, int>::iterator  it;
        {
            jle::map<int, int>  mii;
            mii[0]=0;
            it = mii.begin();
            mii.clear();
            JLE_TEST_EXCEPTION(it->first == 0);
            JLE_TEST_EXCEPTION(++it);
        }
    }

    //  getting info from iterator removed from container
    {
        {
            jle::map<int, int>  mii;
            jle::map<int, int>::iterator  it;
            mii[0]=0;
            it = mii.find(0);
            mii.erase(0);
            JLE_TEST_EXCEPTION(it->first);
        }
    }



    //  advance iterator pointing to end
    {
        jle::map<int, int>  mii;
        mii[0] = 0;
        jle::map<int, int>::iterator  it = mii.end();
        JLE_TEST_EXCEPTION((++it)->first);
    }


    //  advancing iterator reached end
    {
        jle::map<int, int>  mii;
        mii[0] = 0;
        jle::map<int, int>::iterator  it = mii.begin();
        ++it;
        JLE_TEST_EXCEPTION((++it)->first);
    }

    //  comparing iterators from diferent containers
    {
        jle::map<int, int>  mii;
        jle::map<int, int>  mii2;
        mii[0] = 0;
        mii2[1] = 1;
        jle::map<int, int>::iterator  it = mii.begin();
        JLE_TEST_EXCEPTION(it == mii2.begin());
        JLE_TEST_EXCEPTION(it != mii2.begin());
        JLE_TEST_EXCEPTION(it == mii2.end());
        JLE_TEST_EXCEPTION(it != mii2.end());
    }
}

void test_fail_iterators_vector()
{
    JLE_TEST_INIT

    //  advancing iterator pointing to end
    {
        jle::vector<int>  vi;
        vi.push_back(1);
        vi.push_back(2);
        jle::vector<int>::iterator  it = vi.end();
        JLE_TEST_EXCEPTION(*(++it));
    }

    //  iterator deleted container
    {
        jle::vector<int>  vi;
        vi.push_back(1);
        vi.push_back(2);
        jle::vector<int>::iterator  it = vi.begin();
        ++it;
        vi.clear();
        JLE_TEST_EXCEPTION(*it);
    }

    //  deleted iterator
    {
        jle::vector<int>  vi;
        vi.push_back(1);
        vi.push_back(2);
        jle::vector<int>::iterator  it = vi.begin();
        ++it;
        vi.pop_back();
        JLE_TEST_EXCEPTION(*it);
    }
}


int main(void)
{
    JLE_TEST_FILE

        test_vector();
        test_map();
        test_list();
        test_set();
        test_fail_iterators_map();
        test_fail_iterators_vector();

    JLE_TEST_REPORT
}


void jle::alarm_msg(const jle::alarm& al)
{
    std::cout << "ouch...: "  << al << std::endl;

}
