#ifndef __field_types1_hh__
#define __field_types1_hh__

#include <vector.h>
#include "defs.hh"
#include "define_short.hh"
#include "type_info1.hh"

//: A vector of the field types.
class FieldTypes : public vector<mysql_type_info> {
private:
  void init (const ResUse *res);
public:
  FieldTypes () {}
  FieldTypes (const ResUse *res) {init(res);}
  FieldTypes (int i) : vector<mysql_type_info>(i) {}

  FieldTypes& operator = (const ResUse *res) {init(res); return *this;}
  //: Creates a new list based on the info in res
  FieldTypes& operator = (int i) {insert(begin(), i, 
					 mysql_type_info()); return *this;}
  //: Creates a new list with i fields. 
  
  mysql_type_info&  operator [] (int i) {return vector<mysql_type_info>
				  ::operator [] (i);}
  //: Returns the field type for the field with an offset of i.
  const mysql_type_info&  operator [] (int i) const 
    {return vector<mysql_type_info>::operator[] (i);}
  //: Returns the field type for the field with an offset of i.
};

#endif





