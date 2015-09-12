#ifndef JLE_SYNCHR_H
#define JLE_SYNCHR_H


#include <mutex>


#define JLE_SYNCHR  std::lock_guard<std::recursive_mutex> __jle_lock__(jle::synchr::get_sref_main_mutex());


namespace jle {
    namespace synchr {

        void         main_lock           (void);
        void         main_unlock         (void);

        std::recursive_mutex&  get_sref_main_mutex (void);

    }   //  {    namespace synchr {
}; //  namespace jle



#endif // JLE_SYNCHR_H
