#define MYSQLPP_NOT_HEADER
#include "platform.h"

#include "field_names.h"

#include "result.h"

namespace mysqlpp {

void FieldNames::init(const ResUse * res)
{
	int num = res->num_fields();
	reserve(num);

	for (int i = 0; i < num; i++) {
		std::string p(res->fields()[i].name);
		str_to_lwr(p);
		push_back(p);
	}
}

};							// end namespace mysqlpp
