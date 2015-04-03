#include <memory>
#include <cstddef>
#include <exception>

/**
 *     \example  ./support/shared_ptr.cpp
 *     \example  ./support/weak_ptr.cpp
 */



#ifndef JLT_SHARED_PTR_H
#define JLT_SHARED_PTR_H 1

namespace  jle
{


template<typename _Tp>
class weak_ptr;


/**
    @brief  A safe std smart pointer WRAPPER.

    It's almost equal to std::shared_ptr but...
    has the method exprired and if you try to access
    to noninitializated pointer, an exception will be
    throw

    operator bool has also been removed

    _No external dependencies_

    Example:
    \include ./support/shared_ptr.cpp
*/

template<typename _Tp, std::_Lock_policy _Lp = std::__default_lock_policy>
class shared_ptr
{
private:
    std::shared_ptr<_Tp>   ptr;

public:

    /**
    *  @brief  Construct an empty %shared_ptr.
    *  @post   use_count()==0 && get()==0
    */
    constexpr shared_ptr() /*noexcept*/
    :  ptr() {}

    shared_ptr(const shared_ptr&) /*noexcept*/ = default;

    /**
    *  @brief  Construct a %shared_ptr that owns the pointer @a __p.
    *  @param  __p  A pointer that is convertible to element_type*.
    *  @post   use_count() == 1 && get() == __p
    *  @throw  std::bad_alloc, in which case @c delete @a __p is called.
    */
    template<typename _Tp1>
    explicit shared_ptr(_Tp1* __p)
    : ptr(__p) {}


      /**
       *  @brief  Construct a %shared_ptr that owns the pointer @a __p
       *          and the deleter @a __d.
       *  @param  __p  A pointer.
       *  @param  __d  A deleter.
       *  @post   use_count() == 1 && get() == __p
       *  @throw  std::bad_alloc, in which case @a __d(__p) is called.
       *
       *  Requirements: _Deleter's copy constructor and destructor must
       *  not throw
       *
       *  __shared_ptr will release __p by calling __d(__p)
       */
        template<typename _Tp1, typename _Deleter>
        shared_ptr(_Tp1* __p, _Deleter __d)
        : ptr(__p, __d) {}


  /**
   *  @brief  Construct a %shared_ptr that owns a null pointer
   *          and the deleter @a __d.
   *  @param  __p  A null pointer constant.
   *  @param  __d  A deleter.
   *  @post   use_count() == 1 && get() == __p
   *  @throw  std::bad_alloc, in which case @a __d(__p) is called.
   *
   *  Requirements: _Deleter's copy constructor and destructor must
   *  not throw
   *
   *  The last owner will call __d(__p)
   */
    template<typename _Deleter>
    shared_ptr(nullptr_t __p, _Deleter __d)
    : ptr(__p, __d) {}

  /**
   *  @brief  Construct a %shared_ptr that owns the pointer @a __p
   *          and the deleter @a __d.
   *  @param  __p  A pointer.
   *  @param  __d  A deleter.
   *  @param  __a  An allocator.
   *  @post   use_count() == 1 && get() == __p
   *  @throw  std::bad_alloc, in which case @a __d(__p) is called.
   *
   *  Requirements: _Deleter's copy constructor and destructor must
   *  not throw _Alloc's copy constructor and destructor must not
   *  throw.
   *
   *  __shared_ptr will release __p by calling __d(__p)
   */
    template<typename _Tp1, typename _Deleter, typename _Alloc>
    shared_ptr(_Tp1* __p, _Deleter __d, _Alloc __a)
    : ptr(__p, __d, __a) {}

  /**
   *  @brief  Construct a %shared_ptr that owns a null pointer
   *          and the deleter @a __d.
   *  @param  __p  A null pointer constant.
   *  @param  __d  A deleter.
   *  @param  __a  An allocator.
   *  @post   use_count() == 1 && get() == __p
   *  @throw  std::bad_alloc, in which case @a __d(__p) is called.
   *
   *  Requirements: _Deleter's copy constructor and destructor must
   *  not throw _Alloc's copy constructor and destructor must not
   *  throw.
   *
   *  The last owner will call __d(__p)
   */
    template<typename _Deleter, typename _Alloc>
    shared_ptr(nullptr_t __p, _Deleter __d, _Alloc __a)
    : ptr(__p, __d, __a) {}


//  /**
//   *  @brief  Constructs a %shared_ptr instance that stores @a __p
//   *          and shares ownership with @a __r.
//   *  @param  __r  A %shared_ptr.
//   *  @param  __p  A pointer that will remain valid while @a *__r is valid.
//   *  @post   get() == __p && use_count() == __r.use_count()
//   *
//   *  This can be used to construct a @c shared_ptr to a sub-object
//   *  of an object managed by an existing @c shared_ptr.
//   *
//   * @code
//   * shared_ptr< pair<int,int> > pii(new pair<int,int>());
//   * shared_ptr<int> pi(pii, &pii->first);
//   * assert(pii.use_count() == 2);
//   * @endcode
//   */
//    template<typename _Tp1>
//    shared_ptr(const shared_ptr<_Tp1>& __r, _Tp* __p) /*noexcept*/
//    : PENDING


  /**
   *  @brief  If @a __r is empty, constructs an empty %shared_ptr;
   *          otherwise construct a %shared_ptr that shares ownership
   *          with @a __r.
   *  @param  __r  A %shared_ptr.
   *  @post   get() == __r.get() && use_count() == __r.use_count()
   */
    template<typename _Tp1, typename = typename
       std::enable_if<std::is_convertible<_Tp1*, _Tp*>::value>::type>
    shared_ptr(const shared_ptr<_Tp1>& __r) /*noexcept*/
    : ptr(__r.ptr) {}

  /**
   *  @brief  Move-constructs a %shared_ptr instance from @a __r.
   *  @param  __r  A %shared_ptr rvalue.
   *  @post   *this contains the old value of @a __r, @a __r is empty.
   */
    shared_ptr(shared_ptr&& __r) /*noexcept*/
    : ptr(__r.ptr) {}


  /**
   *  @brief  Move-constructs a %shared_ptr instance from @a __r.
   *  @param  __r  A %shared_ptr rvalue.
   *  @post   *this contains the old value of @a __r, @a __r is empty.
   */
    explicit shared_ptr(std::shared_ptr<_Tp>&& __ptr) /*noexcept*/
    : ptr(__ptr) {}

  /**
   *  @brief  Move-constructs a %shared_ptr instance from @a __r.
   *  @param  __r  A %shared_ptr rvalue.
   *  @post   *this contains the old value of @a __r, @a __r is empty.
   */
    template<typename _Tp1, typename = typename
       std::enable_if<std::is_convertible<_Tp1*, _Tp*>::value>::type>
    shared_ptr(shared_ptr<_Tp1>&& __r) /*noexcept*/
    : ptr(__r.ptr) {}


  /**
   *  @brief  Constructs a %shared_ptr that shares ownership with @a __r
   *          and stores a copy of the pointer stored in @a __r.
   *  @param  __r  A weak_ptr.
   *  @post   use_count() == __r.use_count()
   *  @throw  bad_weak_ptr when __r.expired(),
   *          in which case the constructor has no effect.
   */
    template<typename _Tp1>
    explicit shared_ptr(const weak_ptr<_Tp1>& __r)
    : ptr(__r.ptr) {}

    template<typename _Tp1, typename _Del>
    shared_ptr(std::unique_ptr<_Tp1, _Del>&& __r)
    : ptr(__r.ptr) {}

  /**
   *  @brief  Construct an empty %shared_ptr.
   *  @param  __p  A null pointer constant.
   *  @post   use_count() == 0 && get() == nullptr
   */
    constexpr shared_ptr(nullptr_t __p) /*noexcept*/
    : ptr(__p){}

    explicit shared_ptr(const std::shared_ptr<_Tp>&  _ptr)
    :  ptr(_ptr) {}

    shared_ptr& operator=(const shared_ptr&) /*noexcept*/ = default;

    template<typename _Tp1>
    shared_ptr&
    operator=(const shared_ptr<_Tp1>& __r) /*noexcept*/
    {
        ptr=__r;
        return *this;
    }


    shared_ptr&
    operator=(shared_ptr&& __r) /*noexcept*/
    {
        ptr =__r.ptr;
        return *this;
    }

    template<class _Tp1>
    shared_ptr&
    operator=(shared_ptr<_Tp1>&& __r) /*noexcept*/
    {
        ptr = __r.ptr;
        return *this;
    }

    template<typename _Tp1, typename _Del>
    shared_ptr&
    operator=(std::unique_ptr<_Tp1, _Del>&& __r)
    {
        ptr = __r.ptr;
        return *this;
    }








        void
        reset() /*noexcept*/
        { ptr.reset(); }

        template<typename _Tp1>
        void
        reset(_Tp1* __p) // _Tp1 must be complete.
        { ptr.reset(__p); }

        template<typename _Tp1, typename _Deleter>
        void
        reset(_Tp1* __p, _Deleter __d)
        { ptr.reset(__p, __d); }

        template<typename _Tp1, typename _Deleter, typename _Alloc>
        void
        reset(_Tp1* __p, _Deleter __d, _Alloc __a)
        { ptr.reset(__p, __d, __a); }


        // Allow class instantiation when _Tp is [cv-qual] void.
        typename std::add_lvalue_reference<_Tp>::type
        operator*() const
        {
            check_ptr();
            return *ptr;
        }

        _Tp*
        operator->() const
        {
            check_ptr();
            return ptr.get();
        }


        bool expired(void) const {
            return ((ptr.get()==nullptr) ? true: false);
        }

        _Tp*
        get() const
        {
            check_ptr();
            return ptr.get();
        }

//        explicit operator bool() const // never throws
//        { return _M_ptr == 0 ? false : true; }
//
        bool
        unique() const /*noexcept*/
        { return ptr.unique(); }

        long
        use_count() const /*noexcept*/
        { return ptr.use_count(); }

        void
        swap(std::__shared_ptr<_Tp, _Lp>& __other) /*noexcept*/
        {
            ptr.swap(__other);
        }

        template<typename _Tp1>
        bool
        owner_before(std::__shared_ptr<_Tp1, _Lp> const& __rhs) const
        { return ptr.owner_before(__rhs); }

        template<typename _Tp1>
        bool
        owner_before(std::__weak_ptr<_Tp1, _Lp> const& __rhs) const
        { return ptr.owner_before(__rhs); }

//    protected:
//        // This constructor is non-standard, it is used by allocate_shared.
//        template<typename _Alloc, typename... _Args>
//        __shared_ptr(std::_Sp_make_shared_tag __tag, const _Alloc& __a,
//             _Args&&... __args)
//        : ptr(__a, __args... {}
//
//        template<typename _Tp1, std::_Lock_policy _Lp1, typename _Alloc,
//        typename... _Args>
//        friend __shared_ptr<_Tp1, _Lp1>
//        __allocate_shared(const _Alloc& __a, _Args&&... __args);
//
//        // This constructor is used by __weak_ptr::lock() and
//        // shared_ptr::shared_ptr(const weak_ptr&, std::nothrow_t).
//        __shared_ptr(const __weak_ptr<_Tp, _Lp>& __r, std::nothrow_t)
//        : ptr(__r, std::nothrow) {}
//
//      friend class __weak_ptr<_Tp, _Lp>;
//
//    private:
//      void*
//      _M_get_deleter(const std::type_info& __ti) const /*noexcept*/
//      { return ptr._M_get_deleter(__ti); }
//
//      template<typename _Tp1>
//	static _Tp1*
//	_S_raw_ptr(_Tp1* __ptr)
//	{ return __ptr; }
//
//      template<typename _Tp1>
//	static auto
//	_S_raw_ptr(_Tp1 __ptr) -> decltype(std::__addressof(*__ptr))
//	{ return std::__addressof(*__ptr); }
//
//      template<typename _Tp1, _Lock_policy _Lp1> friend class __shared_ptr;
//      template<typename _Tp1, _Lock_policy _Lp1> friend class __weak_ptr;
//
//      template<typename _Del, typename _Tp1, _Lock_policy _Lp1>
//	friend _Del* get_deleter(const __shared_ptr<_Tp1, _Lp1>&) /*noexcept*/;
//
//      _Tp*	   	   _M_ptr;         // Contained pointer.
//      __shared_count<_Lp>  _M_refcount;    // Reference counter.
//    };



private:
    void check_ptr(void) const
    {
        if (expired())
            throw std::runtime_error("shared_ptr pointer not valid");
    }

  // This constructor is non-standard, it is used by allocate_shared.
    template<typename _Alloc, typename... _Args>
    shared_ptr(std::_Sp_make_shared_tag __tag, const _Alloc& __a,
       _Args&&... __args)
    : ptr(__tag, __a, __args...) {}

    template<typename _Tp1, typename _Alloc, typename... _Args>
    friend shared_ptr<_Tp1>
    allocate_shared(const _Alloc& __a, _Args&&... __args);

    // This constructor is non-standard, it is used by weak_ptr::lock().
    shared_ptr(const jle::weak_ptr<_Tp>& __r, std::nothrow_t)
    : ptr(__r, std::nothrow) {}

    friend class jle::weak_ptr<_Tp>;
};

// 20.7.2.2.7 shared_ptr comparisons
template<typename _Tp1, typename _Tp2>
inline bool
operator==(const shared_ptr<_Tp1>& __a,
       const shared_ptr<_Tp2>& __b) /*noexcept*/
{ return __a.get() == __b.get(); }

template<typename _Tp>
inline bool
operator==(const shared_ptr<_Tp>& __a, nullptr_t) /*noexcept*/
{ return !__a; }

template<typename _Tp>
inline bool
operator==(nullptr_t, const shared_ptr<_Tp>& __a) /*noexcept*/
{ return !__a; }

template<typename _Tp1, typename _Tp2>
inline bool
operator!=(const shared_ptr<_Tp1>& __a,
       const shared_ptr<_Tp2>& __b) /*noexcept*/
{ return __a.get() != __b.get(); }

template<typename _Tp>
inline bool
operator!=(const shared_ptr<_Tp>& __a, nullptr_t) /*noexcept*/
{ return (bool)__a; }

template<typename _Tp>
inline bool
operator!=(nullptr_t, const shared_ptr<_Tp>& __a) /*noexcept*/
{ return (bool)__a; }

template<typename _Tp1, typename _Tp2>
inline bool
operator<(const shared_ptr<_Tp1>& __a,
      const shared_ptr<_Tp2>& __b) /*noexcept*/
{
  typedef typename std::common_type<_Tp1*, _Tp2*>::type _CT;
  return std::less<_CT>()(__a.get(), __b.get());
}

template<typename _Tp>
inline bool
operator<(const shared_ptr<_Tp>& __a, nullptr_t) /*noexcept*/
{ return std::less<_Tp*>()(__a.get(), nullptr); }

template<typename _Tp>
inline bool
operator<(nullptr_t, const shared_ptr<_Tp>& __a) /*noexcept*/
{ return std::less<_Tp*>()(nullptr, __a.get()); }

template<typename _Tp1, typename _Tp2>
inline bool
operator<=(const shared_ptr<_Tp1>& __a,
       const shared_ptr<_Tp2>& __b) /*noexcept*/
{ return !(__b < __a); }

template<typename _Tp>
inline bool
operator<=(const shared_ptr<_Tp>& __a, nullptr_t) /*noexcept*/
{ return !(nullptr < __a); }

template<typename _Tp>
inline bool
operator<=(nullptr_t, const shared_ptr<_Tp>& __a) /*noexcept*/
{ return !(__a < nullptr); }

template<typename _Tp1, typename _Tp2>
inline bool
operator>(const shared_ptr<_Tp1>& __a,
      const shared_ptr<_Tp2>& __b) /*noexcept*/
{ return (__b < __a); }

template<typename _Tp>
inline bool
operator>(const shared_ptr<_Tp>& __a, nullptr_t) /*noexcept*/
{ return std::less<_Tp*>()(nullptr, __a.get()); }

template<typename _Tp>
inline bool
operator>(nullptr_t, const shared_ptr<_Tp>& __a) /*noexcept*/
{ return std::less<_Tp*>()(__a.get(), nullptr); }

template<typename _Tp1, typename _Tp2>
inline bool
operator>=(const shared_ptr<_Tp1>& __a,
       const shared_ptr<_Tp2>& __b) /*noexcept*/
{ return !(__a < __b); }

template<typename _Tp>
inline bool
operator>=(const shared_ptr<_Tp>& __a, nullptr_t) /*noexcept*/
{ return !(__a < nullptr); }

template<typename _Tp>
inline bool
operator>=(nullptr_t, const shared_ptr<_Tp>& __a) /*noexcept*/
{ return !(nullptr < __a); }

//template<typename _Tp>
//struct less<shared_ptr<_Tp>> : public std::_Sp_less<shared_ptr<_Tp>>
//{ };

// 20.7.2.2.8 shared_ptr specialized algorithms.
template<typename _Tp>
inline void
swap(shared_ptr<_Tp>& __a, shared_ptr<_Tp>& __b) /*noexcept*/
{ __a.swap(__b); }

// 20.7.2.2.9 shared_ptr casts.
template<typename _Tp, typename _Tp1>
inline shared_ptr<_Tp>
static_pointer_cast(const shared_ptr<_Tp1>& __r) /*noexcept*/
{ return shared_ptr<_Tp>(__r, static_cast<_Tp*>(__r.get())); }

template<typename _Tp, typename _Tp1>
inline shared_ptr<_Tp>
const_pointer_cast(const shared_ptr<_Tp1>& __r) /*noexcept*/
{ return shared_ptr<_Tp>(__r, const_cast<_Tp*>(__r.get())); }

template<typename _Tp, typename _Tp1>
inline shared_ptr<_Tp>
dynamic_pointer_cast(const shared_ptr<_Tp1>& __r) /*noexcept*/
{
  if (_Tp* __p = dynamic_cast<_Tp*>(__r.get()))
return shared_ptr<_Tp>(__r, __p);
  return shared_ptr<_Tp>();
}


/**
    @brief  A safe std weak_ptr WRAPPER.

    It's almost equal to std::weak_ptr but lock will return a jle::shared_ptr


    _No external dependencies_


    Example...

    @include ./support/weak_ptr.cpp

*/
/**
*  @brief  A smart pointer with weak semantics.
*
*  With forwarding constructors and assignment operators.
*/
template<typename _Tp>
class weak_ptr
{
private:
    std::weak_ptr<_Tp>  ptr;
    void check_ptr(void) const
    {
        if (expired())
            throw std::runtime_error("shared_ptr pointer not valid");
    }

public:
    constexpr weak_ptr() /*noexcept*/
    : ptr() { }

    template<typename _Tp1, typename = typename
    std::enable_if<std::is_convertible<_Tp1*, _Tp*>::value>::type>
    weak_ptr(const weak_ptr<_Tp1>& __r) /*noexcept*/
    : ptr(__r) { }

    template<typename _Tp1, typename = typename
    std::enable_if<std::is_convertible<_Tp1*, _Tp*>::value>::type>
    weak_ptr(const shared_ptr<_Tp1>& __r) /*noexcept*/
    : ptr(__r.ptr) { }

    template<typename _Tp1>
    weak_ptr&
    operator=(const weak_ptr<_Tp1>& __r) /*noexcept*/
    {
        ptr=__r;
        return *this;
    }

    template<typename _Tp1>
    weak_ptr&
    operator=(const jle::shared_ptr<_Tp1>& __r) /*noexcept*/
    {
        ptr=__r.ptr;
        return *this;
    }

    shared_ptr<_Tp>
    lock() const
    {
        if(expired())
            return shared_ptr<_Tp>();
        else
        {
            return jle::shared_ptr<_Tp>(ptr.lock());
        }
    }

    bool expired(void) const {
        return ptr.expired();
    }

};

// 20.7.2.3.6 weak_ptr specialized algorithms.
template<typename _Tp>
inline void
swap(weak_ptr<_Tp>& __a, weak_ptr<_Tp>& __b) /*noexcept*/
{ __a.swap(__b); }


/// Primary template owner_less
template<typename _Tp>
struct owner_less;

/// Partial specialization of owner_less for shared_ptr.
template<typename _Tp>
struct owner_less<shared_ptr<_Tp>>
: public std::_Sp_owner_less<shared_ptr<_Tp>, weak_ptr<_Tp>>
{ };

/// Partial specialization of owner_less for weak_ptr.
template<typename _Tp>
struct owner_less<weak_ptr<_Tp>>
: public std::_Sp_owner_less<weak_ptr<_Tp>, shared_ptr<_Tp>>
{ };

/**
*  @brief Base class allowing use of member function shared_from_this.
*/
template<typename _Tp>
class enable_shared_from_this
{
protected:
  constexpr enable_shared_from_this() /*noexcept*/ { }

  enable_shared_from_this(const enable_shared_from_this&) /*noexcept*/ { }

  enable_shared_from_this&
  operator=(const enable_shared_from_this&) /*noexcept*/
  { return *this; }

  ~enable_shared_from_this() { }

public:
  shared_ptr<_Tp>
  shared_from_this()
  { return shared_ptr<_Tp>(this->_M_weak_this); }

  shared_ptr<const _Tp>
  shared_from_this() const
  { return shared_ptr<const _Tp>(this->_M_weak_this); }

private:
  template<typename _Tp1>
void
_M_weak_assign(_Tp1* __p, const std::__shared_count<>& __n) const /*noexcept*/
{ _M_weak_this._M_assign(__p, __n); }

  template<typename _Tp1>
friend void
__enable_shared_from_this_helper(const std::__shared_count<>& __pn,
                 const enable_shared_from_this* __pe,
                 const _Tp1* __px) /*noexcept*/
{
  if (__pe != 0)
    __pe->_M_weak_assign(const_cast<_Tp1*>(__px), __pn);
}

  mutable weak_ptr<_Tp>  _M_weak_this;
};

/**
*  @brief  Create an object that is owned by a shared_ptr.
*  @param  __a     An allocator.
*  @param  __args  Arguments for the @a _Tp object's constructor.
*  @return A shared_ptr that owns the newly created object.
*  @throw  An exception thrown from @a _Alloc::allocate or from the
*          constructor of @a _Tp.
*
*  A copy of @a __a will be used to allocate memory for the shared_ptr
*  and the new object.
*/
template<typename _Tp, typename _Alloc, typename... _Args>
inline shared_ptr<_Tp>
allocate_shared(const _Alloc& __a, _Args&&... __args)
{
  return shared_ptr<_Tp>(std::_Sp_make_shared_tag(), __a,
             std::forward<_Args>(__args)...);
}

/**
*  @brief  Create an object that is owned by a shared_ptr.
*  @param  __args  Arguments for the @a _Tp object's constructor.
*  @return A shared_ptr that owns the newly created object.
*  @throw  std::bad_alloc, or an exception thrown from the
*          constructor of @a _Tp.
*/
template<typename _Tp, typename... _Args>
inline shared_ptr<_Tp>
make_shared(_Args&&... __args)
{
    std::shared_ptr<_Tp> ptr = std::make_shared<_Tp>(__args...);
    return jle::shared_ptr<_Tp>((ptr));
}
//template<typename _Tp>
//inline jle::shared_ptr<_Tp>
//make_shared(_Tp _arg)
//{
//    std::shared_ptr<_Tp> ptr = std::make_shared<_Tp>(_arg);
//    return jle::shared_ptr<_Tp>((ptr));
//}


//template<typename _Tp, typename... _Args>
//inline jle::shared_ptr<_Tp>
//make_shared(_Args&&... __args)
//{
//    std::shared_ptr<_Tp> ptr = std::make_shared<_Tp, _Args...>(__args...);
//    return jle::shared_ptr<_Tp>((ptr));
//}


};  //  namespace  jle

#endif // JLT_SHARED_PTR_H
