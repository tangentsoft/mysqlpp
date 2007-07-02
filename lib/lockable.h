/// \file lockable.h
/// \brief Declares interface that allows a class to declare itself as
/// "lockable".
///
/// The meaning of a class being lockable is very much per-class
/// specific in this version of MySQL++.  In a future version, it will
/// imply that operations that aren't normally thread-safe will use
/// platform mutexes if MySQL++ is configured to support them.  This is
/// planned for a version beyond v2.0. (See the Wishlist for the plan.)
/// In the meantime, do not depend on this mechanism for thread safety;
/// you will have to serialize access to some resources yourself.
///
/// To effect this variability in what it means for an object to be
/// "locked", Lockable is only an interface.  It delegates the actual
/// implementation to a subclass of the Lock interface, using the
/// Bridge pattern.  (See Gamma et al.)

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

#ifndef MYSQLPP_LOCKABLE_H
#define MYSQLPP_LOCKABLE_H

namespace mysqlpp {

/// \brief Abstract base class for lock implementation, used by
/// Lockable.

class MYSQLPP_EXPORT Lock
{
public:
	/// \brief Destroy object
	virtual ~Lock() { }

	/// \brief Lock the object
	///
	/// \return true if object was already locked
	virtual bool lock() = 0;

	/// \brief Unlock the object
	virtual void unlock() = 0;

	/// \brief Returns true if object is locked
	virtual bool locked() const = 0;

	/// \brief Set the lock state.
	virtual void set(bool b) = 0;
};


/// \brief Trivial Lock subclass, using a boolean variable as the
/// lock flag.
///
/// This is the only Lock implementation available in this version of
/// MySQL++.  It will be supplemented with a better implementation for
/// use with threads at a later date.

class MYSQLPP_EXPORT BasicLock : public Lock
{
public:
	/// \brief Create object
	BasicLock(bool is_locked = false) :
	locked_(is_locked)
	{
	}
	
	/// \brief Destroy object
	~BasicLock() { }

	/// \brief Lock the object
	///
	/// \return true if object was already locked
	bool lock()
	{
		if (locked_) {
			return true;
		}
		locked_ = true;
		return false;
	}

	/// \brief Unlock the object
	void unlock() { locked_ = false; }

	/// \brief Returns true if object is locked
	bool locked() const { return locked_; }

	/// \brief Set the lock state.
	void set(bool b) { locked_ = b; }

private:
	bool locked_;
};


/// \brief Interface allowing a class to declare itself as "lockable".
///
/// A class derives from this one to acquire a standard interface for
/// serializing operations that may not be thread-safe.

class MYSQLPP_EXPORT Lockable
{
protected:
	/// \brief Default constructor
	Lockable(bool is_locked) :
	pimpl_(new BasicLock(is_locked))
	{
	}

	/// \brief Destroy object
	virtual ~Lockable()
	{
		delete pimpl_;
	}

	/// \brief Lock the object
	///
	/// \return true if object was already locked
	virtual bool lock() { return pimpl_->lock(); }

	/// \brief Unlock the object
	virtual void unlock() { pimpl_->unlock(); }

	/// \brief Returns true if object is locked
	bool locked() const { return pimpl_->locked(); }

protected:
	/// \brief Set the lock state.  Protected, because this method is
	/// only for use by subclass assignment operators and the like.
	void set_lock(bool b) { pimpl_->set(b); }

private:
	// Don't allow default construction
	Lockable();

	// Pointer to implementation object
	Lock* pimpl_;
};

} // end namespace mysqlpp

#endif // MYSQLPP_LOCKABLE_H

