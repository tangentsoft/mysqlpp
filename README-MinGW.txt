Prerequisite: GCC Version
~~~~~~~~~~~~~~~~~~~~~~~~~
    If your MinGW version isn't using at least GCC 3.4.5, it needs
    to be updated.  Older versions are known to not work with MySQL++.


Prerequisite: MySQL C API DLL Import Library
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Before you can build MySQL++ with MinGW, you will need to create
    a MinGW-compatible import library for MySQL's C API library.
    Using the current default install path for MySQL and assuming
    MySQL++ is in c:\mysql++, the commands to do this are:

        cd C:\Program Files\MySQL\MySQL Server 5.0\lib\opt
        dlltool -k -d c:\mysql++\libmysqlclient.def -l libmysqlclient.a


Prerequisite: MySQL C Development Files
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    You'll also need to have MySQL installed, including all the
    MySQL C development files; MySQL++ is built atop this library.
    The MySQL server installer has, in the past, sometimes installed
    these by default, and sometimes not.  You may need to do a custom
    install to get them.

    The MySQL++ Makefile assumes that you installed the MySQL
    development files in C:\Program Files\MySQL\MySQL Server 5.0\
    If not, you have two options.

    The simplest is to edit Makefile.mingw.  This is a generated
    file, but if that's all the only change to MySQL++ you need,
    it works fine.

    If you're doing deeper work on MySQL++, you should change the
    variable MYSQL_WIN_DIR at the top of mysql++.bkl instead.  Then to
    generate Makefile.mingw from that file, you will need the Win32
    port of Bakefile from http://bakefile.org/  The command to do
    that is:

        bakefile_gen -f mingw


Building the Library and Example Programs
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    With the prerequisites above taken care of, you can build MySQL++
    with this command:

        mingw32-make -f Makefile.mingw

    Notice that we're using the MinGW-specific version of GNU make,
    not the Cygwin or MSYS versions.  Many things will break otherwise:
    path separator handling, shell commands used by the Makefile, etc.

    Once the library is built, you should run the examples.
    At minimum, run resetdb and simple1.

    Once you're satisfied that the library is working correctly, you
    can run the install.bat file at the project root to automatically
    install the library files and headers in subdirectories under
    c:\mysql++.


Cygwin and MinGW Coexistence
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    It's possible to have both Cygwin and MinGW installed and build
    with the MinGW tools without interference from the Cygwin bits.
    The main thing you have to take care of is that MinGW's bin
    directory must precede the Cygwin bin directory in the PATH,
    so that its tools are found first.  If you use Cygwin's bash
    as a command shell in preference to the DOS-like cmd.exe, you
    can use this shell script to temporarily set the environment to
    "MinGW mode" and make it easy to get back to "Cygwin mode":

        #!/bin/sh
        PATH=/c/mingw/bin:/c/windows:/c/windows/system32:/c/cygwin/bin
        echo "Say 'exit' to leave MinGW shell and restore Cygwin environment."
        /usr/bin/bash --rcfile ~/.mingwrc

    I recommend having at least this in the ~/.mingwrc file:

        alias make=mingw32-make
        PS1='MinGW: \W \$ '

    The prompt change reminds you that you are in a sub-shell set up
    for MinGW.  The alias for 'make' ensures you don't accidentally
    run Cygwin's make, which won't work with Makefile.mingw.  We could
    just leave /c/cygwin/bin out of the environment, but there are
    Cygwin tools we want access to, like vim.  As long as all the
    MinGW ones override those Cygwin also provides, we don't need to
    worry about having both in the PATH.  Besides, having the alias
    is nice for those who have 'make' committed to muscle memory.
