#ifndef __datetime1_hh__
#define __datetime1_hh__

#include <coldata.h>
#include <defs.h>
#include <stream2string.h>
#include <tiny_int.h>

#include <string>
#include <sstream>
#include <iostream>

namespace mysqlpp {

struct mysql_dt_base {
  virtual std::ostream& out_stream(std::ostream&) const = 0;

  operator std::string ();
};

template <class T>
struct DTbase {
  virtual short int compare(const T &other) const = 0;

  bool operator == (const T &other) const {return !compare(other);}
  bool operator != (const T &other) const {return compare(other);}
  bool operator <  (const T &other) const {return compare(other) < 0;}
  bool operator <= (const T &other) const {return compare(other) <= 0;}
  bool operator >  (const T &other) const {return compare(other) > 0;}
  bool operator >= (const T &other) const {return compare(other) >= 0;}
};

struct mysql_date : virtual public mysql_dt_base {
  short int year;
  tiny_int  month;
  tiny_int  day;

  std::ostream& out_stream(std::ostream&) const;
  cchar* convert (cchar*);
protected:
  short int compare(const mysql_date *other) const;
};

//: A special type for holding mysql Dates.
// Date, a comparable data structures for holding mysql Dates.  It also
// responds to the stream extraction and insertion operator.
/*!dummy:
  struct Date {
    short int year;  //: The year in 4 digit form
    tiny_int  month; //: The month number
    tiny_int  day;   //: The day of the month

    Date ();                    //:
    Date (cchar* str);          //:
    Date (const ColData &str);  //:
    Date (const string &str);   //:

    short int compare(const Date& other) const;
    //: Compare two dates
    // Returns 0 if *this == other, <0 if *this < *other, >0 if *this > other
  };
*/

struct Date : public mysql_date, public DTbase<Date>
{
  Date () {};
  Date (cchar* str) {convert(str);}
  Date (const ColData &str);
  Date (const std::string &str);

  short int compare(const Date& other) const 
    {return mysql_date::compare(&other);}
};

inline std::ostream& operator << (std::ostream& s, const Date& d)
                                                  {return d.out_stream(s);}

struct mysql_time : virtual public mysql_dt_base {
  tiny_int hour;
  tiny_int minute;  
  tiny_int second;

  std::ostream& out_stream(std::ostream&) const;
  cchar* convert (cchar*);
protected:
  short int compare(const mysql_time *other) const;
};  

//: A special type for holding mysql Dates.
// Time, a comparable data structures for holding mysql Times.  It also
// responds to the stream extraction and insertion operator.
/*!dummy:
  struct Time {
    tiny_int hour;   //: Hour in military time
    tiny_int minute; //:
    tiny_int second; //:

    Time () {};                //:
    Time (cchar* str);         //:
    Time (const ColData &str); //:
    Time (const string &str);  //:

    short int compare(const Time& other) const;
    //: Compare two times
    // Returns 0 if *this == other, <0 if *this < *other, >0 if *this > other
  };
*/

struct Time : public mysql_time, public DTbase<Time>
{
  Time () {};
  Time (cchar* str) {convert(str);}
  Time (const ColData &str);
  Time (const std::string &str);

  short int compare(const Time& other) const 
    {return mysql_time::compare(&other);}
};


inline std::ostream& operator << (std::ostream& s, const Time& d)
                                                 {return d.out_stream(s);}

//: A combinate of Date and Time for holding mysql DateTime's
//!dummy: struct DateTime {};

struct DateTime : public mysql_date, public mysql_time,
		       public DTbase<DateTime>
{
  DateTime () {}
  DateTime (cchar* str) {convert(str);}
  DateTime (const ColData &str);
  DateTime (const std::string &str);

  short int compare(const DateTime& other) const;

  std::ostream& out_stream(std::ostream&) const;
  cchar* convert (cchar*);
};

inline std::ostream& operator << (std::ostream& s, const DateTime& d)
                                                   {return d.out_stream(s);}


inline mysql_dt_base::operator std::string () {
  return stream2string<std::string>(*this);
}

inline Date::Date (const ColData &str) {convert(str.c_str());}
inline Date::Date (const std::string &str)  {convert(str.c_str());}

inline Time::Time (const ColData &str) {convert(str.c_str());}
inline Time::Time (const std::string &str)  {convert(str.c_str());}

inline DateTime::DateTime (const ColData &str) {convert(str.c_str());}
inline DateTime::DateTime (const std::string &str) {convert(str.c_str());}

}; // end namespace mysqlpp

#endif //__datetime1_hh__

