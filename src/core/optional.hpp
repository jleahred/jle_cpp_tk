#ifndef jle_optional__h
#define jle_optional__h

#include <experimental/optional>


/**
 *     \example  ./core/ex_optional.cpp
 */



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

    constexpr nullopt_t nullopt { nullopt_t::_Construct::_Token };


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
        }

        template<typename... _Args>
        void
        emplace(_Args&&... __args)
        {
            static_assert(std::is_constructible<_Tp, _Args&&...>(),
            "Cannot emplace value type from arguments");

            iopt.emplace(__args ...);
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
        }



        void
        swap(optional& __other)
            //noexcept(std::is_nothrow_move_constructible<_Tp>()
            //&& noexcept(swap(declval<_Tp&>(), declval<_Tp&>())))
        {
            iopt.swap(__other.iopt);
        }


        constexpr const _Tp*
        operator->() const
        {
            if(has_value() == false)
                throw std::runtime_error("optional not valid on ->");

            return iopt.operator->();
        }

        _Tp*
        operator->()
        {
            if(has_value() == false)
                throw std::runtime_error("optional not valid on ->");

            return iopt.operator->();
        }

        constexpr const _Tp&
        operator*() const
        {
            if(has_value() == false)
                throw std::runtime_error("optional not valid on *");

            return iopt.operator*();
        }

        _Tp&
        operator*()
        {
            if(has_value() == false)
                throw std::runtime_error("optional not valid on ->");

            return iopt.operator*();
        }

        constexpr explicit operator bool() const noexcept
        {
            return bool(iopt);
        }

        constexpr bool has_value() const noexcept
        {
            return bool(iopt);
        }

        constexpr const _Tp&
        value() const
        {
            return iopt.value();
        }

        _Tp&
        value()
        {
            return iopt.value();
        }

        template<typename _Up>
        constexpr _Tp
        value_or(_Up&& __u) const&
        {
            return iopt.value_or(__u);
        }

        template<typename _Up>
        _Tp
        value_or(_Up&& __u) &&
        {
            return iopt.value_or(__u);
        }

        template<typename _Tpa>
        friend constexpr bool
        operator==(const optional<_Tpa>& __lhs, const optional<_Tpa>& __rhs);

        template<typename _Tpa>
        friend constexpr bool
        operator!=(const optional<_Tpa>& __lhs, const optional<_Tpa>& __rhs);

        template<typename _Tpa>
        friend constexpr bool
        operator<(const optional<_Tpa>& __lhs, const optional<_Tpa>& __rhs);

        template<typename _Tpa>
        friend constexpr bool
        operator>(const optional<_Tpa>& __lhs, const optional<_Tpa>& __rhs);

        template<typename _Tpa>
        friend constexpr bool
        operator<=(const optional<_Tpa>& __lhs, const optional<_Tpa>& __rhs);

        template<typename _Tpa>
        friend constexpr bool
        operator>=(const optional<_Tpa>& __lhs, const optional<_Tpa>& __rhs);

        template<typename _Tpa>
        friend constexpr bool
        operator==(const optional<_Tpa>& __lhs, nullopt_t) noexcept;

        template<typename _Tpa>
        friend constexpr bool
        operator==(nullopt_t, const optional<_Tpa>& __rhs) noexcept;

        template<typename _Tpa>
        friend constexpr bool
        operator!=(const optional<_Tpa>& __lhs, nullopt_t) noexcept;

        template<typename _Tpa>
        friend constexpr bool
        operator!=(nullopt_t, const optional<_Tpa>& __rhs) noexcept;

        template<typename _Tpa>
        friend constexpr bool
        operator<(const optional<_Tpa>& /* __lhs */, nullopt_t) noexcept;

        template<typename _Tpa>
        friend constexpr bool
        operator<(nullopt_t, const optional<_Tpa>& __rhs) noexcept;

        template<typename _Tpa>
        friend constexpr bool
        operator>(const optional<_Tpa>& __lhs, nullopt_t) noexcept;

        template<typename _Tpa>
        friend constexpr bool
        operator>(nullopt_t, const optional<_Tpa>& /* __rhs */) noexcept;

        template<typename _Tpa>
        friend constexpr bool
        operator<=(const optional<_Tpa>& __lhs, nullopt_t) noexcept;

        template<typename _Tpa>
        friend constexpr bool
        operator<=(nullopt_t, const optional<_Tpa>& /* __rhs */) noexcept;

        template<typename _Tpa>
        friend constexpr bool
        operator>=(const optional<_Tpa>& /* __lhs */, nullopt_t) noexcept;

        template<typename _Tpa>
        friend constexpr bool
        operator>=(nullopt_t, const optional<_Tpa>& __rhs) noexcept;

        template<typename _Tpa>
        friend constexpr bool
        operator==(const optional<_Tpa>& __lhs, const _Tpa& __rhs);

        template<typename _Tpa>
        friend constexpr bool
        operator==(const _Tpa& __lhs, const optional<_Tpa>& __rhs);

        template<typename _Tpa>
        friend constexpr bool
        operator!=(const optional<_Tpa>& __lhs, _Tpa const& __rhs);

        template<typename _Tpa>
        friend constexpr bool
        operator!=(const _Tpa& __lhs, const optional<_Tpa>& __rhs);

        template<typename _Tpa>
        friend constexpr bool
        operator<(const optional<_Tpa>& __lhs, const _Tpa& __rhs);

        template<typename _Tpa>
        friend constexpr bool
        operator<(const _Tpa& __lhs, const optional<_Tpa>& __rhs);

        template<typename _Tpa>
        friend constexpr bool
        operator>(const optional<_Tpa>& __lhs, const _Tpa& __rhs);

        template<typename _Tpa>
        friend constexpr bool
        operator>(const _Tpa& __lhs, const optional<_Tpa>& __rhs);

        template<typename _Tpa>
        friend constexpr bool
        operator<=(const optional<_Tpa>& __lhs, const _Tpa& __rhs);

        template<typename _Tpa>
        friend constexpr bool
        operator<=(const _Tpa& __lhs, const optional<_Tpa>& __rhs);

        template<typename _Tpa>
        friend constexpr bool
        operator>=(const optional<_Tpa>& __lhs, const _Tpa& __rhs);

        template<typename _Tpa>
        friend constexpr bool
        operator>=(const _Tpa& __lhs, const optional<_Tpa>& __rhs);

    };


    template<typename _Tp>
    constexpr bool
    operator==(const optional<_Tp>& __lhs, const optional<_Tp>& __rhs)
    {
        return __lhs.iopt == __rhs.iopt;
    }

    template<typename _Tp>
    constexpr bool
    operator!=(const optional<_Tp>& __lhs, const optional<_Tp>& __rhs)
    {
        return __lhs.iopt != __rhs.iopt;
    }

    template<typename _Tp>
    constexpr bool
    operator<(const optional<_Tp>& __lhs, const optional<_Tp>& __rhs)
    {
        return __lhs.iopt < __rhs.iopt;
    }

    template<typename _Tp>
    constexpr bool
    operator>(const optional<_Tp>& __lhs, const optional<_Tp>& __rhs)
    {
        return __lhs.iopt > __rhs.iopt;
    }

    template<typename _Tp>
    constexpr bool
    operator<=(const optional<_Tp>& __lhs, const optional<_Tp>& __rhs)
    {
        return __lhs.iopt <= __rhs.iopt;
    }

    template<typename _Tp>
    constexpr bool
    operator>=(const optional<_Tp>& __lhs, const optional<_Tp>& __rhs)
    {
        return __lhs.iopt >= __rhs.iopt;
    }

    // [X.Y.9] Comparisons with nullopt.
    template<typename _Tp>
    constexpr bool
    operator==(const optional<_Tp>& __lhs, nullopt_t) noexcept
    {
        return __lhs.iopt == nullopt;
    }

    template<typename _Tp>
    constexpr bool
    operator==(nullopt_t, const optional<_Tp>& __rhs) noexcept
    {
        return nullopt == __rhs.iopt;
    }

    template<typename _Tp>
    constexpr bool
    operator!=(const optional<_Tp>& __lhs, nullopt_t) noexcept
    {
        return __lhs.iopt != nullopt;
    }

    template<typename _Tp>
    constexpr bool
    operator!=(nullopt_t, const optional<_Tp>& __rhs) noexcept
    {
        return nullopt != __rhs.iopt;
    }

    template<typename _Tp>
    constexpr bool
    operator<(const optional<_Tp>& /* __lhs */, nullopt_t) noexcept
    {
        return  false;
    }

    template<typename _Tp>
    constexpr bool
    operator<(nullopt_t, const optional<_Tp>& __rhs) noexcept
    {
        return  nullopt < __rhs.iopt;
    }

    template<typename _Tp>
    constexpr bool
    operator>(const optional<_Tp>& __lhs, nullopt_t) noexcept
    {
        return   __lhs.iopt > nullopt;
    }

    template<typename _Tp>
    constexpr bool
    operator>(nullopt_t, const optional<_Tp>& /* __rhs */) noexcept
    {
        return   false;
    }

    template<typename _Tp>
    constexpr bool
    operator<=(const optional<_Tp>& __lhs, nullopt_t) noexcept
    {
        return    __lhs.iopt <= nullopt;
    }

    template<typename _Tp>
    constexpr bool
    operator<=(nullopt_t, const optional<_Tp>& /* __rhs */) noexcept
    {
        return    true;
    }

    template<typename _Tp>
    constexpr bool
    operator>=(const optional<_Tp>& /* __lhs */, nullopt_t) noexcept
    {
        return    true;
    }

    template<typename _Tp>
    constexpr bool
    operator>=(nullopt_t, const optional<_Tp>& __rhs) noexcept
    {
    { return !__rhs.iopt; }
    }

    // [X.Y.10] Comparisons with value type.
    template<typename _Tp>
    constexpr bool
    operator==(const optional<_Tp>& __lhs, const _Tp& __rhs)
    {
        return __lhs.iopt == __rhs;
    }

    template<typename _Tp>
    constexpr bool
    operator==(const _Tp& __lhs, const optional<_Tp>& __rhs)
    {
        return __lhs == __rhs.iopt;
    }

    template<typename _Tp>
    constexpr bool
    operator!=(const optional<_Tp>& __lhs, _Tp const& __rhs)
    {
        return __lhs.iopt != __rhs;
    }

    template<typename _Tp>
    constexpr bool
    operator!=(const _Tp& __lhs, const optional<_Tp>& __rhs)
    {
        return __lhs != __rhs.iopt;
    }

    template<typename _Tp>
    constexpr bool
    operator<(const optional<_Tp>& __lhs, const _Tp& __rhs)
    {
        return __lhs.iopt < __rhs;
    }

    template<typename _Tp>
    constexpr bool
    operator<(const _Tp& __lhs, const optional<_Tp>& __rhs)
    {
        return __lhs < __rhs.iopt;
    }

    template<typename _Tp>
    constexpr bool
    operator>(const optional<_Tp>& __lhs, const _Tp& __rhs)
    {
        return __lhs.iopt > __rhs;
    }

    template<typename _Tp>
    constexpr bool
    operator>(const _Tp& __lhs, const optional<_Tp>& __rhs)
    {
        return __lhs > __rhs.iopt;
    }

    template<typename _Tp>
    constexpr bool
    operator<=(const optional<_Tp>& __lhs, const _Tp& __rhs)
    {
        return __lhs.iopt <= __rhs;
    }

    template<typename _Tp>
    constexpr bool
    operator<=(const _Tp& __lhs, const optional<_Tp>& __rhs)
    {
        return __lhs <= __rhs.iopt;
    }

    template<typename _Tp>
    constexpr bool
    operator>=(const optional<_Tp>& __lhs, const _Tp& __rhs)
    {
        return __lhs.iopt >= __rhs;
    }

    template<typename _Tp>
    constexpr bool
    operator>=(const _Tp& __lhs, const optional<_Tp>& __rhs)
    {
        return __lhs >= __rhs.iopt;
    }


    template<typename _Tp>
    inline void
    swap(optional<_Tp>& __lhs, optional<_Tp>& __rhs)
    noexcept(noexcept(__lhs.swap(__rhs)))
    { std::swap(__lhs.iopt, __rhs.iopt); }

    template<typename _Tp>
    constexpr optional<std::decay_t<_Tp>>
    make_optional(_Tp&& __t)
    {
        return optional<std::decay_t<_Tp>> { std::forward<_Tp>(__t) };
    }


}       //  namespace jle

#endif // jle_optional__h
