#ifndef JLE_EXEC_MAX_FREC__H
#define JLE_EXEC_MAX_FREC__H

#include <map>
#include "core/chrono.h"
#include "core/optional.hpp"




#define JLE_EXEC_MAX_FREC_THIS(___TIMEINTERVAL___)                                               \
            {                   \
            auto __now__jle_exec_max_frec__ = jle::chrono::now(); \
            static auto __map_last_informed__jle__ = std::map<long, jle::optional<jle::chrono::time_point>>{};       \
            if (__map_last_informed__jle__.find(long(this)) == __map_last_informed__jle__.end())             \
                __map_last_informed__jle__[long(this)] =  jle::make_optional(__now__jle_exec_max_frec__- 24h);   \
            auto last_informed = __map_last_informed__jle__[long(this)].Get();                           \
            if (__last_informed__jle_exec_max_frec__ > __now__jle_exec_max_frec__) {                                        \
                __last_informed__jle_exec_max_frec__ =  __now__jle_exec_max_frec__ - 24h;                        \
                __map___last_informed__jle_exec_max_frec____jle_exec_max_frec__[long(this)] =  __last_informed__jle_exec_max_frec__;                            \
            }                                                                           \
            if (__now__jle_exec_max_frec__ - __last_informed__jle_exec_max_frec__  >  (___TIMEINTERVAL___) )  {             \
                __last_informed__jle_exec_max_frec__ = __now__jle_exec_max_frec__;                                         \
                __map___last_informed__jle_exec_max_frec____jle_exec_max_frec__[long(this)] =  __last_informed__jle_exec_max_frec__;



#define JLE_EXEC_MAX_FREC_THIS_NO_FIRST(___TIMEINTERVAL___)     \
            {       \
            auto __now__jle_exec_max_frec__ = jle::chrono::now(); \
            static auto __map___last_informed__jle_exec_max_frec____jle_exec_max_frec__ = std::map<long, jle::optional<jle::chrono::time_point>>{};       \
            if (__map___last_informed__jle_exec_max_frec____jle_exec_max_frec__.find(long(this)) == __map___last_informed__jle_exec_max_frec____jle_exec_max_frec__.end())             \
                __map___last_informed__jle_exec_max_frec____jle_exec_max_frec__[long(this)] =  jle::make_optional(__now__jle_exec_max_frec__);   \
            auto __last_informed__jle_exec_max_frec__ = __map___last_informed__jle_exec_max_frec____jle_exec_max_frec__[long(this)].Get();                           \
            if (__last_informed__jle_exec_max_frec__ > __now__jle_exec_max_frec__) {                                        \
                __last_informed__jle_exec_max_frec__ =  __now__jle_exec_max_frec__ - 24h;                        \
                __map___last_informed__jle_exec_max_frec____jle_exec_max_frec__[long(this)] =  __last_informed__jle_exec_max_frec__;                            \
            }                                                                           \
            if (jle::now() - __last_informed__jle_exec_max_frec__  >  (___TIMEINTERVAL___) )  {             \
                __last_informed__jle_exec_max_frec__ = jle::now();                                         \
                __map___last_informed__jle_exec_max_frec____jle_exec_max_frec__[long(this)] =  __last_informed__jle_exec_max_frec__;




#define JLE_EXEC_MAX_FREC_STATIC(___TIMEINTERVAL___)                                               \
            {                                                                           \
            auto __now__jle_exec_max_frec__ = jle::chrono::now(); \
            static jle::chrono::time_point  __last_informed__jle_exec_max_frec__ =  __now__jle_exec_max_frec__ - 24h;       \
            if (__last_informed__jle_exec_max_frec__ > __now__jle_exec_max_frec__)                                         \
                __last_informed__jle_exec_max_frec__ =  __now__jle_exec_max_frec__-24h;                        \
            if (__now__jle_exec_max_frec__ - __last_informed__jle_exec_max_frec__ >  (___TIMEINTERVAL___) )  {             \
                __last_informed__jle_exec_max_frec__ = __now__jle_exec_max_frec__;

#define JLE_EXEC_MAX_FREC_STATIC_NO_FIRST(___TIMEINTERVAL___)                                               \
            {                                                                           \
            auto __now__jle_exec_max_frec__ = jle::chrono::now(); \
            static jle::chrono::time_point  __last_informed__jle_exec_max_frec__ =  __now__jle_exec_max_frec__;       \
            if (__last_informed__jle_exec_max_frec__ > __now__jle_exec_max_frec__)                                         \
                __last_informed__jle_exec_max_frec__ =  __now__jle_exec_max_frec__-24h;                        \
            if (__now__jle_exec_max_frec__ - __last_informed__jle_exec_max_frec__ >  (___TIMEINTERVAL___) )  {             \
                __last_informed__jle_exec_max_frec__ = __now__jle_exec_max_frec__;




#define JLE_ELSE_EXEC_MAX_FREC  }    else    {


#define JLE_END_EXEC_MAX_FREC  }  }








#endif   // JLE_EXEC_MAX_FREC__H


