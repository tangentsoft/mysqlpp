#include "row.h"
#include "result.h"
#include "exceptions.h"

namespace mysqlpp {

Row::size_type Row::size() const
{
	return res->num_fields();
}

const ColData Row::operator[] (size_type i) const
{
	if (!initialized) {
		if (throw_exceptions)
			throw BadQuery("Row not initialized");
		else
			return ColData();
	}
	
	return ColData(data.at(i).c_str(), res->types(i), is_nulls[i]);
}

const ColData Row::lookup_by_name(const char* i) const
{
	int si = res->field_num(std::string(i));
	if (si < res->num_fields()) {
		return (*this)[si];
	}
	else {
		throw BadFieldName(i);
	}
}

} // end namespace mysqlpp

