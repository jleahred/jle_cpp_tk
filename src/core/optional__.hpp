// <optional> -*- C++ -*-

// Copyright (C) 2013-2014 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file experimental/optional
 *  This is a TS C++ Library header.
 */


/**
 * @defgroup experimental Experimental
 *
 * Components specified by various Technical Specifications.
 */

#if __cplusplus <= 201103L
# include <bits/c++14_warning.h>
#else

#include <utility>
#include <type_traits>
#include <stdexcept>
#include <new>
#include <initializer_list>
#include <bits/functexcept.h>
#include <bits/functional_hash.h>
#include <bits/enable_special_members.h>

namespace jle //_GLIBCXX_VISIBILITY(default)
{
    using namespace std;
//namespace jle
//{
//_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   * @defgroup optional Optional values
   * @ingroup experimental
   *
   * Class template for optional values and surrounding facilities, as
   * described in n3793 "A proposal to add a utility class to represent
   * optional objects (Revision 5)".
   *
   * @{
   */

  // All subsequent [X.Y.n] references are against n3793.

  // [X.Y.4]
  template<typename _Tp>
    class optional;

  // [X.Y.5]
  /// Tag type for in-place construction.
  struct in_place_t { };

  /// Tag for in-place construction.
  constexpr in_place_t in_place { };

  // [X.Y.6]
  /// Tag type to disengage optional objects.
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

  // [X.Y.6]
  /// Tag to disengage optional objects.
  constexpr nullopt_t nullopt { nullopt_t::_Construct::_Token };

  // [X.Y.7]
  /**
   *  @brief Exception class thrown when a disengaged optional object is
   *  dereferenced.
   *  @ingroup exceptions
   */
  class bad_optional_access : public logic_error
  {
  public:
    // XXX Should not be inline
    explicit bad_optional_access(const string& __arg) : logic_error(__arg) { }

    explicit bad_optional_access(const char* __arg) : logic_error(__arg) { }

    virtual ~bad_optional_access() noexcept = default;
  };

  void
  __throw_bad_optional_access(const char*)
  __attribute__((__noreturn__));

  // XXX Does not belong here.
  inline void
  __throw_bad_optional_access(const char* __s)
  { _GLIBCXX_THROW_OR_ABORT(bad_optional_access(__s)); }

  template<typename _Tp, typename _Sfinae = void>
    struct _Has_addressof_impl : std::false_type { };

  template<typename _Tp>
    struct _Has_addressof_impl<_Tp,
      decltype( std::declval<const _Tp&>().operator&(), void() )>
      : std::true_type { };

  /**
    * @brief Trait that detects the presence of an overloaded unary operator&.
    *
    * Practically speaking this detects the presence of such an operator when
    * called on a const-qualified lvalue (i.e.
    * declval<_Tp * const&>().operator&()).
    */
  template<typename _Tp>
    struct _Has_addressof : _Has_addressof_impl<_Tp>::type { };

  /**
    * @brief An overload that attempts to take the address of an lvalue as a
    * constant expression. Falls back to __addressof in the presence of an
    * overloaded addressof operator (unary operator&), in which case the call
    * will not be a constant expression.
    */
  template<typename _Tp, typename enable_if<!_Has_addressof<_Tp>::value,
                                            int>::type...>
    constexpr _Tp* __constexpr_addressof(_Tp& __t)
    { return &__t; }

  /**
    * @brief Fallback overload that defers to __addressof.
    */
  template<typename _Tp, typename enable_if<_Has_addressof<_Tp>::value,
                                            int>::type...>
    inline _Tp* __constexpr_addressof(_Tp& __t)
    { return std::__addressof(__t); }

  /**
    * @brief Class template that holds the necessary state for @ref optional
    * and that has the responsibility for construction and the special members.
    *
    * Such a separate base class template is necessary in order to
    * conditionally enable the special members (e.g. copy/move constructors).
    * Note that this means that @ref _Optional_base implements the
    * functionality for copy and move assignment, but not for converting
    * assignment.
    *
    * @see optional, _Enable_special_members
    */
  template<typename _Tp, bool _ShouldProvideDestructor =
	   !is_trivially_destructible<_Tp>::value>
    class _Optional_base
    {
    private:
      // Remove const to avoid prohibition of reusing object storage for
      // const-qualified types in [3.8/9]. This is strictly internal
      // and even optional itself is oblivious to it.
      using _Stored_type = typename remove_const<_Tp>::type;

    public:
      // [X.Y.4.1] Constructors.

      // Constructors for disengaged optionals.
      constexpr _Optional_base() noexcept
      : _M_empty{} { }

      constexpr _Optional_base(nullopt_t) noexcept
      : _Optional_base{} { }

      // Constructors for engaged optionals.
      constexpr _Optional_base(const _Tp& __t)
      : _M_payload(__t), _M_engaged(true) { }

      constexpr _Optional_base(_Tp&& __t)
      : _M_payload(std::move(__t)), _M_engaged(true) { }

      template<typename... _Args>
        constexpr explicit _Optional_base(in_place_t, _Args&&... __args)
        : _M_payload(std::forward<_Args>(__args)...), _M_engaged(true) { }

      template<typename _Up, typename... _Args,
               typename enable_if<is_constructible<_Tp,
                                                   initializer_list<_Up>&,
                                                   _Args&&...>::value,
                                  int>::type...>
        constexpr explicit _Optional_base(in_place_t,
                                          initializer_list<_Up> __il,
                                          _Args&&... __args)
        : _M_payload(__il, std::forward<_Args>(__args)...),
          _M_engaged(true) { }

      // Copy and move constructors.
      _Optional_base(const _Optional_base& __other)
      {
        if (__other._M_engaged)
          this->_M_construct(__other._M_get());
      }

      _Optional_base(_Optional_base&& __other)
      noexcept(is_nothrow_move_constructible<_Tp>())
      {
        if (__other._M_engaged)
          this->_M_construct(std::move(__other._M_get()));
      }

      // [X.Y.4.3] (partly) Assignment.
      _Optional_base&
      operator=(const _Optional_base& __other)
      {
        if (this->_M_engaged && __other._M_engaged)
          this->_M_get() = __other._M_get();
        else
	  {
	    if (__other._M_engaged)
	      this->_M_construct(__other._M_get());
	    else
	      this->_M_reset();
	  }

        return *this;
      }

      _Optional_base&
      operator=(_Optional_base&& __other)
      noexcept(__and_<is_nothrow_move_constructible<_Tp>,
		      is_nothrow_move_assignable<_Tp>>())
      {
	if (this->_M_engaged && __other._M_engaged)
	  this->_M_get() = std::move(__other._M_get());
	else
	  {
	    if (__other._M_engaged)
	      this->_M_construct(std::move(__other._M_get()));
	    else
	      this->_M_reset();
	  }
	return *this;
      }

      // [X.Y.4.2] Destructor.
      ~_Optional_base()
      {
        if (this->_M_engaged)
          this->_M_payload.~_Stored_type();
      }

      // The following functionality is also needed by optional, hence the
      // protected accessibility.
    protected:
      constexpr bool _M_is_engaged() const noexcept
      { return this->_M_engaged; }

      // The _M_get operations have _M_engaged as a precondition.
      _Tp&
      _M_get() noexcept
      { return _M_payload; }

      constexpr const _Tp&
      _M_get() const noexcept
      { return _M_payload; }

      // The _M_construct operation has !_M_engaged as a precondition
      // while _M_destruct has _M_engaged as a precondition.
      template<typename... _Args>
        void
        _M_construct(_Args&&... __args)
        noexcept(is_nothrow_constructible<_Stored_type, _Args...>())
        {
          ::new (std::__addressof(this->_M_payload))
            _Stored_type(std::forward<_Args>(__args)...);
          this->_M_engaged = true;
        }

      void
      _M_destruct()
      {
        this->_M_engaged = false;
        this->_M_payload.~_Stored_type();
      }

      // _M_reset is a 'safe' operation with no precondition.
      void
      _M_reset()
      {
        if (this->_M_engaged)
          this->_M_destruct();
      }

    private:
      struct _Empty_byte { };
      union {
          _Empty_byte _M_empty;
          _Stored_type _M_payload;
      };
      bool _M_engaged = false;
    };

  /// Partial specialization that is exactly identical to the primary template
  /// save for not providing a destructor, to fulfill triviality requirements.
  template<typename _Tp>
    class _Optional_base<_Tp, false>
    {
    private:
      using _Stored_type = typename remove_const<_Tp>::type;

    public:
      constexpr _Optional_base() noexcept
      : _M_empty{} { }

      constexpr _Optional_base(nullopt_t) noexcept
      : _Optional_base{} { }

      constexpr _Optional_base(const _Tp& __t)
      : _M_payload(__t), _M_engaged(true) { }

      constexpr _Optional_base(_Tp&& __t)
      : _M_payload(std::move(__t)), _M_engaged(true) { }

      template<typename... _Args>
        constexpr explicit _Optional_base(in_place_t, _Args&&... __args)
        : _M_payload(std::forward<_Args>(__args)...), _M_engaged(true) { }

      template<typename _Up, typename... _Args,
               typename enable_if<is_constructible<_Tp,
                                                   initializer_list<_Up>&,
                                                   _Args&&...>::value,
                                  int>::type...>
        constexpr explicit _Optional_base(in_place_t,
                                          initializer_list<_Up> __il,
                                          _Args&&... __args)
        : _M_payload(__il, std::forward<_Args>(__args)...),
          _M_engaged(true) { }

      _Optional_base(const _Optional_base& __other)
      {
        if (__other._M_engaged)
          this->_M_construct(__other._M_get());
      }

      _Optional_base(_Optional_base&& __other)
      noexcept(is_nothrow_move_constructible<_Tp>())
      {
        if (__other._M_engaged)
          this->_M_construct(std::move(__other._M_get()));
      }

      _Optional_base&
      operator=(const _Optional_base& __other)
      {
	if (this->_M_engaged && __other._M_engaged)
	  this->_M_get() = __other._M_get();
	else
	  {
	    if (__other._M_engaged)
	      this->_M_construct(__other._M_get());
	    else
	      this->_M_reset();
	  }
	return *this;
      }

      _Optional_base&
      operator=(_Optional_base&& __other)
      noexcept(__and_<is_nothrow_move_constructible<_Tp>,
		      is_nothrow_move_assignable<_Tp>>())
      {
	if (this->_M_engaged && __other._M_engaged)
	  this->_M_get() = std::move(__other._M_get());
	else
	  {
	    if (__other._M_engaged)
	      this->_M_construct(std::move(__other._M_get()));
	    else
	      this->_M_reset();
	  }
	return *this;
      }

      // Sole difference
      // ~_Optional_base() noexcept = default;

    protected:
      constexpr bool _M_is_engaged() const noexcept
      { return this->_M_engaged; }

      _Tp&
      _M_get() noexcept
      { return _M_payload; }

      constexpr const _Tp&
      _M_get() const noexcept
      { return _M_payload; }

      template<typename... _Args>
        void
        _M_construct(_Args&&... __args)
        noexcept(is_nothrow_constructible<_Stored_type, _Args...>())
        {
          ::new (std::__addressof(this->_M_payload))
            _Stored_type(std::forward<_Args>(__args)...);
          this->_M_engaged = true;
        }

      void
      _M_destruct()
      {
        this->_M_engaged = false;
        this->_M_payload.~_Stored_type();
      }

      void
      _M_reset()
      {
        if (this->_M_engaged)
          this->_M_destruct();
      }

    private:
      struct _Empty_byte { };
      union
      {
	_Empty_byte _M_empty;
	_Stored_type _M_payload;
      };
      bool _M_engaged = false;
    };

  /**
    * @brief Class template for optional values.
    */
  template<typename _Tp>
    class optional
    : private _Optional_base<_Tp>,
      private _Enable_copy_move<
        // Copy constructor.
        is_copy_constructible<_Tp>::value,
        // Copy assignment.
        __and_<is_copy_constructible<_Tp>, is_copy_assignable<_Tp>>::value,
        // Move constructor.
        is_move_constructible<_Tp>::value,
        // Move assignment.
        __and_<is_move_constructible<_Tp>, is_move_assignable<_Tp>>::value,
        // Unique tag type.
        optional<_Tp>>
    {
      static_assert(__and_<__not_<is_same<typename remove_cv<_Tp>::type,
					  nullopt_t>>,
			   __not_<is_same<typename remove_cv<_Tp>::type,
					  in_place_t>>,
			   __not_<is_reference<_Tp>>>(),
                    "Invalid instantiation of optional<T>");

    private:
      using _Base = _Optional_base<_Tp>;

    public:
      using value_type = _Tp;

      // _Optional_base has the responsibility for construction.
      using _Base::_Base;

      // [X.Y.4.3] (partly) Assignment.
      optional&
      operator=(nullopt_t) noexcept
      {
        this->_M_reset();
        return *this;
      }

      template<typename _Up>
        typename enable_if<
                 is_same<_Tp, typename decay<_Up>::type>::value,
                 optional&
               >::type
        operator=(_Up&& __u)
        {
          static_assert(__and_<is_constructible<_Tp, _Up>,
			       is_assignable<_Tp&, _Up>>(),
                        "Cannot assign to value type from argument");

          if (this->_M_is_engaged())
            this->_M_get() = std::forward<_Up>(__u);
          else
            this->_M_construct(std::forward<_Up>(__u));

          return *this;
        }

      template<typename... _Args>
	void
	emplace(_Args&&... __args)
	{
	  static_assert(is_constructible<_Tp, _Args&&...>(),
			"Cannot emplace value type from arguments");

	  this->_M_reset();
	  this->_M_construct(std::forward<_Args>(__args)...);
	}

      template<typename _Up, typename... _Args>
        typename enable_if<
                 is_constructible<_Tp,
                                  initializer_list<_Up>&,
                                  _Args&&...>::value
               >::type
	emplace(initializer_list<_Up> __il, _Args&&... __args)
	{
	  this->_M_reset();
	  this->_M_construct(__il, std::forward<_Args>(__args)...);
	}

      // [X.Y.4.2] Destructor is implicit, implemented in _Optional_base.

      // [X.Y.4.4] Swap.
      void
      swap(optional& __other)
      noexcept(is_nothrow_move_constructible<_Tp>()
               && noexcept(swap(declval<_Tp&>(), declval<_Tp&>())))
      {
        using std::swap;

        if (this->_M_is_engaged() && __other._M_is_engaged())
          swap(this->_M_get(), __other._M_get());
        else if (this->_M_is_engaged())
	  {
	    __other._M_construct(std::move(this->_M_get()));
	    this->_M_destruct();
	  }
        else if (__other._M_is_engaged())
	  {
	    this->_M_construct(std::move(__other._M_get()));
	    __other._M_destruct();
	  }
      }

      // [X.Y.4.5] Observers.
      constexpr const _Tp*
      operator->() const
      { return __constexpr_addressof(this->_M_get()); }

      _Tp*
      operator->()
      { return std::__addressof(this->_M_get()); }

      constexpr const _Tp&
      operator*() const
      { return this->_M_get(); }

      _Tp&
      operator*()
      { return this->_M_get(); }

      constexpr explicit operator bool() const noexcept
      { return this->_M_is_engaged(); }

      constexpr const _Tp&
      value() const
      {
	return this->_M_is_engaged()
	  ?  this->_M_get()
	  : (__throw_bad_optional_access("Attempt to access value of a "
		                         "disengaged optional object"),
	     this->_M_get());
      }

      _Tp&
      value()
      {
        if (this->_M_is_engaged())
          return this->_M_get();

        __throw_bad_optional_access("Attempt to access value of a "
				    "disengaged optional object");
      }

      template<typename _Up>
	constexpr _Tp
	value_or(_Up&& __u) const&
	{
	  static_assert(__and_<is_copy_constructible<_Tp>,
			       is_convertible<_Up&&, _Tp>>(),
			"Cannot return value");

	  return this->_M_is_engaged()
	    ? this->_M_get()
	    : static_cast<_Tp>(std::forward<_Up>(__u));
	}

      template<typename _Up>
	_Tp
	value_or(_Up&& __u) &&
	{
	  static_assert(__and_<is_move_constructible<_Tp>,
			       is_convertible<_Up&&, _Tp>>(),
			"Cannot return value" );

	  return this->_M_is_engaged()
	    ? std::move(this->_M_get())
	    : static_cast<_Tp>(std::forward<_Up>(__u));
	}
    };

  // [X.Y.8] Comparisons between optional values.
  template<typename _Tp>
    constexpr bool
    operator==(const optional<_Tp>& __lhs, const optional<_Tp>& __rhs)
    {
      return static_cast<bool>(__lhs) == static_cast<bool>(__rhs)
	     && (!__lhs || *__lhs == *__rhs);
    }

  template<typename _Tp>
    constexpr bool
    operator!=(const optional<_Tp>& __lhs, const optional<_Tp>& __rhs)
    { return !(__lhs == __rhs); }

  template<typename _Tp>
    constexpr bool
    operator<(const optional<_Tp>& __lhs, const optional<_Tp>& __rhs)
    {
      return static_cast<bool>(__rhs) && (!__lhs || *__lhs < *__rhs);
    }

  template<typename _Tp>
    constexpr bool
    operator>(const optional<_Tp>& __lhs, const optional<_Tp>& __rhs)
    { return __rhs < __lhs; }

  template<typename _Tp>
    constexpr bool
    operator<=(const optional<_Tp>& __lhs, const optional<_Tp>& __rhs)
    { return !(__rhs < __lhs); }

  template<typename _Tp>
    constexpr bool
    operator>=(const optional<_Tp>& __lhs, const optional<_Tp>& __rhs)
    { return !(__lhs < __rhs); }

  // [X.Y.9] Comparisons with nullopt.
  template<typename _Tp>
    constexpr bool
    operator==(const optional<_Tp>& __lhs, nullopt_t) noexcept
    { return !__lhs; }

  template<typename _Tp>
    constexpr bool
    operator==(nullopt_t, const optional<_Tp>& __rhs) noexcept
    { return !__rhs; }

  template<typename _Tp>
    constexpr bool
    operator!=(const optional<_Tp>& __lhs, nullopt_t) noexcept
    { return static_cast<bool>(__lhs); }

  template<typename _Tp>
    constexpr bool
    operator!=(nullopt_t, const optional<_Tp>& __rhs) noexcept
    { return static_cast<bool>(__rhs); }

  template<typename _Tp>
    constexpr bool
    operator<(const optional<_Tp>& /* __lhs */, nullopt_t) noexcept
    { return false; }

  template<typename _Tp>
    constexpr bool
    operator<(nullopt_t, const optional<_Tp>& __rhs) noexcept
    { return static_cast<bool>(__rhs); }

  template<typename _Tp>
    constexpr bool
    operator>(const optional<_Tp>& __lhs, nullopt_t) noexcept
    { return static_cast<bool>(__lhs); }

  template<typename _Tp>
    constexpr bool
    operator>(nullopt_t, const optional<_Tp>& /* __rhs */) noexcept
    { return false; }

  template<typename _Tp>
    constexpr bool
    operator<=(const optional<_Tp>& __lhs, nullopt_t) noexcept
    { return !__lhs; }

  template<typename _Tp>
    constexpr bool
    operator<=(nullopt_t, const optional<_Tp>& /* __rhs */) noexcept
    { return true; }

  template<typename _Tp>
    constexpr bool
    operator>=(const optional<_Tp>& /* __lhs */, nullopt_t) noexcept
    { return true; }

  template<typename _Tp>
    constexpr bool
    operator>=(nullopt_t, const optional<_Tp>& __rhs) noexcept
    { return !__rhs; }

  // [X.Y.10] Comparisons with value type.
  template<typename _Tp>
    constexpr bool
    operator==(const optional<_Tp>& __lhs, const _Tp& __rhs)
    { return __lhs && *__lhs == __rhs; }

  template<typename _Tp>
    constexpr bool
    operator==(const _Tp& __lhs, const optional<_Tp>& __rhs)
    { return __rhs && __lhs == *__rhs; }

  template<typename _Tp>
    constexpr bool
    operator!=(const optional<_Tp>& __lhs, _Tp const& __rhs)
    { return !__lhs || !(*__lhs == __rhs); }

  template<typename _Tp>
    constexpr bool
    operator!=(const _Tp& __lhs, const optional<_Tp>& __rhs)
    { return !__rhs || !(__lhs == *__rhs); }

  template<typename _Tp>
    constexpr bool
    operator<(const optional<_Tp>& __lhs, const _Tp& __rhs)
    { return !__lhs || *__lhs < __rhs; }

  template<typename _Tp>
    constexpr bool
    operator<(const _Tp& __lhs, const optional<_Tp>& __rhs)
    { return __rhs && __lhs < *__rhs; }

  template<typename _Tp>
    constexpr bool
    operator>(const optional<_Tp>& __lhs, const _Tp& __rhs)
    { return __lhs && __rhs < *__lhs; }

  template<typename _Tp>
    constexpr bool
    operator>(const _Tp& __lhs, const optional<_Tp>& __rhs)
    { return !__rhs || *__rhs < __lhs; }

  template<typename _Tp>
    constexpr bool
    operator<=(const optional<_Tp>& __lhs, const _Tp& __rhs)
    { return !__lhs || !(__rhs < *__lhs); }

  template<typename _Tp>
    constexpr bool
    operator<=(const _Tp& __lhs, const optional<_Tp>& __rhs)
    { return __rhs && !(*__rhs < __lhs); }

  template<typename _Tp>
    constexpr bool
    operator>=(const optional<_Tp>& __lhs, const _Tp& __rhs)
    { return __lhs && !(*__lhs < __rhs); }

  template<typename _Tp>
    constexpr bool
    operator>=(const _Tp& __lhs, const optional<_Tp>& __rhs)
    { return !__rhs || !(__lhs < *__rhs); }

  // [X.Y.11]
  template<typename _Tp>
    inline void
    swap(optional<_Tp>& __lhs, optional<_Tp>& __rhs)
    noexcept(noexcept(__lhs.swap(__rhs)))
    { __lhs.swap(__rhs); }

  template<typename _Tp>
    constexpr optional<typename decay<_Tp>::type>
    make_optional(_Tp&& __t)
    { return optional<typename decay<_Tp>::type> { std::forward<_Tp>(__t) }; }

  // @} group optional
//_GLIBCXX_END_NAMESPACE_VERSION
//}
}   //  namespace jle

//namespace std {
//  // [X.Y.12]
//  template<typename _Tp>
//    struct hash<jle::optional<_Tp>>
//    {
//      using result_type = size_t;
//      using argument_type = jle::optional<_Tp>;
//
//      size_t
//      operator()(const jle::optional<_Tp>& __t) const
//      noexcept(noexcept(hash<_Tp> {}(*__t)))
//      {
//        // We pick an arbitrary hash for disengaged optionals which hopefully
//        // usual values of _Tp won't typically hash to.
//        constexpr size_t __magic_disengaged_hash = static_cast<size_t>(-3333);
//        return __t ? hash<_Tp> {}(*__t) : __magic_disengaged_hash;
//      }
//    };
//}

#endif // C++14

