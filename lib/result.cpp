/***********************************************************************
 result.cpp - Implements the Result, ResNSel, and ResUse classes.

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

#include "result.h"

#include "connection.h"

namespace mysqlpp {

ResNSel::ResNSel(Connection* q) :
success(q->success()),
insert_id(q->insert_id()),
rows(q->affected_rows()),
info(q->info())
{
}


ResUse::ResUse(MYSQL_RES* result, Connection* c, bool te) :
OptionalExceptions(te),
conn_(c),
initialized_(false),
names_(0),
types_(0),
fields_(this)
{
	if (!result) {
		result_ = 0;
		types_ = 0;
		names_ = 0;
		return;
	}
	result_ = result;
	names_ = new FieldNames(this);
	if (names_) {
		types_ = new FieldTypes(this);
	}
	table_ = fields(0).table;
	initialized_ = true;
}


ResUse::~ResUse()
{
	purge();
}


void
ResUse::copy(const ResUse& other)
{
	if (initialized_) {
		purge();
	}

	set_exceptions(other.throw_exceptions());

	if (!other.result_) {
		result_ = 0;
		types_ = 0;
		names_ = 0;
		initialized_ = other.initialized_;
		return;
	}

	result_ = other.result_;
	fields_ = Fields(this);

	if (other.names_) {
		names_ = new FieldNames(*other.names_);
	}
	else {
		names_ = 0;
	}
	
	if (other.types_) {
		types_ = new FieldTypes(*other.types_);
	}
	else {
		types_ = 0;
	}

	table_ = other.table_;

	conn_ = other.conn_;
	initialized_ = true;
}


int
ResUse::field_num(const std::string& i) const
{
	if (!names_) {
		names_ = new FieldNames(this);
	}

	size_t index = (*names_)[i];
	if ((index >= names_->size()) && throw_exceptions()) {
		throw BadFieldName(i.c_str());
	}
	
	return int(index);
}


std::string&
ResUse::field_name(int i)
{
	if (!names_) {
		names_ = new FieldNames(this);
	}
	return (*names_)[i];
}


const std::string&
ResUse::field_name(int i) const
{
	if (!names_) {
		names_ = new FieldNames(this);
	}
	return (*names_)[i];
}


FieldNames&
ResUse::field_names()
{
	if (!names_) {
		names_ = new FieldNames(this);
	}
	return *names_;
}


const FieldNames&
ResUse::field_names() const
{
	if (!names_) {
		names_ = new FieldNames(this);
	}
	return *names_;
}


void
ResUse::reset_field_names()
{
	delete names_;
	names_ = 0;
	names_ = new FieldNames(this);
}


mysql_type_info&
ResUse::field_type(int i)
{
	if (!types_) {
		types_ = new FieldTypes(this);
	}
	return (*types_)[i];
}


const mysql_type_info&
ResUse::field_type(int i) const
{
	if (!types_) {
		types_ = new FieldTypes(this);
	}
	return (*types_)[i];
}


FieldTypes&
ResUse::field_types()
{
	if (!types_) {
		types_ = new FieldTypes(this);
	}
	return *types_;
}


const FieldTypes&
ResUse::field_types() const
{
	if (!types_) {
		types_ = new FieldTypes(this);
	}
	return *types_;
}


void
ResUse::reset_field_types()
{
	delete types_;
	types_ = 0;
	types_ = new FieldTypes(this);
}


ResUse&
ResUse::operator =(const ResUse& other)
{
	if (this == &other) {
		return *this;
	}
	copy(other);
	other.result_ = 0;
	return *this;
}

} // end namespace mysqlpp

