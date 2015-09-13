#include "int_div0.h"


#include <stdexcept>
#include <stdlib.h>
#include <time.h>
#include <iostream>



#ifdef _WIN32
   #include <float.h>
#endif



namespace jle {


    jmp_buf env_jmp_div0;
    bool    init_jump_div0= false;

    void bypass_sigfpe(int /*sig_no*/)
    {
        if( signal(SIGFPE, bypass_sigfpe) == SIG_ERR )
        std::cerr << "div0 handler not initialized";
#ifdef _WIN32
        _fpreset();
        longjmp(env_jmp_div0, 1);
#elif __linux__
        siglongjmp(env_jmp_div0, 1);
#endif
    }

    void init_handle_signal_FPE_intdiv0 (void)
    {
    static bool initialized = false;

    if (initialized)   return;
    initialized = true;

    if( signal(SIGFPE, bypass_sigfpe) == SIG_ERR )
        throw std::runtime_error("div0 handler not initialized");
    }



};      //  end namespace jle



