#ifndef JLE_LIST__HPP
#define JLE_LIST__HPP




#include <list>

#include "container_support.h"
#include "core/alarm.h"





namespace jle {





template<typename T>
class list {
public:

        class iterator   {
        public:
            iterator(const iterator&) = default;
            iterator(iterator&&) = default;
            iterator& operator=(const iterator&) = default;
            iterator& operator=(iterator&&) = default;

            friend class jle::list<T>;
            friend class jle::list<T>::const_iterator;
            iterator() : registered_owner(0), last_updated_container(0)  {}
            iterator&   operator++();
            iterator    operator++(int);
            bool        operator==(const iterator& it) const;
            bool        operator!=(const iterator& it) const;

            T* operator->(void);
            T& operator* (void);

        private:
            typename std::list<T>::iterator iiterator;
            typename std::list<T>::iterator i_end_iterator;
            int                             registered_owner;
            int                             last_updated_container;

            bool is_valid (void) const;
        };


        class const_iterator  {
        public:
            const_iterator(const const_iterator&) = default;
            const_iterator(const_iterator&&) = default;
            const_iterator& operator=(const const_iterator&) = default;
            const_iterator& operator=(const_iterator&&) = default;

            friend class jle::list<T>;
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
            typename std::list<T>::const_iterator   iiterator;
            typename std::list<T>::const_iterator   i_end_iterator;
            int                                     registered_owner;
            int                                     last_updated_container;

            bool is_valid (void) const;
        };



        class reverse_iterator   {
        public:
            reverse_iterator(const reverse_iterator&) = default;
            reverse_iterator(reverse_iterator&&) = default;
            reverse_iterator& operator=(const reverse_iterator&) = default;
            reverse_iterator& operator=(reverse_iterator&&) = default;

            friend class jle::list<T>;
            friend class jle::list<T>::const_reverse_iterator;
            reverse_iterator() : registered_owner(0), last_updated_container(0)  {}
            reverse_iterator&   operator++();
            reverse_iterator    operator++(int);
            bool                operator==(const reverse_iterator& it) const;
            bool                operator!=(const reverse_iterator& it) const;

            T* operator->(void);
            T& operator* (void);

        private:
            typename std::list<T>::reverse_iterator iiterator;
            typename std::list<T>::reverse_iterator i_rend_iterator;
            int                             registered_owner;
            int                             last_updated_container;

            bool is_valid (void) const;
        };


        class const_reverse_iterator  {
        public:
            const_reverse_iterator(const const_reverse_iterator&) = default;
            const_reverse_iterator(const_reverse_iterator&&) = default;
            const_reverse_iterator& operator=(const const_reverse_iterator&) = default;
            const_reverse_iterator& operator=(const_reverse_iterator&&) = default;

            friend class jle::list<T>;
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
            typename std::list<T>::const_reverse_iterator   iiterator;
            typename std::list<T>::const_reverse_iterator   i_rend_iterator;
            int                                             registered_owner;
            int                                             last_updated_container;

            bool is_valid (void) const;
        };





    list()  : registered_as(internal_for_containers::register_container(true)) {};
    ~list()
    {
        try{
            internal_for_containers::unregister_container(registered_as);
        } JLE_CATCH_CALLFUNCION(jle::alarm_msg, "exception on destructor", "catched exception on destructor")
    };

    list(std::initializer_list<T> il)  : ilist(il), registered_as(internal_for_containers::register_container(true))  {};
    list(const list<T>& l);
    list(list<T>&& l);
    list<T>& operator=(const list<T>& l);
    list<T>& operator=(list<T>&& l);

    //  comparison operators
    bool operator==( const list<T>& rhs ) const   { return ilist == rhs.ilist; }
    bool operator!=( const list<T>& rhs ) const  { return ilist != rhs.ilist; }
    bool operator< ( const list<T>& rhs ) const  { return ilist < rhs.ilist; }
    bool operator<=( const list<T>& rhs ) const  { return ilist <= rhs.ilist; }
    bool operator> ( const list<T>& rhs ) const  { return ilist > rhs.ilist; }
    bool operator>=( const list<T>& rhs ) const  { return ilist >= rhs.ilist; }



    //  iterators
    iterator                    begin       ();
    const_iterator              cbegin      () const;
    iterator                    end         ();
    const_iterator              cend        () const;

    reverse_iterator            rbegin      ();
    const_reverse_iterator      crbegin     () const;
    reverse_iterator            rend        ();
    const_reverse_iterator      crend       () const;


    //  capacity
    bool                empty   ()  const;
    unsigned            size    ()  const;
    unsigned            max_size() const;
    void                resize  (unsigned sz, T c = T());

    //  access
    T&                  front ( );
    const T&            front ( ) const;
    T&                  back ( );
    const T&            back ( ) const;


    //  modifiers
    void                assign      ( unsigned n, const T& u );
    void                push_front  ( const T& x );
    void                pop_front   ( );
    void                push_back   ( const T& x );
    void                pop_back    ( );
    iterator            insert      ( const iterator& position, const T& x );
    void                insert      ( const iterator& position, unsigned n, const T& x );
    iterator            erase       ( const iterator& position );
    iterator            erase       ( const iterator& first, const iterator& last );
    void                clear       ( );





    //  operations
    void remove ( const T& value );
    void unique ( );
    //void merge ( list<T,std::Allocator>& x );
    void sort ( );
    void reverse ( );





private:
    std::list<T>        ilist;
    int                 registered_as;

};






//--------------------------------------------------------------------------------
//        IMPLEMENTATION
//--------------------------------------------------------------------------------


template <typename T>
list<T>::list(const list<T>& l)
  : registered_as (internal_for_containers::register_container( ilist.empty() ? true : false))
{
    ilist = l.ilist;

    if (ilist.empty() == false)
        internal_for_containers::register_container_size_change(registered_as);
}

template <typename T>
list<T>::list(list<T>&& l)
  : registered_as (internal_for_containers::register_container( ilist.empty() ? true : false))
{
    ilist = std::move(l.ilist);

    if (ilist.empty() == false)
        internal_for_containers::register_container_size_change(registered_as);
}

template <typename T>
list<T>& list<T>::operator=(const list<T>& l)
{
    ilist = l.ilist;
    registered_as = internal_for_containers::register_container( ilist.empty() ? true : false);
    if (ilist.empty() == false)
        internal_for_containers::register_container_size_change(registered_as);
    return *this;
}

template <typename T>
list<T>& list<T>::operator=(list<T>&& v)
{
    ilist = std::move(v.ilist);
    registered_as = internal_for_containers::register_container( ilist.empty() ? true : false);
    if (ilist.empty() == false)
        internal_for_containers::register_container_size_change(registered_as);
    return *this;
}





//  iterators
template <typename T>
bool list<T>::iterator::is_valid (void) const
{
    if (last_updated_container==0 || internal_for_containers::get_registered_container_last_size_change(registered_owner) != last_updated_container)
        return false;
    else
        return true;
}

template <typename T>
typename list<T>::iterator  list<T>::begin       ()
{
    typename list<T>::iterator it;
    it.iiterator = ilist.begin();

    it.registered_owner = registered_as;
    it.i_end_iterator = ilist.end();
    it.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);

    return it;
}


template <typename T>
typename list<T>::iterator  list<T>::end       ()
{
    typename list<T>::iterator it;
    it.iiterator = ilist.end();
    it.registered_owner = registered_as;
    it.i_end_iterator = ilist.end();
    return it;
}





template <typename T>
typename list<T>::iterator&  list<T>::iterator::operator++()
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "list", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "list", "++ on end iterator", jle::al::priority::error);

    ++iiterator;
    return *this;
}

template <typename T>
typename list<T>::iterator  list<T>::iterator::operator++(int)
{
    typename list<T>::iterator  result = *this;
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "list", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "list", "++ on end iterator", jle::al::priority::error);

    ++iiterator;
    return result;
}



template <typename T>
bool  list<T>::iterator::operator==(const list<T>::iterator& it) const
{
    if (registered_owner!=0  &&  it.registered_owner != 0   &&  registered_owner !=  it.registered_owner)
        throw jle::alarm(JLE_HERE, "list", "on different owners", jle::al::priority::error);

    return it.iiterator == iiterator;
}

template <typename T>
bool  list<T>::iterator::operator!=(const list<T>::iterator& it) const
{
    return !operator==(it);
}


template <typename T>
T* list<T>::iterator::operator->(void)
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "list", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "list", "-> on end iterator", jle::al::priority::error);

    return iiterator.operator->();
}

template <typename T>
T& list<T>::iterator::operator*(void)
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "list", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "list", "* on end iterator", jle::al::priority::error);

    return *iiterator;
}






//  const_iterators       ---------------------------------------------------
template <typename T>
bool list<T>::const_iterator::is_valid (void) const
{
    if (last_updated_container==0  ||  internal_for_containers::get_registered_container_last_size_change(registered_owner) != last_updated_container)
        return false;
    else
        return true;
}

template <typename T>
typename list<T>::const_iterator  list<T>::cbegin       () const
{
    typename list<T>::const_iterator it;
    it.iiterator = ilist.begin();

    it.registered_owner = registered_as;
    it.i_end_iterator = ilist.end();

    it.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);

    return it;
}


template <typename T>
typename list<T>::const_iterator  list<T>::cend       ()  const
{
    typename list<T>::const_iterator it;
    it.registered_owner = registered_as;
    it.i_end_iterator = ilist.end();
    it.iiterator = ilist.end();
    return it;
}



template <typename T>
typename list<T>::const_iterator&  list<T>::const_iterator::operator++()
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "list", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "list", "++ on end iterator", jle::al::priority::error);

    ++iiterator;
    return *this;
}

template <typename T>
typename list<T>::const_iterator  list<T>::const_iterator::operator++(int)
{
    typename list<T>::const_iterator result = *this;
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "list", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "list", "++ on end iterator", jle::al::priority::error);

    ++iiterator;
    return result;
}


template <typename T>
bool  list<T>::const_iterator::operator==(const const_iterator& it) const
{
    if(registered_owner!=0  &&  it.registered_owner != 0   &&  registered_owner !=  it.registered_owner)
        throw jle::alarm(JLE_HERE, "list", "on different owners", jle::al::priority::error);

    return it.iiterator == iiterator;
}

template <typename T>
bool  list<T>::const_iterator::operator!=(const const_iterator& it) const
{
    return !operator==(it);
}


template <typename T>
const T* list<T>::const_iterator::operator->(void)
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "list", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "list", "-> on end iterator", jle::al::priority::error);

    return iiterator.operator->();
}

template <typename T>
const T& list<T>::const_iterator::operator*(void)
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "list", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "list", "* on end iterator", jle::al::priority::error);

    return *iiterator;
}





//  reverse_iterators
template <typename T>
bool list<T>::reverse_iterator::is_valid (void) const
{
    if (last_updated_container==0 || internal_for_containers::get_registered_container_last_size_change(registered_owner) != last_updated_container)
        return false;
    else
        return true;
}

template <typename T>
typename list<T>::reverse_iterator  list<T>::rbegin       ()
{
    typename list<T>::reverse_iterator it;
    it.iiterator = ilist.rbegin();

    it.registered_owner = registered_as;
    it.i_rend_iterator = ilist.rend();
    it.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);

    return it;
}


template <typename T>
typename list<T>::reverse_iterator  list<T>::rend       ()
{
    typename list<T>::reverse_iterator it;
    it.iiterator = ilist.rend();
    it.registered_owner = registered_as;
    it.i_rend_iterator = ilist.rend();
    return it;
}





template <typename T>
typename list<T>::reverse_iterator&  list<T>::reverse_iterator::operator++()
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "list", "reverse_iterator not valid", jle::al::priority::error);

    if (i_rend_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "list", "++ on end reverse_", jle::al::priority::error);

    ++iiterator;
    return *this;
}

template <typename T>
typename list<T>::reverse_iterator  list<T>::reverse_iterator::operator++(int)
{
    typename list<T>::reverse_iterator  result = *this;
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "list", "reverse_iterator not valid", jle::al::priority::error);

    if (i_rend_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "list", "++ on end reverse_iterator", jle::al::priority::error);

    ++iiterator;
    return result;
}



template <typename T>
bool  list<T>::reverse_iterator::operator==(const list<T>::reverse_iterator& it) const
{
    if (registered_owner!=0  &&  it.registered_owner != 0   &&  registered_owner !=  it.registered_owner)
        throw jle::alarm(JLE_HERE, "list", "on different owners", jle::al::priority::error);

    return it.iiterator == iiterator;
}

template <typename T>
bool  list<T>::reverse_iterator::operator!=(const list<T>::reverse_iterator& it) const
{
    return !operator==(it);
}


template <typename T>
T* list<T>::reverse_iterator::operator->(void)
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "list", "reverse_iterator not valid", jle::al::priority::error);

    if (i_rend_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "list", "-> on rend reverse_iterator", jle::al::priority::error);

    return iiterator.operator->();
}

template <typename T>
T& list<T>::reverse_iterator::operator*(void)
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "list", "reverse_iterator not valid", jle::al::priority::error);

    if (i_rend_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "list", "* on end reverse_iterator", jle::al::priority::error);

    return *iiterator;
}






//  const_reverse_iterators       ---------------------------------------------------
template <typename T>
bool list<T>::const_reverse_iterator::is_valid (void) const
{
    if (last_updated_container==0  ||  internal_for_containers::get_registered_container_last_size_change(registered_owner) != last_updated_container)
        return false;
    else
        return true;
}

template <typename T>
typename list<T>::const_reverse_iterator  list<T>::crbegin       () const
{
    typename list<T>::const_reverse_iterator it;
    it.iiterator = ilist.rbegin();

    it.registered_owner = registered_as;
    it.i_rend_iterator = ilist.rend();

    it.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);

    return it;
}


template <typename T>
typename list<T>::const_reverse_iterator  list<T>::crend       ()  const
{
    typename list<T>::const_reverse_iterator it;
    it.registered_owner = registered_as;
    it.i_rend_iterator = ilist.rend();
    it.iiterator = ilist.rend();
    return it;
}



template <typename T>
typename list<T>::const_reverse_iterator&  list<T>::const_reverse_iterator::operator++()
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "list", "reverse_iterator not valid", jle::al::priority::error);

    if (i_rend_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "list", "++ on end reverse_iterator", jle::al::priority::error);

    ++iiterator;
    return *this;
}

template <typename T>
typename list<T>::const_reverse_iterator  list<T>::const_reverse_iterator::operator++(int)
{
    typename list<T>::const_reverse_iterator result = *this;
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "list", "reverse_iterator not valid", jle::al::priority::error);

    if (i_rend_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "list", "++ on end reverse_iterator", jle::al::priority::error);

    ++iiterator;
    return result;
}


template <typename T>
bool  list<T>::const_reverse_iterator::operator==(const const_reverse_iterator& it) const
{
    if(registered_owner!=0  &&  it.registered_owner != 0   &&  registered_owner !=  it.registered_owner)
        throw jle::alarm(JLE_HERE, "list", "on different owners", jle::al::priority::error);

    return it.iiterator == iiterator;
}

template <typename T>
bool  list<T>::const_reverse_iterator::operator!=(const const_reverse_iterator& it) const
{
    return !operator==(it);
}


template <typename T>
const T* list<T>::const_reverse_iterator::operator->(void)
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "list", "reverse_iterator not valid", jle::al::priority::error);

    if (i_rend_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "list", "-> on rend reverse_iterator", jle::al::priority::error);

    return iiterator.operator->();
}

template <typename T>
const T& list<T>::const_reverse_iterator::operator*(void)
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "list", "reverse_iterator not valid", jle::al::priority::error);

    if (i_rend_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "list", "* on end reverse_iterator", jle::al::priority::error);

    return *iiterator;
}




//  capacity
template <typename T>
bool   list<T>::empty   ()  const
{
    return ilist.empty();
}

template <typename T>
unsigned   list<T>::size    ()  const
{
    return unsigned(ilist.size());
}


template <typename T>
unsigned   list<T>::max_size() const
{
    return unsigned(ilist.max_size());
}


template <typename T>
void   list<T>::resize  (unsigned sz, T c)
{
    internal_for_containers::register_container_size_change(registered_as);
    ilist.resize(sz, c);
}






//  access
template<typename T>
T&       list<T>::front ( )
{
    if (size()==0)
        throw jle::alarm(JLE_HERE, "list", "front on empty list", jle::al::priority::error);

    return ilist.front();
}

template<typename T>
const T& list<T>::front ( ) const
{
    if (size()==0)
        throw jle::alarm(JLE_HERE, "list", "front on empty list", jle::al::priority::error);

    return ilist.front();
}

template<typename T>
T&       list<T>::back ( )
{
    if (size()==0)
        throw jle::alarm(JLE_HERE, "list", "back on empty list", jle::al::priority::error);

    return ilist.back();
}

template<typename T>
const T& list<T>::back ( ) const
{
    if (size()==0)
        throw jle::alarm(JLE_HERE, "list", "back on empty list", jle::al::priority::error);

    return ilist.back();
}



//  modifiers
template<typename T>
void     list<T>::assign      ( unsigned n, const T& u )
{
    internal_for_containers::register_container_size_change(registered_as);
    ilist.assign(n, u);
}

template<typename T>
void     list<T>::push_front  ( const T& x )
{
    internal_for_containers::register_container_size_change(registered_as);
    ilist.push_front(x);
}

template<typename T>
void     list<T>::pop_front   ( )
{
    if (size()==0)
        throw jle::alarm(JLE_HERE, "list",  "pop_front on empty list", jle::al::priority::error);
    internal_for_containers::register_container_size_change(registered_as);
    ilist.pop_front();
}

template<typename T>
void     list<T>::push_back   ( const T& x )
{
    if (internal_for_containers::get_registered_container_last_size_change(registered_as) == 0)
        internal_for_containers::register_container_size_change(registered_as);
    ilist.push_back(x);
}

template<typename T>
void     list<T>::pop_back    ( )
{
    if (size()==0)
        throw jle::alarm(JLE_HERE, "list", "pop_back on empty list", jle::al::priority::error);
    internal_for_containers::register_container_size_change(registered_as);
    ilist.pop_back();
}

template<typename T>
typename list<T>::iterator list<T>::insert      ( const typename list<T>::iterator& position, const T& x )
{
    if (position.registered_owner  != registered_as)
        throw jle::alarm(JLE_HERE, "list", "insert with extrange iterator", jle::al::priority::error);

    if (position.iiterator != position.i_end_iterator)
        if (internal_for_containers::get_registered_container_last_size_change(position.registered_owner) != position.last_updated_container)
            throw jle::alarm(JLE_HERE, "list", "insert with invalid iterator", jle::al::priority::error);

    internal_for_containers::register_container_size_change(registered_as);
    typename list<T>::iterator result;
    result.iiterator =  ilist.insert(position.iiterator, x);

    result.registered_owner = registered_as;
    result.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);
    result.i_end_iterator = ilist.end();

    return result;
}

template<typename T>
void     list<T>::insert      ( const typename list<T>::iterator& position, unsigned n, const T& x )
{
    if (position.registered_owner  != registered_as)
        throw jle::alarm(JLE_HERE, "list", "insert with extrange iterator", jle::al::priority::error);

    if (position.iiterator != position.i_end_iterator)
        if (internal_for_containers::get_registered_container_last_size_change(position.registered_owner) != position.last_updated_container)
            throw jle::alarm(JLE_HERE, "list", "insert with invalid iterator", jle::al::priority::error);

    internal_for_containers::register_container_size_change(registered_as);
    ilist.insert(position.iiterator, n, x);
}

template<typename T>
typename list<T>::iterator list<T>::erase       ( const typename list<T>::iterator& position )
{
    if (position.registered_owner  != registered_as)
        throw jle::alarm(JLE_HERE, "list", "erase with extrange iterator", jle::al::priority::error);
    if (position.iiterator  == ilist.end())
        throw jle::alarm(JLE_HERE, "list", "erase over end iterator", jle::al::priority::error);
    if (position.is_valid() == false)
        throw jle::alarm(JLE_HERE, "list", "erase over iterator invalid", jle::al::priority::error);

    internal_for_containers::register_container_size_change(registered_as);

    typename list<T>::iterator result;
    result.iiterator = ilist.erase(position.iiterator);

    result.registered_owner = registered_as;
    result.i_end_iterator = ilist.end();
    if (ilist.empty() == false)
        result.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);
    else
        result.last_updated_container = 0;        //  no valid value

    return result;
}


template<typename T>
typename list<T>::iterator list<T>::erase       ( const typename list<T>::iterator& first, const typename list<T>::iterator& last )
{
    if (first.registered_owner  != registered_as  ||  last.registered_owner  != registered_as)
        throw jle::alarm(JLE_HERE, "list", "erase with extrange iterator (first or second)", jle::al::priority::error);
    if (first.iiterator  == ilist.end())
        throw jle::alarm(JLE_HERE, "list", "erase over end iterator (first)", jle::al::priority::error);
    if (first.is_valid() == false)
        throw jle::alarm(JLE_HERE, "list", "erase over iterator invalid (first)", jle::al::priority::error);

    typename list<T>::iterator result;

    if (first == last)
    {
        result = erase (first);
    }
    else
    {
        internal_for_containers::register_container_size_change(registered_as);

        typename list<T>::iterator next;
        bool bFindLastIterator = false;

        next = first;

        do
        {
            if (next.iiterator == last.iiterator)
                bFindLastIterator = true;

            result.iiterator = ilist.erase(next.iiterator);
            result.registered_owner = registered_as;
            result.i_end_iterator = ilist.end();
            if (ilist.empty() == false)
                result.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);
            else
                result.last_updated_container = 0;        //  no valid value

            next = result;

        }
        while ((bFindLastIterator == false) && (result.iiterator != ilist.end()));


        if ((last.iiterator != ilist.end()) && (bFindLastIterator == false))
            throw jle::alarm(JLE_HERE, "list", "erase from first iterator to end of container (last iterator not found)", jle::al::priority::error);

    }
    return result;
}

template<typename T>
void     list<T>::clear       ( )
{
    internal_for_containers::register_container_size_change(registered_as);
    ilist.clear();
}






//  operations
template<typename T>
void list<T>::remove ( const T& value )
{
    internal_for_containers::register_container_size_change(registered_as);
    ilist.remove(value);
}

template<typename T>
void list<T>::unique ( )
{
    internal_for_containers::register_container_size_change(registered_as);
    ilist.unique();
}

//void merge ( list<T,std::Allocator>& x );
template<typename T>
void list<T>::sort ( )
{
    internal_for_containers::register_container_size_change(registered_as);
    ilist.sort();
}

template<typename T>
void list<T>::reverse ( )
{
    internal_for_containers::register_container_size_change(registered_as);
    ilist.reverse();
}




template<typename T>
std::ostream& operator<< (std::ostream& os, const jle::list<T>& l)
{
    using ::operator<<;
    os << "list(";

    auto i=0;
    for(auto it=l.cbegin(); it!=l.cend(); ++it) {
        ++i;
        if(i>=7)
        {
            os << "...";
            break;
        }

        os << *it << ", ";
    }

    os << ")";
    return os;
}





};   //namespace jle {

#endif // JLE_LIST__HPP
