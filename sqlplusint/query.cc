#include "query3.hh"

MysqlQuery::MysqlQuery(const MysqlQuery &q) :
SQLQuery(q),
mysql(q.mysql),
throw_exceptions(q.throw_exceptions)
{
}

bool MysqlQuery::exec(const std::string& str)
{
	return mysql->exec(str);
}

