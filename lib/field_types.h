#ifndef MYSQLPP_FIELD_TYPES_H
#define MYSQLPP_FIELD_TYPES_H

/// \file field_types.h
/// \brief Declares a class to hold a list of SQL field type info.

#include "type_info.h"

#include <vector>

namespace mysqlpp {

class ResUse;

/// \brief A vector of SQL field types.
class FieldTypes : public std::vector<mysql_type_info> {
private:
  void init (const ResUse *res);
public:
  FieldTypes () {}
  FieldTypes (const ResUse *res) {init(res);}
  FieldTypes (int i) : std::vector<mysql_type_info>(i) {}

  /// \brief Creates a new list based on the info in res
  FieldTypes& operator =(const ResUse *res) {init(res); return *this;}

  /// \brief Creates a new list with a given number of fields.
  ///
  /// Initializes the list from the data returned by MySQL C API
  /// function \c mysql_type_info().
  ///
  /// \param i size of field list to create
  FieldTypes& operator =(int i)
  {
  	insert(begin(), i, mysql_type_info());
	return *this;
  }
  
  /// \brief Returns a field type within the list given its index.
  mysql_type_info& operator [](int i)
  {
  	return std::vector<mysql_type_info>::operator[](i);
  }
  const mysql_type_info& operator [](int i) const 
  {
  	return std::vector<mysql_type_info>::operator[](i);
  }
};

} // end namespace mysqlpp

#endif

