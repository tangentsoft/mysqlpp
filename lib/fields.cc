#include "fields.h"

#include "result.h"

using namespace mysqlpp;


Fields::size_type Fields::size() const 
{
	return res->num_fields();
}

const Field& Fields::operator[](Fields::size_type i) const
{
    res->field_seek(i); 
    return res->fetch_field();
}


