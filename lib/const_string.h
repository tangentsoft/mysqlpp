#ifndef MYSQLPP_CONST_STRING_H
#define MYSQLPP_CONST_STRING_H

#include <defs.h>

#include <stdexcept>
#include <string>
#include <iostream>

namespace mysqlpp {

//: A special string that is created from a existing const char *
// It contains a small subset of the standard string class.  When an
// object is created only a link to the const char * is created.  The
// data is NOT copied.  thus the const char * needs to stick around
// for the life of the class.
class const_string {
private:
  const char *str_data; 
public:
  typedef const char   value_type;      //:
  typedef unsigned int size_type;       //:
  typedef          int difference_type; //:

  typedef const char&     const_reference; //:
  typedef const_reference reference;       //:
  typedef const char*     const_pointer;   //:
  typedef const_pointer   pointer;         //:

  typedef const char*    const_iterator;   //:
  typedef const_iterator iterator;         
  //: The same as const_iterator because the data can not be changed.
  
  const_string() : str_data("") {}                  //:
  const_string(const char *str) : str_data(str) {}  //:
  const_string& operator = (const char *str)        //:
    {str_data = str; return *this;}
 
  size_type size() const //: 
  { register int i = 0;
    while (str_data[i]) i++;
    return i;
  }

  const_iterator  begin() const {return str_data;}          //:
  const_iterator  end()   const {return str_data + size();} //:

  size_type length() const {return size();}   //:
  size_type max_size() const {return size();} //:
  
  const_reference operator[](size_type pos) const {return str_data[pos];} //:
  const_reference at(size_type pos) const //: 
  {
    if (pos >= size()) throw std::out_of_range("");
    else return str_data[pos];
  }
  
  const char* c_str() const {return str_data;} //:
  const char* data()  const {return str_data;} //:
  
  int compare(const const_string& str) const //:
  {
    const char* str1 = str_data;
    const char* str2 = str.str_data;
    while (*str1 == *str2 && (*str1 || *str2)) {str1++; str2++;}
	return *str1-*str2;
  }
};


//! with_class = const_string

//:
inline std::ostream& operator << (std::ostream &o, const const_string &str)
{
  return o << str.c_str();
}

//:
inline int compare (const const_string &lhs, const const_string &rhs)
{
  return lhs.compare(rhs);
}

//:
inline bool operator == (const_string &lhs, const_string &rhs) 
{
  return compare(lhs,rhs) == 0;
}

//:
inline bool operator != (const_string &lhs, const_string &rhs)
{
  return compare(lhs,rhs) != 0;
}

//:
inline bool operator < (const_string &lhs, const_string &rhs)
{
  return compare(lhs,rhs) < 0;
}

//:
inline bool operator <= (const_string &lhs, const_string &rhs)
{
  return compare(lhs,rhs) <= 0;
}

//:
inline bool operator > (const_string &lhs, const_string &rhs)
{
  return compare(lhs,rhs) > 0;
}

//:
inline bool operator >= (const_string &lhs, const_string &rhs)
{
  return compare(lhs,rhs) >= 0;
}

}; // end namespace mysqlpp

#endif

