#ifdef __WIN32__
#include <Windows32/Base.h>
#include <Windows32/Defines.h>
#include <Windows32/Structures.h>
#include <winsock.h>
#define errno WSAGetLastError()
#endif

#include "datetime3.hh"
#include <iomanip.h>

ostream& mysql_date::out_stream (ostream& s) const {
  char fill = s.fill('0');
#ifdef __USLC__
  long flags = s.setf(ios::right);
#else
  ios::fmtflags flags = s.setf(ios::right);
#endif
  s << setw(4) << year << '-' 
    << setw(2) << month << '-'
    << setw(2) << day;
  s.flags(flags);
  s.fill(fill);
  return s;
}

ostream& mysql_time::out_stream (ostream& s) const {
  char fill = s.fill('0');
#ifdef __USLC__
  long flags = s.setf(ios::right);
#else
  ios::fmtflags flags = s.setf(ios::right);
#endif
  s << setw(2) << hour << ':' 
    << setw(2) << minute << ':'
    << setw(2) << second;
  s.flags(flags);
  s.fill(fill);
  return s;
}

ostream& MysqlDateTime::out_stream (ostream& s) const {
  mysql_date::out_stream(s);
  s << " ";
  mysql_time::out_stream(s);
  return s;
}

cchar* mysql_date::convert (cchar* str) {
  char num[5];
  num[0] =*str++; num[1] =*str++; num[2] =*str++; num[3] =*str++; num[4] = 0;
  year = strtol(num,0,10);
  if (*str == '-') str++;
  num[0] = *str++; num[1] = *str++; num[2] = 0;
  month = strtol(num,0,10);
  if (*str == '-') str++;
  num[0] = *str++; num[1] = *str++; num[2] = 0;
  day = strtol(num,0,10);
  return str;
}

cchar* mysql_time::convert (cchar* str) {
  char num[5];
  num[0] = *str++; num[1] = *str++; num[2] = 0;
  hour = strtol(num,0,10);
  if (*str == ':') str++;
  num[0] = *str++; num[1] = *str++; num[2] = 0;
  minute = strtol(num,0,10);
  if (*str == ':') str++;
  num[0] = *str++; num[1] = *str++; num[2] = 0;
  second = strtol(num,0,10);
  return str;
}

cchar* MysqlDateTime::convert (cchar* str) {
  str = mysql_date::convert(str);
  if (*str == ' ') str++;
  str = mysql_time::convert(str);
  return str;
}

short int mysql_date::compare(const mysql_date* other) const {
  if (year != other->year) return year - other->year;
  if (month != other->month) return month - other->month;
  return day - other->day;
}

short int mysql_time::compare(const mysql_time* other) const {
  if (hour != other->hour) return hour - other->hour;
  if (minute != other->minute) return minute - other->minute;
  return second - other->second;
}

short int MysqlDateTime::compare(const MysqlDateTime &other) const {
  int x;
  x = mysql_date::compare(&other);
  if (x) return x;
  return mysql_time::compare(&other);
}





