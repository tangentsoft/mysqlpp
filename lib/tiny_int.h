#ifndef MYSQLPP_TINY_INT_H
#define MYSQLPP_TINY_INT_H

namespace mysqlpp {

class tiny_int {
  char value;
public:
  tiny_int() {}
  tiny_int(short int v) : value(char(v)) {}
  operator short int() const {return (short int)value;};
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

}; // end namespace mysqlpp

#endif

