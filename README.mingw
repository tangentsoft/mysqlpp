Prerequisite: Create Import Library
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Before you can build MySQL++ with MinGW, you will need to
	create a MinGW-compatible import library for MySQL's C API
	library.  Assuming you installed MySQL in c:\mysql and MySQL++
	in c\mysql++, the commands to do this are:

		mkdir c:\mysql\lib\opt
		cd c:\mysql\lib\opt
	    dlltool -k -d c:\mysql++\libmysqlclient.def -l libmysqlclient.a


Building the Library and Example Programs
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Now you can build MySQL++ with this command:
	
		mingw32-make -f Makefile.mingw

	Notice that we're using the MinGW-specific version of GNU
	make, not the Cygwin or MSYS versions.	This is in order to
	get proper path separator handling.

	If you didn't install MySQL in c:\mysql, it's probably simplest
	to just change the Makefile.mingw files.  Theoretically you
	could adjust the Bakefiles instead, but due to the way we're
	using Bakefile, it's a little difficult to rebuild Makefiles
	on Windows right now.

	If you want to change the install location, that is in
	install.bat.
