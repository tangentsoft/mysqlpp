# What It Is

MySQL++ is a C++ wrapper for the MySQL and MariaDB C APIs.  It is built
on the same principles as the Standard C++ Library to make dealing with
the database as easy as dealing with `std` containers. MySQL++ also
provides facilities that let you avoid the most repetitive sorts of SQL
within your own code, providing native C++ interfaces for these common
tasks.

MySQL++’s development home is its [Fossil repository][home]. You may be
reading this via its read-only [GitHub mirror][ghm], intended for use by
automation tooling based on Git. Checkins to the Fossil repository get
pushed into the GitHub mirror within an hour.

[ghm]:  https://github.com/tangentsoft/mysqlpp
[home]: https://tangentsoft.com/mysqlpp/


# Prerequisites

To build MySQL++, you must have the MySQL/MariaDB C API development
files installed.

On Unixy systems — Linux, macOS, Cygwin, \*BSD, Solaris... — you are
most likely using a binary MySQL/MariaDB package where the server,
client, and development pieces are packaged separately.  The package
containing the development files is typically called something like
`MySQL-devel`, `libmysqlclient-dev`, etc.

If you're building on Windows with Visual C++ or MinGW, you need to
install the native Windows port of MySQL or MariaDB.  That includes the
development files in the installer, but depending on how it was built,
those files might not be installed by default.  You might have to select
a “complete” or “custom” install to get what you need to build MySQL++
against that version of MySQL/MariaDB.

Another common pitfall is that the Windows installers install
MySQL/MariaDB to a path with the version number embedded, which means we
cannot ship a single set of build system files that works with all
versions. We simply point at the General Availability version of
MySQL/MariaDB at the time of release, which may well be different from
the version installed on your computer.  You will have to adjust the
link and include file paths in the build system files accordingly.


# Additional Things to Read

Each major platform we support has a dedicated `README-*.txt`
file for it containing information specific to that platform.
Please read it.

For authorship information, see [the CREDITS.txt file][f1].

For license information, see [the COPYING.txt file][f2].

If you want to change MySQL++ itself, see [the HACKERS.md file][f3].

If you want to change the MySQL++ user manual, also read
[the user manual’s README][umr].

If you want to change the MySQL++ reference manual, see
[the Doxygen manual][dgm].

You should have received a user manual and a reference manual with
MySQL++. If not, you can read a recent version [online][docs].

Search the MySQL++ [mailing list archives][ml] and [its forum][for] if
you have more questions.


[dgm]:  https://www.doxygen.nl/manual/
[docs]: https://tangentsoft.com/mysqlpp/doc/
[f1]:   https://tangentsoft.com/mysqlpp/doc/trunk/CREDITS.txt
[f2]:   https://tangentsoft.com/mysqlpp/doc/trunk/COPYING.txt
[f3]:   https://tangentsoft.com/mysqlpp/doc/trunk/HACKERS.md
[for]:  https://tangentsoft.com/mysqlpp/froum/
[ml]:   http://lists.mysql.com/plusplus/
[umr]:  https://tangentsoft.com/mysqlpp/doc/trunk/doc/userman/README.md


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
