#ifndef __mysql_defs_h_
#define __mysql_defs_h_

#include <platform.h>

#include <mysql.h>

namespace mysqlpp {

#if defined(NO_LONG_LONGS)
typedef unsigned long ulonglong;
typedef long longlong;
#elif defined (__WIN32__) || defined(_WIN32)
typedef unsigned __int64 ulonglong;
typedef __int64 longlong;
#else
typedef unsigned long long ulonglong;
typedef long long longlong;
#endif

typedef MYSQL_FIELD Field;
typedef const char cchar;
typedef longlong longgint;
#ifndef uint
typedef unsigned int uint;
#endif

class Connection;
class ResUse;
class Result;
class FieldNames;

const bool use_exceptions = true;

}; // end namespace mysqlpp

#endif

