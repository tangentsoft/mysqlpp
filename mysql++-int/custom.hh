
// This file is generated from the perl script custom.pl, please do 
// not modify this file directly.

#ifndef __sql_custom_h__
#define __sql_custom_h__

enum sql_dummy_type {sql_dummy};
enum sql_cmp_type {sql_use_compare};

inline int sql_cmp(const string &a, const string &b) {
  return a.compare(b);
}

inline int sql_cmp(char a,char b) {
  return a-b;
}

inline int sql_cmp(unsigned char a,unsigned char b) {
  return a-b;
}

inline int sql_cmp(tiny_int a,tiny_int b) {
  return a-b;
}

inline int sql_cmp(int a,int b) {
  return a-b;
}

inline int sql_cmp(unsigned int a,unsigned int b) {
  return a-b;
}

inline int sql_cmp(short int a,short int b) {
  return a-b;
}

inline int sql_cmp(unsigned short int a,unsigned short int b) {
  return a-b;
}

inline int sql_cmp(double a,double b) {
  if (a == b) return 0;
  if (a <  b) return -1;
  return 1;
}

inline int sql_cmp(float a,float b) {
  if (a == b) return 0;
  if (a <  b) return -1;
  return 1;
}

#ifndef NO_LONG_LONGS

inline int sql_cmp(long long a,long long b) {
  if (a == b) return 0;
  if (a <  b) return -1;
  return 1;
}

inline int sql_cmp(unsigned long long a,unsigned long long b) {
  if (a == b) return 0;
  if (a <  b) return -1;
  return 1;
}

#endif // NO_LONG_LONGS

#include "custom-macros.hh"

#endif

