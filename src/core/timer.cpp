#include "timer.h"

#include "core/synchr.h"



namespace jle { namespace timer {

void check_timers_loop()
{
    jle::synchr::main_lock();
    while(true) {
        std::cout << "." << std::endl;
        jle::synchr::main_unlock();
        std::this_thread::sleep_for(100ms);
        jle::synchr::main_lock();
    }
}





namespace internal {


}}};      //  namespace jle::timer::internal {


