#include <iostream>


#include "support/shared_ptr.hpp"

struct Foo {
    Foo() { std::cout << "Foo...\n"; }
    ~Foo() { std::cout << "~Foo...\n"; }
};

struct D {
    void operator()(Foo* p) const {
        std::cout << "Call delete for Foo object...\n";
        delete p;
    }
};

int main()
{
    {
        std::cout << "constructor with no managed object\n";
        jle::shared_ptr<Foo> sh1;
    }

    {
        std::cout << "constructor with object\n";
        jle::shared_ptr<Foo> sh2(new Foo);
        jle::shared_ptr<Foo> sh3(sh2);
        std::cout << sh2.use_count() << '\n';
        std::cout << sh3.use_count() << '\n';
    }

    {
        std::cout << "constructor with object and deleter\n";
        jle::shared_ptr<Foo> sh4(new Foo, D());
    }

    {
        std::cout << "valid pointer" << std::endl;
        auto ptr = jle::make_shared<int>(22);
        std::cout << "expired " << int(ptr.expired()) << std::endl;
    }

    {
        std::cout << "invalid pointer" << std::endl;
        auto ptr = jle::make_shared<int>(22);
        ptr.reset();
        std::cout << "expired " << int(ptr.expired()) << std::endl;
    }

    {
        std::cout << "invalid pointer" << std::endl;
        jle::shared_ptr<int> ptr;
        std::cout << "expired " << int(ptr.expired()) << std::endl;
    }

    {
        jle::shared_ptr<int> foo = jle::make_shared<int> (10);
        // same as:
        jle::shared_ptr<int> foo2 (new int(10));

        auto bar = jle::make_shared<int> (20);

        auto baz = jle::make_shared<std::pair<int,int>> (30,40);

        std::cout << "*foo: " << *foo << '\n';
        std::cout << "*bar: " << *bar << '\n';
        std::cout << "*baz: " << baz->first << ' ' << baz->second << '\n';
    }
}