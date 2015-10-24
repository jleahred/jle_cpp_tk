#ifndef JLE_VECTOR__H___
#define JLE_VECTOR__H___



#include <vector>

#include "core/alarm.h"
#include "container_support.h"



namespace jle {
//---------------------------------------------------------------------------

template<typename T>
class vector
{
public:

        class iterator   {
        public:
            iterator(const iterator&) = default;
            iterator(iterator&&) = default;
            iterator& operator=(const iterator&) = default;
            iterator& operator=(iterator&&) = default;

            friend class jle::vector<T>;
            friend class jle::vector<T>::const_iterator;
            iterator() : registered_owner(0), last_updated_container(0)  {}
            iterator&   operator++();
            iterator    operator++(int);
            bool        operator==(const iterator& it) const;
            bool        operator!=(const iterator& it) const;

            T* operator->(void);
            T& operator* (void);

        private:
            typename std::vector<T>::iterator   iiterator;
            typename std::vector<T>::iterator   i_end_iterator;
            int                                 registered_owner;
            int                                 last_updated_container;

            bool is_valid (void) const;
        };


        class const_iterator  {
        public:
            const_iterator(const const_iterator&) = default;
            const_iterator(const_iterator&&) = default;
            const_iterator& operator=(const const_iterator&) = default;
            const_iterator& operator=(const_iterator&&) = default;

            friend class jle::vector<T>;
            const_iterator() : registered_owner(0), last_updated_container(0)  {}
            const_iterator(const iterator& it) :
                            iiterator (it.iiterator),
                            i_end_iterator (it.i_end_iterator),
                            registered_owner(it.registered_owner),
                            last_updated_container(it.last_updated_container)
                            {
                            };
            const_iterator&     operator=(const iterator& it)
            {
                iiterator = it.iiterator;
                i_end_iterator = it.i_end_iterator;
                registered_owner = it.registered_owner;
                last_updated_container = it.last_updated_container;
                return *this;
            }



            const_iterator&     operator++();
            const_iterator      operator++(int);
            bool                operator==(const const_iterator& it) const;
            bool                operator!=(const const_iterator& it) const;

            const T* operator->(void);
            const T& operator* (void);

        private:
            typename std::vector<T>::const_iterator     iiterator;
            typename std::vector<T>::const_iterator     i_end_iterator;
            int                                         registered_owner;
            int                                         last_updated_container;

            bool is_valid (void) const;
        };



        class reverse_iterator   {
        public:
            reverse_iterator(const reverse_iterator&) = default;
            reverse_iterator(reverse_iterator&&) = default;
            reverse_iterator& operator=(const reverse_iterator&) = default;
            reverse_iterator& operator=(reverse_iterator&&) = default;

            friend class jle::vector<T>;
            friend class jle::vector<T>::const_reverse_iterator;
            reverse_iterator() : registered_owner(0), last_updated_container(0)  {}
            reverse_iterator&   operator++();
            reverse_iterator    operator++(int);
            bool                operator==(const reverse_iterator& it) const;
            bool                operator!=(const reverse_iterator& it) const;

            T* operator->(void);
            T& operator* (void);

        private:
            typename std::vector<T>::reverse_iterator   iiterator;
            typename std::vector<T>::reverse_iterator   i_rend_iterator;
            int                                         registered_owner;
            int                                         last_updated_container;

            bool is_valid (void) const;
        };


        class const_reverse_iterator  {
        public:
            const_reverse_iterator(const const_reverse_iterator&) = default;
            const_reverse_iterator(const_reverse_iterator&&) = default;
            const_reverse_iterator& operator=(const const_reverse_iterator&) = default;
            const_reverse_iterator& operator=(const_reverse_iterator&&) = default;

            friend class jle::vector<T>;
            const_reverse_iterator() : registered_owner(0), last_updated_container(0)  {}
            const_reverse_iterator(const iterator& it) :
                            iiterator (it.iiterator),
                            i_rend_iterator (it.i_rend_iterator),
                            registered_owner(it.registered_owner),
                            last_updated_container(it.last_updated_container)
                            {
                            };
            const_reverse_iterator&     operator=(const reverse_iterator& it)
            {
                iiterator = it.iiterator;
                i_rend_iterator = it.i_rend_iterator;
                registered_owner = it.registered_owner;
                last_updated_container = it.last_updated_container;
                return *this;
            }



            const_reverse_iterator&     operator++();
            const_reverse_iterator      operator++(int);
            bool                        operator==(const const_reverse_iterator& it) const;
            bool                        operator!=(const const_reverse_iterator& it) const;

            const T* operator->(void);
            const T& operator* (void);

        private:
            typename std::vector<T>::const_reverse_iterator   iiterator;
            typename std::vector<T>::const_reverse_iterator   i_rend_iterator;
            int                                               registered_owner;
            int                                               last_updated_container;

            bool is_valid (void) const;
        };





    explicit vector() : registered_as(internal_for_containers::register_container(true))  {};
    explicit vector (std::size_t n, const T& value= T()) :  ivector{n, value}, registered_as(internal_for_containers::register_container(true))  {};
    template <class InputIterator>
    vector ( InputIterator first, InputIterator last) : ivector(first, last), registered_as(internal_for_containers::register_container(true))  {};

    ~vector()
    {
        try{
            internal_for_containers::unregister_container(registered_as);
        } JLE_CATCH_CALLFUNCION(jle::alarm_msg, "exception on destructor", "catched exception on destructor")
    };

    vector(std::initializer_list<T> il)  : ivector(il), registered_as(internal_for_containers::register_container(true))  {};
    vector(const vector<T>& l);
    vector(vector<T>&& l);
    vector<T>& operator=(const vector<T>& l);
    vector<T>& operator=(vector<T>&& l);

    //  comparison operators
    bool operator==( const vector<T>& rhs )   { return ivector == rhs.ivector; }
    bool operator!=( const vector<T>& rhs )   { return ivector != rhs.ivector; }
    bool operator< ( const vector<T>& rhs )   { return ivector < rhs.ivector; }
    bool operator<=( const vector<T>& rhs )   { return ivector <= rhs.ivector; }
    bool operator> ( const vector<T>& rhs )   { return ivector > rhs.ivector; }
    bool operator>=( const vector<T>& rhs )   { return ivector >= rhs.ivector; }


    //  iterators
    iterator                    begin       ();
    const_iterator              cbegin      () const;
    iterator                    end         ();
    const_iterator              cend        () const;

    reverse_iterator            rbegin      ();
    const_reverse_iterator      crbegin     () const;
    reverse_iterator            rend        ();
    const_reverse_iterator      crend       () const;


    T&                  operator[]          ( unsigned n );
    const T&            operator[]          ( unsigned n ) const;
    void                clear               (void);
    unsigned long       size                () const;
    bool                empty               () const;
    const T&            at                  ( unsigned n ) const;
    T&                  at                  ( unsigned n );
    T&                  front               (void);
    const T&            front               (void) const;
    T&                  back                (void);
    const T&            back                (void) const;
    void                pop_back            (void);
    void                push_back           (const T& x);

    void                reserve             (size_t n);


//    iterator insert ( iterator position, const T& x )
//    void insert ( iterator position, unsigned n, const T& x )
//    template <class InputIterator>
//    void insert ( iterator position, InputIterator first, InputIterator last )
//iterator erase ( iterator position )
//iterator erase ( iterator first, iterator last )

private:
    std::vector<T>  ivector;
    int             registered_as;

    void check_limits(unsigned n, const char* text) const;        //    hidding

};





//        IMPLEMENTATION    ----------------------------------------------------------



template <typename T>
vector<T>::vector(const vector<T>& l)
  : registered_as (internal_for_containers::register_container( ivector.empty() ? true : false))
{
    ivector = l.ivector;

    if (ivector.empty() == false)
        internal_for_containers::register_container_size_change(registered_as);
}


template <typename T>
vector<T>::vector(vector<T>&& l)
  : registered_as (internal_for_containers::register_container( ivector.empty() ? true : false))
{
    ivector = std::move(l.ivector);

    if (ivector.empty() == false)
        internal_for_containers::register_container_size_change(registered_as);
}


template <typename T>
vector<T>& vector<T>::operator=(const vector<T>& v)
{
    ivector = v.ivector;
    registered_as = internal_for_containers::register_container( ivector.empty() ? true : false);
    if (ivector.empty() == false)
        internal_for_containers::register_container_size_change(registered_as);
    return *this;
}


template <typename T>
vector<T>& vector<T>::operator=(vector<T>&& v)
{
    ivector = std::move(v.ivector);
    registered_as = internal_for_containers::register_container( ivector.empty() ? true : false);
    if (ivector.empty() == false)
        internal_for_containers::register_container_size_change(registered_as);
    return *this;
}

//template <typename T>
//vector<T>& vector<T>::operator=(vector<T>&& v)
//{
//    ivector = std::move(v.ivector);
//    registered_as = internal_for_containers::register_container( ivector.empty() ? true : false);
//    if (ivector.empty() == false)
//        internal_for_containers::register_container_size_change(registered_as);
//    return *this;
//}





//  iterators
template <typename T>
bool vector<T>::iterator::is_valid (void) const
{
    if (last_updated_container==0 || internal_for_containers::get_registered_container_last_size_change(registered_owner) != last_updated_container)
        return false;
    else
        return true;
}

template <typename T>
typename vector<T>::iterator  vector<T>::begin       ()
{
    typename vector<T>::iterator it;
    it.iiterator = ivector.begin();

    it.registered_owner = registered_as;
    it.i_end_iterator = ivector.end();
    it.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);

    return it;
}


template <typename T>
typename vector<T>::iterator  vector<T>::end       ()
{
    typename vector<T>::iterator it;
    it.iiterator = ivector.end();
    it.registered_owner = registered_as;
    it.i_end_iterator = ivector.end();
    return it;
}





template <typename T>
typename vector<T>::iterator&  vector<T>::iterator::operator++()
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "vector", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "vector", "++ on end iterator", jle::al::priority::error);

    ++iiterator;
    return *this;
}

template <typename T>
typename vector<T>::iterator  vector<T>::iterator::operator++(int)
{
    typename vector<T>::iterator  result = *this;
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "vector", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "vector", "++ on end iterator", jle::al::priority::error);

    ++iiterator;
    return result;
}



template <typename T>
bool  vector<T>::iterator::operator==(const vector<T>::iterator& it) const
{
    if (registered_owner!=0  &&  it.registered_owner != 0   &&  registered_owner !=  it.registered_owner)
        throw jle::alarm(JLE_HERE, "vector", "on different owners", jle::al::priority::error);

    return it.iiterator == iiterator;
}

template <typename T>
bool  vector<T>::iterator::operator!=(const vector<T>::iterator& it) const
{
    return !operator==(it);
}


template <typename T>
T* vector<T>::iterator::operator->(void)
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "vector", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "vector", "-> on end iterator", jle::al::priority::error);

    return iiterator.operator->();
}

template <typename T>
T& vector<T>::iterator::operator*(void)
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "vector", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "vector", "* on end iterator", jle::al::priority::error);

    return *iiterator;
}






//  const_iterators       ---------------------------------------------------
template <typename T>
bool vector<T>::const_iterator::is_valid (void) const
{
    if (last_updated_container==0  ||  internal_for_containers::get_registered_container_last_size_change(registered_owner) != last_updated_container)
        return false;
    else
        return true;
}

template <typename T>
typename vector<T>::const_iterator  vector<T>::cbegin       () const
{
    typename vector<T>::const_iterator it;
    it.iiterator = ivector.begin();

    it.registered_owner = registered_as;
    it.i_end_iterator = ivector.end();

    it.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);

    return it;
}


template <typename T>
typename vector<T>::const_iterator  vector<T>::cend       ()  const
{
    typename vector<T>::const_iterator it;
    it.registered_owner = registered_as;
    it.i_end_iterator = ivector.end();
    it.iiterator = ivector.end();
    return it;
}



template <typename T>
typename vector<T>::const_iterator&  vector<T>::const_iterator::operator++()
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "vector", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "vector", "++ on end iterator", jle::al::priority::error);

    ++iiterator;
    return *this;
}

template <typename T>
typename vector<T>::const_iterator  vector<T>::const_iterator::operator++(int)
{
    typename vector<T>::const_iterator result = *this;
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "vector", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "vector", "++ on end iterator", jle::al::priority::error);

    ++iiterator;
    return result;
}


template <typename T>
bool  vector<T>::const_iterator::operator==(const const_iterator& it) const
{
    if(registered_owner!=0  &&  it.registered_owner != 0   &&  registered_owner !=  it.registered_owner)
        throw jle::alarm(JLE_HERE, "vector", "on different owners", jle::al::priority::error);

    return it.iiterator == iiterator;
}

template <typename T>
bool  vector<T>::const_iterator::operator!=(const const_iterator& it) const
{
    return !operator==(it);
}


template <typename T>
const T* vector<T>::const_iterator::operator->(void)
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "vector", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "vector", "-> on end iterator", jle::al::priority::error);

    return iiterator.operator->();
}

template <typename T>
const T& vector<T>::const_iterator::operator*(void)
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "vector", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "vector", "* on end iterator", jle::al::priority::error);

    return *iiterator;
}





//  reverse_iterators
template <typename T>
bool vector<T>::reverse_iterator::is_valid (void) const
{
    if (last_updated_container==0 || internal_for_containers::get_registered_container_last_size_change(registered_owner) != last_updated_container)
        return false;
    else
        return true;
}

template <typename T>
typename vector<T>::reverse_iterator  vector<T>::rbegin       ()
{
    typename vector<T>::reverse_iterator it;
    it.iiterator = ivector.rbegin();

    it.registered_owner = registered_as;
    it.i_rend_iterator = ivector.rend();
    it.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);

    return it;
}


template <typename T>
typename vector<T>::reverse_iterator  vector<T>::rend       ()
{
    typename vector<T>::reverse_iterator it;
    it.iiterator = ivector.rend();
    it.registered_owner = registered_as;
    it.i_rend_iterator = ivector.rend();
    return it;
}





template <typename T>
typename vector<T>::reverse_iterator&  vector<T>::reverse_iterator::operator++()
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "vector", "reverse_iterator not valid", jle::al::priority::error);

    if (i_rend_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "vector", "++ on end reverse_", jle::al::priority::error);

    ++iiterator;
    return *this;
}

template <typename T>
typename vector<T>::reverse_iterator  vector<T>::reverse_iterator::operator++(int)
{
    typename vector<T>::reverse_iterator  result = *this;
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "vector", "reverse_iterator not valid", jle::al::priority::error);

    if (i_rend_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "vector", "++ on end reverse_iterator", jle::al::priority::error);

    ++iiterator;
    return result;
}



template <typename T>
bool  vector<T>::reverse_iterator::operator==(const vector<T>::reverse_iterator& it) const
{
    if (registered_owner!=0  &&  it.registered_owner != 0   &&  registered_owner !=  it.registered_owner)
        throw jle::alarm(JLE_HERE, "vector", "on different owners", jle::al::priority::error);

    return it.iiterator == iiterator;
}

template <typename T>
bool  vector<T>::reverse_iterator::operator!=(const vector<T>::reverse_iterator& it) const
{
    return !operator==(it);
}


template <typename T>
T* vector<T>::reverse_iterator::operator->(void)
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "vector", "reverse_iterator not valid", jle::al::priority::error);

    if (i_rend_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "vector", "-> on rend reverse_iterator", jle::al::priority::error);

    return iiterator.operator->();
}

template <typename T>
T& vector<T>::reverse_iterator::operator*(void)
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "vector", "reverse_iterator not valid", jle::al::priority::error);

    if (i_rend_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "vector", "* on end reverse_iterator", jle::al::priority::error);

    return *iiterator;
}






//  const_reverse_iterators       ---------------------------------------------------
template <typename T>
bool vector<T>::const_reverse_iterator::is_valid (void) const
{
    if (last_updated_container==0  ||  internal_for_containers::get_registered_container_last_size_change(registered_owner) != last_updated_container)
        return false;
    else
        return true;
}

template <typename T>
typename vector<T>::const_reverse_iterator  vector<T>::crbegin       () const
{
    typename vector<T>::const_reverse_iterator it;
    it.iiterator = ivector.rbegin();

    it.registered_owner = registered_as;
    it.i_rend_iterator = ivector.rend();

    it.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);

    return it;
}


template <typename T>
typename vector<T>::const_reverse_iterator  vector<T>::crend       ()  const
{
    typename vector<T>::const_reverse_iterator it;
    it.registered_owner = registered_as;
    it.i_rend_iterator = ivector.rend();
    it.iiterator = ivector.rend();
    return it;
}



template <typename T>
typename vector<T>::const_reverse_iterator&  vector<T>::const_reverse_iterator::operator++()
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "vector", "reverse_iterator not valid", jle::al::priority::error);

    if (i_rend_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "vector", "++ on end reverse_iterator", jle::al::priority::error);

    ++iiterator;
    return *this;
}

template <typename T>
typename vector<T>::const_reverse_iterator  vector<T>::const_reverse_iterator::operator++(int)
{
    typename vector<T>::const_reverse_iterator result = *this;
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "vector", "reverse_iterator not valid", jle::al::priority::error);

    if (i_rend_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "vector", "++ on end reverse_iterator", jle::al::priority::error);

    ++iiterator;
    return result;
}


template <typename T>
bool  vector<T>::const_reverse_iterator::operator==(const const_reverse_iterator& it) const
{
    if(registered_owner!=0  &&  it.registered_owner != 0   &&  registered_owner !=  it.registered_owner)
        throw jle::alarm(JLE_HERE, "vector", "on different owners", jle::al::priority::error);

    return it.iiterator == iiterator;
}

template <typename T>
bool  vector<T>::const_reverse_iterator::operator!=(const const_reverse_iterator& it) const
{
    return !operator==(it);
}


template <typename T>
const T* vector<T>::const_reverse_iterator::operator->(void)
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "vector", "reverse_iterator not valid", jle::al::priority::error);

    if (i_rend_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "vector", "-> on rend reverse_iterator", jle::al::priority::error);

    return iiterator.operator->();
}

template <typename T>
const T& vector<T>::const_reverse_iterator::operator*(void)
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "vector", "reverse_iterator not valid", jle::al::priority::error);

    if (i_rend_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "vector", "* on end reverse_iterator", jle::al::priority::error);

    return *iiterator;
}




//  ------------------------------------------------------------------------------------



template <typename T>
void   vector<T>::check_limits         ( unsigned n, const char* text ) const
{
    if (n>=ivector.size())
    {
        std::ostringstream oss;
        oss << "out of limits " << "ivector.size() " << ivector.size() << " < " << n << " on... " << text;
        throw jle::alarm(JLE_HERE, "vector", std::string("out of limits ") +
                    oss.str() , jle::al::priority::error);
    }
}



template <typename T>
T&   vector<T>::operator[]         ( unsigned n )
{
    check_limits(n, "operator[]");
    return  ivector[n];
}


template <typename T>
const T&  vector<T>::operator[]         ( unsigned n ) const
{
    check_limits(n, "const operator[]");
    return  ivector[n];
}


template <typename T>
void vector<T>::clear             (void)
{
    internal_for_containers::register_container_size_change(registered_as);
    ivector.clear();
}



template <typename T>
unsigned long     vector<T>::size            () const
{
    return ivector.size();
}

template <typename T>
bool    vector<T>::empty             () const
{
    return ivector.empty();
}

template <typename T>
const T&  vector<T>::at                 ( unsigned n ) const
{
    check_limits(n , "at");
    return ivector.at(n);
}

template <typename T>
T& vector<T>::at                 ( unsigned n )
{
    check_limits(n , "at");
    return ivector.at(n);
}

template <typename T>
T&     vector<T>::front             (void)
{
    check_limits(0 , "front");
    return ivector.front();
}

template <typename T>
const T&  vector<T>::front             (void) const
{
    check_limits(0 , "front");
    return ivector.front();
}

template <typename T>
T&     vector<T>::back             (void)
{
    check_limits(0 , "back");
    return ivector.back();
}

template <typename T>
const T&  vector<T>::back             (void) const
{
    check_limits(0 , "back");
    return ivector.back();
}


template <typename T>
void     vector<T>::pop_back         (void)
{
    check_limits(0 , "pop_back");
    internal_for_containers::register_container_size_change(registered_as);
    ivector.pop_back();
}

template <typename T>
void     vector<T>::push_back         (const T& x)
{
    if (internal_for_containers::get_registered_container_last_size_change(registered_as) == 0)
        internal_for_containers::register_container_size_change(registered_as);
    ivector.push_back(x);
}



template <typename T>
void     vector<T>::reserve         (size_t n)
{
    internal_for_containers::register_container_size_change(registered_as);
    ivector.reserver(n);
}





//---------------------------------------------------------------------------
}      //  namespace jle {


#endif    //    JLE_VECTOR__H___
