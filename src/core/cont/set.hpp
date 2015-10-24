#ifndef JLE_CONTAINERS_SET_GUARD
#define JLE_CONTAINERS_SET_GUARD




#include <set>

#include "container_support.h"
#include "alarm.h"



namespace jle {
//---------------------------------------------------------------------------



template<typename key_type>
class set
{
public:

        class iterator   {
        public:
            iterator(const iterator&) = default;
            iterator(iterator&&) = default;
            iterator& operator=(const iterator&) = default;
            iterator& operator=(iterator&&) = default;

            friend class jle::set<key_type>;
            friend class jle::set<key_type>::const_iterator;
            iterator() : registered_owner(0), last_updated_container(0)  {};
            iterator&   operator++();
            iterator    operator++(int);
            bool        operator==(const iterator& it) const;
            bool        operator!=(const iterator& it) const;

            key_type* operator->(void);
            key_type& operator* (void);

        private:
            typename std::set<key_type>::iterator iiterator;
            typename std::set<key_type>::iterator i_end_iterator;
            int                                   registered_owner;
            int                                   last_updated_container;

            bool is_valid (void) const;
        };


        class const_iterator  {
        public:
            const_iterator(const const_iterator&) = default;
            const_iterator(const_iterator&&) = default;
            const_iterator& operator=(const const_iterator&) = default;
            const_iterator& operator=(const_iterator&&) = default;

            friend class jle::set<key_type>;
            const_iterator() : registered_owner(0), last_updated_container(0)  {};
            const_iterator(const iterator& it) :
                            iiterator (it.iiterator),
                            i_end_iterator (it.i_end_iterator),
                            registered_owner(it.registered_owner),
                            last_updated_container(it.last_updated_container)
                            {
                            };
            const_iterator&     operator=(const iterator& it)
            {
                registered_owner = it.registered_owner;
                last_updated_container = it.last_updated_container;
                iiterator = it.iiterator;
                i_end_iterator = it.i_end_iterator;
                return *this;
            }

            const_iterator&     operator=(const_iterator&& it)=default;

            const_iterator&     operator++();
            const_iterator      operator++(int);
            bool                operator==(const const_iterator& it) const;
            bool                operator!=(const const_iterator& it) const;

            const key_type* operator->(void);
            const key_type& operator*(void);

        private:
            typename std::set<key_type>::const_iterator  iiterator;
            typename std::set<key_type>::const_iterator  i_end_iterator;
            int                                          registered_owner;
            int                                          last_updated_container;

            bool is_valid (void) const;
        };





    set()  : registered_as(internal_for_containers::register_container(true)) {};
    ~set()
    {
        try{
            internal_for_containers::unregister_container(registered_as);
        } JLE_CATCH_CALLFUNCION(jle::alarm_msg, "exception on destructor", "catched exception on destructor")
    };

    set(std::initializer_list<T> il)  : iset(il), registered_as(internal_for_containers::register_container(true))  {};
    set(const set<key_type>& s);
    set(set<key_type>&& s);
    set<key_type>& operator=(const set<key_type>& s);
    set<key_type>& operator=(set<key_type>&& s);

    //  comparison operators
    bool operator==( const set<T>& rhs )   { return iset == rhs.iset; }
    bool operator!=( const set<T>& rhs )   { return iset != rhs.iset; }
    bool operator< ( const set<T>& rhs )   { return iset < rhs.iset; }
    bool operator<=( const set<T>& rhs )   { return iset <= rhs.iset; }
    bool operator> ( const set<T>& rhs )   { return iset > rhs.iset; }
    bool operator>=( const set<T>& rhs )   { return iset >= rhs.iset; }




    //  capacity
    bool            empty   ()  const;
    unsigned        size    ()  const;



    //  operations
    void                clear       ();
    unsigned            count       (const key_type& x) const;
    iterator            find        (const key_type& x);
    const_iterator      find        (const key_type& x) const;
    iterator            lower_bound (const key_type& x);
    const_iterator      lower_bound (const key_type& x) const;
    iterator            upper_bound (const key_type& x);
    const_iterator      upper_bound (const key_type& x) const;


    //  iterators
    iterator            begin       ();
    const_iterator      cbegin      () const;
    iterator            end         ();
    const_iterator      cend        () const;


    //  modifiers
    unsigned            erase       (const key_type& x);
    iterator            erase       (iterator position);
    void                erase       (const iterator& first, const iterator& last);
    std::pair<iterator, bool>  insert   (const key_type& x );

private:
    std::set<key_type>      iset;
    int                     registered_as;  // can't be constant. assign operator
};





//--------------------------------------------------------------------------------
//        IMPLEMENTATION
//--------------------------------------------------------------------------------


template <typename key_type>
set<key_type>::set(const set<key_type>& s)
  : registered_as (internal_for_containers::register_container( iset.empty() ? true : false))
{
    iset = s.iset;
    if(iset.empty() == false)
        internal_for_containers::register_container_size_change(registered_as);
}

template <typename T>
set<T>::set(set<T>&& l)
  : registered_as (internal_for_containers::register_container( iset.empty() ? true : false))
{
    iset = std::move(l.iset);

    if (iset.empty() == false)
        internal_for_containers::register_container_size_change(registered_as);
}


template <typename key_type>
set<key_type>& set<key_type>::operator=(const set<key_type>& s)
{
    iset = s.iset;
    registered_as = internal_for_containers::register_container( iset.empty() ? true : false);
    if(iset.empty() == false)
        internal_for_containers::register_container_size_change(registered_as);
    return *this;
}

template <typename T>
set<T>& set<T>::operator=(set<T>&& v)
{
    iset = std::move(v.iset);
    registered_as = internal_for_containers::register_container( iset.empty() ? true : false);
    if (iset.empty() == false)
        internal_for_containers::register_container_size_change(registered_as);
    return *this;
}



//  iterators       ---------------------------------------------------

template <typename key_type>
bool set<key_type>::iterator::is_valid (void) const
{
    if (last_updated_container==0  ||  internal_for_containers::get_registered_container_last_size_change(registered_owner) != last_updated_container)
        return false;
    else
        return true;
}

template <typename key_type>
typename set<key_type>::iterator  set<key_type>::begin       ()
{
    typename set<key_type>::iterator it;
    it.iiterator = iset.begin();

    it.registered_owner = registered_as;
    it.i_end_iterator = iset.end();

    if (iset.empty()==false)
        it.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);
    else
        it.last_updated_container = 0;        //  not valid value

    return it;
}


template <typename key_type>
typename set<key_type>::iterator  set<key_type>::end       ()
{
    typename set<key_type>::iterator it;
    it.iiterator = iset.end();
    it.registered_owner = registered_as;
    it.i_end_iterator = iset.end();
    it.last_updated_container = 0;    //    no value
    return it;
}




template <typename key_type>
typename set<key_type>::iterator&  set<key_type>::iterator::operator++()
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "set", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "set", "++ on end iterator", jle::al::priority::error);

    ++iiterator;
    return *this;
}

template <typename key_type>
typename set<key_type>::iterator  set<key_type>::iterator::operator++(int)
{
    typename set<key_type>::iterator  result = *this;

    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "set", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "set", "++ on end iterator", jle::al::priority::error);

    ++iiterator;
    return result;
}

template <typename key_type>
bool  set<key_type>::iterator::operator==(const set<key_type>::iterator& it) const
{
    if(registered_owner!=0  &&  it.registered_owner != 0   &&  registered_owner !=  it.registered_owner)
        throw jle::alarm(JLE_HERE, "set", "on different owners", jle::al::priority::error);

    return it.iiterator == iiterator;
}

template <typename key_type>
bool  set<key_type>::iterator::operator!=(const iterator& it) const
{
    return !operator==(it);
}


template <typename key_type>
key_type* set<key_type>::iterator::operator->(void)
{
    if (last_updated_container==0
            ||  internal_for_containers::get_registered_container_last_size_change(registered_owner) != last_updated_container
            ||  internal_for_containers::get_registered_container_last_size_change(registered_owner) == 0)
        throw jle::alarm(JLE_HERE, "set", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "set", "-> on end iterator", jle::al::priority::error);

    return iiterator.operator->();
}

template <typename key_type>
key_type&   set<key_type>::iterator::operator*(void)
{
    if (last_updated_container==0
            ||  internal_for_containers::get_registered_container_last_size_change(registered_owner) != last_updated_container
            ||  internal_for_containers::get_registered_container_last_size_change(registered_owner) == 0)
        throw jle::alarm(JLE_HERE, "set", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "set", "* on end iterator", jle::al::priority::error);

    return *iiterator;
}











//  const_iterators       ---------------------------------------------------

template <typename key_type>
bool set<key_type>::const_iterator::is_valid (void) const
{
    if (last_updated_container==0  ||  internal_for_containers::get_registered_container_last_size_change(registered_owner) != last_updated_container)
        return false;
    else
        return true;
}


template <typename key_type>
typename set<key_type>::const_iterator  set<key_type>::cbegin       () const
{
    typename set<key_type>::const_iterator it;
    it.iiterator = iset.begin();

    it.registered_owner = registered_as;
    it.i_end_iterator = iset.end();

    if (iset.empty() == false)
        it.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);
    else
        it.last_updated_container = 0;        //  not valid value

    return it;
}


template <typename key_type>
typename set<key_type>::const_iterator  set<key_type>::cend       ()  const
{
    typename set<key_type>::const_iterator it;
    it.registered_owner = registered_as;
    it.i_end_iterator = iset.end();
    it.iiterator = iset.end();
    it.last_updated_container = 0;    //  no value
    return it;
}




template <typename key_type>
typename set<key_type>::const_iterator&  set<key_type>::const_iterator::operator++()
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "set", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "set", "++ on end iterator", jle::al::priority::error);

    ++iiterator;
    return *this;
}

template <typename key_type>
typename set<key_type>::const_iterator  set<key_type>::const_iterator::operator++(int)
{
    typename set<key_type>::const_iterator  result = *this;

    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "set", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "set", "++ on end iterator", jle::al::priority::error);

    ++iiterator;
    return result;
}

template <typename key_type>
bool  set<key_type>::const_iterator::operator==(const const_iterator& it) const
{
    if(registered_owner!=0  &&  it.registered_owner != 0   &&  registered_owner !=  it.registered_owner)
        throw jle::alarm(JLE_HERE, "set", "on different owners", jle::al::priority::error);

    return it.iiterator == iiterator;
}

template <typename key_type>
bool  set<key_type>::const_iterator::operator!=(const const_iterator& it) const
{
    return !operator==(it);
}


template <typename key_type>
const key_type* set<key_type>::const_iterator::operator->(void)
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "set", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "set", "-> on end iterator", jle::al::priority::error);

    return iiterator.operator->();
}

template <typename key_type>
const key_type&    set<key_type>::const_iterator::operator*(void)
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "set", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "set", "* on end iterator", jle::al::priority::error);

    return *iiterator;
}



template <typename key_type>
bool    set<key_type>::empty   ()  const
{
    return iset.empty();
}

template <typename key_type>
unsigned  set<key_type>::size    ()  const
{
    return unsigned(iset.size());
}




template <typename key_type>
void   set<key_type>::clear       ()
{
    internal_for_containers::register_container_size_change(registered_as);
    iset.clear();
}

template <typename key_type>
unsigned   set<key_type>::count       (const key_type& x) const
{
    return iset.count(x);
}









template <typename key_type>
typename set<key_type>::iterator
set<key_type>::find        (const key_type& x)
{
    typename set<key_type>::iterator it;
    it.iiterator = iset.find(x);
    if (it.iiterator != iset.end())
    {
        it.registered_owner = registered_as;
        it.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);
        it.i_end_iterator = iset.end();
    }
    else
    {
        it.registered_owner = 0;
        it.last_updated_container = 0;
    }

    return it;
}

template <typename key_type>
typename set<key_type>::const_iterator
set<key_type>::find        (const key_type& x) const
{
    typename set<key_type>::const_iterator it;
    it.iiterator = iset.find(x);
    if (it.iiterator != iset.end())
    {
        it.registered_owner = registered_as;
        it.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);
        it.i_end_iterator = iset.end();
    }
    else
    {
        it.registered_owner = 0;
        it.last_updated_container = 0;
    }

    return it;
}


template <typename key_type>
typename set<key_type>::iterator
set<key_type>::lower_bound (const key_type& x)
{
    typename set<key_type>::iterator it;
    it.iiterator = iset.lower_bound(x);
    if (it.iiterator != iset.end())
    {
        it.registered_owner = registered_as;
        it.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);
        it.i_end_iterator = iset.end();
    }
    else
    {
        it.registered_owner = 0;
        it.last_updated_container = 0;
    }

    return it;
}

template <typename key_type>
typename set<key_type>::const_iterator
set<key_type>::lower_bound (const key_type& x) const
{
    typename set<key_type>::const_iterator it;
    it.iiterator = iset.lower_bound(x);
    if (it.iiterator != iset.end())
    {
        it.registered_owner = registered_as;
        it.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);
        it.i_end_iterator = iset.end();
    }
    else
    {
        it.registered_owner = 0;
        it.last_updated_container = 0;
    }

    return it;
}

template <typename key_type>
typename set<key_type>::iterator
set<key_type>::upper_bound (const key_type& x)
{
    typename set<key_type>::iterator it;
    it.iiterator = iset.upper_bound(x);
    if (it.iiterator != iset.end())
    {
        it.registered_owner = registered_as;
        it.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);
        it.i_end_iterator = iset.end();
    }
    else
    {
        it.registered_owner = 0;
        it.last_updated_container = 0;
    }

    return it;
}


template <typename key_type>
typename set<key_type>::const_iterator
set<key_type>::upper_bound (const key_type& x) const
{
    typename set<key_type>::const_iterator it;
    it.iiterator = iset.upper_bound(x);
    if (it.iiterator != iset.end())
    {
        it.registered_owner = registered_as;
        it.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);
        it.i_end_iterator = iset.end();
    }
    else
    {
        it.registered_owner = 0;
        it.last_updated_container = 0;
    }

    return it;
}




//  modifiers
template <typename key_type>
unsigned   set<key_type>::erase       (const key_type& x)
{
    internal_for_containers::register_container_size_change(registered_as);
    return unsigned(iset.erase(x));
}


template <typename key_type>
typename set<key_type>::iterator  set<key_type>::erase  (iterator position)
{
    if (position.registered_owner  != registered_as)
        throw jle::alarm(JLE_HERE, "set", "erase with extrange iterator", jle::al::priority::error);
    if (position.iiterator  == iset.end())
        throw jle::alarm(JLE_HERE, "set", "erase over end iterator", jle::al::priority::error);
    if (position.is_valid() == false)
        throw jle::alarm(JLE_HERE, "set", "erase over iterator invalid", jle::al::priority::error);



    internal_for_containers::register_container_size_change(registered_as);

    typename set<key_type>::iterator result;
    result.iiterator = ++(position.iiterator);
    --(position.iiterator);
    iset.erase(position.iiterator);

    result.registered_owner = registered_as;
    result.i_end_iterator = iset.end();
    if (iset.empty() == false)
        result.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);
    else
        result.last_updated_container = 0;        //  not valid value

    return result;
}

template <typename key_type>
void   set<key_type>::erase  (const iterator& first, const iterator& last)
{
    if (first.registered_owner  != registered_as  ||  last.registered_owner  != registered_as)
        throw jle::alarm(JLE_HERE, "set", "erase with extrange iterator (first or second)", jle::al::priority::error);
    if (first.iiterator  == iset.end())
        throw jle::alarm(JLE_HERE, "set", "erase over end iterator (first)", jle::al::priority::error);
    if (first.is_valid() == false)
        throw jle::alarm(JLE_HERE, "set", "erase over iterator invalid (first)", jle::al::priority::error);

    typename set<key_type>::iterator result;

    internal_for_containers::register_container_size_change(registered_as);

    if (first == last)
    {
        iset.erase(first.iiterator);
    }
    else
    {
        //std::pair <int,int> pair_last;
        //pair_last = *iset.lower_bound(last.iiterator->second);
        //if ((iset.value_comp() (*first.iiterator, pair_last)) == true )

        if ((iset.value_comp() (*first.iiterator, *last.iiterator)) == true )
        {
            // True (sorted first ... last)
            iset.erase(first.iiterator, last.iiterator);
        }
        else
        {
            // False (NO sorted last ... first)
            throw jle::alarm(JLE_HERE, "set", "erase over iterators with invalid order (first > last)", jle::al::priority::error);
        }
    }
}

template <typename key_type>
std::pair<typename set<key_type>::iterator, bool> set<key_type>::insert (const key_type& x )
{
    internal_for_containers::register_container_size_change(registered_as);
    std::pair<typename std::set<key_type>::iterator, bool> presult = iset.insert(x);

    typename set<key_type>::iterator it;
    it.registered_owner = registered_as;
    it.iiterator = presult.first;
    it.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);
    it.i_end_iterator = iset.end();

    return std::make_pair(it, presult.second);
}








//---------------------------------------------------------------------------
}      //  namespace jle {


#endif
