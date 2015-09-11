#ifndef JLE_SYNCHR
#define JLE_SYNCHR


#include <mutex>



namespace jle {
    namespace synchr {

        void         main_lock           (void);
        void         main_unlock         (void);

        std::mutex&  get_sref_main_mutex (void);

    }   //  {    namespace synchr {
}; //  namespace jle



#endif // JLE_SYNCHR
