
#ifndef __field_names1_hh__
#define __field_names1_hh__

#include <vector.h>
#include <algo.h>
#include "defs"
#include "define_short"
#include "coldata1.hh"
#include "string_util.hh"

//: A vector of the field names.
class FieldNames : public vector<string> {
private:
  void init (const ResUse *res);
public:
  FieldNames () {}
  FieldNames (const ResUse *res) {init(res);}
  FieldNames (int i) : vector<string>(i) {}

  FieldNames& operator = (const ResUse *res) {init(res); return *this;}  
  //: Creates a new list from the data in res.
  FieldNames& operator = (int i) {insert(begin(), i, ""); return *this;} 
  //: Creates a new list with i field names.
  
  string&  operator [] (int i) {return vector<string>::operator [] (i);}
  //: returns the field name of the field with that index number

  const string&  operator [] (int i) const 
    {return vector<string>::operator [] (i);}
  //: returns the field name of the field with that index number

  uint     operator [] (string i) const
  {
	  string temp(i); str_to_lwr(temp);
	  return find(begin(),end(), temp) - begin();
	}
  //: returns the index number of the field with that name
};

#endif

