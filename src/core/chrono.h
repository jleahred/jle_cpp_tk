#ifndef JLE_CHRONO_H
#define JLE_CHRONO_H

#include <chrono>
#include <ostream>
using namespace std::literals;



/**
 *     \example  ./core/ex_chrono.cpp
 */



namespace jle {

    //! Here we have helpers and functions for time_point and durations
    namespace  chrono {

    using  duration   = std::chrono::milliseconds;

    struct year  {  int __value;  };
    struct month {  int __value;  };
    struct day   {  int __value;  };


    /** \brief  similar to std::chrono::time_point but monotonic with adjustments to system_clock
     */
    class time_point {
            std::chrono::steady_clock::time_point  tp;
            //  todo: correction

            time_point(const std::chrono::steady_clock::time_point&  _tp) :  tp(_tp) {}


        public:
            time_point()=delete;
            time_point(const time_point&)=default;
            time_point(      time_point&&);
            time_point& operator=(const time_point&)=default;
            time_point& operator=(      time_point&&);

            time_point& operator +=  (const duration&   dtn);
            time_point& operator -=  (const duration&   dtn);
            time_point  operator +   (const duration&   dtn)  const;
            time_point  operator -   (const duration&   dtn)  const;
            duration    operator -   (const time_point& _tp)  const;
            bool        operator ==  (const time_point& rhs)  const;
            bool        operator !=  (const time_point& rhs)  const;
            bool        operator <   (const time_point& rhs)  const;
            bool        operator >   (const time_point& rhs)  const;
            bool        operator >=  (const time_point& rhs)  const;
            bool        operator <=  (const time_point& rhs)  const;


            friend  time_point  now  (void);
            friend  time_point  today(void);
            friend  time_point  make_from_date   (const year& y, const month& m, const day& d);

            friend  std::tuple<std::tm, int>  get_tm_millisecs(const jle::chrono::time_point& tp);
    };

    time_point      now     (void);

    time_point      today   (void);

    time_point      make_from_date   (const year&, const month&, const day&);


    /** \brief  get a std::tm and milliseconds in int from time_point
     */
    std::tuple<std::tm, int>  get_tm_millisecs(const jle::chrono::time_point& tp);



}; };       //  namespace jle {  namespace  chrono {




namespace jle {  namespace  chrono { namespace  stream {

    std::ostream& operator<<(std::ostream& out, const ::jle::chrono::time_point &);
    std::ostream& operator<<(std::ostream& out, const ::jle::chrono::duration &);

    template<typename T, typename __p>
    std::ostream& operator<<(std::ostream& out, const ::std::chrono::duration<T, __p> &d)
    {
        return operator<<(out, std::chrono::duration_cast<std::chrono::milliseconds>(d));
    }

}; }; };
using namespace jle::chrono::stream;




#endif // JLE_CHRONO_H
