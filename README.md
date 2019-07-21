# What It Is

MySQL++ is a C++ wrapper for the MySQL and MariaDB C APIs.  It is built
around STL principles, to make dealing with the database as easy as
dealing with an STL container.  MySQL++ relieves the programmer of
dealing with cumbersome C data structures, generation of repetitive SQL
statements, and manual creation of C++ data structures to mirror the
database schema.

MySQL++’s development home is its [Fossil repository][home]. You may be
reading this via its [GitHub mirror][ghm], but that repository is
read-only, meant as a download-only mirror and for use by automation
tooling based on Git. Checkins to the Fossil repository get pushed
into the GitHub mirror within a day.

[ghm]:  https://github.com/tangentsoft/mysqlpp
[home]: https://tangentsoft.com/mysqlpp/


# Prerequisites

To build MySQL++, you must have the MySQL/MariaDB C API development
files installed.

On Unixy systems (Linux, Mac OS X, Cygwin, \*BSD, Solaris...), the MySQL
development files are installed if you build MySQL from source.  If you
installed MySQL as a binary package, then the development files are
often packaged separately from the MySQL server itself.  It's common for
the package containing the development files to be called something like
`MySQL-devel`, `libmysqlclient-dev`, etc.

If you're building on Windows with Visual C++ or MinGW, you need to
install the native Windows port of MySQL or MariaDB.  The way that’s
packaged changes occasionally, so you might have to select a "complete"
version rather than the server-only version. You might also have to
choose a “custom” installation option to get the development files.

Another pitfall is that the project files we ship assume that you've
installed the current General Availability release of MySQL in its
default location on your computer. If you've installed a different
version, or if those packaging the development files move them around —
which happens occasionally! — you'll have to adjust the link and include
file paths in the project settings.


# Additional Things to Read

Each major platform we support has a dedicated `README-*.txt`
file for it containing information specific to that platform.
Please read it.

For authorship information, see [the CREDITS.txt file][f1].

For license information, see [the COPYING.txt file][f2].

If you want to change MySQL++, see [the HACKERS.md file][f3].

You should have received a user manual and a reference manual with
MySQL++. If not, you can read a recent version [online][docs].

Search the MySQL++ [mailing list archives][ml] and [its forum][for] if
you have more questions.


[docs]: https://tangentsoft.com/mysqlpp/doc/
[f1]:   https://tangentsoft.com/mysqlpp/doc/trunk/CREDITS.txt
[f2]:   https://tangentsoft.com/mysqlpp/doc/trunk/COPYING.txt
[f3]:   https://tangentsoft.com/mysqlpp/doc/trunk/HACKERS.md
[for]:  https://tangentsoft.com/mysqlpp/froum/
[ml]:   http://lists.mysql.com/plusplus/


# Building the Library

MySQL++ uses [Bakefile](http://bakefile.org/) to generate
platform-specific project files and makefiles from a single set
of input files.  We currently support these build systems:

*   **autoconf:** Use this method for most Unix type platforms along
    with [the generic instructions][unix].  We have supplemental
    information for [Linux][linux], [macOS][macos], [Cygwin][cyg], and
    [Solaris][sol].

*   **MinGW:** We ship `Makefile.mingw` for MinGW. See
    [its README][mingw] for details.

*   **Visual C++:** We ship Visual Studio 2003, 2005, and 2008 project
    files. Newer versions of Visual C++ can upgrade these automatically,
    though the result sometimes requires manual adjustment. No older
    version of Visual C++ will build MySQL++, due to compiler
    limitations.  See [its README][vcpp] for more details.

*   **Xcode:** We ship an Xcode project file, but it hasn't been well
    tested, since the autoconf method works just fine on macOS.

[cyg]:   https://tangentsoft.com/mysqlpp/doc/trunk/README-Cygwin.txt
[linux]: https://tangentsoft.com/mysqlpp/doc/trunk/README-Linux.txt
[macos]: https://tangentsoft.com/mysqlpp/doc/trunk/README-Mac-OS-X.txt
[mingw]: https://tangentsoft.com/mysqlpp/doc/trunk/README-MinGW.txt
[sol]:   https://tangentsoft.com/mysqlpp/doc/trunk/README-Solaris.txt
[unix]:  https://tangentsoft.com/mysqlpp/doc/trunk/README-Unix.txt
[vcpp]:  https://tangentsoft.com/mysqlpp/doc/trunk/README-Visual-C%2B%2B.txt



# Unsupported C++ Compilers and Operating Systems

If you get MySQL++ to build under an operating system or with a C++
compiler not listed above, we’re likely to accept a patch for it.  See
[the HACKERS file][f3] for guidance.


# Example Programs

You may want to try out the programs in the examples subdirectory
to ensure that the MySQL++ API and your MySQL database are both
working properly.  Also, [these examples][exr] give many examples of
the proper use of MySQL++.

[exr]: /doc/trunk/README-examples.txt


# If You Want to Hack on MySQL++...

If you intend to submit a change to the MySQL++ project, see
[the HACKERS file][f3].

If you want to change the MySQL++ user manual, also read
[the user manual’s README][umr].

If you want to change the MySQL++ reference manual, see
[the Doxygen manual][dgm].


[dgm]: http://www.doxygen.nl/manual/
[umr]: https://tangentsoft.com/mysqlpp/doc/trunk/doc/userman/README.md
