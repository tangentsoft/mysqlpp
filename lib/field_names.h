#ifndef MYSQLPP_FIELD_NAMES_H
#define MYSQLPP_FIELD_NAMES_H

/// \file field_names.h
/// \brief Declares a class to hold a list of field names.

#include "coldata.h"
#include "string_util.h"

#include <algorithm>
#include <vector>

namespace mysqlpp {

class ResUse;

/// \brief Holds a list of SQL field names
class FieldNames : public std::vector<std::string> {
private:
  void init (const ResUse *res);
public:
  FieldNames () {}
  FieldNames (const ResUse *res) {init(res);}
  FieldNames (int i) : std::vector<std::string>(i) {}

  /// \brief Creates a new list from the data in res.
  FieldNames& operator = (const ResUse *res) {init(res); return *this;}  

  /// \brief Creates a new list with i field names.
  FieldNames& operator = (int i) {insert(begin(), i, ""); return *this;} 
  
  /// \brief Get the name of a field given its index.
  std::string& operator [] (int i)
  {
  	return std::vector<std::string>::operator [] (i);
  }
  const std::string& operator [] (int i) const
  {
  	return std::vector<std::string>::operator [] (i);
  }

  /// \brief Get the index number of a field given its name
  uint operator [] (std::string i) const
  {
	  std::string temp(i); str_to_lwr(temp);
	  return uint(std::find(begin(),end(), temp) - begin());
  }
};

} // end namespace mysqlpp

#endif

