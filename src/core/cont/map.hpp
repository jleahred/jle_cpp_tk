#ifndef JLE_CONTAINERS_MAP_GUARD
#define JLE_CONTAINERS_MAP_GUARD




#include <map>

#include "container_support.h"
#include "core/alarm.h"



namespace jle {
//---------------------------------------------------------------------------



template<typename key_type, typename T>
class map
{
public:

        class iterator   {
        public:
            iterator(const iterator&) = default;
            iterator(iterator&&) = default;
            iterator& operator=(const iterator&) = default;
            iterator& operator=(iterator&&) = default;

            friend class jle::map<key_type, T>;
            friend class jle::map<key_type, T>::const_iterator;
            iterator() : registered_owner(0), last_updated_container(0)  {};
            iterator&   operator++();
            iterator    operator++(int);
            bool        operator==(const iterator& it) const;
            bool        operator!=(const iterator& it) const;

            std::pair<const key_type, T>* operator->(void);
            std::pair<const key_type, T>& operator*(void);

        private:
            typename std::map<key_type, T>::iterator iiterator;
            typename std::map<key_type, T>::iterator i_end_iterator;
            int                                      registered_owner;
            int                                      last_updated_container;

            bool is_valid (void) const;
        };


        class const_iterator  {
        public:
            const_iterator(const const_iterator&) = default;
            const_iterator(const_iterator&&) = default;
            const_iterator& operator=(const const_iterator&) = default;
            const_iterator& operator=(const_iterator&&) = default;

            friend class jle::map<key_type, T>;
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

            const_iterator&     operator++();
            const_iterator      operator++(int);
            bool                operator==(const const_iterator& it) const;
            bool                operator!=(const const_iterator& it) const;

            const std::pair<const key_type, T>* operator->(void);
            const std::pair<const key_type, T>& operator*(void);

        private:
            typename std::map<key_type, T>::const_iterator  iiterator;
            typename std::map<key_type, T>::const_iterator  i_end_iterator;
            int                                             registered_owner;
            int                                             last_updated_container;

            bool is_valid (void) const;
        };





    map()  : registered_as(internal_for_containers::register_container(true)) {};
    ~map()
    {
        try{
            internal_for_containers::unregister_container(registered_as);
        } catch(...){
            jle::alarm_msg(jle::alarm(JLE_HERE, "exception on destructor", "catched exception on destructor", jle::al::priority::error));
        }
    };

    map(const map<key_type, T>& m);
    map(map<key_type, T>&& m) = default;
    map<key_type, T>& operator=(const map<key_type, T>& m);
    map<key_type, T>& operator=(map<key_type, T>&& m) = default;





    //  capacity
    bool            empty   ()  const;
    unsigned        size    ()  const;


    //  access
    T& operator[] (const key_type& x);


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
    std::pair<iterator, bool>  insert   (const std::pair<key_type, T>& x );

private:
    std::map<key_type, T>   imap;
    int                     registered_as;  //  can't be constant. assign operator
};





//--------------------------------------------------------------------------------
//        IMPLEMENTATION
//--------------------------------------------------------------------------------


template <typename key_type, typename T>
map<key_type, T>::map(const map<key_type, T>& m)
  : registered_as (internal_for_containers::register_container( imap.empty() ? true : false))
{
    imap = m.imap;
    if(imap.empty() == false)
        internal_for_containers::register_container_size_change(registered_as);
}

template <typename key_type, typename T>
map<key_type, T>& map<key_type, T>::operator=(const map<key_type, T>& m)
{
    imap = m.imap;
    registered_as = internal_for_containers::register_container( imap.empty() ? true : false);
    if(imap.empty() == false)
        internal_for_containers::register_container_size_change(registered_as);
    return *this;
}



//  iterators       ---------------------------------------------------

template <typename key_type, typename T>
bool map<key_type, T>::iterator::is_valid (void) const
{
    if (last_updated_container==0  ||  internal_for_containers::get_registered_container_last_size_change(registered_owner) != last_updated_container)
        return false;
    else
        return true;
}

template <typename key_type, typename T>
typename map<key_type, T>::iterator  map<key_type, T>::begin       ()
{
    typename map<key_type, T>::iterator it;
    it.iiterator = imap.begin();

    it.registered_owner = registered_as;
    it.i_end_iterator = imap.end();

    if (imap.empty()==false)
        it.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);
    else
        it.last_updated_container = 0;        //  no valid value

    return it;
}


template <typename key_type, typename T>
typename map<key_type, T>::iterator  map<key_type, T>::end       ()
{
    typename map<key_type, T>::iterator it;
    it.iiterator = imap.end();
    it.registered_owner = registered_as;
    it.i_end_iterator = imap.end();
    it.last_updated_container = 0;    //  no value
    return it;
}




template <typename key_type, typename T>
typename map<key_type, T>::iterator&  map<key_type, T>::iterator::operator++()
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "map", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "map", "++ on end iterator", jle::al::priority::error);

    ++iiterator;
    return *this;
}

template <typename key_type, typename T>
typename map<key_type, T>::iterator  map<key_type, T>::iterator::operator++(int)
{
    typename map<key_type, T>::iterator  result = *this;

    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "map", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "map", "++ on end iterator", jle::al::priority::error);

    ++iiterator;
    return result;
}

template <typename key_type, typename T>
bool  map<key_type, T>::iterator::operator==(const map<key_type, T>::iterator& it) const
{
    if(registered_owner!=0  &&  it.registered_owner != 0   &&  registered_owner !=  it.registered_owner)
        throw jle::alarm(JLE_HERE, "map", "on different owners", jle::al::priority::error);

    return it.iiterator == iiterator;
}

template <typename key_type, typename T>
bool  map<key_type, T>::iterator::operator!=(const iterator& it) const
{
    return !operator==(it);
}


template <typename key_type, typename T>
std::pair<const key_type, T>* map<key_type, T>::iterator::operator->(void)
{
    if (last_updated_container==0
            ||  internal_for_containers::get_registered_container_last_size_change(registered_owner) != last_updated_container
            ||  internal_for_containers::get_registered_container_last_size_change(registered_owner) == 0)
        throw jle::alarm(JLE_HERE, "map", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "map", "-> on end iterator", jle::al::priority::error);

    return iiterator.operator->();
}

template <typename key_type, typename T>
std::pair<const key_type, T>&   map<key_type, T>::iterator::operator*(void)
{
    if (last_updated_container==0
            ||  internal_for_containers::get_registered_container_last_size_change(registered_owner) != last_updated_container
            ||  internal_for_containers::get_registered_container_last_size_change(registered_owner) == 0)
        throw jle::alarm(JLE_HERE, "map", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "map", "* on end iterator", jle::al::priority::error);

    return *iiterator;
}











//  const_iterators       ---------------------------------------------------

template <typename key_type, typename T>
bool map<key_type, T>::const_iterator::is_valid (void) const
{
    if (last_updated_container==0  ||  internal_for_containers::get_registered_container_last_size_change(registered_owner) != last_updated_container)
        return false;
    else
        return true;
}


template <typename key_type, typename T>
typename map<key_type, T>::const_iterator  map<key_type, T>::cbegin       () const
{
    typename map<key_type, T>::const_iterator it;
    it.iiterator = imap.begin();

    it.registered_owner = registered_as;
    it.i_end_iterator = imap.end();

    if (imap.empty()==false)
        it.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);
    else
        it.last_updated_container = 0;        //  no valid value

    return it;
}


template <typename key_type, typename T>
typename map<key_type, T>::const_iterator  map<key_type, T>::cend       ()  const
{
    typename map<key_type, T>::const_iterator it;
    it.registered_owner = registered_as;
    it.i_end_iterator = imap.end();
    it.iiterator = imap.end();
    it.last_updated_container = 0;    //  no value
    return it;
}




template <typename key_type, typename T>
typename map<key_type, T>::const_iterator&  map<key_type, T>::const_iterator::operator++()
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "map", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "map", "++ on end iterator", jle::al::priority::error);

    ++iiterator;
    return *this;
}

template <typename key_type, typename T>
typename map<key_type, T>::const_iterator  map<key_type, T>::const_iterator::operator++(int)
{
    typename map<key_type, T>::const_iterator  result = *this;

    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "map", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "map", "++ on end iterator", jle::al::priority::error);

    ++iiterator;
    return result;
}

template <typename key_type, typename T>
bool  map<key_type, T>::const_iterator::operator==(const const_iterator& it) const
{
    if(registered_owner!=0  &&  it.registered_owner != 0   &&  registered_owner !=  it.registered_owner)
        throw jle::alarm(JLE_HERE, "map", "on different owners", jle::al::priority::error);

    return it.iiterator == iiterator;
}

template <typename key_type, typename T>
bool  map<key_type, T>::const_iterator::operator!=(const const_iterator& it) const
{
    return !operator==(it);
}


template <typename key_type, typename T>
const std::pair<const key_type, T>* map<key_type, T>::const_iterator::operator->(void)
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "map", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "map", "-> on end iterator", jle::al::priority::error);

    return iiterator.operator->();
}

template <typename key_type, typename T>
const std::pair<const key_type, T>&    map<key_type, T>::const_iterator::operator*(void)
{
    if (is_valid() == false)
        throw jle::alarm(JLE_HERE, "map", "iterator not valid", jle::al::priority::error);

    if (i_end_iterator == iiterator)
        throw jle::alarm(JLE_HERE, "map", "* on end iterator", jle::al::priority::error);

    return *iiterator;
}



template <typename key_type, typename T>
bool    map<key_type, T>::empty   ()  const
{
    return imap.empty();
}

template <typename key_type, typename T>
unsigned  map<key_type, T>::size    ()  const
{
    return unsigned(imap.size());
}


template <typename key_type, typename T>
T&  map<key_type, T>::operator[]  (const key_type& x)
{
    if (find(x) == end())
        internal_for_containers::register_container_size_change(registered_as);
    return imap[x];
}


template <typename key_type, typename T>
void   map<key_type, T>::clear       ()
{
    internal_for_containers::register_container_size_change(registered_as);
    imap.clear();
}

template <typename key_type, typename T>
unsigned   map<key_type, T>::count       (const key_type& x) const
{
    return imap.count(x);
}









template <typename key_type, typename T>
typename map<key_type, T>::iterator
map<key_type, T>::find        (const key_type& x)
{
    typename map<key_type, T>::iterator it;
    it.iiterator = imap.find(x);
    if (it.iiterator != imap.end())
    {
        it.registered_owner = registered_as;
        it.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);
        it.i_end_iterator = imap.end();
    }
    else
    {
        it.registered_owner = 0;
        it.last_updated_container = 0;
    }

    return it;
}

template <typename key_type, typename T>
typename map<key_type, T>::const_iterator
map<key_type, T>::find        (const key_type& x) const
{
    typename map<key_type, T>::const_iterator it;
    it.iiterator = imap.find(x);
    if (it.iiterator != imap.end())
    {
        it.registered_owner = registered_as;
        it.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);
        it.i_end_iterator = imap.end();
    }
    else
    {
        it.registered_owner = 0;
        it.last_updated_container = 0;
    }

    return it;
}


template <typename key_type, typename T>
typename map<key_type, T>::iterator
map<key_type, T>::lower_bound (const key_type& x)
{
    typename map<key_type, T>::iterator it;
    it.iiterator = imap.lower_bound(x);
    if (it.iiterator != imap.end())
    {
        it.registered_owner = registered_as;
        it.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);
        it.i_end_iterator = imap.end();
    }
    else
    {
        it.registered_owner = 0;
        it.last_updated_container = 0;
    }

    return it;
}

template <typename key_type, typename T>
typename map<key_type, T>::const_iterator
map<key_type, T>::lower_bound (const key_type& x) const
{
    typename map<key_type, T>::const_iterator it;
    it.iiterator = imap.lower_bound(x);
    if (it.iiterator != imap.end())
    {
        it.registered_owner = registered_as;
        it.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);
        it.i_end_iterator = imap.end();
    }
    else
    {
        it.registered_owner = 0;
        it.last_updated_container = 0;
    }

    return it;
}

template <typename key_type, typename T>
typename map<key_type, T>::iterator
map<key_type, T>::upper_bound (const key_type& x)
{
    typename map<key_type, T>::iterator it;
    it.iiterator = imap.upper_bound(x);
    if (it.iiterator != imap.end())
    {
        it.registered_owner = registered_as;
        it.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);
        it.i_end_iterator = imap.end();
    }
    else
    {
        it.registered_owner = 0;
        it.last_updated_container = 0;
    }

    return it;
}


template <typename key_type, typename T>
typename map<key_type, T>::const_iterator
map<key_type, T>::upper_bound (const key_type& x) const
{
    typename map<key_type, T>::const_iterator it;
    it.iiterator = imap.upper_bound(x);
    if (it.iiterator != imap.end())
    {
        it.registered_owner = registered_as;
        it.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);
        it.i_end_iterator = imap.end();
    }
    else
    {
        it.registered_owner = 0;
        it.last_updated_container = 0;
    }

    return it;
}




//  modifiers
template <typename key_type, typename T>
unsigned   map<key_type, T>::erase       (const key_type& x)
{
    internal_for_containers::register_container_size_change(registered_as);
    return unsigned(imap.erase(x));
}


template <typename key_type, typename T>
typename map<key_type, T>::iterator  map<key_type, T>::erase  (iterator position)
{
    if (position.registered_owner  != registered_as)
        throw jle::alarm(JLE_HERE, "map", "erase with extrange iterator", jle::al::priority::error);
    if (position.iiterator  == imap.end())
        throw jle::alarm(JLE_HERE, "map", "erase over end iterator", jle::al::priority::error);
    if (position.is_valid() == false)
        throw jle::alarm(JLE_HERE, "map", "erase over iterator invalid", jle::al::priority::error);



    internal_for_containers::register_container_size_change(registered_as);

    typename map<key_type, T>::iterator result;
    result.iiterator = ++(position.iiterator);
    --(position.iiterator);
    imap.erase(position.iiterator);

    result.registered_owner = registered_as;
    result.i_end_iterator = imap.end();
    if (imap.empty()==false)
        result.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);
    else
        result.last_updated_container = 0;        //  no valid value

    return result;
}

template <typename key_type, typename T>
void   map<key_type, T>::erase  (const iterator& first, const iterator& last)
{
    if (first.registered_owner  != registered_as  ||  last.registered_owner  != registered_as)
        throw jle::alarm(JLE_HERE, "map", "erase with extrange iterator (first or second)", jle::al::priority::error);
    if (first.iiterator  == imap.end())
        throw jle::alarm(JLE_HERE, "map", "erase over end iterator (first)", jle::al::priority::error);
    if (first.is_valid() == false)
        throw jle::alarm(JLE_HERE, "map", "erase over iterator invalid (first)", jle::al::priority::error);

    typename map<key_type, T>::iterator result;

    internal_for_containers::register_container_size_change(registered_as);

    if (first == last)
    {
        imap.erase(first.iiterator);
    }
    else
    {
        //std::pair <int,int> pair_last;
        //pair_last = *imap.lower_bound(last.iiterator->second);
        //if ((imap.value_comp() (*first.iiterator, pair_last)) == true )

        if ((imap.value_comp() (*first.iiterator, *last.iiterator)) == true )
        {
            // True (sorted first ... last)
            imap.erase(first.iiterator, last.iiterator);
        }
        else
        {
            // False (NO sorted last ... first)
            throw jle::alarm(JLE_HERE, "map", "erase over iterators with invalid order (first > last)", jle::al::priority::error);
        }
    }
}

template <typename key_type, typename T>
std::pair<typename map<key_type, T>::iterator, bool> map<key_type, T>::insert (const std::pair<key_type, T>& x )
{
    internal_for_containers::register_container_size_change(registered_as);
    std::pair<typename std::map<key_type, T>::iterator, bool> presult = imap.insert(x);

    typename map<key_type, T>::iterator it;
    it.registered_owner = registered_as;
    it.iiterator = presult.first;
    it.last_updated_container = internal_for_containers::get_registered_container_last_size_change(registered_as);
    it.i_end_iterator = imap.end();

    return std::make_pair(it, presult.second);
}








//---------------------------------------------------------------------------
}      //  namespace jle {


#endif
