//  generated on 2015-10-28 23:45:49.841
//  generated on 2015-10-28 00:00:00.000
//  this is a test


#include <cstdint>
#include <string>


namespace idl { namespace pr {
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    jt_simple
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

struct jt_simple {

    //  fields  ·································

        int32_t      i;  
        std::string  j;  
        std::string  s;  
        
    //  ·········································


    //  ctor
    jt_simple() = delete;
    jt_simple (
          int32_t _i,
          std::string _j,
          std::string _s
    );
    jt_simple (const jt_simple& ) = default;
    jt_simple (      jt_simple&&) = default;

    //  assign
    jt_simple& operator=(const jt_simple& ) = default;
    jt_simple& operator=(      jt_simple&&) = default;

    //  comparison
    bool operator==(const jt_simple& ) const;
    bool operator< (const jt_simple& ) const;
};
std::ostream& operator<< (std::ostream& o, const jt_simple& t);




//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    jt_inline
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

struct jt_inline {

    //  fields  ·································

        int32_t      i;  
        std::string  s;  
        
    //  ·········································


    //  ctor
    jt_inline() = delete;
    jt_inline (
          int32_t _i,
          std::string _s
    );
    jt_inline (const jt_inline& ) = default;
    jt_inline (      jt_inline&&) = default;

    //  assign
    jt_inline& operator=(const jt_inline& ) = default;
    jt_inline& operator=(      jt_inline&&) = default;

    //  comparison
    bool operator==(const jt_inline& ) const;
    bool operator< (const jt_inline& ) const;
};
std::ostream& operator<< (std::ostream& o, const jt_inline& t);




namespace nested {
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    jt_inline
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

struct jt_inline {

    //  fields  ·································

        int32_t      i;  
        std::string  s;  
        
    //  ·········································


    //  ctor
    jt_inline() = delete;
    jt_inline (
          int32_t _i,
          std::string _s
    );
    jt_inline (const jt_inline& ) = default;
    jt_inline (      jt_inline&&) = default;

    //  assign
    jt_inline& operator=(const jt_inline& ) = default;
    jt_inline& operator=(      jt_inline&&) = default;

    //  comparison
    bool operator==(const jt_inline& ) const;
    bool operator< (const jt_inline& ) const;
};
std::ostream& operator<< (std::ostream& o, const jt_inline& t);





} // namepace  nested {


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    jt_inline2
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

struct jt_inline2 {

    //  fields  ·································

        int32_t      i;    
        int64_t      i64;  
        std::string  s;    
        
    //  ·········································


    //  ctor
    jt_inline2() = delete;
    jt_inline2 (
          int32_t _i,
          int64_t _i64,
          std::string _s
    );
    jt_inline2 (const jt_inline2& ) = default;
    jt_inline2 (      jt_inline2&&) = default;

    //  assign
    jt_inline2& operator=(const jt_inline2& ) = default;
    jt_inline2& operator=(      jt_inline2&&) = default;

    //  comparison
    bool operator==(const jt_inline2& ) const;
    bool operator< (const jt_inline2& ) const;
};
std::ostream& operator<< (std::ostream& o, const jt_inline2& t);





} // namepace  idl { namespace pr {



