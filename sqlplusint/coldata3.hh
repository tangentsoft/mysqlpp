#ifndef __coldata3_hh__
#define __coldata3_hh__
#ifdef __WIN32__
#include <winsock.h>
#endif
#include <mysql.h>
#include <string>
#include "coldata2.hh"
#include "convert3.hh"
#include "null3.hh"

template <class Str> template <class T, class B> 
mysql_ColData<Str>::operator Null<T,B> () const {
  if ((*this)[0] == 'N' && (*this)[1] == 'U' && 
      (*this)[2] == 'L' && (*this)[3] == 'L' && Str::size() == 4)
    return Null<T,B>(null);
  else return Null<T,B>(conv(T()));
}

#endif
