#ifndef __row2_hh__
#define __row2_hh__

#include "string.h"
#include "row1.hh"
#include "result2.hh"

#include <string>

inline Row::size_type Row::size() const {return res->num_fields();}

inline const ColData Row::operator [] (size_type i) const {
  if (!initialized) {
	if (throw_exceptions) throw BadQuery("Row not initialized");
	else return ColData();
  }
  return ColData(data[i].c_str(), res->types(i),is_nulls[i]);
}

inline const ColData Row::lookup_by_name(const char* i) const {
  return (*this)[res->field_num(std::string(i))];
}

// RowTemplate

#endif










