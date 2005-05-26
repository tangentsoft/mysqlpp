/***********************************************************************
 manip.cpp - Implements MySQL++'s various quoting/escaping stream
	manipulators.

 Copyright (c) 1998 by Kevin Atkinson, (c) 1999, 2000 and 2001 by
 MySQL AB, and (c) 2004, 2005 by Educational Technology Resources, Inc.
 Others may also hold copyrights on code in this file.  See the CREDITS
 file in the top directory of the distribution for details.

 This file is part of MySQL++.

 MySQL++ is free software; you can redistribute it and/or modify it
 under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 MySQL++ is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with MySQL++; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
 USA
***********************************************************************/

#include "manip.h"

using namespace std;

// Manipulator stuff is _always_ in namespace mysqlpp.
namespace mysqlpp {

/// \brief Set to true if you want to suppress automatic quoting
///
/// Works only for ColData inserted into C++ streams.

bool dont_quote_auto = false;


/// \brief Inserts a SQLString into a stream, quoted and escaped.
///
/// If in.is_string is set and in.dont_escape is \e not set, the string
/// is quoted and escaped.
///
/// If both in.is_string and in.dont_escape are set, the string is
/// quoted but not escaped.
///
/// If in.is_string is not set, the data is inserted as-is.  This is
/// the case when you initialize SQLString with one of the constructors
/// taking an integral type, for instance.

SQLQueryParms& operator <<(quote_type2 p, SQLString& in)
{
	if (in.is_string) {
		if (in.dont_escape) {
			SQLString in2 = '\'' + in + '\'';
			 in2.processed = true;
			 return *p.qparms << in2;
		}
		else {
			char* s = new char[in.size() * 2 + 1];
			mysql_escape_string(s, in.c_str(), in.size());
			SQLString in2 = SQLString('\'') + s + '\'';
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


/// \brief Inserts a C++ string into a stream, quoted and escaped
///
/// Because std::string lacks the type information we need, the string
/// is both quoted and escaped, always.

template <>
ostream& operator <<(quote_type1 o, const string& in)
{
	char* s = new char[in.size() * 2 + 1];
	mysql_escape_string(s, in.c_str(),
			static_cast<unsigned long>(in.size()));
	*o.ostr << '\'' << s << '\'';
	delete[] s;
	return *o.ostr;
}


/// \brief Inserts a C string into a stream, quoted and escaped
///
/// Because C strings lack the type information we need, the string
/// is both quoted and escaped, always.

template <>
ostream& operator <<(quote_type1 o, const char* const& in)
{
	unsigned int size = strlen(in);
	char* s = new char[size * 2 + 1];
	mysql_escape_string(s, in, size);
	*o.ostr << '\'' << s << '\'';
	delete[] s;
	return *o.ostr;
}


/// \brief Utility function used by operator<<(quote_type1, ColData)

template<class Str>
inline ostream& _manip(quote_type1 o, const ColData_Tmpl<Str>& in)
{
	if (in.escape_q()) {
		char* s = new char[in.size() * 2 + 1];
		mysql_escape_string(s, in.c_str(), in.size());
		if (in.quote_q())
			*o.ostr << '\'' << s << '\'';
		else
			*o.ostr << s;
		delete[] s;
	}
	else if (in.quote_q()) {
		*o.ostr << '\'' << in << '\'';
	}
	else {
		*o.ostr << in;
	}
	return *o.ostr;
}


/// \brief Inserts a ColData into a stream, quoted and escaped
///
/// Because ColData was designed to contain MySQL type data, we may
/// choose not to actually quote or escape the data, if it is not
/// needed.

template <>
ostream& operator <<(quote_type1 o, const ColData_Tmpl<string>& in)
{
	return _manip(o, in);
}


/// \brief Inserts a ColData with const string into a stream, quoted and
/// escaped
///
/// Because ColData was designed to contain MySQL type data, we may
/// choose not to actually quote or escape the data, if it is not
/// needed.

template <>
ostream& operator <<(quote_type1 o, const ColData_Tmpl<const_string>& in)
{
	return _manip(o, in);
}


/// \brief Inserts a ColData into a stream.
///
/// Because ColData was designed to contain MySQL type data, this
/// operator has the information needed to choose to quote and/or escape
/// the data as it is inserted into the stream, even if you don't use
/// any of the quoting or escaping manipulators.

ostream& operator <<(ostream& o, const ColData_Tmpl<string>& in)
{
	if (dont_quote_auto || (o.rdbuf() == cout.rdbuf()) ||
			(o.rdbuf() == cerr.rdbuf())) {
		return o << in.get_string();
	}

	if (in.escape_q()) {
		char* s = new char[in.size() * 2 + 1];
		mysql_escape_string(s, in.c_str(), in.size());
		if (in.quote_q())
			o << '\'' << s << '\'';
		else
			o << s;
		delete[] s;
	}
	else if (in.quote_q()) {
		o << '\'' << in.get_string() << '\'';
	}
	else {
		o << in.get_string();
	}
	return o;
}


/// \brief Inserts a ColData with const string into a stream.
///
/// Because ColData was designed to contain MySQL type data, this
/// operator has the information needed to choose to quote and/or escape
/// the data as it is inserted into the stream, even if you don't use
/// any of the quoting or escaping manipulators.

ostream& operator <<(ostream& o, const ColData_Tmpl<const_string>& in)
{
	if (dont_quote_auto || (o.rdbuf() == cout.rdbuf()) ||
			(o.rdbuf() == cerr.rdbuf())) {
		return o << in.get_string();
	}

	if (in.escape_q()) {
		char* s = new char[in.size() * 2 + 1];
		mysql_escape_string(s, in.c_str(), in.size());
		if (in.quote_q())
			o << '\'' << s << '\'';
		else
			o << s;
		delete[] s;
	}
	else if (in.quote_q()) {
		o << '\'' << in.get_string() << '\'';
	}
	else {
		o << in.get_string();
	}
	return o;
}


/// \brief Insert a ColData into a SQLQuery
///
/// This operator appears to be a workaround for a weakness in one
/// compiler's implementation of the C++ type system.  See Wishlist for
/// current plan on what to do about this.

SQLQuery& operator <<(SQLQuery& o, const ColData_Tmpl<string>& in)
{
	if (dont_quote_auto) {
		o << in.get_string();
		return o;
	}
	if (in.escape_q()) {
		char* s = new char[in.size() * 2 + 1];
		mysql_escape_string(s, in.c_str(), in.size());
		if (in.quote_q())
			static_cast<ostream&>(o) << '\'' << s << '\'';
		else
			static_cast<ostream&>(o) << s;
		delete[] s;
	}
	else if (in.quote_q()) {
		static_cast<ostream&>(o) << '\'' << in.get_string() << '\'';
	}
	else {
		static_cast<ostream&>(o) << in.get_string();
	}
	return o;
}


/// \brief Insert a ColData with const string into a SQLQuery
///
/// This operator appears to be a workaround for a weakness in one
/// compiler's implementation of the C++ type system.  See Wishlist for
/// current plan on what to do about this.

SQLQuery& operator <<(SQLQuery& o, const ColData_Tmpl<const_string>& in)
{
	if (dont_quote_auto) {
		o << in.get_string();
		return o;
	}
	if (in.escape_q()) {
		char* s = new char[in.size() * 2 + 1];
		mysql_escape_string(s, in.c_str(), in.size());
		if (in.quote_q())
			static_cast<ostream&>(o) << '\'' << s << '\'';
		else
			static_cast<ostream&>(o) << s;
		delete[] s;
	}
	else if (in.quote_q()) {
		static_cast<ostream&>(o) << '\'' << in.get_string() << '\'';
	}
	else {
		static_cast<ostream&>(o) << in.get_string();
	}
	return o;
}


/// \brief Inserts a SQLString into a stream, quoting it unless it's
/// data that needs no quoting.
///
/// We make the decision to quote the data based on the in.is_string
/// flag.  You can set it yourself, but SQLString's ctors should set
/// it correctly for you.

SQLQueryParms& operator <<(quote_only_type2 p, SQLString& in)
{
	if (in.is_string) {
		SQLString in2 = '\'' + in + '\'';
		in2.processed = true;
		return *p.qparms << in2;
	}
	else {
		in.processed = true;
		return *p.qparms << in;
	}
}


/// \brief Inserts a ColData into a stream, quoted
///
/// Because ColData was designed to contain MySQL type data, we may
/// choose not to actually quote the data, if it is not needed.

template <>
ostream& operator <<(quote_only_type1 o, const ColData_Tmpl<string>& in)
{
	if (in.quote_q()) {
		*o.ostr << '\'' << in << '\'';
	}
	else {
		*o.ostr << in;
	}
	return *o.ostr;
}


/// \brief Inserts a ColData with const string into a stream, quoted
///
/// Because ColData was designed to contain MySQL type data, we may
/// choose not to actually quote the data, if it is not needed.

template <>
ostream& operator <<(quote_only_type1 o,
		const ColData_Tmpl<const_string>& in)
{
	if (in.quote_q()) {
		*o.ostr << '\'' << in << '\'';
	}
	else {
		*o.ostr << in;
	}
	return *o.ostr;
}


/// \brief Inserts a SQLString into a stream, double-quoting it (")
/// unless it's data that needs no quoting.
///
/// We make the decision to quote the data based on the in.is_string
/// flag.  You can set it yourself, but SQLString's ctors should set
/// it correctly for you.

SQLQueryParms& operator <<(quote_double_only_type2 p, SQLString& in)
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


/// \brief Inserts a ColData into a stream, double-quoted (")
///
/// Because ColData was designed to contain MySQL type data, we may
/// choose not to actually quote the data, if it is not needed.

template <>
ostream& operator <<(quote_double_only_type1 o,
		const ColData_Tmpl<string>& in)
{
	if (in.quote_q()) {
		*o.ostr << '\'' << in << '\'';
	}
	else {
		*o.ostr << in;
	}
	return *o.ostr;
}


/// \brief Inserts a ColData with const string into a stream,
/// double-quoted (")
///
/// Because ColData was designed to contain MySQL type data, we may
/// choose not to actually quote the data, if it is not needed.

template <>
ostream& operator <<(quote_double_only_type1 o,
		const ColData_Tmpl<const_string>& in)
{
	if (in.quote_q()) {
		*o.ostr << '\'' << in << '\'';
	}
	else {
		*o.ostr << in;
	}
	return *o.ostr;
}


SQLQueryParms& operator <<(escape_type2 p, SQLString& in)
{
	if (in.is_string && ! in.dont_escape) {
		char* s = new char[in.size() * 2 + 1];
		mysql_escape_string(s, in.c_str(), in.size());
		SQLString in2 = s;
		in2.processed = true;
		*p.qparms << in2;
		delete[] s;
		return *p.qparms;
	}
	else {
		in.processed = true;
		return *p.qparms << in;
	}
}


/// \brief Inserts a C++ string into a stream, escaping special SQL
/// characters
///
/// Because std::string lacks the type information we need, the string
/// is always escaped, even if it doesn't need it.

template <>
std::ostream& operator <<(escape_type1 o, const std::string& in)
{
	char* s = new char[in.size() * 2 + 1];
	mysql_escape_string(s, in.c_str(),
			static_cast<unsigned long>(in.size()));
	*o.ostr << s;
	delete[] s;
	return *o.ostr;
}


/// \brief Inserts a C string into a stream, escaping special SQL
/// characters
///
/// Because C's type system lacks the information we need to second-
/// guess this manipulator, we always run the escaping algorithm on
/// the data, even if it's not needed.

template <>
ostream& operator <<(escape_type1 o, const char* const& in)
{
	unsigned int size = strlen(in);
	char* s = new char[size * 2 + 1];
	mysql_escape_string(s, in, size);
	*o.ostr << s;
	delete[] s;
	return *o.ostr;
}


/// \brief Utility function used by operator<<(escape_type1, ColData)

template <class Str>
inline ostream& _manip(escape_type1 o, const ColData_Tmpl<Str>& in)
{
	if (in.escape_q()) {
		char* s = new char[in.size() * 2 + 1];
		mysql_escape_string(s, in.c_str(), in.size());
		delete[] s;
	}
	else {
		*o.ostr << in;
	}
	return *o.ostr;
}


/// \brief Inserts a ColData into a stream, escaping special SQL
/// characters
///
/// Because ColData was designed to contain MySQL type data, we may
/// choose not to escape the data, if it is not needed.

template <>
std::ostream& operator <<(escape_type1 o,
		const ColData_Tmpl<std::string>& in)
{
	return _manip(o, in);
}


/// \brief Inserts a ColData with const string into a stream, escaping
/// special SQL characters
///
/// Because ColData was designed to contain MySQL type data, we may
/// choose not to escape the data, if it is not needed.

template <>
std::ostream& operator <<(escape_type1 o, const ColData_Tmpl<const_string>& in)
{
	return _manip(o, in);
}

} // end namespace mysqlpp

