//  generated on 2015-11-09 18:14:00.178
//  generated on 2015-11-08 23:59:59.000
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
//    (record)   t_type_modifiers
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

struct t_type_modifiers {

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
    t_type_modifiers() = delete;
    t_type_modifiers (
          int32_t _int_field,
          std::string _string_field,
          jle::optional<std::string> _opt_string,
          jle::optional<jle::dbl> _rec_float,
          std::string _def_string0,
          std::string _def_string1,
          jle::list<std::string> _string_list,
          jle::list<int32_t> _int_list
    );
    t_type_modifiers (const t_type_modifiers& ) = default;
    t_type_modifiers (      t_type_modifiers&&) = default;

    //  assign
    t_type_modifiers& operator=(const t_type_modifiers& ) = default;
    t_type_modifiers& operator=(      t_type_modifiers&&) = default;

    //  comparison
    bool operator==(const t_type_modifiers& ) const;
    bool operator< (const t_type_modifiers& ) const;
};
std::ostream& operator<< (std::ostream& o, const t_type_modifiers& t);



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    (union)   t_union
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

struct t_union {

    enum class  en_field {
        s,  
        i,  
        d   
    };

    //  fields  ...................................

        const  jle::optional<std::string>  s;  
        const  jle::optional<int32_t>      i;  
        const  jle::optional<jle::dbl>     d;  

        const  en_field   field;

    //  ...........................................


    //  ctor
    t_union() = delete;
    t_union (const std::string& p);
    t_union (const int32_t& p);
    t_union (const jle::dbl& p);

    t_union (const t_union& ) = default;
    t_union (      t_union&&) = default;

    //  assign
    t_union& operator=(const t_union& ) = default;
    t_union& operator=(      t_union&&) = default;

    //  comparison
    bool operator==(const t_union& ) const;
    bool operator< (const t_union& ) const;
};
std::ostream& operator<< (std::ostream& o, const t_union& t);




}  }   //  namespace idl { namespace pr {



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    (record)   t_composed
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

struct t_composed {

    //  fields  ...................................

        int32_t            i2;    
        idl::pr::t_simple  comp;  
    //  ...........................................


    //  ctor
    t_composed() = delete;
    t_composed (
          int32_t _i2,
          idl::pr::t_simple _comp
    );
    t_composed (const t_composed& ) = default;
    t_composed (      t_composed&&) = default;

    //  assign
    t_composed& operator=(const t_composed& ) = default;
    t_composed& operator=(      t_composed&&) = default;

    //  comparison
    bool operator==(const t_composed& ) const;
    bool operator< (const t_composed& ) const;
};
std::ostream& operator<< (std::ostream& o, const t_composed& t);








