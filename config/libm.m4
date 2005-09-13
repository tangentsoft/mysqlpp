dnl @synopsis LIB_MATH
dnl 
dnl This macro figures out how to link a program using common math
dnl functions.
dnl
dnl On some platforms (e.g. Solaris), you must specifically add -lm to
dnl the link line to use standard math functions like floor(3).
dnl
dnl @category C
dnl @author Warren Young <warren@etr-usa.com>
dnl @version 1.1, 2005-09-06

AC_DEFUN([LIB_MATH],
[
	AC_SEARCH_LIBS([floor], [m])
])

