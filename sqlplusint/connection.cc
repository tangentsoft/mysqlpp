
#include "connection3.hh"
#include "result3.hh"

Connection::Connection (const char *db, const char *host, const char *user, 
			const char *passwd, bool te) 
  : throw_exceptions(te), locked(false)
{
	mysql_init(&mysql);
  if (real_connect (db, host, user, passwd,3306,0,60,NULL,0))
	{
    locked = false;
    Success = is_connected = true;
  }
  else
  {
    locked = false; Success = is_connected = false;
    if (throw_exceptions) throw BadQuery(error());
  }
}

Connection::Connection (const char *db, const char *host, const char *user, 
			const char *passwd, uint port, my_bool compress = 0,
			unsigned int connect_timeout = 60, bool te = true,
			const char *socket_name = "", unsigned client_flag = 0)
  : throw_exceptions(te), locked(false)
{
	mysql_init(&mysql);
  if (real_connect (db, host, user, passwd, port, compress,		connect_timeout,socket_name, client_flag))
  {
    locked = false;
    Success = is_connected = true;
  }
  else
  {
    locked = false; Success = is_connected = false;
    if (throw_exceptions) throw BadQuery(error());
  }
}

bool Connection::real_connect (cchar *db, cchar *host, cchar *user,
			       cchar *passwd, uint port, my_bool compress = 0,
			       unsigned int connect_timeout = 60,
			       const char *socket_name = "", unsigned int client_flag = 0)
{
  mysql.options.compress = compress;
  mysql.options.connect_timeout=connect_timeout;
  locked = true; //mysql.options.my_cnf_file="my";

	mysql_options(&mysql, MYSQL_READ_DEFAULT_FILE, "my");

	if (mysql_real_connect(&mysql,host,user,passwd,db, port,socket_name,client_flag))
  {
    locked = false;
    Success = is_connected = true;
  }
  else
  {
    locked = false; Success = is_connected = false;
    if (throw_exceptions) throw BadQuery(error());
  }
	//	mysql.options.my_cnf_file=0;
  if (!Success) return Success;
  if (db && db[0]) // if db is not empty
    Success = select_db(db);
  return Success;
}

Connection::~Connection () {
	mysql_close(&mysql);
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
  locked = true; // mysql.options.my_cnf_file="my";

	mysql_options(&mysql, MYSQL_READ_DEFAULT_FILE, "my");

	if (mysql_real_connect(&mysql,host,user,passwd,db, 3306,NULL,0)) {
    locked = false;
    Success = is_connected = true;
  } else {
    locked = false;
    if (throw_exceptions) throw BadQuery(error());
    Success = is_connected = false;
  }
	//	mysql.options.my_cnf_file=0;
  if (!Success) return Success;
  if (db && db[0]) // if db is not empty
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

bool Connection::exec(const string &str) {
	Success = !mysql_query(&mysql,str.c_str());
	if (!Success && throw_exceptions) throw BadQuery(error());
	return Success;
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



