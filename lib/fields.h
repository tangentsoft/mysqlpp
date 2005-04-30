#ifndef MYSQLPP_FIELDS_H
#define MYSQLPP_FIELDS_H

/// \file fields.h
/// \brief Declares a class for holding information about a set of
/// fields.

#include "resiter.h"

namespace mysqlpp {

class ResUse;

/// \brief A container similar to \c std::vector for holding
/// mysqlpp::Field records.
class Fields : public const_subscript_container<Fields, Field>
{
private:
  mutable ResUse *res;
public:
  Fields() {}
  Fields(ResUse *r) : res(r) {}
  size_type size() const; ///< get the number of fields

  /// \brief Returns a field given its index.
  const Field& operator [] (size_type i) const;
  const Field& operator [] (int i) const {return operator [](size_type(i));} 
};

} // end namespace mysqlpp

#endif

