#ifndef MYSQLPP_CONVERT_H
#define MYSQLPP_CONVERT_H

#include "platform.h"

#include "defs.h"

#include <stdlib.h>

namespace mysqlpp {

template <class Type> class mysql_convert;

#define mysql__convert(TYPE, FUNC) \
  template <> \
  class mysql_convert<TYPE> {\
  private:\
    TYPE num;\
  public:\
    mysql_convert(const char* str, const char *& end) { \
      num = FUNC(str, const_cast<char **>(&end));}\
    operator TYPE () {return num;}\
  };\

#if defined(_MSC_VER)
#	pragma warning(disable: 4244)
#endif

	mysql__convert(float, strtod)
	mysql__convert(double, strtod)

#if defined(_MSC_VER)
#	pragma warning(default: 4244)
#endif

#undef mysql__convert
#define mysql__convert(TYPE, FUNC) \
  template <> \
  class mysql_convert<TYPE> {\
  private:\
    TYPE num;\
  public:\
    mysql_convert(const char* str, const char *& end) { \
      num = FUNC(str, const_cast<char **>(&end),10);}\
    operator TYPE () {return num;}\
  };\

#if defined(_MSC_VER)
#	pragma warning(disable: 4244)
#endif

	mysql__convert(char, strtol)
	mysql__convert(signed char, strtol)
	mysql__convert(int, strtol)
	mysql__convert(short int, strtol)
	mysql__convert(long int, strtol)

	mysql__convert(unsigned char, strtoul)
	mysql__convert(unsigned int, strtoul)
	mysql__convert(unsigned short int, strtoul)
	mysql__convert(unsigned long int, strtoul)

#if defined(_MSC_VER)
#	pragma warning(default: 4244)
#endif

#if !defined(NO_LONG_LONGS)
#if defined(__GNUC__)
mysql__convert(longlong, strtoll)
mysql__convert(ulonglong, strtoull)
#elif defined(_MSC_VER)
mysql__convert(longlong, _strtoi64)
mysql__convert(ulonglong, _strtoui64)
#else
#error Fix me! I need the "string to 64-bit int" function for your platform!
#endif
#endif // !defined(NO_LONG_LONGS)

} // end namespace mysqlpp

#endif

