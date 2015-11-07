//  generated on 2015-11-07 08:45:55.256
//  generated on 2015-11-06 23:59:59.000
//  this is a test


#include <cstdint>
#include <string>
#include "core/tuple.hpp"
#include "core/optional.hpp"
#include "core/dbl.h"
#include "core/cont/list.hpp"




namespace idl { namespace pr {
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    (record)   t_simple
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

struct t_simple {

    //  fields  ...................................

        int32_t      i;  
        std::string  j;  
        std::string  s;  
    //  ...........................................


    //  ctor
    t_simple() = delete;
    t_simple (
          int32_t _i,
          std::string _j,
          std::string _s
    );
    t_simple (const t_simple& ) = default;
    t_simple (      t_simple&&) = default;

    //  assign
    t_simple& operator=(const t_simple& ) = default;
    t_simple& operator=(      t_simple&&) = default;

    //  comparison
    bool operator==(const t_simple& ) const;
    bool operator< (const t_simple& ) const;
};
std::ostream& operator<< (std::ostream& o, const t_simple& t);



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    (record)   t_inline
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

struct t_inline {

    //  fields  ...................................

        int32_t      i;  
        std::string  s;  
    //  ...........................................


    //  ctor
    t_inline() = delete;
    t_inline (
          int32_t _i,
          std::string _s
    );
    t_inline (const t_inline& ) = default;
    t_inline (      t_inline&&) = default;

    //  assign
    t_inline& operator=(const t_inline& ) = default;
    t_inline& operator=(      t_inline&&) = default;

    //  comparison
    bool operator==(const t_inline& ) const;
    bool operator< (const t_inline& ) const;
};
std::ostream& operator<< (std::ostream& o, const t_inline& t);



namespace nested {
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    (record)   t_inline
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

struct t_inline {

    //  fields  ...................................

        int32_t      i;  
        std::string  s;  
    //  ...........................................


    //  ctor
    t_inline() = delete;
    t_inline (
          int32_t _i,
          std::string _s
    );
    t_inline (const t_inline& ) = default;
    t_inline (      t_inline&&) = default;

    //  assign
    t_inline& operator=(const t_inline& ) = default;
    t_inline& operator=(      t_inline&&) = default;

    //  comparison
    bool operator==(const t_inline& ) const;
    bool operator< (const t_inline& ) const;
};
std::ostream& operator<< (std::ostream& o, const t_inline& t);




}   //  namespace nested {



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    (record)   t_inline2
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

struct t_inline2 {

    //  fields  ...................................

        int32_t      i;    
        int64_t      i64;  
        std::string  s;    
    //  ...........................................


    //  ctor
    t_inline2() = delete;
    t_inline2 (
          int32_t _i,
          int64_t _i64,
          std::string _s
    );
    t_inline2 (const t_inline2& ) = default;
    t_inline2 (      t_inline2&&) = default;

    //  assign
    t_inline2& operator=(const t_inline2& ) = default;
    t_inline2& operator=(      t_inline2&&) = default;

    //  comparison
    bool operator==(const t_inline2& ) const;
    bool operator< (const t_inline2& ) const;
};
std::ostream& operator<< (std::ostream& o, const t_inline2& t);



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    (tuple)  t_tuple
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

struct t_tuple {

    //  fields  ...................................
    jle::tuple<
                int32_t      ,  
                std::string  
    >  tuple;
    //  ...........................................


    jle::tuple<
                int32_t      ,  
                std::string  
    >&
    operator*() { return  tuple;  }

    //  ctor
    t_tuple() = delete;
    t_tuple (
          int32_t p0,
          std::string p1
    );
    t_tuple (const t_tuple& ) = default;
    t_tuple (      t_tuple&&) = default;

    //  assign
    t_tuple& operator=(const t_tuple& ) = default;
    t_tuple& operator=(      t_tuple&&) = default;

    //  comparison
    bool operator==(const t_tuple& ) const;
    bool operator< (const t_tuple& ) const;
};
std::ostream& operator<< (std::ostream& o, const t_tuple& t);



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    (tuple)  t_tuple2
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

struct t_tuple2 {

    //  fields  ...................................
    jle::tuple<
                int32_t      ,  
                std::string  
    >  tuple;
    //  ...........................................


    jle::tuple<
                int32_t      ,  
                std::string  
    >&
    operator*() { return  tuple;  }

    //  ctor
    t_tuple2() = delete;
    t_tuple2 (
          int32_t p0,
          std::string p1
    );
    t_tuple2 (const t_tuple2& ) = default;
    t_tuple2 (      t_tuple2&&) = default;

    //  assign
    t_tuple2& operator=(const t_tuple2& ) = default;
    t_tuple2& operator=(      t_tuple2&&) = default;

    //  comparison
    bool operator==(const t_tuple2& ) const;
    bool operator< (const t_tuple2& ) const;
};
std::ostream& operator<< (std::ostream& o, const t_tuple2& t);



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    (enumeration)  t_color
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

enum class t_color {
                red     ,  
                blue    ,  
                green   ,  
                orange  ,  
                pink    ,  
                white   ,  
                black   
};
std::ostream& operator<< (std::ostream& os, const t_color& t);



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    (record)   t_example
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

struct t_example {

    //  fields  ...................................

        int32_t                     int_field;     
        std::string                 string_field;  
        jle::optional<std::string>  opt_string;    
        jle::optional<jle::dbl>     rec_float;     
        std::string                 def_string0;   
        std::string                 def_string1;   
        jle::list<std::string>      string_list;   
        jle::list<int32_t>          int_list;      
    //  ...........................................


    //  ctor
    t_example() = delete;
    t_example (
          int32_t _int_field,
          std::string _string_field,
          jle::optional<std::string> _opt_string,
          jle::optional<jle::dbl> _rec_float,
          std::string _def_string0,
          std::string _def_string1,
          jle::list<std::string> _string_list,
          jle::list<int32_t> _int_list
    );
    t_example (const t_example& ) = default;
    t_example (      t_example&&) = default;

    //  assign
    t_example& operator=(const t_example& ) = default;
    t_example& operator=(      t_example&&) = default;

    //  comparison
    bool operator==(const t_example& ) const;
    bool operator< (const t_example& ) const;
};
std::ostream& operator<< (std::ostream& o, const t_example& t);




}  }   //  namespace idl { namespace pr {








