# Configure paths for mysql
# Dale K. Hawkins 2000-01-07

# This is written using Owen Taylor's glib.m4 as a template (thanks Owen!)

dnl AM_PATH_MYSQL([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND ]]])
dnl Test for MYSQL, and define MYSQL_CFLAGS and MYSQL_LIBS
dnl
AC_DEFUN(AM_PATH_MYSQL,
[dnl 
AC_REQUIRE_CPP
AC_REQUIRE([AC_PROG_CC])

dnl Get the cflags and libraries from the mysql-config script
dnl
dnl Configuration flags...
AC_ARG_WITH(mysql-prefix,[  --with-mysql-prefix=PFX   Prefix where MYSQL is installed (optional)],
            mysql_config_prefix="$withval", mysql_config_prefix="")

AC_ARG_WITH(mysql-exec-prefix,[  --with-mysql-exec-prefix=PFX Exec prefix where MYSQL is installed (optional)],
            mysql_config_exec_prefix="$withval", mysql_config_exec_prefix="")

AC_ARG_WITH(mysql-include-dir,[  --with-mysql-include-dir=PFX   Where to find mysql include files (optional)],
            mysql_include_dir="$withval")

AC_ARG_WITH(mysql-lib-dir,[  --with-mysql-lib-dir=PFX   Where to find mysql libraries (optional)],
            mysql_lib_dir="$withval")

AC_ARG_ENABLE(mysqltest, [  --disable-mysqltest       Do not try to compile and run a test MYSQL program],
		    , enable_mysqltest=yes)

dnl  echo "Init checks"
dnl  echo "MYSQL_CFLAGS = $MYSQL_CFLAGS"
dnl  echo "MYSQL_LIBS = $MYSQL_LIBS"

  if test x$mysql_include_dir != x ; then
    test "x$MYSQL_CFLAGS" != x || MYSQL_CFLAGS="-I$mysql_include_dir"
  fi

  if test x$mysql_lib_dir != x ; then
    test "x$MYSQL_LIBS" != x || MYSQL_LIBS="-L$mysql_lib_dir -lmysqlclient"
  fi

dnl  echo "First checks"
dnl  echo "MYSQL_CFLAGS = $MYSQL_CFLAGS"
dnl  echo "MYSQL_LIBS = $MYSQL_LIBS"

  if test x$mysql_config_exec_prefix != x ; then
     mysql_config_args="$mysql_config_args --exec-prefix=$mysql_config_exec_prefix"
     if test x${MYSQL_CONFIG+set} != xset ; then
        MYSQL_CONFIG=$mysql_config_exec_prefix/bin/mysql-config
     fi
  fi
  if test x$mysql_config_prefix != x ; then
     mysql_config_args="$mysql_config_args --prefix=$mysql_config_prefix"
     if test x${MYSQL_CONFIG+set} != xset ; then
        MYSQL_CONFIG=$mysql_config_prefix/bin/mysql-config
     fi
  fi

  PATH="$PATH:."

  AC_PATH_PROG(MYSQL_CONFIG, mysql-config, no)

  min_mysql_version=ifelse([$1], ,3.22.0,$1)
  AC_MSG_CHECKING(for MYSQL - version >= $min_mysql_version)
  no_mysql=""
  if test "$MYSQL_CONFIG" = "no" ; then
    no_mysql=yes
  else
    test "x$MYSQL_CFLAGS" != x || MYSQL_CFLAGS=`$MYSQL_CONFIG $mysql_config_args --cflags`
    test "x$MYSQL_LIBS" != x || MYSQL_LIBS=`$MYSQL_CONFIG $mysql_config_args --libs`
    mysql_config_major_version=`$MYSQL_CONFIG $mysql_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    mysql_config_minor_version=`$MYSQL_CONFIG $mysql_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    mysql_config_micro_version=`$MYSQL_CONFIG $mysql_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
    if test "x$enable_mysqltest" = "xyes" ; then
      ac_save_CFLAGS="$CFLAGS"
      ac_save_LIBS="$LIBS"
      CFLAGS="$CFLAGS $MYSQL_CFLAGS"
      LIBS="$MYSQL_LIBS $LIBS"


dnl
dnl Now check if the installed MYSQL is sufficiently new. (Also sanity
dnl checks the results of mysql-config to some extent
dnl
      rm -f conf.mysqltest
      AC_TRY_RUN([
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>

int 
main ()
{
  int major, minor, micro;
  int header_major, header_minor, header_micro;
  char *tmp_version;
  char *tmp_version2;

  system ("touch conf.mysqltest");


  /* HP/UX 9 (%@#!) writes to sscanf strings */
  tmp_version = strdup("$min_mysql_version");
  if (sscanf(tmp_version, "%d.%d.%d", &major, &minor, &micro) != 3) {
     printf("%s, bad version string\n", "$min_mysql_version");
     exit(1);
   }

  /* HP/UX 9 (%@#!) writes to sscanf strings */
  tmp_version2 = strdup(MYSQL_SERVER_VERSION);
  if (sscanf(tmp_version2, "%d.%d.%d", &header_major, &header_minor, &header_micro) != 3) {
     printf("%s, bad version string\n", MYSQL_SERVER_VERSION);
     exit(1);
   }

  if ((header_major != $mysql_config_major_version) ||
      (header_minor != $mysql_config_minor_version) ||
      (header_micro != $mysql_config_micro_version))
    {
      printf("\n*** 'mysql-config --version' returned %d.%d.%d, but MYSQL (%d.%d.%d)\n", 
             $mysql_config_major_version, $mysql_config_minor_version, $mysql_config_micro_version,
             header_major, header_minor, header_micro);
      printf ("*** was found! If mysql-config was correct, then it is best\n");
      printf ("*** to remove the old version of MYSQL. You may also be able to fix the error\n");
      printf("*** by modifying your LD_LIBRARY_PATH enviroment variable, or by editing\n");
      printf("*** /etc/ld.so.conf. Make sure you have run ldconfig if that is\n");
      printf("*** required on your system.\n");
      printf("*** If mysql-config was wrong, set the environment variable MYSQL_CONFIG\n");
      printf("*** to point to the correct copy of mysql-config, and remove the file config.cache\n");
      printf("*** before re-running configure\n");
    } 
  else
    {
      if ((header_major > major) ||
         ((header_major == major) && (header_minor > minor)) ||
         ((header_major == major) && (header_minor == minor) && (header_micro >= micro)))
      {
        return 0;
       }
     else
      {
        printf("*** You need a version of MYSQL newer than %d.%d.%d. The latest version of\n",
	       major, minor, micro);
        printf("*** MYSQL is available from http://mysql.com.\n");
        printf("***\n");
        printf("*** If you have already installed a sufficiently new version, this error\n");
        printf("*** probably means that the wrong copy of the mysql-config shell script is\n");
        printf("*** being found. The easiest way to fix this is to remove the old version\n");
        printf("*** of MYSQL, but you can also set the MYSQL_CONFIG environment to point to the\n");
        printf("*** correct copy of mysql-config. (In this case, you will have to\n");
        printf("*** modify your LD_LIBRARY_PATH enviroment variable, or edit /etc/ld.so.conf\n");
        printf("*** so that the correct libraries are found at run-time))\n");
      }
    }
  return 1;
}
],, no_mysql=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
       CFLAGS="$ac_save_CFLAGS"
       LIBS="$ac_save_LIBS"
     fi
  fi

dnl  echo "Final checks"
dnl  echo "MYSQL_CFLAGS = $MYSQL_CFLAGS"
dnl  echo "MYSQL_LIBS = $MYSQL_LIBS"

  if test "x$no_mysql" = x ; then
     AC_MSG_RESULT(yes)
     ifelse([$2], , :, [$2])     
  elif test "x$MYSQL_CFLAGS" != x -a "x$MYSQL_LIBS" != x; then
     AC_MSG_RESULT(maybe)
         echo " ** MYSQL_CFLAGS and MYSQL_LIBS were set"
	 echo " ** assuming their values are OK."
	 mysqlconfigok=yes
         ifelse([$2], , :, [$2])
  else
     AC_MSG_RESULT(no)
     if test "$MYSQL_CONFIG" = "no" ; then
       echo "*** The mysql-config script installed by MYSQL could not be found"
       echo "*** If MYSQL was installed in PREFIX, make sure PREFIX/bin is in"
       echo "*** your path, or set the MYSQL_CONFIG environment variable to the"
       echo "*** full path to mysql-config."
     else
       if test -f conf.mysqltest ; then
        :
       else
          echo "*** Could not run MYSQL test program, checking why..."
          CFLAGS="$CFLAGS $MYSQL_CFLAGS"
          LIBS="$LIBS $MYSQL_LIBS"
          AC_TRY_LINK([
#include <mysql.h>
#include <stdio.h>
],      [ return (MYSQL_VERSION_ID != 0); ],
        [ echo "*** The test program compiled, but did not run. This usually means"
          echo "*** that the run-time linker is not finding MYSQL or finding the wrong"
          echo "*** version of MYSQL. If it is not finding MYSQL, you'll need to set your"
          echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
          echo "*** to the installed location  Also, make sure you have run ldconfig if that"
          echo "*** is required on your system"
	  echo "***"
          echo "*** If you have an old version installed, it is best to remove it, although"
          echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH" ],
        [ echo "*** The test program failed to compile or link. See the file config.log for the"
          echo "*** exact error that occured. This usually means MYSQL was incorrectly installed"
          echo "*** or that you have moved MYSQL since it was installed. In the latter case, you"
          echo "*** may want to edit the mysql-config script: $MYSQL_CONFIG" ])
          CFLAGS="$ac_save_CFLAGS"
          LIBS="$ac_save_LIBS"
       fi
     fi

     MYSQL_CFLAGS=""
     MYSQL_LIBS=""
     ifelse([$3], , :, [$3])
  fi
  AC_SUBST(MYSQL_CFLAGS)
  AC_SUBST(MYSQL_LIBS)
  rm -f conf.mysqltest

])
