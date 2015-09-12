#include <iostream>

#include "core/alarm.h"
#include "core/signal_slot.hpp"
#include "core/timer.h"





//  Function to receive timer events
void test_timer(void)
{
    std::cout << jle::chrono::now() << "  on_timer in fucntion" << std::endl;
}



//  class to receive timer events
class Test_timer :  public jle::signal_receptor {
public:

    Test_timer() {
        //  configuring a timer for current object  THIS
        JLE_TIMER_THIS(7s, on_timer2)
    }

    void on_timer() {
        std::cout << jle::chrono::now() <<  "  on timer in class" << std::endl;
    }

    void on_timer2() {
        std::cout << jle::chrono::now() << "  on timer2 in class" << std::endl;
    }
};



int main()
{
    std::cout << jle::chrono::now() << "  starting..." << std::endl;
    //  configure timer for function
    JLE_TIMER_FUNCT(1s, test_timer);

    //  configure a timer for a method of an object
    Test_timer tt;
    JLE_TIMER_METHOD(2s, tt, on_timer);

    //  example call once
    JLE_ONE_SHOOT_FUNCT(3s, [](){ std::cout << jle::chrono::now() <<  "  CALL ONCE... " << std::endl;});


    //  program stop after 10s
    JLE_ONE_SHOOT_FUNCT(10s, [](){ std::cout << "CLOSING... ";  jle::timer::stop_main_loop();});
    jle::timer::start_main_loop();
}


void jle::alarm_msg(const jle::alarm& al)
{
    std::cout << al << std::endl;
}
