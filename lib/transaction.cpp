/***********************************************************************
 transaction.cpp - Implements the Transaction class.

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

#define MYSQLPP_NOT_HEADER
#include "common.h"

#include "transaction.h"

#include "connection.h"
#include "query.h"

using namespace std;
using namespace mysqlpp;


//// ctor //////////////////////////////////////////////////////////////

Transaction::Transaction(Connection& conn, bool consistent) :
conn_(conn),
finished_(true)		// don't bother rolling it back if ctor fails
{
	// Begin the transaction set
	Query q(conn_.query());
	q << "START TRANSACTION";
	if (consistent) {
		q << " WITH CONSISTENT SNAPSHOT";
	}
	q.execute();

	// Setup succeeded, so mark our transaction as not-finished.
	finished_ = false;
}


//// dtor //////////////////////////////////////////////////////////////

Transaction::~Transaction()
{
	if (!finished_) {
		try {
			rollback();
		}
		catch (...) {
			// eat all exceptions
		}
	}
}


//// commit ////////////////////////////////////////////////////////////

void
Transaction::commit()
{
	Query q(conn_.query());
	q << "COMMIT";
	q.execute();

	finished_ = true;
}


//// rollback //////////////////////////////////////////////////////////

void
Transaction::rollback()
{
	Query q(conn_.query());
	q << "ROLLBACK";
	q.execute();

	finished_ = true;
}


