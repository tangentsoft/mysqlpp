
#include "query3.hh"

MysqlQuery::MysqlQuery(const MysqlQuery &q) : SQLQuery(q) {
  throw_exceptions = q.throw_exceptions;
  mysql = q.mysql;
}

MysqlQuery& MysqlQuery::operator = (const MysqlQuery &q) {
  throw_exceptions = q.throw_exceptions;
  mysql = q.mysql;
  SQLQuery::operator = (q);
  return *this;
}

bool MysqlQuery::exec (const std::string& str) { return mysql->exec(str);}





