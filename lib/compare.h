#ifndef MYSQLPP_COMPARE_H
#define MYSQLPP_COMPARE_H

#include "row.h"

#include <cstring>
#include <functional>

namespace mysqlpp {

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

template <class BinaryPred>
class MysqlCmpCStr : public MysqlCmp<BinaryPred, const char *>
{
public:
  MysqlCmpCStr(uint i, const BinaryPred &f, const char* c) : MysqlCmp<BinaryPred, const char *> (i,f,c) {}
  bool operator () (const Row& cmp1) const 
    {return MysqlCmp<BinaryPred, const char*>::func(
			MysqlCmp<BinaryPred, const char*>::cmp2, cmp1[index]);}
};

//: A special function for using in find_if function where i is the field index number.
// This is a more generic form of mysql_cmp_cstr will work with any
// CmpType that MysqlString can convert to.  However, this is not
// neary as effecent.  Only use when obsoletely nessary.
template <class BinaryPred, class CmpType>
MysqlCmp <BinaryPred, CmpType>
mysql_cmp(uint i, const BinaryPred &func, const CmpType &cmp2)
{
  return MysqlCmp<BinaryPred, CmpType>(i, func, cmp2);
}

typedef std::binary_function<const char*, const char*, bool> bin_char_pred;

struct cstr_equal_to : bin_char_pred {
  bool operator () (const char *x, const char *y) const
    {return !std::strcmp(x,y);}
};
struct cstr_not_equal_to : bin_char_pred {
  bool operator () (const char *x, const char *y) const
    {return std::strcmp(x,y) != 0;}
};
struct cstr_less : bin_char_pred {
  bool operator () (const char *x, const char *y) const
    {return std::strcmp(x,y) > 0; }
};
struct cstr_less_equal : bin_char_pred {
  bool operator () (const char *x, const char *y) const
    {return std::strcmp(x,y) >= 0; }
};
struct cstr_greater : bin_char_pred {
  bool operator () (const char *x, const char *y) const
    {return std::strcmp(x,y) < 0; }
};
struct cstr_greater_equal : bin_char_pred {
  bool operator () (const char *x, const char *y) const
    {return std::strcmp(x,y) <= 0; }
};

//:A special function for using in find_if fucntion where i is the field index
//:number.
//
// func should be one of cstr_equal_to(), cstr_not_equal_to(),
// cstr_less(), cstr_less_equal(), cstr_less_equal(), cstr_less_equal().
template <class BinaryPred>
MysqlCmpCStr <BinaryPred>
mysql_cmp_cstr (uint i, const BinaryPred &func, const char *cmp2) {
  return MysqlCmpCStr<BinaryPred>(i, func, cmp2);
}

} // end namespace mysqlpp

#endif

