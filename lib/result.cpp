#include "result.h"

#include "connection.h"

namespace mysqlpp {

ResNSel::ResNSel(Connection* q) :
success(q->success()),
insert_id(q->insert_id()),
rows(q->affected_rows()),
info(q->info())
{
}

ResUse::ResUse(MYSQL_RES* result, Connection* m, bool te) :
mysql(m),
throw_exceptions(te),
initialized(false),
_fields(this)
{
	if (!result) {
		mysql_res = 0;
		_types = 0;
		_names = 0;
		return;
	}
	mysql_res = result;
	_names = new FieldNames(this);
	if (_names) {
		_types = new FieldTypes(this);
	}
	_table = fields(0).table;
	initialized = true;
}

ResUse::~ResUse()
{
	if (mysql) {
		mysql->unlock();
	}
	purge();
}

void ResUse::copy(const ResUse& other)
{
	if (!other.mysql_res) {
		mysql_res = 0;
		_types = 0;
		_names = 0;
		return;
	}

	if (initialized) {
		purge();
	}

	throw_exceptions = other.throw_exceptions;
	mysql_res = other.mysql_res;
	_fields = other._fields;

	if (other._names) {
		_names = new FieldNames(*other._names);
	}
	else {
		_names = NULL;
	}
	
	if (other._types) {
		_types = new FieldTypes(*other._types);
	}
	else {
		_types = NULL;
	}

	mysql = other.mysql;
	initialized = true;
}

} // end namespace mysqlpp

