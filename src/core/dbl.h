#ifndef JLE_DBL_H__
#define JLE_DBL_H__

#include <iosfwd>



/**
 *     \example  ./core/ex_dbl.cpp
 */



namespace jle {



class dbl {

    public:
        dbl             ()              = delete;
        dbl             (const dbl&)    = default;
        dbl             (dbl&&)         = default;
        dbl& operator=  (const dbl&)    = default;
        dbl& operator=  (dbl&&)         = default;

        /** I don't like implicit conversions.
         *  But in this case, it is necessary
         */
        dbl(double v)        : value(v){};  /*removed explicit explicit*/

        /// convert to double
        explicit operator double() const  { return value; };


    private:
        double  value;
};


///  as double version  std::
dbl pow (const dbl& base,    const dbl& exp );

///  as double version  std::
dbl log (const dbl& d                       );

///  as double version  std::
dbl exp (const dbl& d                       );

/**
 * @brief  Get number of decimals base 10 (aprox)
 *
 * This is not exact but quit accurate
 */
int get_decimals10(const jle::dbl& d);


//---------------------------------------------------------------------------
};      //namespace jle {


std::ostream& operator<< (std::ostream& os, const jle::dbl& d);

jle::dbl operator+ (const jle::dbl& d1, const jle::dbl& d2);
jle::dbl operator- (const jle::dbl& d1, const jle::dbl& d2);
jle::dbl operator* (const jle::dbl& d1, const jle::dbl& d2);
jle::dbl operator/ (const jle::dbl& d1, const jle::dbl& d2);

jle::dbl& operator+= (jle::dbl& d1, const jle::dbl& d2);
jle::dbl& operator-= (jle::dbl& d1, const jle::dbl& d2);
jle::dbl& operator*= (jle::dbl& d1, const jle::dbl& d2);
jle::dbl& operator/= (jle::dbl& d1, const jle::dbl& d2);


bool     operator    ==  (const jle::dbl&  d1, const jle::dbl&  d2);
bool     operator    !=  (const jle::dbl&  d1, const jle::dbl&  d2);
bool     operator    <   (const jle::dbl&  d1, const jle::dbl&  d2);
bool     operator    <=  (const jle::dbl&  d1, const jle::dbl&  d2);
bool     operator    >   (const jle::dbl&  d1, const jle::dbl&  d2);
bool     operator    >=  (const jle::dbl&  d1, const jle::dbl&  d2);


#endif // JLE_DBL_H__
