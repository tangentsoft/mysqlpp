#include "coldata.h"
#include "const_string.h"

#include <string>

using namespace mysqlpp;

template class ColData_Tmpl<std::string>;
template class ColData_Tmpl<const_string>;
