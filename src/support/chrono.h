#ifndef JLE_CHRONO_H
#define JLE_CHRONO_H

#include <chrono>
#include <ostream>
using namespace std::literals;




namespace jle {  namespace  chrono {

    using  time_point = std::chrono::steady_clock::time_point;
    using  duration   = std::chrono::milliseconds;

    time_point      now (void);




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
