/// \file manip.h
/// \brief Declares \c std::ostream manipulators useful with SQL syntax.
///
/// These manipulators let you automatically quote elements or escape
/// characters that are special in SQL when inserting them into an
/// \c std::ostream. Since mysqlpp::Query is an ostream, these
/// manipulators make it easier to build syntactically-correct SQL
/// queries.
///
/// This file also includes \c operator<< definitions for ColData_Tmpl,
/// one of the MySQL++ string-like classes.  When inserting such items
/// into a stream, they are automatically quoted and escaped as
/// necessary unless the global variable dont_quote_auto is set to true.
/// These operators are smart enough to turn this behavior off when
/// the stream is \c cout or \c cerr, however, since quoting and
/// escaping are surely not required in that instance.

/***********************************************************************
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

#ifndef MYSQLPP_MANIP_H
#define MYSQLPP_MANIP_H

#include "defs.h"

#include "datetime.h"
#include "myset.h"
#include "sql_string.h"

#include <mysql.h>

#include <iostream>

/// All global symbols in MySQL++ are in namespace mysqlpp.  This is
/// needed because many symbols are rather generic (e.g. Row, Query...),
/// so there is a serious danger of conflicts.
namespace mysqlpp {

class Query;

extern bool dont_quote_auto;


/// \enum quote_type0
/// \anchor quote_manip
///
/// The standard 'quote' manipulator.
///
/// Insert this into a stream to put single quotes around the next item
/// in the stream, and escape characters within it that are 'special'
/// in SQL.  This is the most generally useful of the manipulators.


enum quote_type0
{
	quote					///< insert into a std::ostream to single-quote and escape next item
};


#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.

struct quote_type1
{
	std::ostream * ostr;
	quote_type1(std::ostream * o) :
	ostr(o)
	{
	}
};


MYSQLPP_EXPORT inline quote_type1 operator <<(std::ostream& o,
		quote_type0 /*esc */)
{
	return quote_type1(&o);
}


class SQLQueryParms;
struct quote_type2
{
	SQLQueryParms *qparms;
	quote_type2(SQLQueryParms* p) :
	qparms(p)
	{
	}
};


MYSQLPP_EXPORT inline quote_type2 operator <<(SQLQueryParms& p,
		quote_type0 /*esc */)
{
	return quote_type2(&p);
}


MYSQLPP_EXPORT SQLQueryParms& operator <<(quote_type2 p,
		SQLString& in);


template <class T>
inline std::ostream& operator <<(quote_type1 o, const T & in)
{
	return *o.ostr << in;
}


MYSQLPP_EXPORT std::ostream& operator <<(std::ostream& o,
		const ColData_Tmpl<std::string>& in);


MYSQLPP_EXPORT std::ostream& operator <<(std::ostream& o,
		const ColData_Tmpl<const_string>& in);


MYSQLPP_EXPORT Query& operator <<(Query& o,
		const ColData_Tmpl<std::string>& in);


MYSQLPP_EXPORT Query& operator <<(Query& o,
		const ColData_Tmpl<const_string>& in);


template <>
MYSQLPP_EXPORT std::ostream& operator <<(quote_type1 o,
		const std::string& in);


template <>
MYSQLPP_EXPORT std::ostream& operator <<(quote_type1 o,
		const char* const& in);


template <>
MYSQLPP_EXPORT std::ostream& operator <<(quote_type1 o,
		const ColData_Tmpl<std::string>& in);


template <>
MYSQLPP_EXPORT std::ostream& operator <<(quote_type1 o,
		const ColData_Tmpl<const_string>& in);


template <>
MYSQLPP_EXPORT inline std::ostream& operator <<(quote_type1 o,
		char* const& in)
{
	return operator <<(o, const_cast<const char* const&>(in));
}


template <>
MYSQLPP_EXPORT inline std::ostream& operator <<(quote_type1 o,
		const Date& in)
{
	return *o.ostr << '\'' << in << '\'';
}


template <>
MYSQLPP_EXPORT inline std::ostream& operator <<(quote_type1 o,
		const Time& in)
{
	return *o.ostr << '\'' << in << '\'';
}


template <>
MYSQLPP_EXPORT inline std::ostream& operator <<(quote_type1 o,
		const DateTime& in)
{
	return *o.ostr << '\'' << in << '\'';
}


template <class ST>
inline std::ostream& operator <<(quote_type1 o, const Set<ST>& in)
{
	return *o.ostr << '\'' << in << '\'';
}

#endif // !defined(DOXYGEN_IGNORE)


/// \enum quote_only_type0
/// \anchor quote_only_manip
///
/// The 'quote_only' manipulator.
///
/// Similar to <a href="#quote_manip">quote manipulator</a>, except that
/// it doesn't escape special SQL characters.

enum quote_only_type0
{
	quote_only				///< insert into a std::ostream to single-quote next item
};


#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.

struct quote_only_type1
{
	std::ostream* ostr;
	quote_only_type1(std::ostream* o) :
	ostr(o)
	{
	}
};


MYSQLPP_EXPORT inline quote_only_type1 operator <<(std::ostream& o,
		quote_only_type0 /* esc */)
{
	return quote_only_type1(&o);
}


struct quote_only_type2
{
	SQLQueryParms* qparms;
	quote_only_type2(SQLQueryParms* p) :
	qparms(p)
	{
	}
};


MYSQLPP_EXPORT inline quote_only_type2 operator <<(SQLQueryParms& p,
		quote_only_type0 /* esc */)
{
	return quote_only_type2(&p);
}

MYSQLPP_EXPORT SQLQueryParms& operator <<(quote_only_type2 p,
		SQLString& in);


template <class T>
inline std::ostream& operator <<(quote_only_type1 o, const T& in)
{
	return *o.ostr << in;
}


template <>
MYSQLPP_EXPORT inline std::ostream& operator <<(quote_only_type1 o,
		const std::string& in)
{
	return *o.ostr << '\'' << in << '\'';
}


template <>
MYSQLPP_EXPORT std::ostream& operator <<(quote_only_type1 o,
		const ColData_Tmpl<std::string>& in);


template <>
MYSQLPP_EXPORT std::ostream& operator <<(quote_only_type1 o,
		const ColData_Tmpl<const_string>& in);


template <>
MYSQLPP_EXPORT inline std::ostream& operator <<(quote_only_type1 o,
		const Date& in)
{
	return *o.ostr << '\'' << in << '\'';
}


template <>
MYSQLPP_EXPORT inline std::ostream& operator <<(quote_only_type1 o,
		const Time& in)
{
	return *o.ostr << '\'' << in << '\'';
}


template <>
MYSQLPP_EXPORT inline std::ostream& operator <<(quote_only_type1 o,
		const DateTime& in)
{
	return *o.ostr << '\'' << in << '\'';
}


template <class ST>
inline std::ostream& operator <<(quote_only_type1 o, const Set<ST>& in)
{
	return *o.ostr << '\'' << in << '\'';
}

#endif // !defined(DOXYGEN_IGNORE)


/// \enum quote_double_only_type0
/// \anchor quote_double_manip
///
/// The 'double_quote_only' manipulator.
///
/// Similar to <a href="#quote_only_manip">quote_only manipulator</a>,
/// except that it uses double quotes instead of single quotes.

enum quote_double_only_type0
{
	quote_double_only		///< insert into a std::ostream to double-quote next item
};


#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.

struct quote_double_only_type1
{
	std::ostream* ostr;
	quote_double_only_type1(std::ostream* o) :
	ostr(o)
	{
	}
};


MYSQLPP_EXPORT inline quote_double_only_type1 operator <<(
		std::ostream& o, quote_double_only_type0 /* esc */)
{
	return quote_double_only_type1(&o);
}


struct quote_double_only_type2
{
	SQLQueryParms *qparms;
	quote_double_only_type2(SQLQueryParms* p) :
	qparms(p)
	{
	}
};


MYSQLPP_EXPORT inline quote_double_only_type2 operator <<(
		SQLQueryParms& p, quote_double_only_type0 /* esc */)
{
	return quote_double_only_type2(&p);
}


MYSQLPP_EXPORT SQLQueryParms& operator <<(quote_double_only_type2 p,
		SQLString& in);


template <class T>
inline std::ostream& operator <<(quote_double_only_type1 o, const T& in)
{
	return *o.ostr << in;
}


template <>
MYSQLPP_EXPORT inline std::ostream& operator <<(
		quote_double_only_type1 o, const std::string& in)
{
	return *o.ostr << '"' << in << '"';
}


template <>
MYSQLPP_EXPORT std::ostream& operator <<(quote_double_only_type1 o,
		const ColData_Tmpl<std::string>& in);


template <>
MYSQLPP_EXPORT std::ostream & operator <<(quote_double_only_type1 o,
		const ColData_Tmpl<const_string>& in);


template <>
MYSQLPP_EXPORT inline std::ostream& operator <<(
		quote_double_only_type1 o, const Date& in)
{
	return *o.ostr << '"' << in << '"';
}


template <>
MYSQLPP_EXPORT inline std::ostream& operator <<(
		quote_double_only_type1 o, const Time& in)
{
	return *o.ostr << '"' << in << '"';
}


template <>
MYSQLPP_EXPORT inline std::ostream& operator <<(
		quote_double_only_type1 o, const DateTime& in)
{
	return *o.ostr << '"' << in << '"';
}


template <class ST>
inline std::ostream& operator <<(quote_double_only_type1 o,
		const Set<ST>& in)
{
	return *o.ostr << '"' << in << '"';
}

#endif // !defined(DOXYGEN_IGNORE)


/// \enum escape_type0
/// The 'escape' manipulator.
///
/// Calls mysql_escape_string() in the MySQL C API on the following
/// argument to prevent any special SQL characters from being
/// interpreted.

enum escape_type0 { escape };


#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.

struct escape_type1
{
	std::ostream* ostr;
	escape_type1(std::ostream* o) :
	ostr(o)
	{
	}
};


MYSQLPP_EXPORT inline escape_type1 operator <<(std::ostream& o,
		escape_type0 /*esc */)
{
	return escape_type1(&o);
}


struct escape_type2
{
	SQLQueryParms *qparms;
	escape_type2(SQLQueryParms* p) :
	qparms(p)
	{
	}
};


MYSQLPP_EXPORT inline escape_type2 operator <<(SQLQueryParms& p,
		escape_type0 /*esc */)
{
	return escape_type2(&p);
}

#endif // !defined(DOXYGEN_IGNORE)


/// \brief Inserts a SQLString into a stream, escaping special SQL
/// characters
///
/// We actually only do the escaping if in.is_string is set but
/// in.dont_escape is not.  If that is not the case, we insert the
/// string data directly.

MYSQLPP_EXPORT SQLQueryParms& operator <<(escape_type2 p,
		SQLString& in);


/// \brief Inserts any type T into a stream that has an operator<<
/// defined for it.
///
/// Does not actually escape that data!  Use one of the other forms of
/// operator<< for the escape manipulator if you need escaping.  This
/// template exists to catch cases like inserting an \c int after the
/// escape manipulator: you don't actually want escaping in this
/// instance.

template <class T>
inline std::ostream& operator <<(escape_type1 o, const T& in)
{
	return *o.ostr << in;
}


template <>
MYSQLPP_EXPORT std::ostream& operator <<(escape_type1 o,
		const std::string& in);


template <>
MYSQLPP_EXPORT std::ostream& operator <<(escape_type1 o,
		const char* const& in);


template <>
MYSQLPP_EXPORT std::ostream& operator <<(escape_type1 o,
		const ColData_Tmpl<std::string>& in);


template <>
MYSQLPP_EXPORT std::ostream& operator <<(escape_type1 o,
		const ColData_Tmpl<const_string>& in);


/// \brief Inserts a C string into a stream, escaping special SQL
/// characters.
///
/// This version exists solely to handle constness problems.  We force
/// everything to the completely-const version: operator<<(escape_type1,
/// const char* const&).

template <>
MYSQLPP_EXPORT inline std::ostream& operator <<(escape_type1 o,
		char* const& in)
{
	return operator <<(o, const_cast<const char* const&>(in));
}


/// \enum do_nothing_type0
/// \anchor do_nothing_manip
///
/// The 'do_nothing' manipulator.
///
/// Does exactly what it says: nothing. Used as a dummy manipulator when
/// you are required to use some manipulator but don't want anything to
/// be done to the following item. When used with SQLQueryParms it will
/// make sure that it does not get formatted in any way, overriding any
/// setting set by the template query.

enum do_nothing_type0
{
	do_nothing				///< insert into a std::ostream to override manipulation of next item
};


#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.

struct do_nothing_type1
{
	std::ostream* ostr;
	do_nothing_type1(std::ostream* o) :
	ostr(o)
	{
	}
};


MYSQLPP_EXPORT inline do_nothing_type1 operator <<(std::ostream& o,
		do_nothing_type0 /*esc */)
{
	return do_nothing_type1(&o);
}


template <class T>
inline std::ostream& operator <<(do_nothing_type1 o, const T& in)
{
	return *o.ostr << in;
}


struct do_nothing_type2
{
	SQLQueryParms *qparms;
	do_nothing_type2(SQLQueryParms* p) :
	qparms(p)
	{
	}
};


MYSQLPP_EXPORT inline do_nothing_type2 operator <<(SQLQueryParms& p,
		do_nothing_type0 /* esc */)
{
	return do_nothing_type2(&p);
}


MYSQLPP_EXPORT SQLQueryParms& operator <<(do_nothing_type2 p,
		SQLString& in);

#endif // !defined(DOXYGEN_IGNORE)


/// \enum ignore_type0
/// \anchor ignore_manip
///
/// The 'ignore' manipulator.
///
/// Only valid when used with SQLQueryParms. It's a dummy manipulator
/// like the <a href="#do_nothing_manip>do_nothing manipulator</a>,
/// except that it will not override formatting set by the template
/// query.  It is simply ignored.

enum ignore_type0
{
	ignore					///< insert into a std::ostream as a dummy manipulator
};


#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.

struct ignore_type2
{
	SQLQueryParms* qparms;
	ignore_type2(SQLQueryParms* p) :
	qparms(p)
	{
	}
};


MYSQLPP_EXPORT inline ignore_type2 operator <<(SQLQueryParms& p,
		ignore_type0 /* esc*/)
{
	return ignore_type2(&p);
}


MYSQLPP_EXPORT SQLQueryParms& operator <<(ignore_type2 p,
		SQLString& in);

#endif // !defined(DOXYGEN_IGNORE)

} // end namespace mysqlpp

#endif
