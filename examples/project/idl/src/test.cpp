#include "test.h"

#include "core/tuple.hpp"


namespace idl { namespace pr {
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    jt_simple
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

jt_simple::jt_simple (
          int32_t _i,
          std::string _j,
          std::string _s
    )  :
          i(_i),
          j(_j),
          s(_s)
    {}


//  comparison
bool jt_simple::operator==(const jt_simple& r) const
{
    if(
        i == r.i  &&
        j == r.j  &&
        s == r.s
    )     return true;
    else  return false;
}

bool jt_simple::operator< (const jt_simple& r) const
{
    if(std::tie(
          i,
          j,
          s)
       <
       std::tie(
          r.i,
          r.j,
          r.s)
    )     return true;
    else  return false;

}

std::ostream& operator<< (std::ostream& os, const jt_simple& t)
{
    os  << " jt_simple { " 
           << "i: " << t.i << ", "
           << "j: " << t.j << ", "
           << "s: " << t.s
        << " }";
    return os;
}




//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    jt_inline
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

jt_inline::jt_inline (
          int32_t _i,
          std::string _s
    )  :
          i(_i),
          s(_s)
    {}


//  comparison
bool jt_inline::operator==(const jt_inline& r) const
{
    if(
        i == r.i  &&
        s == r.s
    )     return true;
    else  return false;
}

bool jt_inline::operator< (const jt_inline& r) const
{
    if(std::tie(
          i,
          s)
       <
       std::tie(
          r.i,
          r.s)
    )     return true;
    else  return false;

}

std::ostream& operator<< (std::ostream& os, const jt_inline& t)
{
    os  << " jt_inline { " 
           << "i: " << t.i << ", "
           << "s: " << t.s
        << " }";
    return os;
}




namespace nested {
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    jt_inline
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

jt_inline::jt_inline (
          int32_t _i,
          std::string _s
    )  :
          i(_i),
          s(_s)
    {}


//  comparison
bool jt_inline::operator==(const jt_inline& r) const
{
    if(
        i == r.i  &&
        s == r.s
    )     return true;
    else  return false;
}

bool jt_inline::operator< (const jt_inline& r) const
{
    if(std::tie(
          i,
          s)
       <
       std::tie(
          r.i,
          r.s)
    )     return true;
    else  return false;

}

std::ostream& operator<< (std::ostream& os, const jt_inline& t)
{
    os  << " jt_inline { " 
           << "i: " << t.i << ", "
           << "s: " << t.s
        << " }";
    return os;
}





}   //  namespace nested {



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    jt_inline2
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

jt_inline2::jt_inline2 (
          int32_t _i,
          int64_t _i64,
          std::string _s
    )  :
          i(_i),
          i64(_i64),
          s(_s)
    {}


//  comparison
bool jt_inline2::operator==(const jt_inline2& r) const
{
    if(
        i == r.i  &&
        i64 == r.i64  &&
        s == r.s
    )     return true;
    else  return false;
}

bool jt_inline2::operator< (const jt_inline2& r) const
{
    if(std::tie(
          i,
          i64,
          s)
       <
       std::tie(
          r.i,
          r.i64,
          r.s)
    )     return true;
    else  return false;

}

std::ostream& operator<< (std::ostream& os, const jt_inline2& t)
{
    os  << " jt_inline2 { " 
           << "i: " << t.i << ", "
           << "i64: " << t.i64 << ", "
           << "s: " << t.s
        << " }";
    return os;
}





}  }   //  namespace idl { namespace pr {






