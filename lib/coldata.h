#ifndef MYSQLPP_COLDATA_H
#define MYSQLPP_COLDATA_H

#include "platform.h"

#include "const_string.h"
#include "convert.h"
#include "defs.h"
#include "exceptions.h"
#include "null.h"
#include "string_util.h"
#include "type_info.h"

#include <mysql.h>

#include <typeinfo>
#include <string>

#include <stdlib.h>

namespace mysqlpp {

//!  with_class = ColData_Tmpl

//: Base class for auto-converting column data.  Do not use directly. 
//
// A smart string. It will automatically convert it self to any of the
// basic C types. 
//
// When used with binary operators it will
// automatically convert it self to the type used on the other side of
// the operator if it is a basic type.
//
// However, be careful when using it with binary operators as. 
// 
// MysqlStr("12.86") + 2
// 
// will return 14 because 2 is an integer. What you wanted to say was 
//
// MysqlStr("12.86") + 2.0
//
// If this type of thing scares you define the micro NO_BINARY_OPERS
// to turn of this behavior.
//
// This class also has some basic information about the type of data
// stored in it.
//
// <bf>Do not use this class directly.</bf>
//  Use the typedef ColData or MutableColData instead.
template <class Str>
class ColData_Tmpl : public Str {
private:
  mysql_type_info _type;
	std::string buf;
	bool _null;
public:
  explicit ColData_Tmpl (bool n, mysql_type_info t = mysql_type_info::string_type) 
    : _type(t), _null(n) {}
  explicit ColData_Tmpl (const char *str, 
		 mysql_type_info t = mysql_type_info::string_type, bool n = false)
    : Str(str), _type(t), _null(n) {buf=str;}
  ColData_Tmpl () {}
  mysql_type_info type() {return _type;}
  //: Returns the current mysql type of current item

  bool quote_q() const {return _type.quote_q();}
  //: Returns true or false depending on if the data is of a type that
  //: should be quoted

  bool escape_q() const {return _type.escape_q();}
  //: Returns true of false depending on if the data is of a type that
  //: should be escaped
  
  template<class Type> Type conv (Type dummy) const;

  //!dummy: operator TYPE() const;
  //: Converts the column data to TYPE.
  // If all the charters are not read during the conversion to TYPE it
  // will through BadConversion.
  //
  // TYPE is defined for all the build in types.
  //
  // (Note, This is not an actual template)
  void it_is_null (void) {_null=true;}
	inline const bool is_null(void) const {return _null;}
	inline const std::string&  get_string(void) const {return buf;}
  operator cchar*() const {return buf.c_str();}
  operator  signed char() const {return conv((signed char)0);}
  operator  unsigned char() const {return conv((unsigned char)0);}
  operator  int() const {return conv((int)0);}
  operator  unsigned int() const {return conv((unsigned int)0);}
  operator  short int() const {return conv((short int)0);}
  operator  unsigned short int() const {return conv((unsigned short int)0);}
  operator  long int() const {return conv((long int)0);}
  operator  unsigned long int() const {return conv((unsigned long int)0);}
  operator  longlong() const {return conv((longlong)0);}
  operator  ulonglong() const {return conv((ulonglong)0);}
  operator  float() const {return conv((float)0);}
  operator  double() const {return conv((double)0);}

  template <class T, class B> operator Null<T,B> () const;
};

//: The Type that is returned by constant rows
typedef ColData_Tmpl<const_string> ColData;
//: The Type that is returned by mutable rows
typedef ColData_Tmpl<std::string>       MutableColData;
//: For backwards compatibility. Do not use.
typedef ColData MysqlString;
//: For backwards compatibility. Do not use.
typedef ColData MysqlStr;



#ifndef NO_BINARY_OPERS

#define oprsw(opr, other, conv) \
  template<class Str> \
  inline other operator opr (ColData_Tmpl<Str> x, other y) \
    {return (conv)x opr y;} \
  template<class Str> \
  inline other operator opr (other x, ColData_Tmpl<Str> y) \
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

operator_binary_int(longlong, longlong)
operator_binary_int(ulonglong, ulonglong)

#endif // NO_BINARY_OPERS


template <class Str> template<class T, class B> 
ColData_Tmpl<Str>::operator Null<T,B> () const {
  if ((*this)[0] == 'N' && (*this)[1] == 'U' && 
      (*this)[2] == 'L' && (*this)[3] == 'L' && Str::size() == 4)
    return Null<T,B>(null);
  else return Null<T,B>(conv(T()));
}


template <class Str> template<class Type> 
Type ColData_Tmpl<Str>::conv (Type dummy) const {
	std::string strbuf = buf;
	strip_all_blanks(strbuf);
  size_t len = strbuf.size();
  const char *str = strbuf.c_str();
  const char *end = str;
  Type num = mysql_convert<Type>(str, end);
  if (*end == '.') {
    end++;
    for (;*end == '0'; end++);
  }
  if (*end != '\0' && end != NULL ) {
    throw BadConversion (typeid(Type).name(), Str::c_str(), end - str, len);
  }
  return num;
}

} // end namespace mysqlpp

#endif

