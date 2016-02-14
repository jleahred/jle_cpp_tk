#ifndef JLE_BASIC_TYPES_H
#define JLE_BASIC_TYPES_H




namespace jle
{

    class non_copyable {
    public:
        non_copyable() {}
        ~non_copyable() {}
        non_copyable(non_copyable&&) = default;
        non_copyable& operator=(non_copyable&&) = default;

        non_copyable(const non_copyable&) = delete;
        non_copyable& operator=(const non_copyable&) = delete;
    };

};  //  end namespace


#endif // JLE_BASIC_TYPES_H
