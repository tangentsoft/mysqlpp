
#include "connection3.hh"
#include "result3.hh"

pointer_tracker<MYSQL,Connection> 
Connection::others = pointer_tracker<MYSQL, Connection>();

Connection::Connection (const char *db, const char *host = "", const char *user = "", 
			const char *passwd = "", bool te = false) 
  : throw_exceptions(te), locked(false)
{
  connect (db, host, user, passwd);
  others.insert(&mysql,this);
}

Connection::~Connection () {
  for (set<MysqlChild*>::iterator i = children.begin(); 
       i != children.end();
       i++) 
    {
      (*i)->parent_leaving();
    }
  others.remove(&mysql,this);
}

bool Connection::select_db (const char *db) {
  bool suc = !(mysql_select_db(&mysql, db));
  if (throw_exceptions && !suc) throw MysqlBadQuery(error());
  else return suc;
}

bool Connection::reload() {
  bool suc = !mysql_reload(&mysql);
  if (throw_exceptions && !suc) throw MysqlBadQuery(error());
  else return suc;
}

bool Connection::shutdown () {
  bool suc = !(mysql_shutdown(&mysql));
  if (throw_exceptions && !suc) throw MysqlBadQuery(error());
  else return suc;
}  

bool Connection::connect (cchar *db, cchar *host, cchar *user, cchar *passwd) {
  locked = true;
  if (mysql_connect(&mysql, host, user, passwd)) {
    locked = false;
    Success = is_connected = true;
  } else {
    locked = false;
    if (throw_exceptions) throw BadQuery(error());
    Success = is_connected = false;
  }
  if (!Success) return Success;
  if (db[0]) // if db is not empty
    Success = select_db(db);
  return Success;
}

string Connection::info () {
  char *i = mysql_info(&mysql);
  if (!i)
    return string();
  else
    return string(i);
}

ResNSel Connection::execute(const string &str, bool throw_excptns) {
  Success = false;
  if (lock()) 
    if (throw_excptns) throw BadQuery(error());
    else return ResNSel();
  Success = !mysql_query(&mysql, str.c_str()); 
  unlock();
  if (!Success) 
    if (throw_excptns) throw BadQuery(error());
    else return ResNSel();
  return ResNSel(this);
}

Result Connection::store(const string &str, bool throw_excptns) {
  Success = false;
  if (lock()) 
    if (throw_excptns) throw BadQuery(error());
    else return Result();
  Success = !mysql_query(&mysql, str.c_str()); 
  unlock();
  if (!Success) 
    if (throw_excptns) throw BadQuery(error());
    else return Result();
  return Result(mysql_store_result(&mysql));
}
  
ResUse Connection::use(const string &str, bool throw_excptns) {
  Success = false;
  if (lock()) 
    if (throw_excptns) throw BadQuery(error());
    else return ResUse();
  Success = !mysql_query(&mysql, str.c_str()); 
  if (!Success) 
    if (throw_excptns) throw BadQuery(error());
    else return ResUse();
  return ResUse(mysql_use_result(&mysql), this);
}

void Connection::storein(MutableRes& con, const string &s) {
  con = use(s);
}


