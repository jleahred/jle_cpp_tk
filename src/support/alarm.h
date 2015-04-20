#ifndef  JLE_ALARM_H
#define  JLE_ALARM_H


#include <string>
#include <sstream>
#include <list>

#include "support/chrono.h"
#include "support/string.h"



namespace jle {

//! small helpers for alarms
namespace al {


#define  JLE_HERE  JLE_SS(__PRETTY_FUNCTION__ << "(" << __FILE__ << ":" << __LINE__ << ")")


//---------------------------------------------------------------------------


enum class priority   {
                        critic       ,
                        error        ,
                        warning      ,
                        debug        ,
                        stats
                      };


enum class type       {
                        unknown       ,
                        no_permisions ,
                        overflow      ,
                        realtime      ,
                        logic_error
                      };





struct  base_alarm {
    base_alarm() = delete;
    base_alarm(const base_alarm&) = default;
    base_alarm& operator=(const base_alarm&) = default;
    base_alarm& operator=(base_alarm&&) = default;


    std::string             code_source      ;
    std::string             subject         ;
    std::string             message         ;
    al::priority            priority        ;
    al::type                type            ;
    jle::chrono::time_point time_point      ;

    base_alarm (
                        const std::string&      _code_source  ,
                        const std::string&      _subject      ,
                        const std::string&      _message      ,
                        al::priority            _priority     ,
                        al::type                _type
        )
        :   code_source (_code_source),
            subject     (_subject),
            message     (_message),
            priority    (_priority),
            type        (_type),
            time_point  (jle::chrono::now())
            {};


    virtual ~base_alarm(){};

    friend std::ostream& operator<< (std::ostream& os, const ::jle::al::base_alarm& a);
};
std::ostream& operator<< (std::ostream& os, const ::jle::al::base_alarm& a);


};      //  namespace al







struct alarm : public  al::base_alarm
{
    alarm() = delete;
    alarm(const alarm&) = default;
    alarm& operator=(const alarm&) = default;
    alarm& operator=(alarm&&) = default;

    explicit alarm (
                        const std::string&      _codeSource,
                        const std::string&      _subject,
                        const std::string&      _message,
                        al::priority            _priority,
                        al::type                _type = al::type::unknown
        )
        :     base_alarm(_codeSource, _subject, _message, _priority, _type)
            , alarm_id(++class_alarm_id%=2000000000)
            , stack_alarms() {}

        const int   alarm_id;

    alarm&   add  (const base_alarm&  prev_alarm) {
            stack_alarms.push_back(prev_alarm);
            return *this;
    };

    alarm&  add (const alarm& prev_alarm) {
        stack_alarms.push_back(prev_alarm);

        std::list<base_alarm>::const_iterator it= prev_alarm.stack_alarms.begin();
        while (it != prev_alarm.stack_alarms.end())
        {
            stack_alarms.push_back(*it);
            ++it;
        }
        return *this;
    };


    std::list<base_alarm>   stack_alarms;

    friend std::ostream& operator<< (std::ostream& os, const ::jle::alarm& a);

private:
    static int  class_alarm_id;
};



//---------------------------------------------------------------------------


extern void alarm_msg (const alarm& error);


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------



//  rethrow will ignore when alarms are equals
#define JLE_CATCH_RETHROW(__SUBJECT__, __DESCRIPTION__)    \
                  catch (jle::Alarm& __alarm__) {                   \
                    if (__alarm__.subject == __SUBJECT__   &&  __alarm__.message ==(__DESCRIPTION__ +std::string(" alarm")))  \
                        throw __alarm__;  \
                    jle::Alarm  jle_error(                              \
                                JLE_HERE,                            \
                                __SUBJECT__,                      \
                                __DESCRIPTION__  +std::string(" alarm"), \
                                __alarm__.priority,                      \
                                __alarm__.type                            \
                        );                                              \
                    jle_error.add(__alarm__);                         \
                    throw (jle_error);                                   \
                } catch (std::exception& e) {                           \
                    throw jle::alarm(                                 \
                                JLE_HERE,                            \
                                __SUBJECT__,                      \
                                __DESCRIPTION__  +std::string(" exception ") + e.what(),\
                                jle::alPriorCritic);                    \
                } catch (...) {                                         \
                    throw jle::alarm(                                 \
                                JLE_HERE,                            \
                                __SUBJECT__,                      \
                                __DESCRIPTION__   +std::string(" ..."), \
                                jle::alPriorCritic);                    \
                }

#define JLE_CATCH_CALLFUNCION(__FUNCTION2CALL__, __SUBJECT__, __DESCRIPTION__)    \
                  catch (const jle::alarm& __alarm__) {                   \
                    jle::alarm  jle_error(                              \
                                JLE_HERE,                            \
                                __SUBJECT__,                      \
                                __DESCRIPTION__  + std::string(" alarm"),\
                                __alarm__.priority,                      \
                                __alarm__.type                            \
                        );                                              \
                    jle_error.add(__alarm__);				\
                    __FUNCTION2CALL__ (jle_error);                                   \
                } catch (std::exception& e) {                           \
                    __FUNCTION2CALL__ (jle::alarm(                                 \
                                JLE_HERE,                            \
                                __SUBJECT__,                      \
                                __DESCRIPTION__  + std::string(" exception ") + e.what(), \
                                jle::alPriorCritic));                    \
                } catch (...) {                                         \
                    __FUNCTION2CALL__ (jle::alarm(                                 \
                                JLE_HERE,                            \
                                __SUBJECT__,                      \
                                __DESCRIPTION__  + std::string(" exception"),\
                                jle::alPriorCritic));                    \
                }


};   // end namespace jle {


std::ostream& operator<< (std::ostream& os, const ::jle::alarm& a);




#endif   // JLE_ALARM_H
