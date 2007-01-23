/// \file transaction.h
/// \brief Declares the Transaction class.
///
/// This object works with the Connection class to automate the use of
/// MySQL transactions.  It allows you to express these transactions
/// directly in C++ code instead of sending the raw SQL commands.

/***********************************************************************
 Copyright (c) 2006 by Educational Technology Resources, Inc.  Others
 may also hold copyrights on code in this file.  See the CREDITS file
 in the top directory of the distribution for details.

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

#if !defined(MYSQLPP_TRANSACTION_H)
#define MYSQLPP_TRANSACTION_H

#include "common.h"

namespace mysqlpp {

#if !defined(DOXYGEN_IGNORE)
// Make Doxygen ignore this
class MYSQLPP_EXPORT Connection;
#endif

/// \brief Helper object for creating exception-safe SQL transactions.

class MYSQLPP_EXPORT Transaction
{
public:
	/// \brief Constructor
	///
	/// \param conn The connection we use to manage the transaction set
	/// \param consistent Whether to use "consistent snapshots" during
	/// the transaction. See the documentation for "START TRANSACTION"
	/// in the MySQL manual for more on this.
	Transaction(Connection& conn, bool consistent = false);

	/// \brief Destructor
	///
	/// If the transaction has not been committed or rolled back by the
	/// time the destructor is called, it is rolled back.  This is the
	/// right thing because one way this can happen is if the object is
	/// being destroyed as the stack is unwound to handle an exception.
	/// In that instance, you certainly want to roll back the
	/// transaction.
	~Transaction();

	/// \brief Commits the transaction
	///
	/// This commits all updates to the database using the connection
	/// we were created with since this object was created.  This is a
	/// no-op if the table isn't stored using a transaction-aware
	/// storage engine.  See CREATE TABLE in the MySQL manual for
	/// details.
	void commit();

	/// \brief Rolls back the transaction
	///
	/// This abandons all SQL statements made on the connection since
	/// this object was created.  This only works on tables stored using
	/// a transaction-aware storage engine.  See CREATE TABLE in the
	/// MySQL manual for details.
	void rollback();

private:
	Connection& conn_;	///! Connection to send queries through
	bool finished_;		///! True when we commit or roll back xaction
};

} // end namespace mysqlpp

#endif // !defined(MYSQLPP_TRANSACTION_H)

