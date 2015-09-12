#include "synchr.h"


namespace jle {
    namespace synchr {

    std::recursive_mutex&  get_sref_main_mutex(void)
    {
        static auto result = new std::recursive_mutex;
        return *result;
    }

    void main_lock (void)
    {
        get_sref_main_mutex().lock();
    }

    void main_unlock (void)
    {
        get_sref_main_mutex().unlock();
    }


}}; //  namespace jle {    namespace synchr {
