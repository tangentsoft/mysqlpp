/***********************************************************************
 row.cpp - Implements the Row class.

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

#include "row.h"
#include "result.h"
#include "exceptions.h"

namespace mysqlpp {

Row::Row(const MYSQL_ROW& d, const ResUse* r,
		unsigned long* jj, bool te) :
OptionalExceptions(te),
res_(r),
initialized_(false)
{
	if (!d || !r) {
		if (throw_exceptions()) {
			throw BadQuery("ROW or RES is NULL");
		}
		else {
			return;
		}
	}

	data_.clear();
	is_nulls_.clear();
	initialized_ = true;

	for (size_type i = 0; i < size(); ++i) {
		data_.insert(data_.end(),
				(d[i] ?  std::string(d[i], jj[i]) : std::string("NULL")));
		is_nulls_.insert(is_nulls_.end(), d[i] ? false : true);
	}
}


Row::~Row()
{
	data_.clear();
	is_nulls_.clear();
	initialized_ = false;
}


Row::size_type Row::size() const
{
	return res_->num_fields();
}

const ColData Row::at(size_type i) const
{
	if (initialized_) {
		const std::string& s = data_.at(i);
		return ColData(s.c_str(), s.length(), res_->types(i),
				is_nulls_[i]);
	}
	else {
		if (throw_exceptions())
			throw std::out_of_range("Row not initialized");
		else
			return ColData();
	}
}

const ColData Row::operator [](const char* field) const
{
	size_type si = res_->field_num(std::string(field));
	if (si < size()) {
		return at(si);
	}
	else {
		throw BadFieldName(field);
	}
}


value_list_ba<FieldNames, do_nothing_type0>
Row::field_list(const char* d) const
{
	return value_list_ba<FieldNames, do_nothing_type0>
			(parent().names(), d, do_nothing);
}

template <class Manip>
value_list_ba<FieldNames, Manip>
Row::field_list(const char *d, Manip m) const
{
	return value_list_ba<FieldNames, Manip>(parent().names(), d, m);
}

template <class Manip>
value_list_b<FieldNames, Manip>
Row::field_list(const char *d, Manip m, const std::vector<bool>& vb) const
{
	return value_list_b<FieldNames, Manip>(parent().names(), vb, d, m);
}

value_list_b<FieldNames, quote_type0>
Row::field_list(const char* d, const std::vector<bool>& vb) const
{
	return value_list_b<FieldNames, quote_type0>(parent().names(),
			vb, d, quote);
}

value_list_b<FieldNames, quote_type0>
Row::field_list(const std::vector<bool>& vb) const
{
	return value_list_b<FieldNames, quote_type0>(parent().names(),
			vb, ",", quote);
}

template <class Manip> value_list_b<FieldNames, Manip>
Row::field_list(const char* d, Manip m, bool t0, bool t1, bool t2,
		bool t3, bool t4, bool t5, bool t6, bool t7, bool t8, bool t9,
		bool ta, bool tb, bool tc) const
{
	std::vector<bool> vb;
	create_vector(parent().names().size(), vb, t0, t1, t2, t3, t4,
			t5, t6, t7, t8, t9, ta, tb, tc);
	return value_list_b<FieldNames, Manip>(parent().names(), vb, d, m);
}

value_list_b<FieldNames, quote_type0>
Row::field_list(const char *d, bool t0, bool t1, bool t2, bool t3,
		bool t4, bool t5, bool t6, bool t7, bool t8, bool t9, bool ta,
		bool tb, bool tc) const
{
	std::vector<bool> vb;
	create_vector(parent().names().size(), vb, t0, t1, t2, t3, t4,
			t5, t6, t7, t8, t9, ta, tb, tc);
	return value_list_b<FieldNames, quote_type0>(parent().names(),
			vb, d, quote);
}

value_list_b<FieldNames, quote_type0>
Row::field_list(bool t0, bool t1, bool t2, bool t3, bool t4, bool t5,
		bool t6, bool t7, bool t8, bool t9, bool ta, bool tb,
		bool tc) const
{
	std::vector<bool> vb;
	create_vector(parent().names().size(), vb, t0, t1, t2, t3, t4,
			t5, t6, t7, t8, t9, ta, tb, tc);
	return value_list_b<FieldNames, quote_type0>(parent().names(),
			vb, ",", quote);
}

equal_list_ba<FieldNames, Row, quote_type0>
Row::equal_list(const char* d, const char* e) const
{
	return equal_list_ba<FieldNames, Row, quote_type0>(
			parent().names(), *this, d, e, quote);
}

template <class Manip>
equal_list_ba<FieldNames, Row, Manip>
Row::equal_list(const char* d, const char* e, Manip m) const 
{
	return equal_list_ba<FieldNames, Row, Manip>(
			parent().names(), *this, d, e, m);
}

} // end namespace mysqlpp

