#ifndef __coldata3_hh__
#define __coldata3_hh__
#ifdef __WIN32__
#include <Windows32/Base.h>
#include <Windows32/Defines.h>
#include <Windows32/Structures.h>
#include <winsock.h>
#define errno WSAGetLastError()
#endif
#include <mysql.h>
#include <string>
#include "coldata2.hh"
#include "convert3.hh"
#include "null3.hh"

template <class Str> template <class T, class B> 
mysql_ColData<Str>::operator Null<T,B> () const {
  if ((*this)[0] == 'N' && (*this)[1] == 'U' && 
      (*this)[2] == 'U' && (*this)[3] == 'L' && Str::size() == 4)
    return Null<T,B>(null);
  else return Null<T,B>(conv(T()));
}

#endif
