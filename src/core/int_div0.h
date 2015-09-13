#ifndef JLE_INT_DIV0__H
#define JLE_INT_DIV0__H


#include <signal.h>
#include <setjmp.h>
#include <string.h>   //  memcpy


//!  \example  ./core/ex_int_div0.cpp



namespace jle {


#ifdef _WIN32

    #define JLE_HANDLE_DIV0_INIT                        \
        {                                    \
        jle::init_handle_signal_FPE_intdiv0();                \
        if (setjmp(jle::env_jmp_div0)!=0)                \
        {                                \
            throw std::runtime_error("SIGNAL FPE  probably int div0");    \
        }                                \
        else                                \
        {

    #define JLE_HANDLE_DIV0_INIT_A(__A__)                        \
        {                                    \
        jmp_buf previus_env_jmp_div0##__A__;                    \
        memcpy(previus_env_jmp_div0##__A__, jle::env_jmp_div0, sizeof(jmp_buf));    \
        bool    previus_init_jump_div0##__A__ = jle::init_jump_div0;        \
        jle::init_jump_div0 = true;                    \
        jle::init_handle_signal_FPE_intdiv0();                \
        if (setjmp(jle::env_jmp_div0)!=0)                    \
        {                                \
            memcpy(jle::env_jmp_div0, previus_env_jmp_div0##__A__, sizeof(jmp_buf)); \
            jle::init_jump_div0 = previus_init_jump_div0##__A__;            \
            throw std::runtime_error("SIGNAL FPE  probably int div0");    \
        }                                \
        else                                \
        {

#elif __linux__
    #define JLE_HANDLE_DIV0_INIT                        \
        {                                    \
        jle::init_handle_signal_FPE_intdiv0();                \
        if (sigsetjmp(jle::env_jmp_div0, 1)!=0)                \
        {                                \
            throw std::runtime_error("SIGNAL FPE  probably int div0");    \
        }                                \
        else                                \
        {

        #define JLE_HANDLE_DIV0_INIT_A(__A__)                        \
        {                                    \
        jmp_buf previus_env_jmp_div0##__A__;                    \
        memcpy(previus_env_jmp_div0##__A__, jle::env_jmp_div0, sizeof(jmp_buf));    \
        bool    previus_init_jump_div0##__A__ = jle::init_jump_div0;        \
        jle::init_jump_div0 = true;                    \
        jle::init_handle_signal_FPE_intdiv0();                \
        if (sigsetjmp(jle::env_jmp_div0, 1)!=0)                \
        {                                \
            memcpy(jle::env_jmp_div0, previus_env_jmp_div0##__A__, sizeof(jmp_buf)); \
            jle::init_jump_div0 = previus_init_jump_div0##__A__;            \
            throw std::runtime_error("SIGNAL FPE  probably int div0");    \
        }                                \
        else                                \
        {

#else
    #error  "Unknown platfom..."
#endif


#define JLE_HANDLE_DIV0_END                        \
    }                                    \
    }


#define JLE_HANDLE_DIV0_END_A(__A__)                        \
    }                                    \
    memcpy(jle::env_jmp_div0, previus_env_jmp_div0##__A__, sizeof(jmp_buf));    \
    jle::init_jump_div0 = previus_init_jump_div0##__A__;            \
    }



    //  Using JLE_HANDLE_DIV0 doesn't require to call this function
    //  it is called by such macro
    void init_handle_signal_FPE_intdiv0 (void);
    extern jmp_buf env_jmp_div0;
    extern bool    init_jump_div0;






};      //  end namespace jle

#endif      //  JLE_INT_DIV0__H

