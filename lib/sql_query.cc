#define MYSQLPP_NOT_HEADER
#include "platform.h"

#include "sql_query.h"

#include "exceptions.h"

using namespace std;
using namespace mysqlpp;


SQLQuery::SQLQuery(const SQLQuery &q) :
stringstream(const_cast<SQLQuery&>(q).str()),	// yes, the cast is evil -- got a better idea?
Success(q.Success),
errmsg(q.errmsg),
def(q.def)
{
}


void SQLQuery::reset() {
  seekg(0);
  seekp(0);
  clear();
  stringstream::str("");

  parsed.clear();
  def.clear();
}

char * SQLQuery::preview_char() {
  *this << std::ends;
  size_t length = rdbuf()->str().size();
  char *s = new char[length + 1];
  get(s, length, '\0'); 
  seekg (0,std::ios::beg);
  seekp (-1,std::ios::cur);
  return s;
}

SQLString * pprepare (char option, SQLString &S, bool replace = true) {
  if (S.processed) return &S;
  if (option == 'r' || (option == 'q' && S.is_string)) {
    char *s = new char[S.size()*2 + 1];
    mysql_escape_string(s, const_cast<char *>(S.c_str()),
			(unsigned long)S.size());
    SQLString *ss = new SQLString("'");
    *ss += s;
    *ss += "'";
    delete[] s;
    if (replace) {S = *ss; S.processed = true; return &S;}
    return ss;
  } else if (option == 'R' || (option == 'Q' && S.is_string)) {
    SQLString *ss = new SQLString("'" + S + "'");
    if (replace) {S = *ss; S.processed = true; return &S;}
    return ss;
  } else {
    if (replace) S.processed = true;
    return &S;
  }
}

void SQLQuery::proc(SQLQueryParms& p) {
  seekg (0,std::ios::beg);
  seekp (0,std::ios::beg);
  char      num;
  SQLString *ss;
  SQLQueryParms *c;
  for (std::vector<SQLParseElement>::iterator i = parsed.begin();
       i != parsed.end(); i++) {
    *this << i->before;
    num    = i->num;
    if (num == -1) {
      // do nothing    
    } else {
      if (num < (int)p.size()) c = &p;
      else if (num < (int)def.size()) c = &def;
      else {
	*this << " ERROR";
	throw SQLQueryNEParms("Not enough parameters to fill the template.");
      }
      ss = pprepare(i->option, (*c)[num], c->bound());
      *this << *ss;
      if (ss != &(*c)[num]) delete ss;
    }
  }
} 

std::string SQLQuery::str(SQLQueryParms &p) {
  if (!parsed.empty()) proc(p);
  *this << std::ends;
  return stringstream::str();
}

std::string SQLQuery::str(SQLQueryParms &p, query_reset r) {
  std::string tmp = str(p);
  if (r==RESET_QUERY) reset();
  return tmp;
}

SQLQueryParms SQLQueryParms::operator + (const SQLQueryParms &other) const {
  if (other.size() <= size()) return *this;
  SQLQueryParms New = *this;
  size_t i;
  for(i = size(); i < other.size(); i++) {
    New.push_back(other[i]);
  }
  return New;
}

void SQLQuery::parse() {
  std::string str = "";
  char num[4];
  long int n;
  char option;
  std::string name;
  char *s, *s0;
  s0 = s = preview_char();
  while (*s) {
    if (*s == '%') {
      s++;
      if (*s == '%') {
	str += *s++;
      } else if (*s >= '0' && *s <= '9') {
	num[0] = *s;
	s++;
	if (*s >= '0' && *s <= '9') {
	  num[1] = *s;
	  num[2] = 0;
          s++;
		if (*s >= '0' && *s <= '9') {
			num[2] = *s;
			num[3] = 0;
			s++;
		}
		else num[2] = 0;
	} else {
	  num[1] = 0;
	}
        n = strtol(num,NULL,10);
        option = ' ';
        if (*s == 'q' || *s == 'Q' || *s == 'r' || *s == 'R')
	  option = *s++;
	if (*s == ':') {
	  s++;
	  for (;(*s>='A' && *s<='Z') || *s=='_' || (*s>='a' && *s<='z'); s++) {
	    name += *s;
	  }
	  if (*s == ':') s++;
          if (n >= (long int)parsed_names.size())
	    parsed_names.insert(parsed_names.end(),
				(std::vector<std::string>::size_type)(n+1)
				- parsed_names.size(), std::string());
	  parsed_names[n] = name;
	  parsed_nums[name] = n;
	}
	    
	parsed.push_back( SQLParseElement(str, option, char(n)) );
	str = "";
	name = "";
      } else {
	str += '%';
      }
    } else {
      str += *s++;
    }
  }
  parsed.push_back( SQLParseElement(str,' ',-1) );
  delete[] s0;
}

