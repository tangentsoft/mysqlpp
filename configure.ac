# This file is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation; either version 2.1 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with MySQL++; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
# USA

# Standard autotools stuff
AC_INIT(mysql++, 3.0.0, plusplus@lists.mysql.com, mysql++)
AC_CONFIG_HEADER(config.h)
AC_DISABLE_STATIC
AC_CANONICAL_SYSTEM


# Break package version up into major, minor and bugfix components.
MYSQLPP_VERSION_MAJOR=`echo $PACKAGE_VERSION | cut -f1 -d.`
AC_SUBST(MYSQLPP_VERSION_MAJOR)
MYSQLPP_VERSION_MINOR=`echo $PACKAGE_VERSION | cut -f2 -d.`
AC_SUBST(MYSQLPP_VERSION_MINOR)
MYSQLPP_VERSION_BUGFIX=`echo $PACKAGE_VERSION | cut -f3 -d.`
AC_SUBST(MYSQLPP_VERSION_BUGFIX)


# Include Bakefile macros
AC_BAKEFILE([m4_include(config/autoconf_inc.m4)])


# Check for Standard C support
AC_PROG_CC
AC_HEADER_STDC


# Figure out whether/how to handle threading support, if available.
AC_ARG_ENABLE(thread-check,
		[  --enable-thread-check  Check for threads, and use if available. ],
		[ thread_check=yes ])
if test "x$thread_check" = "xyes"
then
	ACX_PTHREAD
	LIBS="$PTHREAD_LIBS $LIBS"
	CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
	CC="$PTHREAD_CC"
fi


# Checks for libraries and local system features
AC_CHECK_HEADERS(zlib.h, AC_CHECK_LIB(z, gzread, [],
		[ AC_MSG_ERROR([zlib is required]) ]))
AC_CHECK_HEADERS(synch.h)
LIB_MATH
LIB_SOCKET_NSL
MYSQL_API_LOCATION
MYSQL_WITH_SSL
AX_C_LOCALTIME_R
AC_CHECK_LIB(intl, main)


# If this is Cygwin, add a linker flag to suppress a silly link message.
case "${host}" in
	*cygwin*)
		LDFLAGS="$LDFLAGS -Wl,--enable-auto-import"
		;;
esac


# Check for Standard C++ support, and extensions.  This must be near
# the end, because the CPLUSPLUS directive makes autoconf use C++
# compiler for all subsequent tests!
AC_PROG_CXX
AC_LANG_CPLUSPLUS
STL_SLIST_EXTENSION


#
# Configure process complete; write out files generated from *.in.
#
AC_OUTPUT([Makefile mysql++.spec doc/userman/userman.dbx lib/Doxyfile lib/mysql++.h])

