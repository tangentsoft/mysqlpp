dnl @synopsis MYSQL_SHUTDOWN_ARG
dnl 
dnl This macro determines whether mysql_shutdown() API call takes
dnl extra 'level' argument.  This was added in MySQL 4.1.3 and 5.0.1.
dnl
dnl @version 1.1, 2005/02/28
dnl @author Warren Young <mysqlpp@etr-usa.com>
AC_DEFUN([MYSQL_SHUTDOWN_ARG],
[
	AC_CACHE_CHECK([for level argument to mysql_shutdown()], ac_cv_mysql_shutdown_arg,
	[
		AC_TRY_LINK(
			[ #include <mysql.h> ],
			[ mysql_shutdown(0, 0); ],
			ac_cv_mysql_shutdown_arg=yes,
			ac_cv_mysql_shutdown_arg=no)
	])

	if test x"$ac_cv_mysql_shutdown_arg" = "xyes"
	then
		AC_DEFINE(HAVE_MYSQL_SHUTDOWN_LEVEL_ARG, 1, 
			[ Define if mysql_shutdown() has a second 'level' argument ])
	fi
]) dnl MYSQL_SHUTDOWN_ARG


