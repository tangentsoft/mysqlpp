OS X is Unix under the hood, so README-Unix.txt covers the generic
bits.  I'll just cover a few of the issues specific to OS X here.


Prerequisite: Install the MySQL Development Files
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    MySQL++ is built on top of the MySQL C API library, so for MySQL++
    to build, it needs at least that library and its header files
    installed.  You don't need the server itself installed on your
    development machine, though it's often helpful anyway, for testing.
    You can get MySQL in at least three different ways for OS X:

    - From Fink: http://finkproject.org/  I mention this first
      because it's the method I use to test MySQL++ in development.
      If you ask about OS X on the mailing list, you'll get the most
      help if you're also using the Fink version of MySQL.  Once you
      have Fink installed, install the MySQL C API development
      files with:

      $ fink install mysql15-dev
      
      Then, in configuring MySQL++, give the --with-mysql=/sw flag
      to the configure script.
    
    - From MySQL.com.  I've not tried the official binaries,
      but they're known to work with MySQL++.  As I understand it,
      they install from a typical Mac GUI installer.  I don't know
      if it lets you install the development files separately from
      the server itself, so be sure to check.  The official Windows
      installers, for instance, have in the past installed the server
      by default but not the development files.

      Once it's installed, the configure script shipped with MySQL++
      v3.0 and up should find the library without any help.

    - From MacPorts, http://macports.org.  I have zero information on
      this other than that it's theoretically possible for it to work.
      If you figure out how to make it work, please post the method
      to the mailing list so I can update this document.


Making Universal Binaries
~~~~~~~~~~~~~~~~~~~~~~~~~
    The command line build system will generate libraries that
    only work with the platform you build MySQL++ on.  If you need
    to generate a libmysqlpp that works on both Intel and PowerPC
    machines, something like this may work:

    $ ./configure CXXFLAGS='-arch ppc -arch i386' --disable-dependency-tracking

    This is untested with MySQL++ in particular, but it's said to work
    with other projects.

    Note that with Tiger -- and to a greater extent, Leopard --
    there are really *four* architectures, not two: you have 32-bit
    and 64-bit versions of both PowerPC and Intel.  I'm not sure
    exactly how you'd modify the command above to make a library that
    supports all four, but I'll take a wild guess and say you'll need
    four -arch flags.  Or, you can just avoid the command line build
    system, and do it in...

Xcode
~~~~~
    Until very recently, there's been no official support in MySQL++
    for building on OS X using Xcode.  People have gotten it to work
    before on their own, but it's not something we felt we could
    support directly.

    As of MySQL++ v3.0, we're including Xcode project files.  So far,
    they're completely untested, but being part of the official
    tarball, it obligates us to at least try to support them. :)
    If they don't work, see HACKERS.txt for more info on fixing
    the source files that generate these project files.  We're not
    terribly interested in receiving updated project files.  They can
    be useful for comparison, but ultimately what we care about is
    being able to generate them correctly.
