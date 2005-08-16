/// \file noexceptions.h
/// \brief Declares interface that allows exceptions to be optional
///
/// A class may inherit from OptionalExceptions, which will add to it
/// a mechanism by which a user can tell objects of that class to
/// suppress exceptions.  (They are enabled by default.)  This module
/// also declares a NoExceptions class, objects of which take a
/// reference to any class derived from OptionalExceptions.  The
/// NoExceptions constructor calls the method that disables exceptions,
/// and the destructor reverts them to the previous state.  One uses
/// the NoExceptions object within a scope to suppress exceptions in
/// that block, without having to worry about reverting the setting when
/// the block exits.

/***********************************************************************
 Copyright (c) 2005 by Educational Technology Resources, Inc.
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

#ifndef MYSQLPP_NOEXCEPTIONS_H
#define MYSQLPP_NOEXCEPTIONS_H

namespace mysqlpp {

class NoExceptions;

/// \brief Interface allowing a class to have optional exceptions.
///
/// A class derives from this one to acquire a standard interface for
/// disabling exceptions, possibly only temporarily.  By default,
/// exceptions are enabled.

class OptionalExceptions
{
public:
	/// \brief Default constructor
	///
	/// \param e if true, exceptions are enabled (this is the default)
	OptionalExceptions(bool e = true) :
	exceptions_(e)
	{
	}

	/// \brief Destroy object
	virtual ~OptionalExceptions() { }

	/// \brief Enable exceptions from the object
	void enable_exceptions() { exceptions_ = true; }

	/// \brief Disable exceptions from the object
	void disable_exceptions() { exceptions_ = false; }

	/// \brief Returns true if exceptions are enabled
	bool throw_exceptions() const { return exceptions_; }

protected:
	/// \brief Sets the exception state to a particular value
	///
	/// This method is protected because it is only intended for use by
	/// subclasses' copy constructors and the like.
	void set_exceptions(bool e) { exceptions_ = e; }

	/// \brief Declare NoExceptions to be our friend so it can access
	/// our protected functions.
	friend class NoExceptions;

private:
	bool exceptions_;
};


/// \brief Disable exceptions in an object derived from
/// OptionalExceptions.
///
/// This class was designed to be created on the stack, taking a
/// reference to a subclass of OptionalExceptions. (We call that our
/// "associate" object.) On creation, we save that object's current
/// exception state, and disable exceptions. On destruction, we restore
/// our associate's previous state.

class NoExceptions
{
public:
	/// \brief Constructor
	///
	/// Takes a reference to an OptionalExceptions derivative,
	/// saves that object's current exception state, and disables
	/// exceptions.
	NoExceptions(OptionalExceptions& a) :
	assoc_(a),
	exceptions_were_enabled_(a.throw_exceptions())
	{
		assoc_.disable_exceptions();
	}

	/// \brief Destructor
	///
	/// Restores our associate object's previous exception state.
	~NoExceptions()
	{
		assoc_.set_exceptions(exceptions_were_enabled_);
	}
	
private:
	OptionalExceptions& assoc_;
	bool exceptions_were_enabled_;

	// Hidden assignment operator and copy ctor, because we should not
	// be copied.
	NoExceptions(const NoExceptions&);
	NoExceptions& operator=(const NoExceptions&);
};

} // end namespace mysqlpp

#endif // MYSQLPP_NOEXCEPTIONS_H

