#ifndef __coldata2_hh__
#define __coldata2_hh__

#include <stdlib.h>
#include "coldata1.hh"

#ifndef NO_BINARY_OPERS

#define oprsw(opr, other, conv) \
  template<class Str> \
  inline other operator opr (mysql_ColData<Str> x, other y) \
    {return (conv)x opr y;} \
  template<class Str> \
  inline other operator opr (other x, mysql_ColData<Str> y) \
    {return x opr (conv)y;}

#define operator_binary(other, conv) \
  oprsw(+, other, conv) \
  oprsw(-, other, conv) \
  oprsw(*, other, conv) \
  oprsw(/, other, conv) 

#define operator_binary_int(other, conv) \
  operator_binary(other, conv) \
  oprsw(%, other, conv) \
  oprsw(&, other, conv) \
  oprsw(^, other, conv) \
  oprsw(|, other, conv) \
  oprsw(<<, other, conv) \
  oprsw(>>, other, conv) 

operator_binary(float, double)
operator_binary(double, double)

operator_binary_int(char,long int)
operator_binary_int(int, long int)
operator_binary_int(short int, long int)
operator_binary_int(long int, long int)

operator_binary_int(unsigned char, unsigned long int)
operator_binary_int(unsigned int, unsigned long int)
operator_binary_int(unsigned short int, unsigned long int)
operator_binary_int(unsigned long int, unsigned long int)

#if !defined(NO_LONG_LONGS)
operator_binary_int(longlong, longlong)
operator_binary_int(ulonglong, ulonglong)
#endif

#endif // NO_BINARY_OPERS

#endif
