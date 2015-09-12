#ifndef JLE_TIMER_H
#define JLE_TIMER_H


#include <thread>

#include "core/chrono.h"
#include "core/signal_slot.hpp"



#define JLE_TIMER_FUNCT(__DURATION__, __FUNCTION__ )  \
    jle::timer::internal::register_timer(__DURATION__, -1).connect(__FUNCTION__);


#define JLE_TIMER_METHOD(__DURATION__, __INSTANCE__, __METHOD_NAME__ )  \
    JLE_CONNECT_METHOD(jle::timer::internal::register_timer(__DURATION__, -1), __INSTANCE__, __METHOD_NAME__)


#define JLE_TIMER_THIS(__DURATION__, __METHOD_NAME__ )  \
    JLE_CONNECT_THIS(jle::timer::internal::register_timer(__DURATION__, -1), __METHOD_NAME__)


#define JLE_ONE_SHOOT_FUNCT(__DURATION__, __FUNCTION__ )  \
    jle::timer::internal::register_timer(__DURATION__, 1).connect(__FUNCTION__);


#define JLE_ONE_SHOOT_METHOD(__DURATION__, __INSTANCE__, __METHOD_NAME__ )  \
    JLE_CONNECT_METHOD(jle::timer::internal::register_timer(__DURATION__, 1), __INSTANCE__, __METHOD_NAME__)


#define JLE_ONE_SHOOT_THIS(__DURATION__, __METHOD_NAME__ )  \
    JLE_CONNECT_THIS(jle::timer::internal::register_timer(__DURATION__, 1), __METHOD_NAME__)



//!  \example  ./core/ex_timer.cpp




namespace jle {
    namespace timer {


        void start_main_loop();
        void stop_main_loop();




        namespace internal {

            jle::signal<>&  register_timer(const jle::chrono::duration& dur, int call_times);

            struct Timer : jle::signal_receptor {
                explicit Timer( const jle::chrono::time_point&  _last_call,
                                const jle::chrono::duration&    _interval,
                                int                             _call_times);
                Timer()             = delete;
                Timer(const Timer&) = delete;


                jle::chrono::time_point  last_call;
                jle::chrono::duration    interval;
                int                      call_times;

                jle::signal<>            signal;

                void on_global_timer(void);
            };


        };//        namespace internal {
};//    namespace timer {
};//namespace jle {

#endif // JLE_TIMER_H
