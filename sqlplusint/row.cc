
#include "result1.hh"
#include "row3.hh"

MutableRow<ResUse>::MutableRow(const Row &r) : MutableRow_base<ResUse>(r.res) {
  if (r) {
    reserve(r.size());
    for (size_type i = 0; i < r.size(); i++) {
      push_back(r.raw_data(i));
    }
  }
}

MutableRow<ResUse>::MutableRow(const ResUse &r) : MutableRow_base<ResUse>(&r) {
  insert(begin(), r.columns(), MutableColData());
}

MutableRow<MutableRes>::MutableRow(const Row &r, const MutableRes *_res) 
  : MutableRow_base<MutableRes>(_res) 
{
  if (r) {
    reserve(r.size());
    for (size_type i = 0; i < r.size(); i++) {
      push_back(r.raw_data(i));
    }
  }
}



