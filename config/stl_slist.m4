dnl @synopsis STL_SLIST_EXTENSION
dnl 
dnl This macro determines whether the local STL implementation includes
dnl a singly-linked list template, slist, and if so, where it is.
dnl
dnl @version 1.1, 2005/02/28
dnl @author Warren Young <mysqlpp@etr-usa.com>
AC_DEFUN([STL_SLIST_EXTENSION],
[
	AC_CHECK_HEADER([slist],
			[AC_DEFINE(HAVE_STD_SLIST, 1,
			[Define if <slist> exists on your system])], [])
	AC_CHECK_HEADER([ext/slist],
			[AC_DEFINE(HAVE_EXT_SLIST, 1,
			[Define if <ext/slist> exists on your system])], [])
]) dnl STL_SLIST_EXTENSION

