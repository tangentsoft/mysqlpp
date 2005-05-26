/// \file sql_query.h
/// \brief Declares the base class for mysqlpp::Query, plus some
/// utility classes to be used with it.
///
/// Class SQLQuery contains a large part of the functionality of class
/// Query, which is the only thing that derives from this class.  It is
/// separate for historical reasons only: early on, there was a dream
/// (and some effort) to make MySQL++ database-independent.  Once
/// maintainership shifted to MySQL AB employees in 1999, though, that
/// dream died.
///
/// The current maintainers have no wish to try and revive that dream,
/// so at some point this class's contents will be folded into the Query
/// class. This will probably happen in the next major release, when
/// major ABI breakage is acceptable.

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

#ifndef MYSQLPP_SQL_QUERY_H
#define MYSQLPP_SQL_QUERY_H

#include "defs.h"
#include "sql_string.h"
#include "tiny_int.h"

#include <sstream>
#include <vector>
#include <map>

/// \brief Used to define many similar functions in class SQLQuery.
#define mysql_query_define0(RETURN, FUNC)\
  RETURN FUNC (ss a)\
    {return FUNC (parms() << a);}\
  RETURN FUNC (ss a, ss b)\
    {return FUNC (parms() << a << b);}\
  RETURN FUNC (ss a, ss b, ss c)\
    {return FUNC (parms() << a << b << c);}\
  RETURN FUNC (ss a, ss b, ss c, ss d)\
    {return FUNC (parms() << a << b << c << d);}\
  RETURN FUNC (ss a, ss b, ss c, ss d, ss e)\
    {return FUNC (parms() << a << b << c << d << e);} \
  RETURN FUNC (ss a, ss b, ss c, ss d, ss e, ss f)\
    {return FUNC (parms() << a << b << c << d << e << f);}\
  RETURN FUNC (ss a, ss b, ss c, ss d, ss e, ss f, ss g)\
    {return FUNC (parms() << a << b << c << d << e << f << g);}\
  RETURN FUNC (ss a, ss b, ss c, ss d, ss e, ss f, ss g, ss h)\
    {return FUNC (parms() << a << b << c << d << e << f << g << h);}\
  RETURN FUNC (ss a, ss b, ss c, ss d, ss e, ss f, ss g, ss h, ss i)\
    {return FUNC (parms() << a << b << c << d << e << f << g << h << i);}\
  RETURN FUNC (ss a,ss b,ss c,ss d,ss e,ss f,ss g,ss h,ss i,ss j)\
    {return FUNC (parms() <<a <<b <<c <<d <<e <<f <<g <<h <<i <<j);}\
  RETURN FUNC (ss a,ss b,ss c,ss d,ss e,ss f,ss g,ss h,ss i,ss j,ss k)\
    {return FUNC (parms() <<a <<b <<c <<d <<e <<f <<g <<h <<i <<j <<k);}\
  RETURN FUNC (ss a,ss b,ss c,ss d,ss e,ss f,ss g,ss h,ss i,ss j,ss k,\
		       ss l)\
    {return FUNC (parms() <<a <<b <<c <<d <<e <<f <<g <<h <<i <<j <<k <<l);}\

namespace mysqlpp {

class Query;
class SQLQuery;


/// \brief This class holds the parameter values for filling
/// template queries. 
class SQLQueryParms : public std::vector<SQLString>
{
private:
	friend class Query;

	typedef const SQLString& ss;
	SQLQuery *parent;

public:
	/// \brief Default constructor
	SQLQueryParms() :
	parent(0)
	{
	}
	
	/// \brief Create object
	///
	/// \param p pointer to the query object these parameters are tied
	/// to
	SQLQueryParms(SQLQuery* p) :
	parent(p)
	{
	}
	
	/// \brief Returns true if we are bound to a query object.
	///
	/// Basically, this tells you which of the two ctors were called.
	bool bound()
	{
		return parent != 0;
	}

	/// \brief Clears the list
	void clear()
	{
		erase(begin(), end());
	}

	/// \brief Access element number n
	SQLString& operator [](size_type n)
	{
		if (n >= size())
			insert(end(), (n + 1) - size(), "");
		return std::vector<SQLString>::operator [](n);
	}

	/// \brief Access element number n
	const SQLString& operator [](size_type n) const
	{
		return std::vector<SQLString>::operator [](n);
	}
	
	/// \brief Access the value of the element with a key of str.
	SQLString& operator [](const char *str);

	/// \brief Access the value of the element with a key of str.
	const SQLString& operator [](const char *str) const;

	/// \brief Adds an element to the list
	SQLQueryParms& operator <<(const SQLString& str)
	{
		push_back(str);
		return *this;
	}

	/// \brief Adds an element to the list
	SQLQueryParms& operator +=(const SQLString& str)
	{
		push_back(str);
		return *this;
	}

	/// \brief Build a composite of two parameter lists
	///
	/// If this list is (a, b) and \c other is (c, d, e, f, g), then
	/// the returned list will be (a, b, e, f, g).  That is, all of this
	/// list's parameters are in the returned list, plus any from the
	/// other list that are in positions beyond what exist in this list.
	///
	/// If the two lists are the same length or this list is longer than
	/// the \c other list, a copy of this list is returned.
	SQLQueryParms operator +(const SQLQueryParms& other) const;

#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.
	void set(ss a)
	{
		clear();
		*this << a;
	}
	void set(ss a, ss b)
	{
		clear();
		*this << a << b;
	}
	void set(ss a, ss b, ss c)
	{
		clear();
		*this << a << b << c;
	}
	void set(ss a, ss b, ss c, ss d)
	{
		clear();
		*this << a << b << c << d;
	}
	void set(ss a, ss b, ss c, ss d, ss e)
	{
		clear();
		*this << a << b << c << d << e;
	}
	void set(ss a, ss b, ss c, ss d, ss e, ss f)
	{
		clear();
		*this << a << b << c << d << e << f;
	}
	void set(ss a, ss b, ss c, ss d, ss e, ss f, ss g)
	{
		clear();
		*this << a << b << c << d << e << f << g;
	}
	void set(ss a, ss b, ss c, ss d, ss e, ss f, ss g, ss h)
	{
		clear();
		*this << a << b << c << d << e << f << g << h;
	}
	void set(ss a, ss b, ss c, ss d, ss e, ss f, ss g, ss h, ss i)
	{
		clear();
		*this << a << b << c << d << e << f << g << h << i;
	}
	void set(ss a, ss b, ss c, ss d, ss e, ss f, ss g, ss h, ss i,
			 ss j)
	{
		clear();
		*this << a << b << c << d << e << f << g << h << i << j;
	}
	void set(ss a, ss b, ss c, ss d, ss e, ss f, ss g, ss h, ss i,
			 ss j, ss k)
	{
		clear();
		*this << a << b << c << d << e << f << g << h << i << j << k;
	}
#endif // !defined(DOXYGEN_IGNORE)

	/// \brief Set the template query parameters.
	///
	/// Sets parameter 0 to a, parameter 1 to b, etc. There are
	/// overloaded versions of this function that take anywhere from
	/// one to a dozen parameters.
	void set(ss a, ss b, ss c, ss d, ss e, ss f, ss g, ss h, ss i,
			 ss j, ss k, ss l)
	{
		clear();
		*this << a << b << c << d << e << f << g << h << i << j << k << l;
	}
};


/// \brief Used for indicating whether a query object should auto-reset
/// or not.
enum query_reset { DONT_RESET, RESET_QUERY };


/// \brief Used within SQLQuery to hold elements for parameterized
/// queries.
///
/// Each element has three parts:
///
/// The concept behind the \c before variable needs a little explaining.
/// When a template query is parsed, each parameter is parsed into one
/// of these SQLParseElement objects, but the non-parameter parts of the
/// template also have to be stored somewhere.  MySQL++ chooses to
/// attach the text leading up to a parameter to that parameter.  So,
/// the \c before string is simply the text copied literally into the
/// finished query before we insert a value for the parameter.
///
/// The \c option character is currently one of 'q', 'Q', 'r', 'R' or
/// ' '.  See the "Template Queries" chapter in the user manual for
/// details.
///
/// The position value (\c num) allows a template query to have its
/// parameters in a different order than in the Query method call.
/// An example of how this can be helpful is in the "Template Queries"
/// chapter of the user manual.

struct SQLParseElement {
	/// \brief Create object
	///
	/// \param b the 'before' value
	/// \param o the 'option' value
	/// \param n the 'num' value
	SQLParseElement(std::string b, char o, char n) :
	before(b),
	option(o),
	num(n)
	{
	}
	
	std::string before;		///< string inserted before the parameter
	char option;			///< the parameter option, or blank if none
	char num;				///< the parameter position to use
};


/** \brief The base class for mysqlpp::Query.

	One uses an object of this class to form queries that can be
	sent to the database server via the mysqlpp::Connection object.

	This class is subclassed from \c std::stringstream. This means
	that you can form a SQL query using C++ stream idioms without
	having to create your own \c stringstream object and then dump
	that into the query object.  And of course, it gets you all the
	benefits of C++ streams, such as type safety, which \c sprintf()
	and such do not offer.	Although you can read from this object
	as you would any other stream, this is \e not recommended. It
	may fail in strange ways, and there is no support offered if
	you break it by doing so.

	If you seek within the stream in any way, be sure to reset
	the stream pointer to the end before calling any of the
	SQLQuery-specific methods except for error() and success().
*/

class SQLQuery : public std::stringstream
{
private:
	friend class SQLQueryParms;

	char *preview_char();

protected:
	bool Success;					///< if true, last query succeeded
	char* errmsg;					///< string explaining last query error

	/// \brief List of template query parameters
	std::vector<SQLParseElement> parsed;

	/// \brief Maps template parameter position values to the
	/// corresponding parameter name.
	std::vector<std::string> parsed_names;

	/// \brief Maps template parameter names to their position value.
	std::map<std::string, int> parsed_nums;

	typedef const SQLString& ss;	///< to keep parameters lists short
	typedef SQLQueryParms parms;	///< abstraction; remove when Query and SQLQuery merge

	/// \brief Process a parameterized query list.
	void proc(SQLQueryParms& p);

public:
	/// \brief Default constructor
	SQLQuery() :
	std::stringstream(),
	Success(false),
	errmsg(0),
	def(this)
	{
	}
	
	/// \brief Create a query as a copy of another
	SQLQuery(const SQLQuery& q);

	/// \brief The default template parameters
	///
	/// Used for filling in parameterized queries.
	SQLQueryParms def;

	/// \brief Treat the contents of the query string as a template
	/// query.
	///
	/// This method sets up the internal structures used by all of the
	/// other members that accept template query parameters.  See the
	/// "Template Queries" chapter in the user manual for more
	/// information.
	void parse();

	/// \brief Return the last error message that was set
	///
	/// This function has no real meaning at this level.  It's
	/// overridden with useful behavior in the Query subclass.
	std::string error() const
	{
		return errmsg;
	}
	
	/// \brief Return true if the last query was successful
	bool success() const { return Success; }

	/// \brief Return true if the last query was successful
	operator bool() { return success(); }

	/// \brief Return true if the last query failed
	bool operator !() { return !success(); }

	/// \brief Reset the query object so that it can be reused.
	///
	/// This erases the query string and the contents of the parameterized
	/// query element list.
	void reset();

	/// \brief Build an UPDATE SQL query
	///
	/// \param o SSQLS containing data to match in table
	/// \param n SSQLS that will replace any records in \c o.table() that
	/// match \c o
	template <class T>
	SQLQuery& update(const T& o, const T& n)
	{
		reset();

		// Cast required for VC++ 2003 due to error in overloaded operator
		// lookup logic.  For an explanation of the problem, see:
		// http://groups-beta.google.com/group/microsoft.public.vc.stl/browse_thread/thread/9a68d84644e64f15
		dynamic_cast<std::stringstream&>(*this) << "UPDATE " <<
				o.table() << " SET " << n.equal_list() << " WHERE " <<
				o.equal_list(" AND ", sql_use_compare);
		return *this;
	}

	/// \brief Build an INSERT SQL query for one record
	///
	/// \param v SSQLS which will be inserted into \c v.table()
	template <class T>
	SQLQuery& insert(const T& v)
	{
		reset();

		// See above comment for cast rationale
		dynamic_cast<std::stringstream&>(*this) << "INSERT INTO " <<
				v.table() << " (" << v.field_list() << ") VALUES (" <<
				v.value_list() << ")";
		return *this;
	}

	/// \brief Build an INSERT SQL query for multiple records
	///
	/// \param first iterator pointing to first SSQLS to insert
	/// \param last iterator pointing to record past last SSQLS to
	/// insert
	template <class Iter>
	SQLQuery& insert(Iter first, Iter last)
	{
		reset();
		if (first == last) {
			return *this;	// empty set!
		}
		
		// See above comment for cast rationale
		dynamic_cast<std::stringstream&>(*this) << "INSERT INTO " <<
				first->table() << " (" << first->field_list() <<
				") VALUES (" << first->value_list() << ')';

		Iter it = first + 1;
		while (it != last) {
			dynamic_cast<std::stringstream&>(*this) << ",(" <<
					it->value_list() << ')';
			++it;
		}

		return *this;
	} 

	/// \brief Build a REPLACE SQL query
	///
	/// \param v SSQLS containing data to insert into the table, or
	/// which will replace an existing record if this record matches
	/// another on a unique index
	template <class T>
	SQLQuery& replace(const T& v)
	{
		reset();

		// See above comment for cast rationale
		dynamic_cast<std::stringstream&>(*this) << "REPLACE INTO " <<
				v.table() << " (" << v.field_list() << ") VALUES (" <<
				v.value_list() << ")";
		return *this;
	}

	/// \brief Get built query as a null-terminated C++ string
	std::string str()
	{
		return str(def);
	}

	/// \brief Get built query as a null-terminated C++ string
	///
	/// \param r if equal to \c RESET_QUERY, query object is cleared
	/// after this call
	std::string str(query_reset r)
	{
		return str(def, r);
	}

	/// \brief Get built query as a null-terminated C++ string
	///
	/// \param p template query parameters to use, overriding the ones
	/// this object holds, if any
	std::string str(SQLQueryParms& p);

	/// \brief Get built query as a null-terminated C++ string
	///
	/// \param p template query parameters to use, overriding the ones
	/// this object holds, if any
	/// \param r if equal to \c RESET_QUERY, query object is cleared
	/// after this call
	std::string str(SQLQueryParms& p, query_reset r);

#if !defined(DOXYGEN_IGNORE)
	// Make Doxygen ignore this macro; declaration below is sufficient.
	mysql_query_define0(std::string, str);
#endif // !defined(DOXYGEN_IGNORE)
};


inline SQLString& SQLQueryParms::operator [](const char* str)
{
	if (parent) {
		return operator [](parent->parsed_nums[str]);
	}
	throw; // only here temporary 
}

inline const SQLString& SQLQueryParms::operator[] (const char* str) const
{
	if (parent) {
		return operator [](parent->parsed_nums[str]);
	}
	throw; // only here temporary 
}

} // end namespace mysqlpp

#endif

