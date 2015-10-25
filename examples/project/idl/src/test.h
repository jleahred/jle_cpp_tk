//  generated on 2015-10-25 17:25:36.755
//  generated on 2015-10-24 23:59:59.000
//  this is a test
#include <cstdint>
#include <string>
  
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    t_simple
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
struct t_simple {
    //  fields  ·································
        int32_t      i;  
        std::string  j;  
        std::string  s;  
        
    //  ·········································
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
//    t_inline
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
struct t_inline {
    //  fields  ·································
        int32_t      i;  
        std::string  s;  
        
    //  ·········································
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
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    t_inline2
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
struct t_inline2 {
    //  fields  ·································
        int32_t      i;    
        int64_t      i64;  
        std::string  s;    
        
    //  ·········································
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
