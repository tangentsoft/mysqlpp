
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

Connection::Connection (const char *db, const char *host, const char *user, 
			const char *passwd = "", uint port = 3306, int compress=0, unsigned int connect_timeout = 5, bool te = false, const char *socket_name) 
  : throw_exceptions(te), locked(false)
{
  real_connect (db, host, user, passwd, port, compress, connect_timeout,socket_name);
  others.insert(&mysql,this);
}

bool Connection::real_connect (cchar *db, cchar *host, cchar *user, cchar *passwd, uint port, int compress, unsigned int connect_timeout,  const char *socket_name) {
	if (socket_name && socket_name[0]) mysql.options.unix_socket = (char *)socket_name;  else mysql.options.unix_socket=NULL;
	mysql.options.port = port; mysql.options.compress = compress; mysql.options.connect_timeout=connect_timeout;
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


