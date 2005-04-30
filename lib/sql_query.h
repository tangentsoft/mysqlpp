#ifndef MYSQLPP_SQL_QUERY_H
#define MYSQLPP_SQL_QUERY_H

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

#include "defs.h"
#include "sql_string.h"
#include "tiny_int.h"

#include <sstream>
#include <vector>
#include <map>

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
	friend class Query;

private:
	typedef const SQLString & ss;
	SQLQuery *parent;

public:
	SQLQueryParms() :
	parent(NULL)
	{
	}
	
	SQLQueryParms(SQLQuery * p) :
	parent(p)
	{
	}
	
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

	SQLQueryParms operator +(const SQLQueryParms& other) const;

	/// \brief Set the template query parameters.
	///
	/// Sets element 0 to a, element 1 to b, etc. May specify up to a
	/// dozen parameters.
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


/// Documentation needed!
struct SQLParseElement {
	SQLParseElement(std::string b, char o, char n):before(b),
		option(o), num(n) {
	} std::string before;
	char option;
	char num;
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
	friend class SQLQueryParms;

private:
	char *preview_char();

protected:
	bool Success;
	char *errmsg;
	std::vector < SQLParseElement > parsed;
	std::vector < std::string > parsed_names;
	std::map < std::string, int >parsed_nums;
	typedef const SQLString & ss;
	typedef SQLQueryParms parms;

	/// \brief Process a parameterized query list.
	void proc(SQLQueryParms & p);

public:
	SQLQuery() :
	std::stringstream(),
	Success(false),
	errmsg(0),
	def(this)
	{
	}
	
	SQLQuery(const SQLQuery & q);

	/// \brief The default template parameters
	///
	/// Used for filling in parameterized queries.
	SQLQueryParms def;

	void parse();

	std::string error() const
	{
		return errmsg;
	}
	
	bool success() const { return Success; }
	operator bool() { return success(); }
	bool operator !() { return !success(); }

	/// \brief Reset the query object so that it can be reused.
	///
	/// This erases the query string and the contents of the parameterized
	/// query element list.
	void reset();

	template <class T> SQLQuery& update(const T& o, const T& n)
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

	template <class T> SQLQuery& insert(const T& v)
	{
		reset();
		// See above comment for cast rationale
		dynamic_cast<std::stringstream&>(*this) << "INSERT INTO " <<
				v.table() << " (" << v.field_list() << ") VALUES (" <<
				v.value_list() << ")";
		return *this;
	}

	template <class Iter> SQLQuery& insert(Iter first, Iter last)
	{
		reset();
		if (first == last) {
			return *this;	// empty set!
		}
		
		// See above comment for cast rationale
		dynamic_cast<std::stringstream&>(*this) << "INSERT INTO " <<
				first->table() << " (" << first->field_list() <<
				") VALUES (" << fist->value_list() << ')';

		Iter it = first + 1;
		while (it != last) {
			dynamic_cast<std::stringstream&>(*this) << ",(" <<
					it->value_list() << ')';
			++it;
		}

		return *this;
	} 

	template <class T> SQLQuery& replace(const T& v)
	{
		reset();
		// See above comment for cast rationale
		dynamic_cast<std::stringstream&>(*this) << "REPLACE INTO " <<
				v.table() << " (" << v.field_list() << ") VALUES (" <<
				v.value_list() << ")";
		return *this;
	}

	std::string str()
	{
		return str(def);
	}
	std::string str(query_reset r)
	{
		return str(def, r);
	}
	std::string str(SQLQueryParms& p);
	std::string str(SQLQueryParms& p, query_reset r);

	mysql_query_define0(std::string, str);
};


inline SQLString& SQLQueryParms::operator [](const char *str)
{
	if (parent) {
		return operator [](parent->parsed_nums[str]);
	}
	throw;					// only here temporary 
}

inline const SQLString& SQLQueryParms::operator[] (const char *str) const
{
	if (parent) {
		return operator [](parent->parsed_nums[str]);
	}
	throw;					// only here temporary 
}

} // end namespace mysqlpp

#endif

