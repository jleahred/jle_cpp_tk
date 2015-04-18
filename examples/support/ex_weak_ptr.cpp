#include <iostream>
#include "support/shared_ptr.hpp"

jle::weak_ptr<int> gw;

void f()
{
    jle::shared_ptr<int> spt = gw.lock();  // Has to be copied into a shared_ptr before usage
    if (spt.expired()==false) {
        std::cout << *spt << "\n";
    }
    else {
        std::cout << "gw is expired\n";
    }
}

int main()
{
    {
        auto sp = jle::make_shared<int>(42);
        gw = sp;
        f();
    }

    f();
}
