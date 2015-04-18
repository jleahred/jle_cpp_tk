#ifndef JLE_BASIC_TYPES_H
#define JLE_BASIC_TYPES_H




namespace jle
{

    class non_copyable {
        non_copyable(const non_copyable&) = delete;
        non_copyable(non_copyable&&) = delete;
        non_copyable& operator=(const non_copyable&) = delete;
        non_copyable& operator=(non_copyable&&) = delete;

    public:
        non_copyable() {}
        ~non_copyable() {}
    };

};  //  end namespace


#endif // JLE_BASIC_TYPES_H
