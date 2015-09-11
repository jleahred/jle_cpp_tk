#ifndef JLE_TIMER_H
#define JLE_TIMER_H


#include <thread>

#include "core/chrono.h"
#include "core/signal_slot.hpp"





namespace jle {
    namespace timer {


        void check_timers_loop();




        namespace internal {

            struct Timer {
                jle::chrono::time_point  last_call;
                jle::chrono::duration    interval;

                jle::signal<>            signal;
            };
        };//        namespace internal {
};//    namespace timer {
};//namespace jle {

#endif // JLE_TIMER_H
