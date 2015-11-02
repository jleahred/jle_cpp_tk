#include "test.h"



namespace idl { namespace pr {
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    (record)   t_simple
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

t_simple::t_simple (
          int32_t _i,
          std::string _j,
          std::string _s
    )  :
          i(_i),
          j(_j),
          s(_s)
    {}


//  comparison
bool t_simple::operator==(const t_simple& r) const
{
    if(
        i == r.i  &&
        j == r.j  &&
        s == r.s
    )     return true;
    else  return false;
}

bool t_simple::operator< (const t_simple& r) const
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

std::ostream& operator<< (std::ostream& os, const t_simple& t)
{
    os  << " t_simple { "
           << "i: " << t.i << ", "
           << "j: " << t.j << ", "
           << "s: " << t.s
        << " }";
    return os;
}



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    (record)   t_inline
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

t_inline::t_inline (
          int32_t _i,
          std::string _s
    )  :
          i(_i),
          s(_s)
    {}


//  comparison
bool t_inline::operator==(const t_inline& r) const
{
    if(
        i == r.i  &&
        s == r.s
    )     return true;
    else  return false;
}

bool t_inline::operator< (const t_inline& r) const
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

std::ostream& operator<< (std::ostream& os, const t_inline& t)
{
    os  << " t_inline { "
           << "i: " << t.i << ", "
           << "s: " << t.s
        << " }";
    return os;
}



namespace nested {
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    (record)   t_inline
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

t_inline::t_inline (
          int32_t _i,
          std::string _s
    )  :
          i(_i),
          s(_s)
    {}


//  comparison
bool t_inline::operator==(const t_inline& r) const
{
    if(
        i == r.i  &&
        s == r.s
    )     return true;
    else  return false;
}

bool t_inline::operator< (const t_inline& r) const
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

std::ostream& operator<< (std::ostream& os, const t_inline& t)
{
    os  << " t_inline { "
           << "i: " << t.i << ", "
           << "s: " << t.s
        << " }";
    return os;
}




}   //  namespace nested {



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    (record)   t_inline2
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

t_inline2::t_inline2 (
          int32_t _i,
          int64_t _i64,
          std::string _s
    )  :
          i(_i),
          i64(_i64),
          s(_s)
    {}


//  comparison
bool t_inline2::operator==(const t_inline2& r) const
{
    if(
        i == r.i  &&
        i64 == r.i64  &&
        s == r.s
    )     return true;
    else  return false;
}

bool t_inline2::operator< (const t_inline2& r) const
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

std::ostream& operator<< (std::ostream& os, const t_inline2& t)
{
    os  << " t_inline2 { "
           << "i: " << t.i << ", "
           << "i64: " << t.i64 << ", "
           << "s: " << t.s
        << " }";
    return os;
}



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    (tuple)  t_tuple
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//  ctor
t_tuple::t_tuple(
          int32_t p0,
          std::string p1
)  :  tuple{ std::make_tuple (
              p0,  
              p1   
        )} {};


//  comparison
bool t_tuple::operator==(const t_tuple& rhs) const
{
  return this->tuple == rhs.tuple;
}
bool t_tuple::operator< (const t_tuple& rhs) const
{
  return this->tuple < rhs.tuple;
}

std::ostream& operator<< (std::ostream& os, const t_tuple& t)
{
    using ::operator<<;
    os << t.tuple;
    return os;
}



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    (tuple)  t_tuple2
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//  ctor
t_tuple2::t_tuple2(
          int32_t p0,
          std::string p1
)  :  tuple{ std::make_tuple (
              p0,  
              p1   
        )} {};


//  comparison
bool t_tuple2::operator==(const t_tuple2& rhs) const
{
  return this->tuple == rhs.tuple;
}
bool t_tuple2::operator< (const t_tuple2& rhs) const
{
  return this->tuple < rhs.tuple;
}

std::ostream& operator<< (std::ostream& os, const t_tuple2& t)
{
    using ::operator<<;
    os << t.tuple;
    return os;
}



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    (enumeration)  t_color
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

std::ostream& operator<< (std::ostream& os, const t_color& t)
{
    os << "en(";
    switch(t) {
           case t_color::red:
                      os << "red";
                      break;
           case t_color::blue:
                      os << "blue";
                      break;
           case t_color::green:
                      os << "green";
                      break;
           case t_color::orange:
                      os << "orange";
                      break;
           case t_color::pink:
                      os << "pink";
                      break;
           case t_color::white:
                      os << "white";
                      break;
           case t_color::black:
                      os << "black";
                      break;
           default:
                      throw "ERROR";
    };

    os << ")";
    return os;
}




}  }   //  namespace idl { namespace pr {








