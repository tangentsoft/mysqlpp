#ifndef MYSQLPP_NULL_H
#define MYSQLPP_NULL_H

#include <exceptions.h>

#include <ostream>

namespace mysqlpp {

//! with_class = Null

class null_type {
public:
  template <class Type> operator Type () {throw BadNullConversion();}
};

//: 
const null_type null = null_type();

//: Type to use for the behavior parameter for *Null<>*
struct NullisNull {
  static null_type null_is() {return null_type();}
  static std::ostream&  null_ostr(std::ostream& o) {o << "(NULL)"; return o;}
};

//: Type to use for the behavior parameter for *Null<>*
struct NullisZero {
  static int   null_is() {return 0;}
  static std::ostream& null_ostr(std::ostream &o) {o << 0; return o;}
};

//: Type to use for the behavior parameter for *Null<>*
struct NullisBlank {
  static const char * null_is() {return "";}
  static std::ostream& null_ostr(std::ostream &o) {o << ""; return o;}
};

//: Container class for holding null types.
template <class Type, class Behavior = NullisNull>
class Null {
public:
  Type data;
  bool is_null;
  typedef Type value_type;
public:
  Null () {} //:
  Null (Type x) : data(x), is_null(false) {} 
  //: 
  Null (const null_type &n) : is_null(true) {} 
  //: Gives Null the null value
  // Note: the global const *null* (not NULL) is of value null_type thus 
  // you can say something like *Null<Type> x = null*.
  operator Type& () { 
    if (is_null) return data = Behavior::null_is();
    else return data; } 
  //:
  Null& operator = (const Type &x) {data = x; is_null = false; return *this;}
  //: 
  Null& operator = (const null_type &n) {is_null = true; return *this;}
  //:
};

//: OEP - Specialization for <void>
template <> class Null<void> {
public:
  bool is_null;
  typedef void value_type;
public:
  Null () : is_null(false) {} 
  //: 
  Null (const null_type &n) : is_null(true) {} 
  //: Gives Null the null value
  // Note: the global const *null* (not NULL) is of value null_type thus 
  // you can say something like *Null<Type> x = null*.
  //:
  Null& operator = (const null_type &n) {is_null = true; return *this;}
  //:
};
 

template <class Type, class Behavior>
inline std::ostream& operator << (std::ostream &o, const Null<Type,Behavior> &n) {
  if (n.is_null) return Behavior::null_ostr(o);
  else return o << n.data;
}

}; // end namespace mysqlpp

#endif

