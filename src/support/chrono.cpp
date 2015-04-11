#include "chrono.h"

#include <ctime>
#include <tuple>



namespace {


std::tuple<std::tm, int>  descompose_time_point(const jle::chrono::time_point& tp)
{
    static auto init_machine_time = std::chrono::system_clock::now();
    static auto init_monotonic_time = std::chrono::steady_clock::now();
    std::time_t time_t =  std::chrono::system_clock::to_time_t(init_machine_time + (tp - init_monotonic_time));
    struct tm * ptm;
    ptm = gmtime ( &time_t );

    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count() % 1000;
    return std::make_tuple(*ptm, milliseconds);
}

};      //  namespace {


namespace jle {  namespace  chrono {

time_point      now()
{
    return std::chrono::steady_clock::now();
}




}; };





namespace jle {  namespace  chrono { namespace  stream {

std::ostream& operator<<(std::ostream& out, const jle::chrono::time_point &tp)
{
    std::tm _tm;
    int milliseconds;
    std::tie(_tm, milliseconds) = descompose_time_point(tp);

    out << _tm.tm_year+1900         << "-";
    out.width(2); out.fill('0');
    out << _tm.tm_mon +1            << "-";
    out.width(2); out.fill('0');
    out << _tm.tm_mday +1           << " ";

    out.width(2); out.fill('0');
    out << _tm.tm_hour              << ":";
    out.width(2); out.fill('0');
    out << _tm.tm_min               << ":";
    out.width(2); out.fill('0');
    out << _tm.tm_sec               << ".";
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

