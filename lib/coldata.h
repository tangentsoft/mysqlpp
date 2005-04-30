#ifndef MYSQLPP_COLDATA_H
#define MYSQLPP_COLDATA_H

/// \file coldata.h
/// \brief Declares classes for converting string data to any of 
/// the basic C types.
///
/// Roughly speaking, this defines classes that are the inverse of
/// mysqlpp::SQLString.

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

/// \brief Base class for auto-converting column data.  Do not use directly. 
///
/// A smart string. It will automatically convert itself to any of the
/// basic C types. 
///
/// When used with binary operators it will automatically convert itself
/// to the type used on the other side of the operator if it is a basic
/// type.  However, be careful when using it like this:
/// 
/// \code ColData("12.86") + 2 \endcode
/// 
/// will return 14 because 2 is an integer. To get the result you
/// wanted, you must say:
///
/// \code ColData("12.86") + 2.0 \endcode
///
/// If this type of thing scares you, define the micro NO_BINARY_OPERS
/// to disable this behavior.
///
/// This class also has some basic information about the type of data
/// stored in it.
///
/// <b>Do not use this class directly.</b> Use the typedef ColData or
/// MutableColData instead.

template <class Str>
class ColData_Tmpl : public Str {
private:
  mysql_type_info _type;
	std::string buf;
	bool _null;
public:
  explicit ColData_Tmpl(bool n,
  		mysql_type_info t = mysql_type_info::string_type) :
  _type(t),
  _null(n)
  {
  }

  explicit ColData_Tmpl(const char *str, 
		 mysql_type_info t = mysql_type_info::string_type,
		 bool n = false) :
  Str(str),
  _type(t),
  _null(n)
  {
  	buf=str;
  }

  ColData_Tmpl() { }

  /// \brief Get this object's current MySQL type.
  mysql_type_info type() { return _type; }

  /// \brief Returns true if data of this type should be quoted, false
  /// otherwise.
  bool quote_q() const { return _type.quote_q(); }

  /// \brief Returns true if data of this type should be escaped, false
  /// otherwise.
  bool escape_q() const { return _type.escape_q(); }
  
  template<class Type> Type conv(Type dummy) const;

  void it_is_null (void) {_null=true;}
	inline const bool is_null(void) const {return _null;}
	inline const std::string&  get_string(void) const {return buf;}
  operator cchar*() const {return buf.c_str();}
  operator  signed char() const {return conv(static_cast<signed char>(0));}
  operator  unsigned char() const {return conv(static_cast<unsigned char>(0));}
  operator  int() const {return conv(static_cast<int>(0));}
  operator  unsigned int() const {return conv(static_cast<unsigned int>(0));}
  operator  short int() const {return conv(static_cast<short int>(0));}
  operator  unsigned short int() const {return conv(static_cast<unsigned short int>(0));}
  operator  long int() const {return conv(static_cast<long int>(0));}
  operator  unsigned long int() const {return conv(static_cast<unsigned long int>(0));}
  operator  longlong() const {return conv(static_cast<longlong>(0));}
  operator  ulonglong() const {return conv(static_cast<ulonglong>(0));}
  operator  float() const {return conv(static_cast<float>(0));}
  operator  double() const {return conv(static_cast<double>(0));}

  template <class T, class B> operator Null<T,B> () const;
};

/// \typedef ColData_Tmpl<const_string> ColData
/// \brief The type that is returned by constant rows
typedef ColData_Tmpl<const_string> ColData;

/// \typedef ColData_Tmpl<std::string> MutableColData
/// \brief The type that is returned by mutable rows
typedef ColData_Tmpl<std::string> MutableColData;



#ifndef NO_BINARY_OPERS

#define oprsw(opr, other, conv) \
  template<class Str> \
  inline other operator opr (ColData_Tmpl<Str> x, other y) \
    {return static_cast<conv>(x) opr y;} \
  template<class Str> \
  inline other operator opr (other x, ColData_Tmpl<Str> y) \
    {return x opr static_cast<conv>(y);}

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
Type ColData_Tmpl<Str>::conv (Type /*dummy*/) const {
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

