# Configure paths for sqlplus
# Dale K. Hawkins 2000-01-07

# This is written using Owen Taylor's glib.m4 as a template (thanks Owen!)

dnl AM_PATH_MYSQLPLUS([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND ]]])
dnl Test for MYSQLPLUS, and define MYSQLPLUS_CXXFLAGS and MYSQLPLUS_LIBS
dnl
AC_DEFUN(AM_PATH_MYSQLPLUS,
[dnl 
AC_REQUIRE([AM_PATH_MYSQL])
AC_REQUIRE_CPP
AC_REQUIRE([AC_PROG_CXX])

dnl Save the compile language (restored later)
AC_LANG_SAVE
dnl Use C++ do compiling
AC_LANG_CPLUSPLUS

CONF_BASENAME="mysql++-config"

dnl Get the cflags and libraries from the sqlplus-config script
dnl
dnl Configuration flags...
AC_ARG_WITH(mysqlplus-prefix,[  --with-mysqlplus-prefix=PFX   Prefix where mysql++ is installed (optional)],
            mysqlplus_config_prefix="$withval", mysqlplus_config_prefix="")
AC_ARG_WITH(mysqlplus-exec-prefix,[  --with-mysqlplus-exec-prefix=PFX Exec prefix where mysql++ is installed (optional)],
            mysqlplus_config_exec_prefix="$withval", mysqlplus_config_exec_prefix="")
AC_ARG_ENABLE(mysqlplustest, [  --disable-mysqlplustest       Do not try to compile and run a test mysql++ program],
		    , enable_mysqlplustest=yes)

  if test x$sqlplus_config_exec_prefix != x ; then
     mysqlplus_config_args="$sqlplus_config_args --exec-prefix=$sqlplus_config_exec_prefix"
     if test x${MYSQLPLUS_CONFIG+set} != xset ; then
        MYSQLPLUS_CONFIG=$sqlplus_config_exec_prefix/bin/"$CONF_BASENAME"
     fi
  fi
  if test x$sqlplus_config_prefix != x ; then
     mysqlplus_config_args="$sqlplus_config_args --prefix=$sqlplus_config_prefix"
     if test x${MYSQLPLUS_CONFIG+set} != xset ; then
        MYSQLPLUS_CONFIG=$sqlplus_config_prefix/bin/"$CONF_BASENAME"
     fi
  fi

  AC_PATH_PROG(MYSQLPLUS_CONFIG, "$CONF_BASENAME", no)
  min_mysqlplus_version=ifelse([$1], ,1.5.0,$1)
  AC_MSG_CHECKING(for MYSQLPLUS - version >= $min_mysqlplus_version)
  no_mysqlplus=""
  if test "$MYSQLPLUS_CONFIG" = "no" ; then
    no_mysqlplus=yes
  else
    MYSQLPLUS_CXXFLAGS=`$MYSQLPLUS_CONFIG $sqlplus_config_args --cxxflags`
    MYSQLPLUS_LIBS=`$MYSQLPLUS_CONFIG $mysqlplus_config_args --libs`
    mysqlplus_config_major_version=`$MYSQLPLUS_CONFIG $mysqlplus_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    mysqlplus_config_minor_version=`$MYSQLPLUS_CONFIG $mysqlplus_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    mysqlplus_config_micro_version=`$MYSQLPLUS_CONFIG $mysqlplus_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
    if test "x$enable_mysqlplustest" = "xyes" ; then
      ac_save_CXXFLAGS="$CXXFLAGS"
      ac_save_LIBS="$LIBS"
      CXXFLAGS="$CXXFLAGS $MYSQLPLUS_CXXFLAGS $MYSQL_CFLAGS"
      LIBS="$MYSQLPLUS_LIBS $MYSQL_LIBS $LIBS"


dnl
dnl Now check if the installed MYSQLPLUS is sufficiently new. (Also sanity
dnl checks the results of mysqlplus-config to some extent
dnl
      rm -f conf.mysqlplustest
      AC_TRY_RUN([
#include <sqlplus.hh>
#include <iostream>
#include <stdlib.h>

int 
main ()
{
  int major, minor, micro;
  char *tmp_version;

  system ("touch conf.mysqlplustest");

  /* HP/UX 9 (%@#!) writes to sscanf strings */
  /* Might not be portable ... */
  tmp_version = strdup("$min_mysqlplus_version");
  if (sscanf(tmp_version, "%d.%d.%d", &major, &minor, &micro) != 3) {
     printf("%s, bad version string\n", "$min_mysqlplus_version");
     exit(1);
   }

  if ((sqlplus_major_version != $mysqlplus_config_major_version) ||
      (sqlplus_minor_version != $mysqlplus_config_minor_version) ||
      (sqlplus_micro_version != $mysqlplus_config_micro_version))
    {
      printf("\n*** 'mysql++-config --version' returned %d.%d.%d, but MYSQLPLUS (%d.%d.%d)\n", 
             $mysqlplus_config_major_version, $mysqlplus_config_minor_version, $mysqlplus_config_micro_version,
             sqlplus_major_version, sqlplus_minor_version, sqlplus_micro_version);
      printf ("*** was found! If mysql++-config was correct, then it is best\n");
      printf ("*** to remove the old version of MYSQLPLUS. You may also be able to fix the error\n");
      printf("*** by modifying your LD_LIBRARY_PATH enviroment variable, or by editing\n");
      printf("*** /etc/ld.so.conf. Make sure you have run ldconfig if that is\n");
      printf("*** required on your system.\n");
      printf("*** If mysqlplus-config was wrong, set the environment variable MYSQLPLUS_CONFIG\n");
      printf("*** to point to the correct copy of mysql++-config, and remove the file config.cache\n");
      printf("*** before re-running configure\n");
    } 
  else
    {
      if ((sqlplus_major_version > major) ||
        ((sqlplus_major_version == major) && (sqlplus_minor_version > minor)) ||
        ((sqlplus_major_version == major) && (sqlplus_minor_version == minor) && (sqlplus_micro_version >= micro)))
      {
        return 0;
       }
     else
      {
        printf("\n*** An old version of mysql++ (%d.%d.%d) was found.\n",
               sqlplus_major_version, sqlplus_minor_version, sqlplus_micro_version);
        printf("*** You need a version of MYSQLPLUS newer than %d.%d.%d. The latest version of\n",
	       major, minor, micro);
        printf("*** mysql++ is available from http://mysql.com.\n");
        printf("***\n");
        printf("*** If you have already installed a sufficiently new version, this error\n");
        printf("*** probably means that the wrong copy of the mysql++-config shell script is\n");
        printf("*** being found. The easiest way to fix this is to remove the old version\n");
        printf("*** of mysql++, but you can also set the MYSQLPLUS_CONFIG environment to point to the\n");
        printf("*** correct copy of mysql++-config. (In this case, you may have to\n");
        printf("*** modify your LD_LIBRARY_PATH enviroment variable, or edit /etc/ld.so.conf\n");
        printf("*** so that the correct libraries are found at run-time))\n");
      }
    }
  return 1;
}
],, no_mysqlplus=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
       CXXFLAGS="$ac_save_CXXFLAGS"
       LIBS="$ac_save_LIBS"
     fi
  fi
  if test "x$no_mysqlplus" = x ; then
     AC_MSG_RESULT(yes)
     ifelse([$2], , :, [$2])     
  else
     AC_MSG_RESULT(no)
     if test "$MYSQLPLUS_CONFIG" = "no" ; then
       echo "*** The mysql++-config script installed by mysql++ could not be found"
       echo "*** If mysql++ was installed in PREFIX, make sure PREFIX/bin is in"
       echo "*** your path, or set the MYSQLPLUS_CONFIG environment variable to the"
       echo "*** full path to mysql++-config."
     else
       if test -f conf.mysqlplustest ; then
        :
       else
          echo "*** Could not run mysql++ test program, checking why..."
          CXXFLAGS="$CXXFLAGS $MYSQLPLUS_CXXFLAGS"
          LIBS="$LIBS $MYSQLPLUS_LIBS"
          AC_TRY_LINK([
#include <sqlplus.hh>
#include <stdio.h>
],      [ return ((sqlplus_major_version) || (sqlplus_minor_version) || (sqlplus_micro_version)); ],
        [ echo "*** The test program compiled, but did not run. This usually means"
          echo "*** that the run-time linker is not finding mysql++ or finding the wrong"
          echo "*** version of mysql++. If it is not finding mysql++, you'll need to set your"
          echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
          echo "*** to the installed location  Also, make sure you have run ldconfig if that"
          echo "*** is required on your system"
	  echo "***"
          echo "*** If you have an old version installed, it is best to remove it, although"
          echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH"
          echo "***"],
        [ echo "*** The test program failed to compile or link. See the file config.log for the"
          echo "*** exact error that occured. This usually means mysql++ was incorrectly installed"
          echo "*** or that you have moved mysql++ since it was installed. In the latter case, you"
          echo "*** may want to edit the mysql++-config script: $MYSQLPLUS_CONFIG" ])
          CXXFLAGS="$ac_save_CXXFLAGS"
          LIBS="$ac_save_LIBS"
       fi
     fi
     MYSQLPLUS_CXXFLAGS=""
     MYSQLPLUS_LIBS=""
     ifelse([$3], , :, [$3])
  fi
  AC_SUBST(MYSQLPLUS_CXXFLAGS)
  AC_SUBST(MYSQLPLUS_LIBS)
  rm -f conf.mysqlplustest
dnl Restore the compile language
  AC_LANG_RESTORE

])
