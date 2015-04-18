#ifndef  JLE_ALARM_H
#define  JLE_ALARM_H


#include <string>
#include <sstream>
#include <list>

#include "support/chrono.h"



namespace jle {

//! small helpers for alarms
namespace al {

#define __INTERNAL_ALARM_LOCAL_MTK_SS(__EXPR__) (static_cast<std::ostringstream*>(&(std::ostringstream().flush() << __EXPR__))->str())


#define  JLE_HERE  __INTERNAL_LOCAL_MTK_SS(__PRETTY_FUNCTION__ << "(" << __FILE__ << ":" << __LINE__ << ")")


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
    chrono::time_point      time_point      ;

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
            time_point  (chrono::now())
            {};


    virtual ~base_alarm(){};

    friend std::ostream& operator<< (std::ostream& os, const al::base_alarm& a);
};


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

friend std::ostream& operator<< (std::ostream& os, const alarm& a);

private:
    static int  class_alarm_id;
};


//---------------------------------------------------------------------------


extern void alarm_msg (const alarm& error);


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------



//  rethrow no apila si los mensajes son iguales
#define MTK_CATCH_RETHROW(__SUBJECT__, __DESCRIPTION__)    \
                  catch (mtk::Alarm& __alarm__) {                   \
                    if (__alarm__.subject == __SUBJECT__   &&  __alarm__.message ==(__DESCRIPTION__ +std::string(" alarm")))  \
                        throw __alarm__;  \
                    mtk::Alarm  mtkError(                              \
                                MTK_HERE,                            \
                                __SUBJECT__,                      \
                                __DESCRIPTION__  +std::string(" alarm"), \
                                __alarm__.priority,                      \
                                __alarm__.type                            \
                        );                                              \
                    mtkError.Add(__alarm__);                         \
                    throw (mtkError);                                   \
                } catch (std::exception& e) {                           \
                    throw mtk::Alarm(                                 \
                                MTK_HERE,                            \
                                __SUBJECT__,                      \
                                __DESCRIPTION__  +std::string(" exception ") + e.what(),\
                                mtk::alPriorCritic);                    \
                } catch (...) {                                         \
                    throw mtk::Alarm(                                 \
                                MTK_HERE,                            \
                                __SUBJECT__,                      \
                                __DESCRIPTION__   +std::string(" ..."), \
                                mtk::alPriorCritic);                    \
                }

#define MTK_CATCH_CALLFUNCION(__FUNCTION2CALL__, __SUBJECT__, __DESCRIPTION__)    \
                  catch (const mtk::Alarm& __alarm__) {                   \
                    mtk::Alarm  mtkError(                              \
                                MTK_HERE,                            \
                                __SUBJECT__,                      \
                                __DESCRIPTION__  + std::string(" alarm"),\
                                __alarm__.priority,                      \
                                __alarm__.type                            \
                        );                                              \
                    mtkError.Add(__alarm__);				\
                    __FUNCTION2CALL__ (mtkError);                                   \
                } catch (std::exception& e) {                           \
                    __FUNCTION2CALL__ (mtk::Alarm(                                 \
                                MTK_HERE,                            \
                                __SUBJECT__,                      \
                                __DESCRIPTION__  + std::string(" exception ") + e.what(), \
                                mtk::alPriorCritic));                    \
                } catch (...) {                                         \
                    __FUNCTION2CALL__ (mtk::Alarm(                                 \
                                MTK_HERE,                            \
                                __SUBJECT__,                      \
                                __DESCRIPTION__  + std::string(" exception"),\
                                mtk::alPriorCritic));                    \
                }



};   // end namespace jle {



#endif   // JLE_ALARM_H
