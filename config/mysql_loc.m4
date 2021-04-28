dnl @synopsis MYSQL_C_API_LOCATION
dnl 
dnl This macro tries to find MySQL C API header and library locations.
dnl
dnl We define the following configure script flags:
dnl
dnl		--with-mysql: Give prefix for both library and headers, and try
dnl			to guess subdirectory names for each by tacking common
dnl         suffixes on like /lib and /include.
dnl		--with-mysql-lib: Same as --with-mysql, but for library only.
dnl		--with-mysql-include: Same as --with-mysql, but for headers only.
dnl
dnl @version 1.7, 2021/04/28
dnl @author Warren Young <mysqlpp@etr-usa.com>
AC_DEFUN([MYSQL_C_API_LOCATION],
[
	#
	# Set up configure script macros
	#
	AC_ARG_WITH(mysql,
		[  --with-mysql=<path>     root directory path of MySQL/MariaDB installation],
		[MYSQL_lib_check="$with_mysql/lib/mysql $with_mysql/lib/mariadb $with_mysql/lib"
		MYSQL_inc_check="$with_mysql/include $with_mysql/include/mysql $with_mysql/include/mariadb"],
		[MYSQL_lib_check="/usr/lib64 /usr/lib /usr/lib64/mysql /usr/lib/mysql /usr/lib64/mariadb /usr/lib/mariadb /usr/local/lib64 /usr/local/lib /usr/local/lib/mysql /usr/local/mysql/lib /usr/local/mysql/lib/mysql /usr/mysql/lib/mysql /opt/mysql/lib /opt/mysql/lib/mysql /usr/local/lib/mariadb /usr/local/mariadb/lib /usr/local/mariadb/lib/mariadb /usr/mariadb/lib/mariadb /opt/mariadb/lib /opt/mariadb/lib/mariadb /sw/lib /sw/lib/mysql"
		MYSQL_inc_check="/usr/include/mysql /usr/local/include/mysql /usr/local/mysql/include /usr/local/mysql/include/mysql /usr/mysql/include/mysql /opt/mysql/include/mysql /usr/include/mariadb /usr/local/include/mariadb /usr/local/mariadb/include /usr/local/mariadb/include/mariadb /usr/mariadb/include/mariadb /opt/mariadb/include/mariadb /sw/include/mysql"])
	AC_ARG_WITH(mysql-lib,
		[  --with-mysql-lib=<path> directory path of MySQL/MariaDB library installation],
		[MYSQL_lib_check="$with_mysql_lib $with_mysql_lib/lib64 $with_mysql_lib/lib $with_mysql_lib/lib64/mysql $with_mysql_lib/lib/mysql $with_mysql_lib/lib64/mariadb $with_mysql_lib/lib/mariadb"])
	AC_ARG_WITH(mysql-include,
		[  --with-mysql-include=<path> directory path of MySQL/MariaDB header installation],
		[MYSQL_inc_check="$with_mysql_include $with_mysql_include/include $with_mysql_include/include/mysql $with_mysql_include/include/mariadb"])


	#
	# Decide which C API library to use, based on thread support
	#
	if test "x$acx_pthread_ok" = xyes
	then
		MYSQL_C_LIB_NAME=mysqlclient_r
	else
		MYSQL_C_LIB_NAME=mysqlclient
	fi


	#
	# Look for MySQL C API headers
	#
	AC_MSG_CHECKING([for MySQL include directory])
	MYSQL_C_INC_DIR=
	for m in $MYSQL_inc_check
	do
		if test -d "$m" && test -f "$m/mysql.h"
		then
			MYSQL_C_INC_DIR=$m
			break
		fi
	done

	if test -z "$MYSQL_C_INC_DIR"
	then
		AC_MSG_ERROR([Didn't find the MySQL include dir in '$MYSQL_inc_check'])
	fi

	case "$MYSQL_C_INC_DIR" in
		/* ) ;;
		* )  AC_MSG_ERROR([The MySQL include directory ($MYSQL_C_INC_DIR) must be an absolute path.]) ;;
	esac

	AC_MSG_RESULT([$MYSQL_C_INC_DIR])


	#
	# Look for MySQL C API library
	#
	AC_MSG_CHECKING([for MySQL C API library directory])
	save_CPPFLAGS=$CPPFLAGS
	save_LIBS=$LIBS
	save_LDFLAGS=$LDFLAGS
	CPPFLAGS="$CPPFLAGS -I$MYSQL_C_INC_DIR"
	MYSQL_C_LIB_DIR=
	for m in $MYSQL_lib_check
	do
        if test -d $m
        then
            LDFLAGS="$save_LDFLAGS -L$m"
            LIBS="$save_LIBS -l$MYSQL_C_LIB_NAME"
            AC_TRY_LINK(
                [ #include <mysql.h> ],
                [ mysql_store_result(0); ],
                [ AC_MSG_RESULT([$m])
                  MYSQL_C_LIB_DIR=$m
                  break
                ],
                [ LIBS="$save_LIBS -l$MYSQL_C_LIB_NAME -lz"
                  AC_TRY_LINK(
                      [ #include <mysql.h> ],
                      [ mysql_store_result(0); ],
                      [ AC_MSG_RESULT([$m])
                        MYSQLPP_EXTRA_LIBS="$MYSQLPP_EXTRA_LIBS -lz"
                        MYSQL_C_LIB_DIR=$m
                        break
                      ],
                      [ ]
                  )
                ])
        fi
	done
	CPPFLAGS=$save_CPPFLAGS
	LIBS=$save_LIBS
	LDFLAGS=$save_LDFLAGS

	if test -z "$MYSQL_C_LIB_DIR"
	then
		AC_MSG_RESULT([no joy])
		AC_MSG_ERROR([Didn't find $MYSQL_C_LIB_NAME library in '$MYSQL_lib_check'])
	fi

	case "$MYSQL_C_LIB_DIR" in
		/* ) ;;
		* )  AC_MSG_ERROR([The MySQL library directory ($MYSQL_C_LIB_DIR) must be an absolute path.]) ;;
	esac

	AC_SUBST(MYSQL_C_INC_DIR)
	AC_SUBST(MYSQL_C_LIB_DIR)
	AC_SUBST(MYSQL_C_LIB_NAME)
]) dnl MYSQL_C_API_LOCATION

