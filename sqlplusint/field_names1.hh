
#ifndef __field_names1_hh__
#define __field_names1_hh__

#include "defs.hh"
#include "define_short.hh"
#include "coldata1.hh"
#include "string_util.hh"

#include <vector>
#include <algorithm>

//: A vector of the field names.
class FieldNames : public std::vector<std::string> {
private:
  void init (const ResUse *res);
public:
  FieldNames () {}
  FieldNames (const ResUse *res) {init(res);}
  FieldNames (int i) : std::vector<std::string>(i) {}

  FieldNames& operator = (const ResUse *res) {init(res); return *this;}  
  //: Creates a new list from the data in res.
  FieldNames& operator = (int i) {insert(begin(), i, ""); return *this;} 
  //: Creates a new list with i field names.
  
  std::string&  operator [] (int i) {return std::vector<std::string>::operator [] (i);}
  //: returns the field name of the field with that index number

  const std::string&  operator [] (int i) const
    {return std::vector<std::string>::operator [] (i);}
  //: returns the field name of the field with that index number

  uint     operator [] (std::string i) const
  {
	  std::string temp(i); str_to_lwr(temp);
	  return uint(std::find(begin(),end(), temp) - begin());
	}
  //: returns the index number of the field with that name
};

#endif

