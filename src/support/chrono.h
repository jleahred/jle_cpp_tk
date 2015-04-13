#ifndef JLE_CHRONO_H
#define JLE_CHRONO_H

#include <chrono>
#include <ostream>
using namespace std::literals;




namespace jle {  namespace  chrono {

    using  time_point = std::chrono::steady_clock::time_point;
    using  duration   = std::chrono::milliseconds;

    namespace  t {
        struct year  {  int __value;  };
        struct month {  int __value;  };
        struct day   {  int __value;  };
    };

    time_point      now     (void);

    time_point      today   (void);

    time_point      make_from_date   (const t::year&, const t::month&, const t::day&);


    std::tuple<std::tm, int>  get_tm_millisecs(const jle::chrono::time_point& tp);



}; };       //  namespace jle {  namespace  chrono {




namespace jle {  namespace  chrono { namespace  stream {

    std::ostream& operator<<(std::ostream& out, const jle::chrono::time_point &);
    std::ostream& operator<<(std::ostream& out, const jle::chrono::duration &);

    template<typename T, typename __p>
    std::ostream& operator<<(std::ostream& out, const std::chrono::duration<T, __p> &d)
    {
        return operator<<(out, std::chrono::duration_cast<std::chrono::milliseconds>(d));
    }

}; }; };
using namespace jle::chrono::stream;




#endif // JLE_CHRONO_H
