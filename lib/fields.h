#ifndef __fields1_hh__
#define __fields1_hh__

#include <resiter.h>

namespace mysqlpp {

class ResUse;

//: A vector like container with the raw mysql field info.
class Fields : public const_subscript_container<Fields, Field>
{
private:
  mutable ResUse *res;
public:
  Fields() {}
  Fields(ResUse *r) : res(r) {}
  size_type size() const; //: the number of fields.
  //: Returns the field with an offset of i;
  const Field& operator [] (size_type i) const;
  const Field& operator [] (int i) const {return operator [](size_type(i));} 
};

}; // end namespace mysqlpp

#endif

