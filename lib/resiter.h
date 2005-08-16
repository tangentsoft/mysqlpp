/// \file resiter.h
/// \brief Declares templates for adapting existing classes to
/// be iteratable random-access containers.
///
/// The file name seems to tie it to the mysqlpp::Result class, which
/// is so adapted, but these templates are also used to adapt the
/// mysqlpp::Fields and mysqlpp::Row classes.

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

#ifndef MYSQLPP_RESITER_H
#define MYSQLPP_RESITER_H

#include "defs.h"

#include <iterator>

namespace mysqlpp {

template <class OnType, class ReturnType, class SizeType,
	class DiffType> class subscript_iterator;

/// \brief A base class that one derives from to become a random
/// access container, which can be accessed with subscript notation.
///
/// OnType must have the member functions \c operator[](SizeType) and
// \c size() defined for it.

template <class OnType,
		class ValueType,
		class ReturnType = const ValueType&,
		class SizeType = unsigned int,
		class DiffType = int>
class const_subscript_container
{
public:
	typedef const_subscript_container<OnType, ValueType, ReturnType,
			SizeType, DiffType> this_type; ///< this object's type
	typedef subscript_iterator<const this_type, ReturnType, SizeType,
			DiffType> iterator;			///< mutable iterator type
	typedef iterator const_iterator;	///< constant iterator type
	typedef const std::reverse_iterator<iterator>
			reverse_iterator;			///< mutable reverse iterator type
	typedef const std::reverse_iterator<const_iterator>
			const_reverse_iterator;		///< const reverse iterator type

	typedef ValueType value_type;		///< type of data stored in container
	typedef value_type& reference;		///< reference to value_type
	typedef value_type& const_reference;///< const ref to value_type
	typedef value_type* pointer;		///< pointer to value_type
	typedef value_type* const_pointer;	///< const pointer to value_type

	typedef DiffType difference_type;	///< for index differences
	typedef SizeType size_type;			///< for returned sizes

	/// \brief Destroy object
	virtual ~const_subscript_container() { }

	/// \brief Return count of elements in container
	virtual size_type size() const = 0;	

	/// \brief Return element at given index in container
	virtual ReturnType at(SizeType i) const = 0;

	/// \brief Return maximum number of elements that can be stored
	/// in container without resizing.
	size_type max_size() const { return size(); }

	/// \brief Returns true if container is empty
	bool empty() const { return size() == 0; }

	/// \brief Return iterator pointing to first element in the
	/// container
	iterator begin() const { return iterator(this, 0); }

	/// \brief Return iterator pointing to one past the last element
	/// in the container
	iterator end() const { return iterator(this, size()); }

	/// \brief Return reverse iterator pointing to first element in the
	/// container
	reverse_iterator rbegin() const { return reverse_iterator(end()); }

	/// \brief Return reverse iterator pointing to one past the last
	/// element in the container
	reverse_iterator rend() const { return reverse_iterator(begin()); }
};


/// \brief Iterator that can be subscripted.
///
/// This is the type of iterator used by the const_subscript_container
/// template.

template <class OnType, class ReturnType, class SizeType,
		class DiffType>
class subscript_iterator : public std::iterator<ReturnType, SizeType>
{
public:
	/// \brief Default constructor
	subscript_iterator() { }

	/// \brief Create iterator given the container and a position
	/// within it.
	subscript_iterator(OnType* what, SizeType pos)
	{
		d_ = what;
		i_ = pos;
	}

	/// \brief Return true if given iterator points to the same
	/// container and the same position within the container.
	bool operator ==(const subscript_iterator& j) const
	{
		return (d_ == j.d_ && i_ == j.i_);
	}
	
	/// \brief Return true if given iterator is different from this
	/// one, but points to the same container.
	bool operator !=(const subscript_iterator& j) const
	{
		return (d_ == j.d_ && i_ != j.i_);
	}
	
	/// \brief Return true if the given iterator points to the same
	/// container as this one, and that this iterator's position is
	/// less than the given iterator's.
	bool operator <(const subscript_iterator& j) const
	{
		return (d_ == j.d_ && i_ < j.i_);
	}
	
	/// \brief Return true if the given iterator points to the same
	/// container as this one, and that this iterator's position is
	/// greater than the given iterator's.
	bool operator >(const subscript_iterator & j) const
	{
		return (d_ == j.d_ && i_ > j.i_);
	}
	
	/// \brief Return true if the given iterator points to the same
	/// container as this one, and that this iterator's position is
	/// less than or equal to the given iterator's.
	bool operator <=(const subscript_iterator & j) const
	{
		return (d_ == j.d_ && i_ <= j.i_);
	}
	
	/// \brief Return true if the given iterator points to the same
	/// container as this one, and that this iterator's position is
	/// greater than or equal to the given iterator's.
	bool operator >=(const subscript_iterator & j) const
	{
		return (d_ == j.d_ && i_ >= j.i_);
	}
	
	/// \brief Dereference the iterator, returning a copy of the
	/// pointed-to element within the container.
	ReturnType operator *() const { return d_->at(i_); }
	
	/// \brief Return a copy of the element at the given position
	/// within the container.
	ReturnType operator [](SizeType n) const { return d_->at(n); }
	
	/// \brief Move the iterator to the next element, returning an
	/// iterator to that element
	subscript_iterator& operator ++() { ++i_; return *this; }

	/// \brief Move the iterator to the next element, returning an
	/// iterator to the element we were pointing at before the change
	subscript_iterator operator ++(int)
	{
		subscript_iterator tmp = *this;
		++i_;
		return tmp;
	}

	/// \brief Move the iterator to the previous element, returning an
	/// iterator to that element
	subscript_iterator& operator --()
	{
		--i_;
		return *this;
	}

	/// \brief Move the iterator to the previous element, returning an
	/// iterator to the element we were pointing at before the change
	subscript_iterator operator --(int)
	{
		subscript_iterator tmp = *this;
		--i_;
		return tmp;
	}

	/// \brief Advance iterator position by \c n
	subscript_iterator& operator +=(SizeType n)
	{
		i_ += n;
		return *this;
	}

	/// \brief Return an iterator \c n positions beyond this one
	subscript_iterator operator +(SizeType n) const
	{
		subscript_iterator tmp = *this;
		tmp.i_ += n;
		return tmp;
	}
	
	/// \brief Move iterator position back by \c n
	subscript_iterator& operator -=(SizeType n)
	{
		i_ -= n;
		return *this;
	}

	/// \brief Return an iterator \c n positions before this one
	subscript_iterator operator -(SizeType n) const
	{
		subscript_iterator tmp = *this;
		tmp.i_ -= n;
		return tmp;
	}
	
	/// \brief Return an iterator \c n positions before this one
	DiffType operator -(const subscript_iterator& j) const
	{
		if (d_ == j.d_) {
			return static_cast<SizeType>(i_) - j.i_;
		}
		return 0;
	}

private:
	SizeType i_;
	OnType* d_;
};


#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.

template <class OnType, class ReturnType, class SizeType,
		class DiffType> 
inline subscript_iterator<OnType, ReturnType, SizeType, DiffType>
operator +(SizeType x,
		const subscript_iterator <OnType, ReturnType, SizeType, DiffType>& y)
{
	return y + x;
}

#endif // !defined(DOXYGEN_IGNORE)

} // end namespace mysqlpp

#endif
