#ifdef __WIN32__
#include <Windows32/Base.h>
#include <Windows32/Defines.h>
#include <Windows32/Structures.h>
#include <winsock.h>
#define errno WSAGetLastError()
#endif

#include "sql_query3.hh"
#include "exceptions.hh"
//#include "result3.hh"

SQLQuery::SQLQuery(const SQLQuery &q) {
  *this << q.str();
  Success = q.Success;
  def = q.def;
}


SQLQuery& SQLQuery::operator = (const SQLQuery &q) {
  reset();
  *this << q.str();
  Success = q.Success;
  def = q.def;
  return *this;
}


void SQLQuery::reset() {
  seekg (0L,ios::beg);
  seekp (0L,ios::beg);
  parsed.erase(parsed.begin(), parsed.end());
  def.clear(); clear();
}

char * SQLQuery::preview_char() {
  *this << ends;
#ifdef __USLC__
  strstreambuf *tmpbuf = rdbuf();
  uint length = tmpbuf->pcount();
#else
  uint length = pcount();
#endif
  char *s = new char[length+1]; 
  get(s, length, '\0'); 
  seekg (0,ios::beg);
  seekp (-1,ios::cur);
  return s;
}

SQLString * pprepare (char option, SQLString &S, bool replace = true) {
  if (S.processed) return &S;
  if (option == 'r' || (option == 'q' && S.is_string)) {
    char *s = new char[S.size()*2 + 1];
    mysql_escape_string(s,const_cast<char *>(S.c_str()),S.size());
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
  seekg (0,ios::beg);
  seekp (0,ios::beg);
  char      num;
  SQLString *ss;
  SQLQueryParms *c;
  for (vector<SQLParseElement>::iterator i = parsed.begin();
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

string SQLQuery::str(const SQLQueryParms &p) const {
  SQLQuery *const_this = const_cast<SQLQuery *>(this);
  if (!parsed.empty()) const_this->proc(const_cast<SQLQueryParms&>(p));
  *const_this << ends;
#ifdef __USLC__
  strstreambuf *tmpbuf = const_this->rdbuf();
  uint length = tmpbuf->pcount() + 1;
  char *s = new char[length]; 
#else
  uint length = const_this->pcount() + 1;
  char s[length]; 
#endif
  const_this->get(s, length, '\0'); 
  const_this->seekg (0,ios::beg);
  const_this->seekp (-1,ios::cur);
  return string(s);
}

string SQLQuery::str(const SQLQueryParms &p, query_reset r) {
  string tmp = str(p);
  if (r==RESET_QUERY) reset();
  return tmp;
}

SQLQueryParms SQLQueryParms::operator + (const SQLQueryParms &other) const {
  if (other.size() <= size()) return *this;
  SQLQueryParms New = *this;
  unsigned int i;
  for(i = size(); i < other.size(); i++) {
    New.push_back(other[i]);
  }
  return New;
}

void SQLQuery::parse() {
  string str = "";
  char num[4];
  long int n;
  char option;
  string name;
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
				(vector<string>::size_type)(n+1) 
				- parsed_names.size(), string());
	  parsed_names[n] = name;
	  parsed_nums[name] = n;
	}
	    
	parsed.push_back( SQLParseElement(str,option,n) );
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

