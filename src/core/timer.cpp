#include "timer.h"

#include "core/synchr.h"
#include "core/cont/list.hpp"
#include "core/exec_max_frec.hpp"




namespace {
    jle::signal<>&  get_sref_signal_timer_10ms(void) {
        static auto result = new jle::signal<>{};
        return *result;
    }
    jle::signal<>&  get_sref_signal_timer_100ms(void) {
        static auto result = new jle::signal<>{};
        return *result;
    }
    jle::signal<>&  get_sref_signal_timer_1s(void) {
        static auto result = new jle::signal<>{};
        return *result;
    }

    bool&  get_sref_stopping(void)
    {
        static auto result = new bool(false);
        return *result;
    }

    struct Timer_container :  public jle::signal_receptor {


        jle::list<jle::shared_ptr<jle::timer::internal::Timer>>*
                timer_list = new jle::list<jle::shared_ptr<jle::timer::internal::Timer>>{};
        bool    deleted_pendings=false;

        void add_timer(const jle::shared_ptr<jle::timer::internal::Timer>&  timer) {
            timer_list->push_back(timer);
        }

        void check_deleted_timers(void) {
            if(deleted_pendings) {
                auto it = timer_list->begin();
                while(it != timer_list->end()) {
                    auto& timer = *(*it);
                    if(timer.call_times == 0) {
                        it = timer_list->erase(it);
                    } else {
                        ++it;
                    }
                }
            }
        }

        void deleted_timer() {
            deleted_pendings = true;
        }

    };

    Timer_container&  get_sref_timer_container(void)
    {
        auto result = new Timer_container{};
        return *result;
    }
};


namespace jle { namespace timer {

void start_main_loop()
{
    jle::synchr::main_lock();
    get_sref_stopping() = false;
    while(get_sref_stopping()==false) {
        jle::synchr::main_unlock();
        std::this_thread::sleep_for(9ms);

        get_sref_signal_timer_10ms().emit();


        JLE_EXEC_MAX_FREC_STATIC(90ms)
            get_sref_signal_timer_100ms().emit();
        JLE_END_EXEC_MAX_FREC


        JLE_EXEC_MAX_FREC_STATIC(990ms)
            get_sref_signal_timer_1s().emit();
        JLE_END_EXEC_MAX_FREC

        JLE_EXEC_MAX_FREC_STATIC(10s)
            get_sref_signal_timer_1s().emit();
        JLE_END_EXEC_MAX_FREC


        jle::synchr::main_lock();
    }
}


void stop_main_loop()
{
    get_sref_stopping() = true;
}




namespace internal {

    Timer::Timer(   const jle::chrono::time_point&  _last_call,
                    const jle::chrono::duration&    _interval,
                    int                             _call_times)
        : last_call(_last_call), interval(_interval), call_times(_call_times)
    {
    }


    jle::signal<>&  register_timer(const jle::chrono::duration& dur, int call_times)
    {

        auto new_timer = jle::make_shared<Timer>(jle::chrono::now(), dur, call_times);
        if(dur < 1s) {
            JLE_CONNECT_METHOD(get_sref_signal_timer_10ms(), *new_timer, on_global_timer);
        } else if(dur < 5s) {
            JLE_CONNECT_METHOD(get_sref_signal_timer_100ms(), *new_timer, on_global_timer);
        } else {
            JLE_CONNECT_METHOD(get_sref_signal_timer_1s(), *new_timer, on_global_timer);
        }

        get_sref_timer_container().add_timer(new_timer);

        return new_timer->signal;
    }

    void Timer::on_global_timer(void)
    {
        auto now = jle::chrono::now();
        if(this->last_call + this->interval < now)
        {
            if(this->call_times == 0)
                return;

            this->last_call = now;
            if(this->call_times > 0)
                this->call_times -= 1;

            if(this->call_times == 0)
                get_sref_timer_container().deleted_pendings = true;

            this->signal.emit();
        }
    }

}}};      //  namespace jle::timer::internal {


