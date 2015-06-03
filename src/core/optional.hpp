#ifndef jle_optional__h
#define jle_optional__h

#include <experimental/optional>



namespace jle
{

    struct nullopt_t
    {
        // Do not user-declare default constructor at all for
        // optional_value = {} syntax to work.
        // nullopt_t() = delete;

        // Used for constructing nullopt.
        enum class _Construct { _Token };

        // Must be constexpr for nullopt_t to be literal.
        explicit constexpr nullopt_t(_Construct) { }
    };

    /// Tag type for in-place construction.
    struct in_place_t { };

    /// Tag for in-place construction.
    constexpr in_place_t in_place { };


    /**
     * @brief Class template for optional values.
     */
    template<typename _Tp>
    class optional
    {
    private:
        std::experimental::optional<_Tp>  iopt;

    public:
    //  constructors ..........
        constexpr optional() noexcept  {}

        constexpr optional(nullopt_t) noexcept {}

        // Constructors for engaged optionals.
        constexpr optional(const _Tp& __t)
            : iopt{__t}  {}

        constexpr optional(_Tp&& __t)
            : iopt{__t}  {}

        template<typename... _Args>
        constexpr explicit optional(in_place_t, _Args&&... __args)
            : iopt{in_place, __args ...}  {}

        template<typename _Up, typename... _Args,
            typename std::enable_if<std::is_constructible<_Tp,
               std::initializer_list<_Up>&,
               _Args&&...>::value,
            int>::type...>
        constexpr explicit optional(in_place_t,
                std::initializer_list<_Up> __il,
                _Args&&... __args)
            : iopt{in_place, __il, __args ...}  {}

        // Copy and move constructors.
        optional(const optional& __other)
            : iopt{__other.iopt} {}

        optional(optional&& __other)
            noexcept(std::is_nothrow_move_constructible<_Tp>())
            : iopt{__other.iopt}  {}
    //  constructors ..........






        //using value_type = _Tp;

        optional&
        operator=(nullopt_t) noexcept
        {
            iopt.operator=(std::experimental::nullopt);
            return *this;
//            this->_M_reset();
//            return *this;
        }

        optional&
        operator=(const optional& __other)
        {
            iopt = __other.iopt;
            return *this;

        }

        optional&
        operator=(optional&& __other)
            noexcept(std::__and_<std::is_nothrow_move_constructible<_Tp>,
            std::is_nothrow_move_assignable<_Tp>>())
        {
            iopt = __other.iopt;
            return *this;
        }


        template<typename _Up>
        typename std::enable_if<
                std::is_same<_Tp, typename std::decay<_Up>::type>::value,
                optional&
            >::type
        operator=(_Up&& __u)
        {
            static_assert(std::__and_<std::is_constructible<_Tp, _Up>,
                    std::is_assignable<_Tp&, _Up>>(),
                    "Cannot assign to value type from argument");

            iopt.operator=(__u);
            return *this;
//            if (this->_M_is_engaged())
//            this->_M_get() = std::forward<_Up>(__u);
//            else
//            this->_M_construct(std::forward<_Up>(__u));
//
//            return *this;
        }

        template<typename... _Args>
        void
        emplace(_Args&&... __args)
        {
            static_assert(std::is_constructible<_Tp, _Args&&...>(),
            "Cannot emplace value type from arguments");

            iopt.emplace(__args ...);
//            this->_M_reset();
//            this->_M_construct(std::forward<_Args>(__args)...);
        }

        template<typename _Up, typename... _Args>
        typename std::enable_if<
            std::is_constructible<_Tp,
                  std::initializer_list<_Up>&,
                  _Args&&...>::value
            >::type
        emplace(std::initializer_list<_Up> __il, _Args&&... __args)
        {
            iopt.emplace(__il, __args ...);
//            this->_M_reset();
//            this->_M_construct(__il, std::forward<_Args>(__args)...);
        }

        // [X.Y.4.2] Destructor is implicit, implemented in _Optional_base.

        // [X.Y.4.4] Swap.
        void
        swap(optional& __other)
            //noexcept(std::is_nothrow_move_constructible<_Tp>()
            //&& noexcept(swap(declval<_Tp&>(), declval<_Tp&>())))
        {
            iopt.swap(__other.iopt);
//            using std::swap;
//
//            if (this->_M_is_engaged() && __other._M_is_engaged())
//            swap(this->_M_get(), __other._M_get());
//            else if (this->_M_is_engaged())
//            {
//            __other._M_construct(std::move(this->_M_get()));
//            this->_M_destruct();
//            }
//            else if (__other._M_is_engaged())
//            {
//            this->_M_construct(std::move(__other._M_get()));
//            __other._M_destruct();
//            }
        }

        // [X.Y.4.5] Observers.
        constexpr const _Tp*
        operator->() const
        {
            if(has_value() == false)
                throw std::runtime_error("optional not valid on ->");

            return iopt.operator->();
//            return __constexpr_addressof(this->_M_get());
        }

        _Tp*
        operator->()
        {
            if(has_value() == false)
                throw std::runtime_error("optional not valid on ->");

            return iopt.operator->();
//            return std::__addressof(this->_M_get());
        }

        constexpr const _Tp&
        operator*() const
        {
            if(has_value() == false)
                throw std::runtime_error("optional not valid on *");

            return iopt.operator*();
//            return this->_M_get();
        }

        _Tp&
        operator*()
        {
            if(has_value() == false)
                throw std::runtime_error("optional not valid on ->");

            return iopt.operator*();
//            return this->_M_get();
        }

        constexpr explicit operator bool() const noexcept
        {
            return bool(iopt);
//            return this->_M_is_engaged();
        }

        constexpr bool has_value() const noexcept
        {
            return bool(iopt);
//            return this->_M_is_engaged();
        }

        constexpr const _Tp&
        value() const
        {
            return iopt.value();
//this->_M_get();
//            return this->_M_is_engaged()
//            ?  this->_M_get()
//            : (__throw_bad_optional_access("Attempt to access value of a "
//                 "disengaged optional object"),
//            this->_M_get());
        }

        _Tp&
        value()
        {
            return iopt.value();
//            if (this->_M_is_engaged())
//            return this->_M_get();

//            __throw_bad_optional_access("Attempt to access value of a "
//            "disengaged optional object");
        }

        template<typename _Up>
        constexpr _Tp
        value_or(_Up&& __u) const&
        {
            return iopt.value_or(__u);
//            static_assert(std::__and_<std::is_copy_constructible<_Tp>,
//            std::is_convertible<_Up&&, _Tp>>(),
//            "Cannot return value");
//
//            return this->_M_is_engaged()
//            ? this->_M_get()
//                : static_cast<_Tp>(std::forward<_Up>(__u));
        }

        template<typename _Up>
        _Tp
        value_or(_Up&& __u) &&
        {
            return iopt.value_or(__u);
//            static_assert(std::__and_<std::is_move_constructible<_Tp>,
//            std::is_convertible<_Up&&, _Tp>>(),
//            "Cannot return value" );
//
//            return this->_M_is_engaged()
//            ? std::move(this->_M_get())
//                : static_cast<_Tp>(std::forward<_Up>(__u));
        }
    };



}       //  namespace jle

#endif // jle_optional__h
