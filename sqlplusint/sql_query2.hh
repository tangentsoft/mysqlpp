#ifndef __sql_query2_hh__
#define __sql_query2_hh__

#include "sql_query1.hh"

inline SQLString &SQLQueryParms::operator[] (const char *str) {
  if (parent) return operator[] (parent->parsed_nums[str]);
  throw; // only here temporary 
}
inline const SQLString &SQLQueryParms::operator[] (const char *str) const {
  if (parent) return operator[] (parent->parsed_nums[str]);
  throw; // only here temporary 
}

#endif
