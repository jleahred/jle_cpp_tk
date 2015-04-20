#ifndef JLE_STRING_H
#define JLE_STRING_H


#include <sstream>
#include <vector>

#include "support/chrono.h"



//!  \example  ./support/ex_jle_ss.cpp

//#define JLE_SS(__EXPR__) (static_cast<std::ostringstream*>(&(std::ostringstream().flush() << __EXPR__)->str())
#define JLE_SS(__EXPR__) ((dynamic_cast<std::ostringstream &>(std::ostringstream().seekp(0, std::ios_base::cur) << __EXPR__ )).str())



namespace jle
{



    //! \brief  try to convert from string to duration
    std::tuple<double, bool>
    s_try_s2d                (const std::string&    s, double def_val );

    //! \brief  try to convert from string to integer
    std::tuple<int, bool>
    s_try_s2i                (const std::string&    s, int    def_val );




    /** \brief  try to convert from string to jle::duration
        possible formats...
          - 1d
          - 1d4h
          - 4h5m
          - 5m
          - 3s 895ms
          - 5s
          - 123ms
          - 3500ms
     */
    std::tuple<chrono::duration, bool>
    s_try_s2dur   (const std::string&    s, const chrono::duration& def_val );


    /** \brief  try to convert from string to jle::time_point
        possible formats...
            - 2015-03-29 09:16:33.123
            - 2015-03-29
     */
    std::tuple<jle::chrono::time_point, bool>
    s_try_s2tp   (const std::string&    s, const chrono::time_point& defVal );





    //! \brief  remove from left and right ``char2remove`` from string ``s``
    std::string     s_trim    (const std::string&    s , char char2remove);

    //! \brief  remove from left and right all chars in ``chars2remove`` from string ``s``
    std::string     s_trim    (const std::string&    s , const std::string& chars2remove);


    //! \brief  divide string ``s``  with ``separator``
    std::vector<std::string> s_split (
                                    const std::string&  s,
                                    const std::string&  separator
                                );


    std::string     s_2lower(const std::string& s);
    std::string     s_2upper(const std::string& s);


    std::string     s_align_left (const std::string& s, int size, char char_fill=' ');
    std::string     s_align_right(const std::string& s, int size, char char_fill=' ');

    //! \brief  return a valid utf8 string from a raw one
    //!  If char is "special" as  á, é, í, ó, ú, ñ, ü (and uppercases)
    //!  it will be converted to utf8. Other weird values will be replaced by "_"
    std::string     s_normalize_utf8(const std::string&  s);

}

#endif // JLE_STRING_H
