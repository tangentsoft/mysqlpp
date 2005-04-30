#ifndef MYSQLPP_TINY_INT_H
#define MYSQLPP_TINY_INT_H

/// \file tiny_int.h
/// \brief Declares class for holding a SQL tiny_int

namespace mysqlpp {

/// \brief Class for holding an SQL \c tiny_int object.
///
/// This is required because the closest C++ type, \c char, doesn't
/// have all the right semantics.  For one, inserting a \c char into a
/// stream won't give you a number.

class tiny_int {
  char value;
public:
  tiny_int() {}
  tiny_int(short int v) : value(char(v)) {}
  operator short int() const {return static_cast<short int>(value);};
  tiny_int &operator = (short int v) {value = char(v); return *this;}
  tiny_int &operator += (short int v) {value += char(v); return *this;}
  tiny_int &operator -= (short int v) {value -= char(v); return *this;}
  tiny_int &operator *= (short int v) {value *= char(v); return *this;}
  tiny_int &operator /= (short int v) {value /= char(v); return *this;}
  tiny_int &operator %= (short int v) {value %= char(v); return *this;}
  tiny_int &operator &= (short int v) {value &= char(v); return *this;}
  tiny_int &operator |= (short int v) {value |= char(v); return *this;}
  tiny_int &operator ^= (short int v) {value ^= char(v); return *this;}
  tiny_int &operator <<= (short int v) {value <<= char(v); return *this;}
  tiny_int &operator >>= (short int v) {value >>= char(v); return *this;}
  tiny_int &operator ++ () {value++; return *this;}
  tiny_int &operator -- () {value--; return *this;}
  tiny_int operator ++ (int) {tiny_int tmp=value; value++; return tmp;}
  tiny_int operator -- (int) {tiny_int tmp=value; value--; return tmp;}
  tiny_int operator - (const tiny_int& i) const {return value - i;}
  tiny_int operator + (const tiny_int& i) const {return value + i;}
  tiny_int operator * (const tiny_int& i) const {return value * i;}
  tiny_int operator / (const tiny_int& i) const {return value / i;}
  tiny_int operator % (const tiny_int& i) const {return value % i;}
  tiny_int operator | (const tiny_int& i) const {return value | i;}
  tiny_int operator & (const tiny_int& i) const {return value & i;}
  tiny_int operator ^ (const tiny_int& i) const {return value ^ i;}
  tiny_int operator << (const tiny_int& i) const {return value << i;}
  tiny_int operator >> (const tiny_int& i) const {return value >> i;}
};

} // end namespace mysqlpp

#endif

