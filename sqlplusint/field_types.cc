#ifdef __WIN32__
#include <winsock.h>
#endif

#include "field_types3.hh"
#include "result2.hh"

void FieldTypes::init(const ResUse *res) {
  int num = res->num_fields();
  reserve(num);
  for (int i = 0; i < num; i++) {
    push_back(res->fields(i));
  }
}

