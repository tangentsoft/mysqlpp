
#include "field_names3.hh"
#include "result2.hh"

void FieldNames::init(const ResUse *res) {
  int num = res->num_fields();
  reserve(num);
  for (int i = 0; i < num; i++) {
    push_back(res->fields()[i].name);
  }
}

