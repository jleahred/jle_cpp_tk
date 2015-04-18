#include <iostream>

#include "support/shared_ptr.hpp"
#include "support/test.h"



struct Foo {
    Foo(int _i, int _j) : i(_i), j(_j) { }
    ~Foo() { }

    int i;
    int j;
};




void test_use_count()
{
    JLE_TEST_INIT

    {
        jle::shared_ptr<Foo> p;
        JLE_TEST_ASSERT(p.use_count() == 0);

        jle::shared_ptr<Foo> p1(p);
        JLE_TEST_ASSERT(p.use_count() == 0);
        JLE_TEST_ASSERT(p1.use_count() == 0);

        jle::shared_ptr<Foo> p2(p);
        JLE_TEST_ASSERT(p.use_count() == 0);
        JLE_TEST_ASSERT(p1.use_count() == 0);
        JLE_TEST_ASSERT(p2.use_count() == 0);

        jle::shared_ptr<Foo> p3(p2);
        JLE_TEST_ASSERT(p.use_count() == 0);
        JLE_TEST_ASSERT(p1.use_count() == 0);
        JLE_TEST_ASSERT(p2.use_count() == 0);
        JLE_TEST_ASSERT(p3.use_count() == 0);
    }

    {
        jle::shared_ptr<Foo> p{new Foo{42, 26}};
        JLE_TEST_ASSERT(p.use_count() == 1);

        jle::shared_ptr<Foo> p1(p);
        JLE_TEST_ASSERT(p.use_count() == 2);
        JLE_TEST_ASSERT(p1.use_count() == 2);

        jle::shared_ptr<Foo> p2(p);
        JLE_TEST_ASSERT(p.use_count() == 3);
        JLE_TEST_ASSERT(p1.use_count() == 3);
        JLE_TEST_ASSERT(p2.use_count() == 3);

        jle::shared_ptr<Foo> p3(p2);
        JLE_TEST_ASSERT(p.use_count() == 4);
        JLE_TEST_ASSERT(p1.use_count() == 4);
        JLE_TEST_ASSERT(p2.use_count() == 4);
        JLE_TEST_ASSERT(p3.use_count() == 4);
    }

    {
        jle::shared_ptr<Foo> p= jle::make_shared<Foo>(42, 26);
        JLE_TEST_ASSERT(p.use_count() == 1);

        jle::shared_ptr<Foo> p1(p);
        JLE_TEST_ASSERT(p.use_count() == 2);
        JLE_TEST_ASSERT(p1.use_count() == 2);

        jle::shared_ptr<Foo> p2(p);
        JLE_TEST_ASSERT(p.use_count() == 3);
        JLE_TEST_ASSERT(p1.use_count() == 3);
        JLE_TEST_ASSERT(p2.use_count() == 3);

        jle::shared_ptr<Foo> p3(p2);
        JLE_TEST_ASSERT(p.use_count() == 4);
        JLE_TEST_ASSERT(p1.use_count() == 4);
        JLE_TEST_ASSERT(p2.use_count() == 4);
        JLE_TEST_ASSERT(p3.use_count() == 4);
    }

    {
        jle::shared_ptr<Foo> p= jle::make_shared<Foo>(42, 26);
        JLE_TEST_ASSERT(p.use_count() == 1);
        {
            jle::shared_ptr<Foo> p1(p);
            JLE_TEST_ASSERT(p.use_count() == 2);
            JLE_TEST_ASSERT(p1.use_count() == 2);

            jle::shared_ptr<Foo> p2(p);
            JLE_TEST_ASSERT(p.use_count() == 3);
            JLE_TEST_ASSERT(p1.use_count() == 3);
            JLE_TEST_ASSERT(p2.use_count() == 3);
        }
        jle::shared_ptr<Foo> p3(p);
        JLE_TEST_ASSERT(p.use_count() == 2);
        JLE_TEST_ASSERT(p3.use_count() == 2);
    }

    {
        jle::shared_ptr<Foo> p;
        JLE_TEST_ASSERT(p.use_count() == 0);

        jle::shared_ptr<Foo> p1(p);
        JLE_TEST_ASSERT(p.use_count() == 0);
        JLE_TEST_ASSERT(p1.use_count() == 0);

        jle::shared_ptr<Foo> p2 = jle::make_shared<Foo>(42, 42);
        JLE_TEST_ASSERT(p.use_count() == 0);
        JLE_TEST_ASSERT(p1.use_count() == 0);
        JLE_TEST_ASSERT(p2.use_count() == 1);

        jle::shared_ptr<Foo> p3(p2);
        JLE_TEST_ASSERT(p.use_count() == 0);
        JLE_TEST_ASSERT(p1.use_count() == 0);
        JLE_TEST_ASSERT(p2.use_count() == 2);
        JLE_TEST_ASSERT(p3.use_count() == 2);
    }

}
void test_weak_ptr()
{
    JLE_TEST_INIT

    {
        jle::shared_ptr<Foo> p;
        JLE_TEST_ASSERT(p.use_count() == 0);
        jle::weak_ptr<Foo> w(p);
        JLE_TEST_ASSERT(p.use_count() == 0);
        JLE_TEST_ASSERT(w.expired() == true);
    }

    {
        jle::shared_ptr<Foo> p = jle::make_shared<Foo>(42, 26);
        JLE_TEST_ASSERT(p.use_count() == 1);
        jle::weak_ptr<Foo> w(p);
        JLE_TEST_ASSERT(p.use_count() == 1);
        JLE_TEST_ASSERT(w.expired() == false);
    }

    {
        jle::weak_ptr<Foo> w;
        {
            jle::shared_ptr<Foo> p = jle::make_shared<Foo>(42, 26);
            JLE_TEST_ASSERT(p.use_count() == 1);
            JLE_TEST_ASSERT(p.use_count() == 1);
            w=p;
            JLE_TEST_ASSERT(w.expired() == false);
        }
    }

    {
        jle::weak_ptr<Foo> w;
        {
            jle::shared_ptr<Foo> p = jle::make_shared<Foo>(42, 26);
            JLE_TEST_ASSERT(p.use_count() == 1);
            JLE_TEST_ASSERT(p.use_count() == 1);
            w=p;
            JLE_TEST_ASSERT(w.expired() == false);
        }
        JLE_TEST_ASSERT(w.expired() == true);
    }
}
void test_expired()
{
    JLE_TEST_INIT

    {
        jle::weak_ptr<Foo> w;
        {
            jle::shared_ptr<Foo> p = jle::make_shared<Foo>(42, 26);
            JLE_TEST_ASSERT(p.use_count() == 1);
            JLE_TEST_ASSERT(p.use_count() == 1);
            w=p;
            JLE_TEST_ASSERT(w.expired() == false);
        }
        JLE_TEST_ASSERT(w.expired() == true);
    }
}

void test_operators()
{
    JLE_TEST_INIT

    {
        jle::shared_ptr<Foo> p;
        p = jle::make_shared<Foo>(42, 26);
        JLE_TEST_ASSERT(p.use_count() == 1);
        jle::weak_ptr<Foo> w(p);
        JLE_TEST_ASSERT(p.use_count() == 1);
        JLE_TEST_ASSERT(w.expired() == false);
    }

    {
        jle::shared_ptr<Foo> p {jle::make_shared<Foo>(42, 26)};
        jle::shared_ptr<Foo> p1{p};

        JLE_TEST_ASSERT(p.use_count() == 2);
        jle::weak_ptr<Foo> w(p);
        JLE_TEST_ASSERT(p.use_count() == 2);
        JLE_TEST_ASSERT(w.expired() == false);
    }

    {
        jle::shared_ptr<Foo> p = jle::make_shared<Foo>(42, 26);
        JLE_TEST_ASSERT(p.use_count() == 1);
        jle::weak_ptr<Foo> w(p);
        JLE_TEST_ASSERT(p.use_count() == 1);
        JLE_TEST_ASSERT(w.expired() == false);

        JLE_TEST_ASSERT(w.lock()->i == 42);
        JLE_TEST_ASSERT((*(w.lock())).j == 26);

        JLE_TEST_ASSERT(w.lock().get()->i == 42);
        JLE_TEST_ASSERT((*(w.lock()).get()).j == 26);
    }

}
void test_exception_on_expired()
{
    JLE_TEST_INIT

    {
        jle::shared_ptr<Foo> p;
        JLE_TEST_ASSERT(p.use_count() == 0);
        jle::weak_ptr<Foo> w(p);
        JLE_TEST_ASSERT(p.use_count() == 0);
        JLE_TEST_ASSERT(w.expired() == true);

        p.reset();

        JLE_TEST_EXCEPTION(w.lock()->i == 42);
        JLE_TEST_EXCEPTION((*(w.lock())).j == 26);

        JLE_TEST_EXCEPTION(w.lock().get()->i == 42);
        JLE_TEST_EXCEPTION((*(w.lock()).get()).j == 26);
    }

    {
        jle::shared_ptr<Foo> p;
        JLE_TEST_ASSERT(p.use_count() == 0);
        jle::weak_ptr<Foo> w(p);
        JLE_TEST_ASSERT(p.use_count() == 0);
        JLE_TEST_ASSERT(w.expired() == true);

        p = jle::shared_ptr<Foo>{};
        JLE_TEST_EXCEPTION(w.lock()->i == 42);
        JLE_TEST_EXCEPTION((*(w.lock())).j == 26);

        JLE_TEST_EXCEPTION(w.lock().get()->i == 42);
        JLE_TEST_EXCEPTION((*(w.lock()).get()).j == 26);
    }

    {
        jle::shared_ptr<Foo> p;
        p = jle::make_shared<Foo>(42, 26);
        JLE_TEST_ASSERT(p.use_count() == 1);
        jle::weak_ptr<Foo> w(p);

        p.reset();

        JLE_TEST_EXCEPTION(w.lock()->i == 42);
        JLE_TEST_EXCEPTION((*(w.lock())).j == 26);

        JLE_TEST_EXCEPTION(w.lock().get()->i == 42);
        JLE_TEST_EXCEPTION((*(w.lock()).get()).j == 26);
    }

    {
        jle::shared_ptr<Foo> p {jle::make_shared<Foo>(42, 26)};
        jle::shared_ptr<Foo> p1{p};

        JLE_TEST_ASSERT(p.use_count() == 2);
        jle::weak_ptr<Foo> w(p);
        JLE_TEST_ASSERT(p.use_count() == 2);
        JLE_TEST_ASSERT(w.expired() == false);

        p.reset();

        JLE_TEST_EXCEPTION(p->i == 42);
        JLE_TEST_EXCEPTION((*p).j == 26);
        JLE_TEST_ASSERT(p1.use_count() == 1);
        JLE_TEST_ASSERT(w.expired() == false);

        p1.reset();

        JLE_TEST_EXCEPTION(w.lock().get()->i == 42);
        JLE_TEST_EXCEPTION((*(w.lock()).get()).j == 26);
    }

    {
        jle::shared_ptr<Foo> p = jle::make_shared<Foo>(42, 26);
        JLE_TEST_ASSERT(p.use_count() == 1);
        jle::weak_ptr<Foo> w(p);
        JLE_TEST_ASSERT(p.use_count() == 1);
        JLE_TEST_ASSERT(w.expired() == false);

        JLE_TEST_ASSERT(w.lock()->i == 42);
        JLE_TEST_ASSERT((*(w.lock())).j == 26);

        JLE_TEST_ASSERT(w.lock().get()->i == 42);
        JLE_TEST_ASSERT((*(w.lock()).get()).j == 26);

        p.reset();

        JLE_TEST_EXCEPTION(w.lock()->i == 42);
        JLE_TEST_EXCEPTION((*(w.lock())).j == 26);

        JLE_TEST_EXCEPTION(w.lock().get()->i == 42);
        JLE_TEST_EXCEPTION((*(w.lock()).get()).j == 26);
    }
}
void test_make_shared_ptr()
{

}
void test_reset(void)
{
    {
        jle::shared_ptr<Foo> p {jle::make_shared<Foo>(42, 26)};
        JLE_TEST_ASSERT(p.use_count() == 1);
        jle::weak_ptr<Foo> w(p);
        JLE_TEST_ASSERT(p.use_count() == 1);
        JLE_TEST_ASSERT(w.expired() == false);

        p.reset();

        JLE_TEST_ASSERT(p.use_count() == 0);
        JLE_TEST_ASSERT(w.expired() == true);

        JLE_TEST_EXCEPTION(w.lock()->i == 42);
        JLE_TEST_EXCEPTION((*(w.lock())).j == 26);

        JLE_TEST_EXCEPTION(w.lock().get()->i == 42);
        JLE_TEST_EXCEPTION((*(w.lock()).get()).j == 26);
    }
}


int main()
{
    JLE_TEST_FILE

    test_use_count();
    test_weak_ptr();
    test_expired();
    test_operators();
    test_exception_on_expired();
    test_make_shared_ptr();
    test_reset();

    JLE_TEST_REPORT
}
