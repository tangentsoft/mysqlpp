#ifndef __datetime2_hh__
#define __datetime2_hh__

#include "datetime1.hh"
#include "coldata2.hh"
#include "stream2string2.hh"

inline mysql_dt_base::operator string () {
  return stream2string<string>(*this);
}

inline Date::Date (const ColData &str) {convert(str.c_str());}
inline Date::Date (const string &str)  {convert(str.c_str());}

inline Time::Time (const ColData &str) {convert(str.c_str());}
inline Time::Time (const string &str)  {convert(str.c_str());}

inline DateTime::DateTime (const ColData &str) {convert(str.c_str());}
inline DateTime::DateTime (const string &str) {convert(str.c_str());}

#endif
