#ifndef __resiter1_hh__
#define __resiter1_hh__

#include "defs.hh"
#include "row1.hh"
#include <iterator.h>

template <class OnType, class ReturnType, class SizeType, class DiffType>
class subscript_iterator;

//: A container adapter to make a container into a Random Access Container.

// The requirements are that the container has the member functions
// *operator[] (SizeType)* _and_  *size()* defined. 

template <class OnType, class ValueType, class ReturnType = const ValueType&, class SizeType = unsigned int, class DiffType = int>
class const_subscript_container {
public:
  typedef const_subscript_container<OnType,ValueType,ReturnType,SizeType,DiffType>
                                    this_type; //:

  typedef subscript_iterator<const this_type, ReturnType, SizeType, DiffType> 
                                                 iterator;   //:
  typedef iterator                               const_iterator; //:
  typedef const ::reverse_iterator<iterator>       reverse_iterator; //:
  typedef const ::reverse_iterator<const_iterator> const_reverse_iterator; //:
  
  typedef ValueType   value_type; //:
  typedef value_type& reference;  //:
  typedef value_type& const_reference; //:
  typedef value_type* pointer; //:
  typedef value_type* const_pointer; //:
  
  typedef DiffType          difference_type; //:
  typedef SizeType          size_type; //:
  
  virtual size_type  size() const = 0; //:
  virtual ReturnType operator[] (SizeType i) const = 0; //:

  size_type max_size() const {return size();}    //:
  bool      empty()    const {return size()==0;} //:
  
  iterator  begin() const {return iterator(this, 0);}      //:
  iterator  end()   const {return iterator(this, size());} //:
  
  reverse_iterator rbegin() const {return reverse_iterator(end());}   //:
  reverse_iterator rend()   const {return reverse_iterator(begin());} //:
};

//:
template <class OnType, class ReturnType, class SizeType, class DiffType>
class subscript_iterator : public random_access_iterator<ReturnType, SizeType>
{
private:
  SizeType    i;
  OnType     *d;
public:
  subscript_iterator() {}; 
  subscript_iterator(OnType *what, SizeType pos) {d=what; i=pos;}
 
  bool operator == (const subscript_iterator &j) const
    {if (d == j.d && i==j.i) return true; return false;}
  bool operator != (const subscript_iterator &j) const
    {if (d == j.d && i!=j.i) return true; return false;}
  bool operator < (const subscript_iterator &j) const
    {if (d == j.d && i < j.i) return true; return false;}
  bool operator > (const subscript_iterator &j) const
    {if (d == j.d && i > j.i) return true; return false;}
  bool operator <= (const subscript_iterator &j) const
    {if (d == j.d && i<=j.i) return true; return false;}
  bool operator >= (const subscript_iterator &j) const
    {if (d == j.d && i>=j.i) return true; return false;}

  ReturnType operator -> () const {return &((*d)[i]);}
  ReturnType operator *  () const {return (*d)[i];}
  ReturnType operator [] (SizeType n) const {return (*d)[n];}

  subscript_iterator& operator ++ () {i++; return *this;}
  subscript_iterator  operator ++ (int) 
    {subscript_iterator tmp = *this; i++; return tmp;}
  subscript_iterator& operator -- () {i--; return *this;}
  subscript_iterator  operator -- (int) 
    {subscript_iterator tmp = *this; i--; return tmp;}
  subscript_iterator& operator += (SizeType n) {i=i+n; return *this;}
  subscript_iterator  operator + (SizeType n) const
    {subscript_iterator tmp = *this; tmp.i+=n; return tmp;}
  subscript_iterator& operator -= (SizeType n) {i=i-n; return *this;}
  subscript_iterator  operator - (SizeType n) const
    {subscript_iterator tmp = *this; tmp.i-=n; return tmp;}
  DiffType operator - (const subscript_iterator &j) const
    {if (d == j.d) return (SizeType)i - j.i; return 0;}
};

template <class OnType, class ReturnType, class SizeType, class DiffType>
inline subscript_iterator<OnType,ReturnType,SizeType,DiffType> operator + 
(SizeType x, const subscript_iterator <OnType,ReturnType,SizeType,DiffType>& y) 
{
  return y + x;
}

#endif



