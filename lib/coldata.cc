#include "coldata.h"
#include "const_string.h"

#include <string>

namespace mysqlpp {

template class ColData_Tmpl < std::string >;
template class ColData_Tmpl < const_string >;

};								// end namespace mysqlpp
