#include "chrono.h"

#include <ctime>
#include <tuple>

#include <iostream>


namespace {

    auto  get_init_ref_times (void) {
        static auto init_machine_time = std::chrono::system_clock::now();
        static auto init_monotonic_time = std::chrono::steady_clock::now();

        return std::make_tuple(init_machine_time, init_monotonic_time);
    };

};      //  namespace {


namespace jle {  namespace  chrono {

time_point      now()
{
    return std::chrono::steady_clock::now();
}


time_point      make_from_date   (const t::year& y, const t::month& m, const t::day& d)
{
    static auto ref_times           =   get_init_ref_times();
    static auto init_machine_time   =   std::get<0>(ref_times);
    static auto init_monotonic_time =   std::get<1>(ref_times);


    struct std::tm tm;
    tm.tm_sec = 0;
    tm.tm_min = 0;
    tm.tm_hour = 0;
    tm.tm_mday = d.__value;
    tm.tm_mon = m.__value -1;
    tm.tm_year = y.__value -1900;
    tm.tm_isdst = 0;       // determine whether daylight saving time
    std::time_t tt = std::mktime(&tm) - timezone;
    if (tt == -1) {
        throw "no valid system time";
    }

    auto tp_system = std::chrono::system_clock::from_time_t(tt);


    auto result = tp_system - init_machine_time + init_monotonic_time;
    result -= std::chrono::milliseconds{
                        std::chrono::duration_cast<std::chrono::milliseconds>
                        (result.time_since_epoch()).count() % 1000};
                    ;
    return result + std::chrono::milliseconds{1000};
}


time_point      today   (void)
{
    std::tm tm;
    std::tie(tm, std::ignore) = get_tm_millisecs(std::chrono::steady_clock::now());
    return make_from_date(  t::year{tm.tm_year+1900},
                            t::month{tm.tm_mon+1},
                            t::day{tm.tm_mday});
}




std::tuple<std::tm, int>  get_tm_millisecs(const jle::chrono::time_point& tp)
{
    static auto ref_times = get_init_ref_times();
    static auto init_machine_time = std::get<0>(ref_times);
    static auto init_monotonic_time = std::get<1>(ref_times);

    std::time_t time_t =  std::chrono::system_clock::to_time_t(init_machine_time + (tp - init_monotonic_time));
    struct tm * ptm;
    ptm = gmtime ( &time_t );

    int milliseconds {static_cast<int>(
                        std::chrono::duration_cast<std::chrono::milliseconds>
                                    (tp.time_since_epoch())
                                    .count()) % 1000
                     };
    return std::make_tuple(*ptm, milliseconds);
}



}; };





namespace jle {  namespace  chrono { namespace  stream {

std::ostream& operator<<(std::ostream& out, const jle::chrono::time_point &tp)
{
    std::tm _tm;
    int milliseconds;
    std::tie(_tm, milliseconds) = get_tm_millisecs(tp);

    out << _tm.tm_year+1900         << "-";
    out.width(2); out.fill('0');
    out << _tm.tm_mon +1            << "-";
    out.width(2); out.fill('0');
    out << _tm.tm_mday              << " ";

    out.width(2); out.fill('0');
    out << _tm.tm_hour              << ":";
    out.width(2); out.fill('0');
    out << _tm.tm_min               << ":";
    out.width(2); out.fill('0');
    out << _tm.tm_sec               << ".";
    out.width(3); out.fill('0');
    out << milliseconds;

    return out;
}

std::ostream& operator<<(std::ostream& out, const jle::chrono::duration &d)
{
    long pending = d.count();


    #define __JLE__REDUCE(__NAME__, __VALUE__)   \
            long  __NAME__ = pending % __VALUE__;   \
            pending /= __VALUE__;

    __JLE__REDUCE(milliseconds, 1000);
    __JLE__REDUCE(seconds,      60);
    __JLE__REDUCE(minutes,      60);
    __JLE__REDUCE(hours,        24);

    long days = pending;


    #undef __JLE__REDUCE


    #define __JLE__PRINT(__NAME__, __MAGNITUDE__)   \
        if(__NAME__ != 0)  out << __NAME__ << __MAGNITUDE__ << " ";


    __JLE__PRINT(days, "d");
    __JLE__PRINT(hours, "h");
    __JLE__PRINT(minutes, "m");
    __JLE__PRINT(seconds, "s");
    __JLE__PRINT(milliseconds, "ms");


    #undef __JLE__PRINT

    return out;
}


}; }; };       //  namespace jle {  namespace  chrono {  namespace  stream {

