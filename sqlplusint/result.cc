
#include "result3.hh"

pointer_tracker<MYSQL_RES, ResUse> ResUse::others = 
                                  pointer_tracker<MYSQL_RES, ResUse>();

ResUse::ResUse (MYSQL_RES *result, Connection *m, bool te) 
  : mysql(m), mysql_res(result), throw_exceptions(te), _names(NULL), _types(NULL), _fields(this)
{
  _table = fields(0).table;
  if (mysql) mysql->add_child(this);
  others.insert(mysql_res,this);
}

ResUse::~ResUse () {
  if (mysql) mysql->unlock(); 
  if (mysql) mysql->remove_child(this);
  others.remove(mysql_res,this);
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
  if (mysql) mysql->add_child(this);
  others.insert(mysql_res,this);
}

void MutableRes::populate(ResUse res) {
  Row row = res.fetch_row();
  _columns = row.size();
  while (row) {
    parent::push_back(MutableRow<MutableRes>(row, this));
    row = res.fetch_row();
  }
}





