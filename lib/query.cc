#include "query.h"

namespace mysqlpp {

Query::Query(const Query& q) :
SQLQuery(q),
mysql(q.mysql),
throw_exceptions(q.throw_exceptions)
{
}

bool Query::exec(const std::string& str)
{
	return mysql->exec(str);
}

bool Query::success()
{
	if (Success) {
		return mysql->success();
	}
	else {
		return false;
	}
}

ResNSel Query::execute(const char* str)
{
	return mysql->execute(str);
}

ResNSel Query::execute(parms& p)
{
	query_reset r = parsed.size() ? DONT_RESET : RESET_QUERY;
	return mysql->execute(str(p, r));
}

ResUse Query::use(const char* str)
{
	return mysql->use(str);
}

ResUse Query::use(parms& p)
{
	query_reset r = parsed.size() ? DONT_RESET : RESET_QUERY;
	return mysql->use(str(p, r));
}

Result Query::store(const char* str)
{
	return mysql->store(str);
}

Result Query::store(parms&  p)
{
	query_reset r = parsed.size() ? DONT_RESET : RESET_QUERY;
	return mysql->store(str(p, r));
}

my_ulonglong Query::affected_rows() const
{
	return mysql->affected_rows();
}

my_ulonglong Query::insert_id()
{
	return mysql->insert_id();
}

std::string Query::info()
{
	return mysql->info();
}

std::string Query::error()
{
	if (errmsg) {
		return std::string(errmsg);
	}
	else {
		return mysql->error();
	}
}

void Query::unlock()
{
	mysql->unlock();
}

};								// end namespace mysqlpp

