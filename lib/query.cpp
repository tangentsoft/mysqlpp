/***********************************************************************
 query.cpp - Implements the Query class.

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

#include "query.h"

#include "connection.h"

namespace mysqlpp {

Query::Query(const Query& q) :
std::ostream(0),	
OptionalExceptions(q.exceptions()),
Lockable(q.locked()),
def(q.def),
conn_(q.conn_),
success_(q.success_)
{
	init(&sbuffer_);
}


Query&
Query::operator=(const Query& rhs)
{
	set_exceptions(rhs.throw_exceptions());
	set_lock(rhs.locked());
	def = rhs.def;
	conn_ = rhs.conn_;
	success_ = rhs.success_;

	return *this;
}


my_ulonglong Query::affected_rows() const
{
	return conn_->affected_rows();
}


std::string Query::error()
{
	return conn_->error();
}


bool Query::exec(const std::string& str)
{
	success_ = !mysql_real_query(&conn_->mysql_, str.c_str(),
			static_cast<unsigned long>(str.length()));
	if (!success_ && throw_exceptions()) {
		throw BadQuery(error());
	}
	else {
		return success_;
	}
}


ResNSel Query::execute(const char* str)
{
	success_ = false;
	if (lock()) {
		if (throw_exceptions()) {
			throw LockFailed();
		}
		else {
			return ResNSel();
		}
	}

	success_ = !mysql_query(&conn_->mysql_, str);
	unlock();
	if (success_) {
		return ResNSel(conn_);
	}
	else {
		if (throw_exceptions()) {
			throw BadQuery(error());
		}
		else {
			return ResNSel();
		}
	}
}


#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.

ResNSel Query::execute(SQLQueryParms& p)
{
	query_reset r = parse_elems_.size() ? DONT_RESET : RESET_QUERY;
	return execute(str(p, r).c_str());
}

#endif // !defined(DOXYGEN_IGNORE)


std::string Query::info()
{
	return conn_->info();
}


my_ulonglong Query::insert_id()
{
	return conn_->insert_id();
}


bool Query::lock()
{
    return conn_->lock();
}


bool Query::more_results()
{
#if MYSQL_VERSION_ID > 41000		// only in MySQL v4.1 +
	return mysql_more_results(&conn_->mysql_);
#else
	return false;
#endif
}


void Query::parse()
{
	std::string str = "";
	char num[4];
	std::string name;
	char *s, *s0;
	s0 = s = preview_char();
	while (*s) {
		if (*s == '%') {
			// Following might be a template parameter declaration...
			s++;
			if (*s == '%') {
				// Doubled percent sign, so insert literal percent sign.
				str += *s++;
			}
			else if (isdigit(*s)) {
				// Number following percent sign, so it signifies a
				// positional parameter.  First step: find position
				// value, up to 3 digits long.
				num[0] = *s;
				s++;
				if (isdigit(*s)) {
					num[1] = *s;
					num[2] = 0;
					s++;
					if (isdigit(*s)) {
						num[2] = *s;
						num[3] = 0;
						s++;
					}
					else {
						num[2] = 0;
					}
				}
				else {
					num[1] = 0;
				}
				short int n = atoi(num);

				// Look for option character following position value.
				char option = ' ';
				if (*s == 'q' || *s == 'Q' || *s == 'r' || *s == 'R') {
					option = *s++;
				}

				// Is it a named parameter?
				if (*s == ':') {
					// Save all alphanumeric and underscore characters
					// following colon as parameter name.
					s++;
					for (/* */; isalnum(*s) || *s == '_'; ++s) {
						name += *s;
					}

					// Eat trailing colon, if it's present.
					if (*s == ':') {
						s++;
					}

					// Update maps that translate parameter name to
					// number and vice versa.
					if (n >= static_cast<short>(parsed_names_.size())) {
						parsed_names_.insert(parsed_names_.end(),
								static_cast<std::vector<std::string>::size_type>(
										n + 1) - parsed_names_.size(),
								std::string());
					}
					parsed_names_[n] = name;
					parsed_nums_[name] = n;
				}

				// Finished parsing parameter; save it.
				parse_elems_.push_back(SQLParseElement(str, option, char(n)));
				str = "";
				name = "";
			}
			else {
				// Insert literal percent sign, because sign didn't
				// precede a valid parameter string; this allows users
				// to play a little fast and loose with the rules,
				// avoiding a double percent sign here.
				str += '%';
			}
		}
		else {
			// Regular character, so just copy it.
			str += *s++;
		}
	}

	parse_elems_.push_back(SQLParseElement(str, ' ', -1));
	delete[] s0;
}


SQLString*
Query::pprepare(char option, SQLString& S, bool replace)
{
	if (S.processed) {
		return &S;
	}

	if (option == 'r' || (option == 'q' && S.is_string)) {
		char *s = new char[S.size() * 2 + 1];
		mysql_real_escape_string(&conn_->mysql_, s, S.c_str(),
				static_cast<unsigned long>(S.size()));
		SQLString *ss = new SQLString("'");
		*ss += s;
		*ss += "'";
		delete[] s;

		if (replace) {
			S = *ss;
			S.processed = true;
			return &S;
		}
		else {
			return ss;
		}
	}
	else if (option == 'R' || (option == 'Q' && S.is_string)) {
		SQLString *ss = new SQLString("'" + S + "'");

		if (replace) {
			S = *ss;
			S.processed = true;
			return &S;
		}
		else {
			return ss;
		}
	}
	else {
		if (replace) {
			S.processed = true;
		}
		return &S;
	}
}


char* Query::preview_char()
{
	*this << std::ends;
	size_t length = sbuffer_.str().size();
	char* s = new char[length + 1];
	strncpy(s, sbuffer_.str().c_str(), length);
	s[length] = '\0';
	return s;
}


void Query::proc(SQLQueryParms& p)
{
	sbuffer_.str("");

	char num;
	SQLString* ss;
	SQLQueryParms* c;

	for (std::vector<SQLParseElement>::iterator i = parse_elems_.begin();
			i != parse_elems_.end(); ++i) {
		dynamic_cast<std::ostream&>(*this) << i->before;
		num = i->num;
		if (num != -1) {
			if (num < static_cast<int>(p.size()))
				c = &p;
			else if (num < static_cast<int>(def.size()))
				c = &def;
			else {
				*this << " ERROR";
				throw BadParamCount(
						"Not enough parameters to fill the template.");
			}
			ss = pprepare(i->option, (*c)[num], c->bound());
			dynamic_cast<std::ostream&>(*this) << *ss;
			if (ss != &(*c)[num]) {
				delete ss;
			}
		}
	}
}

void Query::reset()
{
	seekp(0);
	clear();
	sbuffer_.str("");

	parse_elems_.clear();
	def.clear();
}


Result Query::store(const char* str)
{
	success_ = false;

	if (lock()) {
		if (throw_exceptions()) {
			throw LockFailed();
		}
		else {
			return Result();
		}
	}

	success_ = !mysql_query(&conn_->mysql_, str);
	if (success_) {
		MYSQL_RES* res = mysql_store_result(&conn_->mysql_);
		if (res) {
			unlock();
			return Result(res, throw_exceptions());
		}
	}
	unlock();

	// One of the mysql_* calls failed, so decide how we should fail.
	// Notice that we do not throw an exception if we just get a null
	// result set, but no error.  This happens when using store() on a
	// query that may not return results.  Obviously it's better to use
	// exec*() in this situation, but it's not outright illegal, and
	// sometimes you have to do it.
	if (conn_->errnum() && throw_exceptions()) {
		throw BadQuery(error());
	}
	else {
		return Result();
	}
}


#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.

Result Query::store(SQLQueryParms& p)
{
	query_reset r = parse_elems_.size() ? DONT_RESET : RESET_QUERY;
	return store(str(p, r).c_str());
}

#endif // !defined(DOXYGEN_IGNORE)


Result Query::store_next()
{
#if MYSQL_VERSION_ID > 41000		// only in MySQL v4.1 +
	if (lock()) {
		if (throw_exceptions()) {
			throw LockFailed();
		}
		else {
			return Result();
		}
	}

	int ret;
	if ((ret = mysql_next_result(&conn_->mysql_)) == 0) {
		// There are more results, so return next result set.
		MYSQL_RES* res = mysql_store_result(&conn_->mysql_);
		unlock();
		if (res) {
			return Result(res, throw_exceptions());
		} 
		else {
			// Result set is null, but throw an exception only i it is
			// null because of some error.  If not, it's just an empty
			// result set, which is harmless.  We return an empty result
			// set if exceptions are disabled, as well.
			if (conn_->errnum() && throw_exceptions()) {
				throw BadQuery(error());
			} 
			else {
				return Result();
			}
		}
	}
	else {
		// No more results, or some other error occurred.
		unlock();
		if (throw_exceptions()) {
			if (ret > 0) {
				throw BadQuery(error());
			}
			else {
				throw EndOfResultSets();
			}
		}
		else {
			return Result();
		}
	}
#else
	return store();
#endif // MySQL v4.1+
}


std::string Query::str(SQLQueryParms& p)
{
	if (!parse_elems_.empty()) {
		proc(p);
	}

	*this << std::ends;

	return sbuffer_.str();
}


std::string Query::str(SQLQueryParms& p, query_reset r)
{
	std::string tmp = str(p);
	if (r == RESET_QUERY) {
		reset();
	}
	return tmp;
}


bool Query::success()
{
	return success_ && conn_->success();
}


void Query::unlock()
{
	conn_->unlock();
}


ResUse Query::use(const char* str)
{
	success_ = false;
	if (lock()) {
		if (throw_exceptions()) {
			throw LockFailed();
		}
		else {
			return ResUse();
		}
	}

	success_ = !mysql_query(&conn_->mysql_, str);
	if (success_) {
		MYSQL_RES* res = mysql_use_result(&conn_->mysql_);
		if (res) {
			unlock();
			return ResUse(res, conn_, throw_exceptions());
		}
	}
	unlock();

	// One of the mysql_* calls failed, so decide how we should fail.
	if (throw_exceptions()) {
		throw BadQuery(error());
	}
	else {
		return ResUse();
	}
}


#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.

ResUse Query::use(SQLQueryParms& p)
{
	query_reset r = parse_elems_.size() ? DONT_RESET : RESET_QUERY;
	return use(str(p, r).c_str());
}

#endif // !defined(DOXYGEN_IGNORE)

} // end namespace mysqlpp

