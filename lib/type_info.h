#ifndef MYSQLPP_TYPE_INFO_H
#define MYSQLPP_TYPE_INFO_H

#include <defs.h>

#include <mysql.h>

#include <typeinfo>
#include <map>

namespace mysqlpp {

class mysql_type_info;
class mysql_ti_sql_type_info_lookup;

class mysql_ti_sql_type_info {
  friend class mysql_type_info;
  friend class mysql_ti_sql_type_info_lookup;
private:
  const char          *_sql_name;
  const std::type_info     *_c_type;
  const unsigned char  _base_type;
  const bool           _default;
  // mysql_ti_sql_type_info (const mysql_ti_sql_type_info &b);           // can't do
  mysql_ti_sql_type_info& operator=(const mysql_ti_sql_type_info &b); //  "    "
  mysql_ti_sql_type_info () : _base_type(0), _default(false) {} 
  // OEP - didn't init _base_type and _default mysql_ti_sql_type_info () {} 
  // all private, only mysql_type_info can
  // create because there *must* be only one copy
  // of each.
  mysql_ti_sql_type_info (const char *s, const std::type_info &t,
			  const unsigned char bt = 0,  const bool d = false )
    : _sql_name(s), _c_type(&t), _base_type(bt), _default(d) {}
};

struct type_info_cmp {
  bool operator() (const std::type_info *lhs, const std::type_info *rhs) const {
    return lhs->before(*rhs) != 0;
  }
};

class mysql_ti_sql_type_info_lookup {
  friend class mysql_type_info;
private:
  typedef mysql_ti_sql_type_info sql_type_info;

  std::map<const std::type_info *, unsigned char, type_info_cmp> _map;
  
  mysql_ti_sql_type_info_lookup(const sql_type_info types[], const int size);

  const unsigned char& operator [] (const std::type_info &ti) const {
    return _map.find(&ti)->second;
  }
};

//: Class that holds basic type information for ColData.
class mysql_type_info {
private:
  typedef mysql_ti_sql_type_info sql_type_info;
  typedef mysql_ti_sql_type_info_lookup sql_type_info_lookup;
private:
  static const sql_type_info types[62];
  
  static const unsigned char offset = 0;
  static const unsigned char unsigned_offset = 21; 
  static const unsigned char null_offset = 31;
  static const unsigned char unsigned_null_offset = 52;

  static const sql_type_info_lookup lookups;
  
  static unsigned char type(enum_field_types t,
			      bool _unsigned, bool _null = false);
public:
  static const unsigned char string_type = 20;
	unsigned int _length;
	unsigned int _max_length;
private:
  unsigned char num;
  inline const sql_type_info& deref() const;
public:
  //!dummy: static const unsigned char string_type;
  //: The id of the string type.
  mysql_type_info() {} //:
  mysql_type_info(unsigned char n) : num(n) {} //:
  inline mysql_type_info(enum_field_types t,
			 bool _unsigned, bool _null);
  //:
  inline mysql_type_info(const MYSQL_FIELD &f);
  //:
  mysql_type_info(const mysql_type_info &t) : num(t.num) {}
  //:
  mysql_type_info(const std::type_info &t) {num = lookups[t]; }
  //: 
  mysql_type_info& operator = (unsigned char n) {num=n; return *this;}
  //:
  mysql_type_info& operator = (const mysql_type_info &t) 
    {num = t.num; return *this;}
  //:
  mysql_type_info& operator = (const std::type_info &t)
    {num = lookups[t]; return *this;}
  //:
  inline const char*           name()      const;
  //: Returns a implication defined name of the c++ type.
  // Returns the name that would be returned by typeid().name() for the C++ type
  // associated with the sql type.
  inline const char*           sql_name()  const;
  //: Returns the name for the sql type.
  inline const std::type_info&      c_type()    const;
	inline const unsigned int length() const;
	inline const unsigned int max_length() const;
  //: Returns the type_info for the C++ type associated with the sql type.
  inline const mysql_type_info base_type() const;
  //: Returns the type_info for the C++ type inside of the Null type.
  // If the type is not null then this is the same as c_type() 
         int                   id()        const {return num;}
  //: Returns the id of the sql_type.
  // Note: Do not ever use this id directly as it may change between versions.

  bool quote_q()  const;
  //: Returns true if the sql type is of a type that needs to be quoted.
  bool escape_q() const;
  //: Returns true if the sql type is of a type that needs to be escaped.
  
  bool before(mysql_type_info &b) { return num < b.num; }
  //: Provides ordering
  // You can also use id() for the same purpose.
};

inline const mysql_type_info::sql_type_info& mysql_type_info::deref() const
{
  return types[num];
}

inline const char* mysql_type_info::name() const
{
  return deref()._c_type->name();
}

inline const char* mysql_type_info::sql_name() const
{
  return deref()._sql_name;
}

inline const unsigned int mysql_type_info::length() const
{
  return _length;
}

inline const unsigned int mysql_type_info::max_length() const
{
  return _max_length;
}

inline const std::type_info& mysql_type_info::c_type() const
{
  return *deref()._c_type;
}

inline const mysql_type_info mysql_type_info::base_type() const
{
  return mysql_type_info(deref()._base_type);
}

inline mysql_type_info::mysql_type_info(enum_field_types t,
		bool _unsigned, bool _null)
{
  num = type(t,_unsigned,_null);
}

inline mysql_type_info::mysql_type_info(const MYSQL_FIELD &f)
{
  num = type(f.type,
	  (f.flags & UNSIGNED_FLAG) != 0,
	  (f.flags & NOT_NULL_FLAG) == 0);
  _length = f.length;
  _max_length = f.max_length;
}

inline bool operator == (const mysql_type_info& a, const mysql_type_info& b)
{
  return a.id() == b.id();
}

inline bool operator != (const mysql_type_info& a, const mysql_type_info& b)
{
  return a.id() != b.id();
}

inline bool operator == (const std::type_info &a, const mysql_type_info &b)
{
  return a == b.c_type();
}

inline bool operator != (const std::type_info &a, const mysql_type_info &b)
{
  return a != b.c_type();
}

inline bool operator == (const mysql_type_info &a, const std::type_info &b)
{
  return a.c_type() == b;
}

inline bool operator != (const mysql_type_info &a, const std::type_info &b)
{
  return a.c_type() != b;
}

}; // end namespace mysqlpp

#endif

