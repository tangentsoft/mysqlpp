#include "manip.h"

using namespace std;

// Manipulator stuff is _always_ in namespace mysqlpp.
namespace mysqlpp {

extern bool dont_quote_auto;

// quote manipulator

bool dont_quote_auto = false;

SQLQueryParms& operator <<(quote_type2 p, SQLString& in)
{
	if (in.is_string) {
		if (in.dont_escape) {
			SQLString in2 = "'" + in + "'";
			 in2.processed = true;
			 return *p.qparms << in2;
		}
		else {
			char *s = new char[in.size() * 2 + 1];
			mysql_escape_string(s, const_cast < char *>(in.c_str()),
								in.size());
			SQLString in2 = SQLString("'") + s + "'";
			in2.processed = true;
			*p.qparms << in2;
			delete[]s;
			return *p.qparms;
		}
	}
	else {
		in.processed = true;
		return *p.qparms << in;
	}
}

template<>
ostream& operator<<(quote_type1 o, const string& in)
{
	char *s = new char[in.size() * 2 + 1];
	mysql_escape_string(s, const_cast < char *>(in.c_str()),
						in.size());
	*o.ostr << "'" << s << "'";
	delete[] s;
	return *o.ostr;
}

template<>
ostream& operator<<(quote_type1 o, const char* const& in)
{
	unsigned int size;
	for (size = 0; in[size]; size++) ;
	char *s = new char[size * 2 + 1];
	mysql_escape_string(s, const_cast < char *>(in), size);
	*o.ostr << "'" << s << "'";
	delete[] s;
	return *o.ostr;
}

template<class Str>
inline ostream& _manip(quote_type1 o, const ColData_Tmpl<Str>& in)
{
	if (in.escape_q()) {
		char *s = new char[in.size() * 2 + 1];
		mysql_escape_string(s, const_cast<char *>(in.c_str()),
							in.size());
		if (in.quote_q())
			*o.ostr << "'" << s << "'";
		else
			*o.ostr << s;
		delete[] s;
	}
	else if (in.quote_q()) {
		*o.ostr << "'" << in << "'";
	}
	else {
		*o.ostr << in;
	}
	return *o.ostr;
}

template<>
ostream& operator<<(quote_type1 o, const ColData_Tmpl<string>& in)
{
	return _manip(o, in);
}

template<>
ostream& operator<<(quote_type1 o, const ColData_Tmpl<const_string>& in)
{
	return _manip(o, in);
}

ostream& operator<<(ostream& o, const ColData_Tmpl<string>& in)
{
	if (dont_quote_auto || (o.rdbuf() == cout.rdbuf()) ||
			(o.rdbuf() == cerr.rdbuf())) {
		return o << in.get_string();
	}

	if (in.escape_q()) {
		char *s = new char[in.size() * 2 + 1];
		mysql_escape_string(s, const_cast < char *>(in.c_str()),
							in.size());
		if (in.quote_q())
			o << "'" << s << "'";
		else
			o << s;
		delete[] s;
	}
	else if (in.quote_q()) {
		o << "'" << in.get_string() << "'";
	}
	else {
		o << in.get_string();
	}
	return o;
}


ostream& operator<<(ostream& o, const ColData_Tmpl<const_string>& in)
{
	if (dont_quote_auto || (o.rdbuf() == cout.rdbuf()) ||
			(o.rdbuf() == cerr.rdbuf())) {
		return o << in.get_string();
	}

	if (in.escape_q()) {
		char *s = new char[in.size() * 2 + 1];
		mysql_escape_string(s, const_cast < char *>(in.c_str()),
							in.size());
		if (in.quote_q())
			o << "'" << s << "'";
		else
			o << s;
		delete[] s;
	}
	else if (in.quote_q()) {
		o << "'" << in.get_string() << "'";
	}
	else {
		o << in.get_string();
	}
	return o;
}

SQLQuery& operator<<(SQLQuery& o, const ColData_Tmpl<string>& in)
{
	if (dont_quote_auto) {
		o << in.get_string();
		return o;
	}
	if (in.escape_q()) {
		char *s = new char[in.size() * 2 + 1];
		mysql_escape_string(s, const_cast < char *>(in.c_str()),
							in.size());
		if (in.quote_q())
			static_cast<ostream &>(o) << "'" << s << "'";
		else
			static_cast<ostream &>(o) << s;
		delete[] s;
	}
	else if (in.quote_q()) {
		static_cast<ostream &>(o) << "'" << in.get_string() << "'";
	}
	else {
		static_cast<ostream &>(o) << in.get_string();
	}
	return o;
}


SQLQuery& operator<<(SQLQuery& o, const ColData_Tmpl<const_string>& in)
{
	if (dont_quote_auto) {
		o << in.get_string();
		return o;
	}
	if (in.escape_q()) {
		char *s = new char[in.size() * 2 + 1];
		mysql_escape_string(s, const_cast < char *>(in.c_str()),
							in.size());
		if (in.quote_q())
			static_cast<ostream &>(o) << "'" << s << "'";
		else
			static_cast<ostream &>(o) << s;
		delete[] s;
	}
	else if (in.quote_q()) {
		static_cast<ostream &>(o) << "'" << in.get_string() << "'";
	}
	else {
		static_cast<ostream &>(o) << in.get_string();
	}
	return o;
}

// quote only manipulator

SQLQueryParms& operator<<(quote_only_type2 p, SQLString& in)
{
	if (in.is_string) {
		SQLString in2 = "'" + in + "'";
		in2.processed = true;
		return *p.qparms << in2;
	}
	else {
		in.processed = true;
		return *p.qparms << in;
	}
}

template<>
ostream& operator<<(quote_only_type1 o, const ColData_Tmpl<string>& in)
{
	if (in.quote_q()) {
		*o.ostr << "'" << in << "'";
	}
	else {
		*o.ostr << in;
	}
	return *o.ostr;
}

template<>
ostream& operator<<(quote_only_type1 o,
		const ColData_Tmpl<const_string>& in)
{
	if (in.quote_q()) {
		*o.ostr << "'" << in << "'";
	}
	else {
		*o.ostr << in;
	}
	return *o.ostr;
}

// quote double only manipulator

SQLQueryParms& operator<<(quote_double_only_type2 p, SQLString& in)
{
	if (in.is_string) {
		SQLString in2 = "\"" + in + "\"";
		in2.processed = true;
		return *p.qparms << in2;
	}
	else {
		in.processed = true;
		return *p.qparms << in;
	}
}


template<>
ostream& operator<<(quote_double_only_type1 o,
		const ColData_Tmpl<string>& in)
{
	if (in.quote_q()) {
		*o.ostr << "'" << in << "'";
	}
	else {
		*o.ostr << in;
	}
	return *o.ostr;
}

template<>
ostream& operator<<(quote_double_only_type1 o,
		const ColData_Tmpl<const_string>& in)
{
	if (in.quote_q()) {
		*o.ostr << "'" << in << "'";
	}
	else {
		*o.ostr << in;
	}
	return *o.ostr;
}

// escape manipulator

SQLQueryParms& operator<<(escape_type2 p, SQLString& in)
{
	if (in.is_string) {
		if (in.dont_escape) {
			in.processed = true;
			return *p.qparms << in;
		}
		else {
			char *s = new char[in.size() * 2 + 1];
			mysql_escape_string(s, const_cast < char *>(in.c_str()),
								in.size());
			SQLString in2 = s;
			in2.processed = true;
			*p.qparms << in2;
			delete[] s;
			return *p.qparms;
		}
	}
	else {
		in.processed = true;
		return *p.qparms << in;
	}
}

template<>
ostream& operator<<(escape_type1 o, const string& in)
{
	char *s = new char[in.size() * 2 + 1];
	mysql_escape_string(s, const_cast < char *>(in.c_str()),
						in.size());
	*o.ostr << s;
	delete[] s;
	return *o.ostr;
}

template<>
ostream& operator<<(escape_type1 o, const char* const& in)
{
	unsigned int size;
	for (size = 0; in[size]; size++) ;
	char *s = new char[size * 2 + 1];
	mysql_escape_string(s, const_cast < char *>(in), size);
	*o.ostr << s;
	delete[] s;
	return *o.ostr;
}


template<class Str>
inline ostream& _manip(escape_type1 o, const ColData_Tmpl<Str>& in)
{
	if (in.escape_q()) {
		char *s = new char[in.size() * 2 + 1];
		mysql_escape_string(s, const_cast<char*>(in.c_str()),
				in.size());
		delete[] s;
	}
	else {
		*o.ostr << in;
	}
	return *o.ostr;
}

template<>
ostream& operator<<(escape_type1 o, const ColData_Tmpl<string>& in)
{
	return _manip(o, in);
}

template<>
ostream& operator<<(escape_type1 o, const ColData_Tmpl<const_string>& in)
{
	return _manip(o, in);
}

} // end namespace mysqlpp

