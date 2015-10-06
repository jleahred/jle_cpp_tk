#ifndef JLECPPTK_MISC_H
#define JLECPPTK_MISC_H


#include<iostream>

#define  JLE_SS(__EXPR__) ((dynamic_cast<std::ostringstream &>(std::ostringstream().seekp(0, std::ios_base::cur) << __EXPR__ )).str())

#define  JLE_HERE  JLE_SS(__PRETTY_FUNCTION__ << "(" << __FILE__ << ":" << __LINE__ << ")")

#define  JLE_TRACE_S(__X__)  #__X__ << " = " << (__X__) <<"; "
#define  JLE_COUT_TRACE(__X__)  std::cout <<  JLE_TRACE_S(__X__)  << std::endl;


namespace jle {




};



#endif // JLECPPTK_MISC_H
