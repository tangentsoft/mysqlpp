#ifndef MYSQLPP_FIELD_TYPES_H
#define MYSQLPP_FIELD_TYPES_H

#include "type_info.h"

#include <vector>

namespace mysqlpp {

class ResUse;

//: A vector of the field types.
class FieldTypes : public std::vector<mysql_type_info> {
private:
  void init (const ResUse *res);
public:
  FieldTypes () {}
  FieldTypes (const ResUse *res) {init(res);}
  FieldTypes (int i) : std::vector<mysql_type_info>(i) {}

  FieldTypes& operator = (const ResUse *res) {init(res); return *this;}
  //: Creates a new list based on the info in res
  FieldTypes& operator = (int i) {insert(begin(), i, 
					 mysql_type_info()); return *this;}
  //: Creates a new list with i fields. 
  
  mysql_type_info&  operator [] (int i) {return std::vector<mysql_type_info>
				  ::operator [] (i);}
  //: Returns the field type for the field with an offset of i.
  const mysql_type_info&  operator [] (int i) const 
    {return std::vector<mysql_type_info>::operator[] (i);}
  //: Returns the field type for the field with an offset of i.
};

} // end namespace mysqlpp

#endif

