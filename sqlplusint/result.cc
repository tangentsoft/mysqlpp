#include <mysql++-config.hh>

#include "result3.hh"

ResUse::ResUse (MYSQL_RES *result, Connection *m, bool te) 
  : mysql(m), mysql_res(result), throw_exceptions(te), _fields(this)
{
  _table = fields(0).table;
	mysql_res=result;
	_names= new FieldNames(this);
	_types= new FieldTypes(this);
}

ResUse::~ResUse () {
  if (mysql) mysql->unlock(); 
  purge(mysql_res);
}

void ResUse::copy(const ResUse& other) {
  throw_exceptions = other.throw_exceptions;
  mysql_res = other.mysql_res;
  _fields   = other._fields;
  if (other._names)
    _names     = new FieldNames(*other._names);
  else
    _names     = NULL;
  if (other._types)
    _types     = new FieldTypes(*other._types);
  else
    _types     = NULL;
  mysql     = other.mysql;
}

void MutableRes::populate(ResUse res) {
  Row row = res.fetch_row();
  _columns = row.size();
  while (row) {
    parent::push_back(MutableRow<MutableRes>(row, this));
    row = res.fetch_row();
  }
}





