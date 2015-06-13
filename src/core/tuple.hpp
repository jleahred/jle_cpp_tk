#ifndef jle_tuple__h
#define jle_tuple__h


#include <iostream>
#include <tuple>



namespace jle {

    template<typename ...T> using tuple   = typename std::tuple<T...>;


    namespace internal {
        template<std::size_t I = 0, typename F, typename... T>
        inline typename std::enable_if<I == sizeof...(T), void>::type
        apply(const jle::tuple<T...> &, F)
        {
            //std::cout << "END" <<std::endl;
        }

        template<std::size_t I = 0, typename F, typename... T>
        inline typename std::enable_if<I < sizeof...(T), void>::type
        apply(const jle::tuple<T...>& t, F f)
        {
            f(std::get<I>(t), sizeof...(T)==I+1);
            apply<I + 1, F, T...>(t, f);
        }
    }

}



template<typename ...T>
std::ostream& operator<< (std::ostream& os, const jle::tuple<T...>& t)
{
    os << "t(";
    jle::internal::apply(t, [&os=os](auto item, bool last) {
                        os << item;
                        if(last==false)
                            os << ", ";
                    });
    os << ")";
    return os;
}





#endif
