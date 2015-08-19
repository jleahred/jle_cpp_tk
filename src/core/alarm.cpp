#include "alarm.h"

#include <tuple>

#include "core/string.h"






namespace jle {
//---------------------------------------------------------------------------

int alarm::class_alarm_id=0;




void write_time (std::ostream& os, const chrono::time_point& tp)
{
    os.width(2); os.fill('0');
    auto tm_millsecs = chrono::get_tm_millisecs(tp);
    auto tm = std::get<std::tm>(tm_millsecs);

    os << tm.tm_hour << ":";
    os.width(2); os.fill('0');
    os << tm.tm_min << ":";
    os.width(2); os.fill('0');
    os << tm.tm_sec << ":";
    os.width(3); os.fill('0');
    os << std::get<int>(tm_millsecs);
}


void write_body (std::ostream& os, const al::base_alarm&  a)
{
    if      (a.priority == al::priority::critic)
        os << " CRIT.";
    else if (a.priority == al::priority::error)
        os << " ERROR";
    else if (a.priority == al::priority::warning)
        os << " Warn ";
    else if (a.priority == al::priority::debug)
        os << " debug";
    else if (a.priority == al::priority::stats)
        os << " stats";
    else
        os << " [" << int(a.priority) << "]";

    if      (a.type == al::type::no_permisions)
        os << " NoPerm";
    else if (a.type == al::type::overflow)
        os << " Overfl";
    else if (a.type == al::type::realtime)
        os << " RealTm";
    else if (a.type == al::type::unknown)
        os << " Unkown";
    else if (a.type == al::type::logic_error)
        os << " LogErr";
    else
        os << " <" << int(a.type) << ">";


    os  << " # " << a.message
        << " >" << a.subject
        << " @" << a.code_source;
}

namespace al {
std::ostream& operator<< (std::ostream& os, const base_alarm&  a)
{
    write_time(os, a.time_point);
    write_body(os, a);

    return os;
};
}  // namespace al


//---------------------------------------------------------------------------
};      //  namespace jle {



std::ostream& operator<< (std::ostream& os, const ::jle::alarm& a)
{
    os << static_cast<const jle::al::base_alarm&>(a);

    if (a.stack_alarms.empty()==false)
        os << " [" << a.alarm_id<< "]";

    std::list<jle::al::base_alarm>::const_iterator it = a.stack_alarms.cbegin();
    while (it != a.stack_alarms.end())
    {
        os << std::endl;
        os << jle::s_align_right(JLE_SS(it->time_point - a.time_point), 12);
        jle::write_body(os, *it);
        ++it;
    }

    return os;
};

