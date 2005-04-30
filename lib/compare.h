#ifndef MYSQLPP_COMPARE_H
#define MYSQLPP_COMPARE_H

/// \file compare.h
/// Documentation needed!

#include "row.h"

#include <cstring>
#include <functional>

namespace mysqlpp {

/// Documentation needed!

template <class BinaryPred, class CmpType>
class MysqlCmp : public std::unary_function<const Row&, bool>
{
protected:
  unsigned int index;
  BinaryPred   func;
  CmpType      cmp2;
public:
  MysqlCmp(uint i, const BinaryPred &f, const CmpType &c) : index(i),func(f),cmp2(c) {}
  bool operator () (const Row& cmp1) const {return func(cmp2,cmp1[this->index]);}
};


/// Documentation needed!

template <class BinaryPred>
class MysqlCmpCStr : public MysqlCmp<BinaryPred, const char *>
{
public:
  MysqlCmpCStr(uint i, const BinaryPred &f, const char* c) : MysqlCmp<BinaryPred, const char *> (i,f,c) {}
  bool operator () (const Row& cmp1) const 
    {return MysqlCmp<BinaryPred, const char*>::func(
			MysqlCmp<BinaryPred, const char*>::cmp2, cmp1[this->index]);}
};

/// \brief For comparing any two objects, as long as they can be
/// converted to SQLString.
///
/// This template is for creating predicate objects for use with
/// STL algorithms like std::find_if().
///
/// This is a more generic form of mysql_cmp_cstr() which will work
/// with any C++ type that can be converted to mysqlpp::SQLString.
/// This is not nearly as efficient as that function, so use this only
/// when absolutely necessary.
///
/// \param i field index number
/// \param func one of the functors in compare.h, or any compatible functor
/// \param cmp2 what to compare to
template <class BinaryPred, class CmpType>
MysqlCmp <BinaryPred, CmpType>
mysql_cmp(uint i, const BinaryPred &func, const CmpType &cmp2)
{
  return MysqlCmp<BinaryPred, CmpType>(i, func, cmp2);
}

typedef std::binary_function<const char*, const char*, bool> bin_char_pred;

/// \brief Documentation needed!
///
/// Document me!
struct cstr_equal_to : bin_char_pred {
  bool operator () (const char *x, const char *y) const
    {return !std::strcmp(x,y);}
};

/// \brief Documentation needed!
///
/// Document me!
struct cstr_not_equal_to : bin_char_pred {
  bool operator () (const char *x, const char *y) const
    {return std::strcmp(x,y) != 0;}
};

/// \brief Documentation needed!
///
/// Document me!
struct cstr_less : bin_char_pred {
  bool operator () (const char *x, const char *y) const
    {return std::strcmp(x,y) > 0; }
};

/// \brief Documentation needed!
///
/// Document me!
struct cstr_less_equal : bin_char_pred {
  bool operator () (const char *x, const char *y) const
    {return std::strcmp(x,y) >= 0; }
};

/// \brief Documentation needed!
///
/// Document me!
struct cstr_greater : bin_char_pred {
  bool operator () (const char *x, const char *y) const
    {return std::strcmp(x,y) < 0; }
};

/// \brief Documentation needed!
///
/// Document me!
struct cstr_greater_equal : bin_char_pred {
  bool operator () (const char *x, const char *y) const
    {return std::strcmp(x,y) <= 0; }
};

/// \brief For comparing anything to a <tt>const char*</tt>.
///
/// This template is for creating predicate objects for use with
/// STL algorithms like std::find_if().
///
/// \param i field index number
/// \param func one of \c cstr_equal_to(), \c cstr_not_equal_to(),
///		\c cstr_less(), \c cstr_less_equal(), \c cstr_less_equal(), or
///		\c cstr_less_equal().
/// \param cmp2 what to compare to
template <class BinaryPred>
MysqlCmpCStr <BinaryPred>
mysql_cmp_cstr (uint i, const BinaryPred &func, const char *cmp2) {
  return MysqlCmpCStr<BinaryPred>(i, func, cmp2);
}

} // end namespace mysqlpp

#endif

