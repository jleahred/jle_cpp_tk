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



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    (record)   t_type_modifiers
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

t_type_modifiers::t_type_modifiers (
          int32_t _int_field,
          std::string _string_field,
          jle::optional<std::string> _opt_string,
          jle::optional<jle::dbl> _rec_float,
          std::string _def_string0,
          std::string _def_string1,
          jle::list<std::string> _string_list,
          jle::list<int32_t> _int_list
    )  :
          int_field(_int_field),
          string_field(_string_field),
          opt_string(_opt_string),
          rec_float(_rec_float),
          def_string0(_def_string0),
          def_string1(_def_string1),
          string_list(_string_list),
          int_list(_int_list)
    {}


//  comparison
bool t_type_modifiers::operator==(const t_type_modifiers& r) const
{
    if(
        int_field == r.int_field  &&
        string_field == r.string_field  &&
        opt_string == r.opt_string  &&
        rec_float == r.rec_float  &&
        def_string0 == r.def_string0  &&
        def_string1 == r.def_string1  &&
        string_list == r.string_list  &&
        int_list == r.int_list
    )     return true;
    else  return false;
}

bool t_type_modifiers::operator< (const t_type_modifiers& r) const
{
    if(std::tie(
          int_field,
          string_field,
          opt_string,
          rec_float,
          def_string0,
          def_string1,
          string_list,
          int_list)
       <
       std::tie(
          r.int_field,
          r.string_field,
          r.opt_string,
          r.rec_float,
          r.def_string0,
          r.def_string1,
          r.string_list,
          r.int_list)
    )     return true;
    else  return false;

}

std::ostream& operator<< (std::ostream& os, const t_type_modifiers& t)
{
    os  << " t_type_modifiers { "
           << "int_field: " << t.int_field << ", "
           << "string_field: " << t.string_field << ", "
           << "opt_string: " << t.opt_string << ", "
           << "rec_float: " << t.rec_float << ", "
           << "def_string0: " << t.def_string0 << ", "
           << "def_string1: " << t.def_string1 << ", "
           << "string_list: " << t.string_list << ", "
           << "int_list: " << t.int_list
        << " }";
    return os;
}




//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    (union)   t_union
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

t_union::t_union (const std::string& p)
      : s(p), field(en_field::s) {};

t_union::t_union (const int32_t& p)
      : i(p), field(en_field::i) {};

t_union::t_union (const jle::dbl& p)
      : d(p), field(en_field::d) {};




//  comparison
bool t_union::operator==(const t_union& r) const
{
    if(
        s == r.s  &&
        i == r.i  &&
        d == r.d
    )     return true;
    else  return false;
}

bool t_union::operator< (const t_union& r) const
{
    if(std::tie(
          s,
          i,
          d)
       <
       std::tie(
          r.s,
          r.i,
          r.d)
    )     return true;
    else  return false;

}

std::ostream& operator<< (std::ostream& os, const t_union& t)
{
    os  << " t_union { ";
           if(false) {;}
           else if(t.s)  os << "s: " << t.s;
           else if(t.i)  os << "i: " << t.i;
           else if(t.d)  os << "d: " << t.d;
    os  << " }";
    return os;
}




}  }   //  namespace idl { namespace pr {



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//    (record)   t_composed
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

t_composed::t_composed (
          int32_t _i2,
          idl::pr::t_simple _comp
    )  :
          i2(_i2),
          comp(_comp)
    {}


//  comparison
bool t_composed::operator==(const t_composed& r) const
{
    if(
        i2 == r.i2  &&
        comp == r.comp
    )     return true;
    else  return false;
}

bool t_composed::operator< (const t_composed& r) const
{
    if(std::tie(
          i2,
          comp)
       <
       std::tie(
          r.i2,
          r.comp)
    )     return true;
    else  return false;

}

std::ostream& operator<< (std::ostream& os, const t_composed& t)
{
    os  << " t_composed { "
           << "i2: " << t.i2 << ", "
           << "comp: " << t.comp
        << " }";
    return os;
}








