#ifndef JLECPPTK_MISC_H
#define JLECPPTK_MISC_H

#include "support/string.h"


#define  JLE_HERE  JLE_SS(__PRETTY_FUNCTION__ << "(" << __FILE__ << ":" << __LINE__ << ")")

#define  JLE_TRACE_S(__X__)  #__X__ " = " << (__X__) <<"; "
#define  JLE_COUT_TRACE(__X__)  std::cout <<  JLE_TRACE_S(__X__)  << std::endl;


namespace jle {




};



#endif // JLECPPTK_MISC_H
