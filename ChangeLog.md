# Change Log

## 3.3.0, 2021.04.28

*   MySQL++ now detects the availability of C++11 capable compilers on
    Autoconf based systems and adjusts the compiler calling command to
    require that if such options are required.

    The general policy for MySQL++ is to support the oldest version of
    C++ that our oldest supported system provides.  Thus the update to
    C++11: the last compilers still supporting only C++03 or C++98
    dropped out of support since the last time we checked.  It may be
    that this release still supports these older versions of C++, but we
    will no longer go very far out of our way to ensure it.  Patches to
    restore backwards compatibility will be accepted if they don’t break
    forward compatibility.

    **Beware:** This change means builds are only ABI-compatible within
    the 3.x series when built with the same C++ version!  You may need
    to override `CXX`, `CXXFLAGS` and similar to get an ABI-compatible
    build, since older releases would use the generic compiler options,
    which might default to older versions of C++.

    You can adjust the build system to try for C++14 or C++17 by
    changing the `AX_CXX_COMPILE_STDCXX` call in `configure.ac`.
    Although this release contains features that depend on C++17,
    they’re optional, so unless you do this or adjust the build flags
    manually to ask your compiler to use a newer C++ version, these
    features will not be built into MySQL++.

*   Several conditional C++11 thru C++17 improvements by GitHub user
    “BratSinot:”

    *   Added move ctors and no-copy ctors to `ScopedConnection`.

    *   The copy ctor and assignment operators for `class ScopedLock`
        change so the compiler can elide them where it finds that safe.

    *   Added no-copy and no-assign ctors to class NoExceptions.

    *   Added a no-assign operator to `class mysql_ti_sql_type_info`.

    *   Using (`std::string::operator==`) in the inner loop of
        `FieldNames::operator []` to speed up comparisons.

*   Changed the “file slurp” idiom used by the `load_jpeg` example to
    a form that works with C++11, squishing an “address to rvalue”
    complaint with some compilers.

*   Throwing exceptions (if enabled) from a few more places in the code
    now when the underlying C API library reports an error.

*   Fixed a SQL type mapping problem with `TIMESTAMP NULL`.  Patch by
    Fossil repository user `nofree`.

*   Replaced a use of `size()` in bool context on an STL data structure
    where that isn’t a constant-time operation with faster `!empty()`.

*   Updated the stock paths in the MySQL C API library autodiscovery
    Autoconf macro so it'll work on MariaDB based systems where the
    headers and/or library are installed in a `.../mariadb` directory
    instead of `.../mysql` for backwards compatibility.

*   The RPM spec file now declares a build dependency on `mariadb-devel`
    rather than `mysql-devel`.

*   Reordered the `-L.` flag in the build lines to ensure the examples
    and test programs link against the in-tree version of MySQL++ if you
    have it installed at the system level as well.

*   Squished pedantic warnings from newer compilers.

*   Assorted documentation improvements.

*   Assorted build system improvements.


## 3.2.5, 2019.07.21

*   Supports `ulonglong` in MySQL++-to-SQL data type conversions so you
    can pass the return from `Result.insert_id()` and such into another
    SQL expression.  This previously required an explicit cast to one of
    the `sql_*` types when using a C++ compiler that can’t figure out
    that `sql_bigint` is equivalent on its own.

*   Updates to support newer versions of MySQL and MariaDB.

*   Assorted documentation improvements.

*   Assorted build system improvements.


## 3.2.4, 2018.07.26

*   Added the `MYSQLPP_UTF8_CS` and `MYSQLPP_UTF8_COL` macros to allow a
    given bit of code use either the legacy "utf8" character set or the
    new full-Unicode "utf8mb4" version depending on whether MySQL++ is
    built against MySQL 5.5 or newer, the version where `utf8mb4` was
    added.  Using this in the `resetdb` example to demo the feature.

*   Updated the [Unicode chapter of the user manual][umuc] to cover this
    and other changes in the Unicode world over the past several years.

*   Recent versions of MySQL and MariaDB have removed some obsolete
    features, causing MySQL++ build errors in places where the library
    wraps one of these features:

    *   MySQL 8.0 removed the embedded database feature, which caused
        build errors in four `Option::set()` overrides.  For MySQL 8.0
        and newer, these methods are now no-ops.

    *   Removal of the `CLIENT_LONG_PASSWORD` define in MariaDB 10.2
        caused a build error which we now fix with a conditional test
        for this before trying to use it.  Thanks for this fix go to
        Augusto Caringi of Red Hat.

*   Updated the library to work with newer toolchains, particularly
    Clang on current macOS and the C++17 aware versions of GCC:

    *   Methods with throw specifications still have them for ABI
        compatibility on systems with older compilers, but if we detect
        that the compiler is expecting C++14 or newer, we elide them.

    *   If the compiler is detected as expecting C++11 or newer, we now
        use `std::unique_ptr` everywere we used to use `auto_ptr`.

    *   Removed all `register` variable qualifiers.  It's officially
        obsolete in C++17 and it's been anywhere from unnecessary to
        unhelpful in C++ compilers for quite a long time now.

*   It is now possible to build a release tarball (`make dist`) on
    systems like Debian where `/bin/sh` is not Bash.

*   All prior MySQL++ 3.2.x release tarballs contain a symlink from
    `ltmain.sh` to the copy provided by Libtool on the system the
    tarball was made on.  If your system doesn't have Libtool installed
    or it's installed somewhere other than where Red Hat Enterprise
    Linux puts it, that symlink will be broken, preventing those
    tarballs from building until you fix the symlink somehow.  We're now
    dereferencing all symlinks when building the tarball to avoid this
    sort of problem.

There are also several changes to MySQL++ due purely to converting the
MySQL++ source code repository from Subversion to Fossil:

*   Updated the hackers' guide to reflect all of the differences this
    repository conversion entails.

*   Converted the old `HACKERS.txt` and this change log file to Markdown
    format.  Fossil will display plain text just fine, but it has a web
    renderer for Markdown, which gives nicer output.

[umuc]: https://tangentsoft.com/mysqlpp/doc/html/userman/unicode.html


## 3.2.3, 2016.12.29

*   Now using `snprintf()` instead of `_snprintf()` on Visual C++ 2015.

*   Cygwin changed its naming for the MySQL C API library since the last
    time we successfully tried building MySQL++ there.  Reworked the
    autoconf detection macro to make it less dependent on such
    individual platform differences.

    The macro is in fact now entirely side-effect-free, just setting
    MYSQL_C_* variables that the top-level Bakefile uses when and as it
    needs them, rather than expecting that major variables like CPPLFAGS
    and LIBS were overridden by the autoconf macro.  This makes the
    autoconf case more like that of the non-autoconf Bakefile outputs.

*   Squished a Clang warning


## 3.2.2, 2015.03.18

*   The DBDriver::operator= would leak a live connection if you
    assigned a disconnected DBDriver to it.  Patch by Quentin Armitage.

*   Plugged a potential DBDriver instance leak in Connection ctor.
    Patch by Yury Alekseev.

*   Explicitly #including algorithm.h in examples/multiquery.cpp for
    std::max().

*   Added missing "break;" statements in Transaction ctor taking an
    isolation level parameter, which caused only the last one to do
    the right thing.  Problem noticed by Tony Whyman, fix by me.

*   The bootstrap script now checks for the existence of needed build
    tools before trying to use them.  This lets us emit a clearer
    error message than the shell will.  Original patch by Adrian
    Cornish.

*   Fixed a build problem related to std::min/max with Visual C++ 2013
    due to a change in the definitions provided by its header files.

*   Tracked several changes made to Connector/C made since 3.2.1.

*   Assorted documentation updates.

*   Generating HTML with UTF-8 encoding, not 8859-1


## 3.2.1, 2013.12.10

*   Connection copy ctor wasn't copying the "throw exceptions"
    flag.  The copy always had exceptions enabled.

*   The resetdb example would print a bogus message about logging in
    as '1'@'1' instead of showing the actual values passed on the
    command line.  This did not affect how it actually logged in.

*   Previous release tarball was missing a few text files present in
    the source repository.

*   Updated build paths for VC++ and MinGW to track a file layout
    change in Connector/C 6.1.2.


## 3.2.0, 2013.06.20

*   References to MySQL 5.1 have been replaced with 5.6, that
    being the current GA release.

*   All Windows builds now assume you're using Connector/C 6.1
    instead of having installed the MySQL server on your machine
    just to get development files.  If you are running a MySQL
    server on your development machine and don't want to install
    Connector/C just to make MySQL++ happy, see the README-*.txt
    file for your platform to get instructions on fixing the paths
    so the build system finds the development files under the MySQL
    server directory.

*   The generated VC++ 2008 project files now assume you want 64-bit
    executables.  (It is 2013 now, after all.)

    Since the VC++ 2005 project files continue to be configured for
    32-bit builds, the easiest way to get a 32-bit project in VC++
    2008+ is to import the VC++ *2005* project files, rather than
    adjust the build target settings of all 44 vc2008/*.vcproj
    files within the IDE.

    See README-Visual-C++.txt for more on this change.

*   Added Query::replacefrom(): like insertfrom() but it uses
    REPLACE statements instead of INSERT.  Patch by Adrian Cornish.

*   Added support for SQL "unsigned float" columns.  Patch by
    Leonardo Lang.

*   Added "abicheck" top-level build target, which runs the
    ISPRAS ABI checker tool (http://goo.gl/e19lD) against
    a predecided "ABI-stable" version of MySQL++, comparing it
    against the current version.  This should help avoid future
    unplanned ABI breakages, as happened between 3.0.9 and 3.1.0.

    We have chosen not to revert those changes that broke the ABI,
    but instead have chosen to accept the 3.1.0 ABI as the new
    stable ABI for the rest of the 3.x series.  Running the ABI
    checker is now part of the release process, so we should not
    change the ABI again until 4.0!

*   The Query::insert() and replace() overloads that take a pair
    of iterators now work with containers that only provide forward
    iterators.  Idea by Adrian Cornish.

*   Using libtool again on Autotools systems.  It gives us
    relocatable libraries, which is needed in some linking
    situations.  Patch by Adrian Cornish.

*   VC++ release builds now have optimization enabled.  This might
    also affect other toolchains that create separate Debug and
    Release builds, if Bakefile obeys the directive for those
    systems.  (Such as, perhaps, Xcode.)  Initial patch by Matthew
    Bradbury.

*   Fixed a crash in CommandLine mechanism when the usage message is
    called up for programs that have no "extra" parameter and
    corresponding usage appendage.  Initial patch by "Crazy Pete".

*   Query::storein() now tolerates empty result sets, due either to
    DBMS failure or passing a query that never returns results,
    such as INSERT.

*   Exposed `DBDriver::result_empty()` as `Query::result_empty()`.  We
    did this primarily because it was needed as a helper for the
    previous fix, but it could also be useful more broadly.

*   Added -Wextra to the pedantic build flags.

*   Fixed the "escape" manipulator so it will work with Null<>
    wrapped types.  Patch by "Kemet".

*   ssqls2parse now depends on libmysqlpp.  (It wasn't getting
    relinked when you changed the library.)

*   The configure script's test for the existence of `mysql_ssl_set()`
    got broken by a change in the mysql_loc.m4 test, causing it to
    always return "no" even if the platform's MySQL C API library did
    have the function.  Therefore, the SslOption could never be set.
    Fixes this bug: stackoverflow.com/questions/7794408

*   Fixed a missing a "buried headers" ifdef wrapper for
    type_info.cpp.  Patch provided by Xavier Richez.

*   We now export the DBDriver class from the MySQL++ DLL when
    building on Windows.  Client programs can now access it directly,
    instead of being able to access only the fields and members
    exposed through Connection and Query.  Fix suggested by Xavier
    Richez.

*   MinGW builds no longer require that you manually create an import
    library for the MySQL C API DLL from a .def file we provide,
    which would inevitably get outdated.  We can now link directly to
    the DLL, and rely on the MinGW linker to figure out the imports.

*   Split the -l flags out of LDFLAGS in the userman's example
    Makefiles.  They're now in LDLIBS, which causes them to be
    put after -o $(EXE) by the default GNU make rules, where they
    should be.

*   Assorted documentation and build system improvements.

*   Assorted fixes for newer GCCs.


## 3.1.0, 2010.06.03

*   Default MySQL version now 5.1, having entered GA since 3.0.9.

*   Lots of platform updates tracked, lots of warnings from newer
    compilers squished.  Most by me, some by Adrian Cornish.

*   Added Query::insertfrom(), plus associated classes SQLStream,
    NoTransactions, and the InsertPolicy hierarchy.  Also adds
    examples/ssqls6.cpp, which shows how to use this new feature.
    Thanks for this feature go to Rick Gutleber, except for
    RowCountInsertPolicy, by me.

*   Added comparison operators to `tiny_int<T>`.  Thanks for this patch
    go to Edward Diener.

*   Added `SQLTypeAdapter::is_null()`.  This lets you retrieve a
    nullable column from the DB, then re-insert a value from that
    column back into the DB via a template query without explicit
    checks for null-ness; it will do the right thing now.

*   Added `-f` flags to `lib/*.pl` header file generating scripts to
    allow overriding the default limit of 25 fields for tqueries
    and SSQLSes without having to edit the file, as in previous
    versions of MySQL++.  Also added --with-field-limit option to
    the configure script so you can give both -f options at once
    during the build process.  This latter is especially useful
    for automated binary package builders.

*   The file lib/querydef.h (generated by lib/querydef.pl) now
    defines the MYSQLPP_TQUERY_MAX_PARAMETERS macro, giving the
    maximum number of parameters the functions in the generated file
    allow for template queries.  Similarly, lib/ssqls.h (generated
    by lib/ssqls.pl) defines MYSQLPP_SSQLS_MAX_MEMBERS.

*   Added `ConnectionPool::safe_grab()`, which pings the grabbed
    connection before returning it.  If the ping fails, we remove
    the dead connection from the pool and try again.  This doesn't
    eliminate the possibility of returning a dead connection;
    there is no protection against race conditions.  It is merely
    a best-effort attempt.

*   Added `ConnectionPool::remove(Connection*)`, which removes a
    connection from the pool.

*   Added `ConnectionPool::exchange()`, which wraps `remove()`
    and `grab()`.  It works like a customer support counter in a
    store: "I'm sorry, but this `Connection` is defective.  Can I
    have another, please?"

*   Added a feature to Transaction, where it can send a SET
    TRANSACTION ISOLATION LEVEL query before it starts the
    transaction proper, affecting the isolation level and the scope
    of that change.

*   Added ScopedConnection class.  Initial version by Joel Fielder,
    lots of changes by me.  Using it instead of explicit safe_grab()
    and release() calls on the pool in examples/cpool.cpp.

*   Added `FieldNames::operator[](size_type)` overloads.  This is
    necessary to correctly overload the base class versions inherited
    from std::vector.

*   FieldNames wasn't doing field-by-name matches case insentitively,
    most likely since 3.0.0 when we changed it to derive from
    std::vector.  This caused SSQLS (at least) to be case sensitive.
    Fixed this and renamed the stock.sdate field in the SSQLS
    examples to stock.sDate to test this.

*   Added Query::replace(iter, iter), on the model of existing
    insert(iter, iter).  Thanks for this patch go to David Walthour.

*   Added `Query::operator!()`, overriding `basic_ios` version so that
    `if (!query)` does the right thing.  Fixes problem report in
    thread http://lists.mysql.com/plusplus/8594

*   Query copy ctor and assignment operator now perform deeper
    copies, so you can pass Query objects around in certain ways
    that used to cause crashes.  Thanks for this patch go to Steven
    Van Ingelgem.

*   Defined `*_null` variants for every `sql_* typedef`, wrapping
    all those earlier types in `Null<>`.  All example programs now use
    these types instead of explicit `Null<>-wrapped` types.

*   Created a C++ command line parsing mechanism for the examples
    today, and for programs coming in the future.  It uses the
    platform version of getopt() if available, substituting a
    public-domain implementation that gets built into MySQL++
    itself otherwise.  It's documented in the refman -- see
    the CommandLineBase class -- but it's in the newly created
    mysqlpp::internal namespace, which means we don't promise any
    sort of ABI or API stability for it.  You're welcome to use it
    in your own programs, but expect it to change without warning.

*   Added a few missing `MYSQLPP_EXPORT` qualifiers to `operator<<()`
    overloads for stream manipulators to allow their use when MySQL++
    is built as a DLL.

*   Added backticks around table and column identifiers in all
    code paths exercised by dtest.  There are some others remaining,
    particularly things like Row::field_list().  Thanks for this
    patch go to Adrian Cornish.

*   Added mysqlpp::NOW() which returns a value that, when inserted
    into a SQL query, results in a call to SQL's NOW() function.
    DateTime::now() -- added in 3.0.0 -- does the same thing,
    but this is shorter and matches the SQL syntax.

*   Calling DBDriver::disconnect() twice no longer calls
    mysql_close() twice.

*   Worked around a weakness on Windows that caused problems in code
    with a connect(), disconnect(), connect() call pattern, when
    also using a named pipe to connect to the DB.  (There never
    has been a problem with this on other platforms or with other
    IPC mechanisms.)

*   Initializing a SQLTypeAdapter with a floating-point value
    equal to infinity or NaN results in SQL null if the type
    is also nullable, or 0 otherwise.  Previously, we got an
    implementation-defined string, which could give invalid SQL,
    or could give 0, a different value from NaN, infinity, or
    SQL null.  This does collapse the notions of NaN and infinity,
    but the MySQL reference manual doesn't say how you're supposed
    to preserve these differences.

*   Removed the dependency of stadapter.h on `sql_types.h` just to get
    `sql_tinyint*`.  We now `#include "tinyint.h"` instead, and use
    equivalent types to what `sql_types.h` defines.  This is necessary
    because `sql_types.h` should be one of the last things included from
    `mysql++.h`, so that it can define `sql_*` equivalents for
    everything else in MySQL++, which means all those other things need
    to be included by that point.

*   The "find MySQL++" autoconf macro (config/mysql++.m4) now
    searches for the library in lib64 subdirectories everywhere it
    also looks in lib.  We did this for the C API finder macro way
    back in MySQL++ 2.2.0, and only neglected to do it for this
    macro because it's not used by MySQL++ itself, just provided
    as a courtesy to MySQL++ users that also use autoconf.

*   Also improved the "find C API" macro (`config/mysql_loc.m4`)
    so it, too, is more useful in third-party projects.

*   Moved private `FieldNames::str_to_lwr()` method out into its
    own module, in the new mysqlpp::internal namespace, so other
    modules can use it.  Also added an overload for this new global
    method taking std::string instead of char*.

*   userman build process can now find FOP when installed from
    the standard Ubuntu/Debian package repository.

*   No longer generating refman.pdf.  It's difficult to generate
    properly on some platforms, and isn't all that useful anyway
    compared to the HTML version, which builds properly everywhere.

*   Dropped custom.h backwards-compatibility header.  (Deprecated
    over a year ago in MySQL++ 3.0.0.)

*   Fixed a bad pointer access crash in `Connection::set_option()`
    triggered when the option set call fails.  Thanks for this
    patch go to Kevin Regan.

*   ReconnectOption behavior now knows about a fix in MySQL 5.1.6 and
    newer that lets you set it either before the connection is up,
    or after.  If you try to set this option when MySQL++ is built
    against earlier versions of the MySQL C API, it will refuse to
    set the option because it knows the option will be reset when
    the connection comes up.

*   No longer trying to make the C API library read from a "my"
    options file before connect.  It does that by default, and
    the file is supposed to be .my.cnf anyway.

*   Reworked the `load_jpeg` example code for clarity, extracting
    load_jpeg_file() routine as an implementation detail and
    making that use the C++ stringstream "slurp into string" idiom
    instead of allocating a C char buffer and copying that into
    a std::string.

*   Restored support for repeating query parameters.  Apparently
    you could say things like "stuff %0 morestuff %0" in v2.1,
    but this was broken by initial 1-parameter fixes in v2.2.
    This patch reportedly lets us have both things at once, and
    improves handling of 1-parameter template queries in general.
    Thanks for this patch go to Martin Gallwey.

*   Added examples/tquery4.cpp based on test code and feedback
    from Martin.

*   Put Comparable class in namespace mysqlpp, where it should always
    have been.  Aside from being just plain wrong, it caused a
    conflict with Boost if you used its Comparable class and hoist
    them both into the global namespace.  Thanks for this patch go
    to Michael Radzewitz.

*   Fixed all the known problems with the Xcode project files.

*   Skeletal support for SSQLS v2.  Doesn't do anything useful
    yet, it just got caught up in the need to release 3.1 as-is.

*   Various enhancements to the test suite.

*   Many, many documentation enhancements.


## 3.0.9, 2009.02.04

*   It is now possible to use Null<T> objects in const context.

*   Query was emitting thousands separators when used in some
    locales.  Thanks for the fix go to Tomaž Šolc.

*   Restored support for using Query::storein() with template
    queries.  Was mistakenly removed in the 3.0.0 development
    process.

*   GCC 4.4 build fixes.  Thanks for this go to Remi Collet.


## 3.0.8, 2008.11.27 (r2432) The "Thanksgiving" release

*   Fixed a VC++ build error introduced in 3.0.7 in the Sun CC fix.

*   Fixed the Makefile we generate from Makefile.simple for the
    devel RPM.  The recent Bakefile upgrade changed the output so
    our munger stopped munging.

*   Changed all instances of "CREDITS" in header comments to
    "CREDITS.txt".

*   Brought ebuild file up to date with what Gentoo has been
    using for 2.3.2.


## 3.0.7, 2008.11.21

*   Fixed bug where initting a Query with a query string and then
    appending more to it with operator <<() would start overwriting
    the beginning of the buffer.  Thanks for the fix go to Chris
    Frey.

*   Added String::empty(), to parallel std::string better.

*   Added SQLTypeAdapter ctor for Null<String>, to allow SSQLS with
    NULL BLOB columns.  Thanks for this patch go to Russell Robinson.

*   Passing on "throw exceptions" flag in Query::storein() to the
    temporary Row objects it creates.  (Should have done this
    all along.)

*   All MySQL++ classes with operator[]() and at() methods now
    throw the new BadIndex exception on out of range indices now,
    unconditionally.  Before, it was a mishmash of MySQL++-specific
    mechanisms, std::range_error, and unchecked access.

*   Fixed some bugs in `load_jpeg` and `cgi_jpeg` examples that
    prevented them from working correctly on Windows.

*   Squished some warnings and incompatibilities that caused
    difficulty with Solaris CC.

*   Several minor fixes to allow building on Cygwin again.

*   Dedicated VC++ 2008 support.  (Not just a clone of VC++ 2005
    project files in a different directory.)

*   Lots of documentation improvements.


## 3.0.6, 2008.08.16

*   Several fixes to allow thread-aware build on Solaris

*   More improvements to search for Fink version of C API library.

*   Various improvements to Windows installer (install.hta)


## 3.0.5, 2008.08.06

*   Replaced install.bat with new install.hta, which has a GUI and a
    lot of embeded logic for doing The Right Thing, which we couldn't
    do in a dumb batch file.

*   Several fixes to allow it to build on Solaris 10.

*   Fixed a bug in comparison of Null<T> to T: wouldn't always give
    the right result for one particular combination of argument
    values and order of parameters to operator <().  It wouldn't
    fail all the time, either; it depended on the way the system's
    memory allocator dealt with newly allocated RAM.  The bug was
    discovered during the Solaris 10 porting effort, but it is not
    a Solaris-specific bug.

*   Split Linux-specific material out of README-Unix.txt into
    README-Linux.txt, and created README-Solaris.txt.

*   Shipping a vc2008 directory.  Populated by bootstrap script with
    copies of vc2005 files when those are newer, with the idea being
    to update them by hand by running them through VC++2008 before
    release.

*   Several fixes to VS-only examples. They hadn't been updated to
    track several of the changes in MySQL++ v3, so they wouldn't
    build OOTB at all, crashed when you did get them building, and
    emitted a lot of warnings during the build.  Cleaned it all up.

*   Autoconf now explicitly checks whether we need to link to zlib
    to link to MySQL C API library.  It used to be required, but
    lately, MySQL has been shipping the library with zlib stuff
    embedded, causing multiply defined symbol errors on some systems.

*   configure script now looks in more locations for the MySQL C API
    library, adding default locations for Fink and Solaris.


## 3.0.4, 2008.07.02

*   Fixed a few bugs in SslOption that prevented it from actually
    working.  If you've been having SSL problems since upgrading
    to MySQL++ v3, this is why!  Thanks for these patches go to
    Petteri Kiiskinen.

*   Changed type of String's internal "size" typedefs so it'll
    build without errors on 64-bit systems where these ints are
    different sizes.

*   Many user manual improvements.


## 3.0.3, 2008.05.11

*   Fixed query well-formedness handling in Query::store() and use().
    It was quietly eating these errors, confusing them with the
    no-results case.

*   Fixed examples/cpool.cpp to build without thread support.

*   No longer hiding all stdint.h typedefs inside namespace mysqlpp.

*   Fixed mysqlpp::String comparisons of empty strings against
    nonempty strings; if the empty string was on the left hand side,
    it would succeed because it was only comparing against characters
    in the empty side, which matches everything.  (Fixes bug 11588.)


## 3.0.2, 2008.04.13

*   Increased float to string conversion precision from 7 to 9 and
    double from 16 to 17 digits.  Previous values weren't enough
    near the maximum values that each can hold.

*   Replaced Query's safe bool mechanism with an override of
    basic_ios::operator void*() to avoid a conflict between the
    two mechanisms.  As the base class version isn't virtual,
    this is arguably evil, but it should be harmless in typical use.
    Besides, testing Query in bool context usually isn't the right
    thing anyway: test the result set instead.

*   Made ConnectionPool::grab() virtual.

*   Overriding ConnectionPool::grab() and release() in
    examples/cpool.cpp to show how to do connection-in-use count
    limiting.  Also, added a few more output indicator states to
    allow one to better understand program flow.


## 3.0.1, 2008.03.23

*   String objects can now be compared to mysqlpp::null directly.

*   Added a template operator== and operator!= to String, syntactic
    sugar for the existing String::compare() methods.

*   String::compare() now returns 0 ("equal") when one of the
    strings is an uninitialized String() (no refcounted buffer)
    and the other is empty.  It used to consider any initialized
    string greater than an uninitted one.  An uninitialized String
    appears empty, though, so this was incorrect.

*   Made `Connection::thread_aware()`, `thread_start()` and
    `thread_end()` static methods, so they can be called before you create
    your first connection.  Ditto for DBDriver versions of these
    methods.

*   Calling `Connection::thread_start()` and `thread_end()` in
    `examples/cpool.cpp`, as appropriate.  Above changes were necessary
    to make this work sensibly.

*   Made ConnectionPool::release() virtual, so your pool subclass can
    override it.

*   Added ConnectionPool::size(), so a subclass can know the current
    number of extant connections.

*   No longer single-quoting NOW() call generated for default
    init of DateTime type when building queries from SSQLS objects
    in Query::insert(), replace() and update().  The template query
    and stream interfaces of Query treated NOW() correctly already.

*   Fixed a bug that left `SSQLS::table_override_` uninitted if
    you used certain of the generated ctors or `set()` member
    functions instead of others used by the examples.  This could
    cause a crash any time you caused `SSQLS.table()` to be called,
    such as when passing the SSQLS to `Query::insert()`.

*   Minor memset bug fixed in `test/uds.cpp`.  Patch by Dave Jones.


## 3.0.0, 2008.02.29 (r2236) The "Giant Leap Forward" release

**WARNING:** THIS IS NOT A DROP-IN REPLACEMENT FOR MySQL++ v2.x!

You will have to recompile your program against this version of the
library, and you will almost certainly have to make code changes as
well.  Please see thee [API][apib3] and [ABI breakages][abib3] sections
in the user manual for information on migrating your code to this new
version.

[apib3]: https://tangentsoft.com/mysqlpp/doc/html/userman/breakages.html#api-3.0.0
[abib3]: https://tangentsoft.com/mysqlpp/doc/html/userman/breakages.html#abi-3.0.0

*   Added ConnectionPool class, primarily to let multithreaded
    programs share a set of Connection objects safely in situations
    where it isn't acceptable to have a Connection per thread.

*   Created examples/cpool.cpp to demonstrate this new class.

*   Added RefCountedPointer template, which provides automatic
    memory management and data sharing.  It's not intended
    for use outside of MySQL++ itself, but it's the mechanism
    behind everything below where reference counting is mentioned.
    I created the initial version of it, but Jonathan Wakely almost
    completely rebuilt it, and Joseph Artsimovich provided helpful
    commentary and advice as well.

*   Many improvements to Specialized SQL Structures (SSQLS):

    - Renamed `custom* to ssqls*`.  There's still a custom.h which
      #includes ssqls.h for you, but it's only intended to ease
      the transition to the new name.  It will go away in a future
      release, probably as soon as v3.1.

    - SSQLSes are finally compatible with Null<>-wrapped types.  This
      feature is based loosely on the "Waba" patch posted to the
      mailing list back in the v2.0 days, but extended to allow
      Null<T> types for key fields.  (The Waba patch only allowed
      these types in non-key fields.)

    - It's no longer necessary to define a different SSQLS for each
      different field set you use in queries.  That is to say,
      you can define an SSQLS for an entire table and store just a
      subset of the table in it now, with the other fields keeping
      default values.  Removed examples/custom6.cpp, as custom1.cpp
      can now demonstrate the same thing, implicitly.

    - An SSQLS's field order no longer has to match the order of
      fields in the result set it is populated from.

    - As a result of previous, removed `sql_create_c_order_*` macros;
      they have no purpose now.

    - Removed order parameters from `sql_create_complete_*`, which now
      gives it the same functionality as `sql_create_c_names_*` so
      removed the latter, too.

    - Removed "basic" variants of SSQLS creation macros.  They've
      been unofficially deprecated by dint of being all but
      undocumented and unexemplified for a very long time now.

    - It's now possible to use mysqlpp::String, Date, DateTime, and
      Time types in the key field positions in an SSQLS as they
      now support the necessary comparison interfaces.

    - If you use a floating-point data type in one of the key field
      positions, it no longer uses exact comparison logic.  Instead,
      it now does [in]equality comparisons by testing whether the
      difference between two floating-point values is less than a
      configurable threshold defaulting to 0.00001.
    
    - You can now use 'bool' type in an SSQLS.

    - Renamed `_table` static member variable in each SSQLS to `table_`
      and made it private.  There are now public setter and getter
      methods, `table()`.

    - Added per-instance table name overriding via `instance_table()`
      setter.  `table()` getter returns static version if this is not
      set, so it's still a global setting by default.

*   You can now use mysqlpp::null as a template query parameter to
    get a SQL null.

*   Replaced template `ColData_Tmpl<T>`:

    - Used to have `typedef ColData_Tmpl<std::string> MutableColData`.
      It was used only once within MySQL++ itself, and was never
      documented as a class for end users.  This one use within the
      library was a crock, so we just replaced this use with std::string
      and removed the typedef.

    - This left just one use of `ColData_Tmpl<T>`, instantiating it
      with the MySQL++ utility class const_string, basically a
      clone of std::string with all the mutating features removed.
      Folded the functionality of const_string into the template,
      renamed the result to String, and deleted the const_string
      class.  It'd be a complete std::string replacement -- with
      SQL-related enhancements -- if it were modifiable, but MySQL++
      doesn't need it to be modifiable.  Yet, it's still the closest
      thing MySQL++ has to its own string type; thus the name.

    - Replaced its internal buffer management with a much more
      clever reference counted scheme.  This shows its greatest
      advantage in the return from Row::operator[](), which for
      technical reasons must return by value, not by reference
      as is more common.  This lets you pass around Strings by
      value while having the efficiency of reference semantics.
      This can be important with large return values, like BLOBs.

    - Converting String to numeric types (ints, floats...) uses a
      new, cleaner system by Jonathan Wakely.  Unless you were
      abusing weaknesses in the old system, you won't see a
      difference.  It's just more robust and flexible.

*   Redesigned SQLString:

    - It used to derive from std::string, and while MySQL++'s
      internals did use it in place of std::string, these places
      didn't take advantage of the additional features offered
      by SQLString.  So, replaced all those uses with std::string.

    - All the remaining uses are MySQL++ public interfaces that
      need to be able to accept any of many different data types,
      and we want that data to be automatically converted to a
      SQL-compatible string form.  Because it no longer has the
      parentage to be a general-purpose string type and MySQL++ has
      a new contender for that role (String), renamed SQLString to
      SQLTypeAdapter to reflect its new, limited purpose.  ("STA"
      for short.)

    - Since we don't have the std::string base class to manage the
      string buffer any more, using the same reference counted
      buffer mechanism as String.  In addition to saving code by
      not creating yet another buffer management mechanism, it means
      objects of the two classes can share a buffer when you assign
      one to the other or pass one to the other's copy ctor.

    - Added many more conversion ctors.

    - STA interfaces using the 'char' data type now treat them as
      single-character strings instead of one-byte integers, as
      does the Standard C++ Library.

    - Added `mysqlpp::tiny_int` interfaces to STA to replace the
      former char interfaces for those needing one-byte integers.

*   As a result of the ColData -> String redesign, removed
    Row::raw_*().  Before String copies were efficient, this
    was helpful in accessing BLOB data efficiently.  It was also
    required back when ColData didn't deal correctly with embedded
    null characters, but that reason is gone now, too.

*   `Row::operator[](const char*)` no longer unconditionally throws the
    `BadFieldName` exception when you ask for a field that doesn't
    exist.  It will still throw it if exceptions are enabled, but if
    not, it'll just return an empty `String`.  This was necessary to
    make the SSQLS subset and field order independence features work.

*   Similarly, `Result::field_num()` returns -1 when exceptions are
    disabled and you ask for a field that doesn't exist.

*   You can now use the OptionalExceptions mechanism to disable
    exceptions on const MySQL++ objects.

*   Redesigned query result classes:

    - Instead of Result deriving from ResUse, the two derive from
      a common base class -- ResultBase -- containing the bits that
      are truly the same between them.  Before, Result inherited
      several methods that didn't really make sense for "store"
      query result sets.

    - Renamed Result to StoreQueryResult and ResUse to UseQueryResult
      so it's clearer what each is for.

    - Renamed ResNSel to SimpleResult.

    - Made all SimpleResult data members private and hid them behind
      const accessor functions of the same name.

    - The result set classes all used to be friends of Connection
      for various lame reasons.  Since they are created by Query,
      and Query has a good reason for a strong relationship with
      Connection, moved Connection access out of each result set
      class into the code in Query that creates that type of result
      set object.

    - StoreQueryResult now derives from vector<Row> in addition to
      ResultBase; it used to merely emulate a vector of Rows, poorly.
      It can now dispose of the MYSQL_RESULT at the end of object
      construction, because it creates all the Row objects up front
      instead of on the fly.  And as a result of *that*, operator[]
      returns by reference instead of by value, operator -> works
      correctly on iterators, all STL algorithms work, etc., etc.

    - IMPORTANT COMPATIBILITY BREAK: because we used `fetch_row()`
      stuff in Result previously, it was okay to index past the
      end of the result set: you'd just get a falsy Row when you
      did this, just as happens when doing the same thing in a
      "use" query.  The simple1 and simple2 examples did this,
      so it's likely that code exists that takes advantage of this
      misfeature.  New versions of these examples show how to index
      through a StoreQueryResult without running past its end.

    - ResUse used to delay creation of its FieldNames and FieldTypes
      objects until the point of need.  This had several implications
      for thread and exception safety that we fix by just creating
      them in the ctor.  If your code is multi-threaded and was
      avoiding certain usage patterns due to crashes, it's worth
      trying your preferred way again.

    - Result sets create a few data structures to hold information
      common to all rows in that set.  The row objects need access
      to these shared data structures, so on creation each gets
      a pointer back to the result set object that creates it.
      This was efficient, but required that a result set object
      outlive any row objects it creates.  Now these shared data
      structures are reference-counted, decoupling the lifetime of
      the child row objects from their result set parent.

    - Copy operations for result sets used to actually be "moves"
      before, for efficiency.  (MySQL++ itelf only copied result
      sets in returning them by value from the query execution
      methods of Query, so this was acceptable if you didn't do
      anything uncommon with these objects.)  Reference counted
      data structures allow us to have copy semantics now without
      sacrificing efficiency.

    - You can now use Query::storein() with an STL container of Row
      objects now, instead of having to use SSQLSes.  The lifetime
      issue guaranteed a crash if you tried this before.

    - Removed a bunch of unnecessary alias methods:

      - columns() -> num_fields()
      - names()   -> field_names()
      - rows()    -> num_rows()
      - types()   -> field_types()

    - Renamed several methods for grammar reasons:

      - fields(unsigned int)      -> field(unsigned int)
      - names(const std::string&) -> field_num(const std::string&)
      - names(int)                -> field_name(int)
      - types(int)                -> field_type(int)

    - Removed several "smelly" methods:

      - purge()
      - raw_result()
      - reset_names()
      - reset_field_names()
      - reset_types()
      - reset_field_types()

*   Field class used to just be a typedef for the corresponding C
    API class.  Now it's a real C++ class providing a more MySQL++
    sort of interface, plus good OO things like information hiding
    and implementation detail abstraction.  This changes several
    things about the interface.

*   Fields class was basically a specialized std::vector work-alike
    for dealing with the C API to get access to MYSQL_FIELD objects
    and present them as contained Field objects.  New Field type
    let us replace it with "typedef std::vector<Field> Fields"

*   Major improvements to the quoting and escaping mechanisms:

    - Replaced almost all of the type-specific interfaces in manip.h
      with a single version taking STA.  The compiler can convert
      almost anything to STA without losing any information we need
      for correct quoting and escaping.  This has the side benefit
      that we can now do correct quoting and escaping for more data
      types now, including plain C and C++ string types.

    - Fixed a bug in `quote_double_only` manipulator for String: was
      using single quotes by mistake.

    - Escaping and quoting only works in instances where MySQL++
      can tell you're building a SQL query and are using a data type
      that requires it.  This affects many things, but the one most
      likely to cause trouble is that inserting MySQL++'s quoting
      and escaping manipulators in non-Query ostreams is now a no-op.

    - Added `escape_string()` member functions to Query and
      `SQLQueryParms::escape_string()`, and removed the global function
      of the same name.  Because these are tied indirectly to a
      Connection object, this also has the effect that escaping is
      now aware of the current default character set used by the
      database server.  There's only one case where this isn't done
      now, and that's when we're disconnected from the server.

    - Previous two items form a trade-off: if your code was depending
      on MySQL++ to get SQL escaping and it no longer happens for
      what we consider a good reason, you can build a replacement
      mechanism using these new functions.  Quoting needs no special
      support in MySQL++.

    - Removed 'r' and 'R' template query parameter modifiers,
      which meant "always quote" and "always quote and escape"
      regardless of the data type of the parameter.  There are no
      corresponding manipulators (for good reason), so the removal
      restores symmetry.

*   Created DBDriver class from code previously in Connection and
    Query to almost completely wrap the low-level MySQL C API:

    - Connection creates a DBDriver object upon connection and
      passes a pointer to it down to Query objects it creates.
      In turn, they pass the pointer on to any of their children
      that need access to the C API.

    - Nothing outside DBDriver calls the C API directly now, though
      DBDriver leaks C API data structures quite a lot, so this
      feature doesn't constitute "database independence."  See the
      Wishlist for what must be done to get to that point.

*   Completely redesigned the connection option setting mechanism:

    - There's now just a single `Connection::set_option()` method that
      takes a pointer to the abstract Option base class, and there is
      an Option subclass for every connection option we understand.
      Thus, type errors are now caught at compile time instead of
      at run time.

    - Replaced `Connection::enable_ssl()` with SslOption class.

    - Enabling data compression and setting the connection timeout
      are no longer set via parameters to Connection interfaces.
      These are now set with CompressOption and ConnectTimeoutOption.

    - Similarly, removed `client_flag` parameters from Connection's
      ctor and connect() method and added corresponding Option
      subclasses.  There's about a dozen, so rather than list them
      here, look for similarly-named classes in lib/options.h.

*   Added `Connection::count_rows()` to execute `SELECT COUNT(*) FROM
    tablename` queries for you.

*   Moved `Connection::affected_rows()`, `info()` and `insert_id()` methods
    to class Query, as they relate to the most recently-executed
    query, not to the connection.

*   Several method name changes in Connection:

    - `client_info()`   -> `client_version()`
    - `host_info()`     -> `ipc_info()`
    - `proto_info()`    -> `protocol_version()`
    - `server_info()`   -> `server_version()`
    - `stat()`          -> `status()`

*   Removed `Connection::api_version()`.  It does the same thing as
    `client_version()`.

*   Lots of changes to Date, DateTime, and Time classes:

    - If you use the default constructor for DateTime and don't
      subsequently set its year, month, day, hour, minute or second
      data members to nonzero values, it becomes the SQL function
      "NOW()" in a query string.  You can also use DateTime::now()
      as syntactic sugar for this.

    - As a result of above, had to hide all of DateTime's data
      members behind accessor functions, to keep the state of the
      object consistent.  (If it's initialized as "now" and you
      give it an explicit year value, say, it is no longer "now",
      so the setter has to clear the now-flag.)  There are getters
      and setters for year, month, day, hour, minute and second,
      all named after the member.

    - Did the same for Date and Time for consistency, even though it
      isn't technically required.

    - The `sql_timestamp` typedef now aliases DateTime instead of Time.

    - Renamed template class DTbase<T> to Comparable<T>.  The fact
      that it's the common base class of all date and time classes
      is irrelevant; making subclasses comparable is what it does,
      so that's what it should be named after.

    - Added a DateTime ctor taking discrete year, month, day, hour,
      minute, and second values.

    - Implicit conversion from stringish types to the date and time
      types is no longer allowed.  This is part of the "Waba"
      Null<T> patch mentioned above; allowing implicit conversions
      would break this new feature.

    - Added operator std::string and str() methods to all of these
      classes.  Adding this to the existing operator << support, you
      now have several ways to convert these objects to string form.

    - Added `time_t` conversion to Date and Time classes.  DateTime
      already had it, since it's more legitimate to convert time_t
      to DateTime, but you could already "slice" it with something
      like Time(DateTime(time(0))) so there's no point pretending
      you can't get from time_t to Date or Time.  Might as well
      legitimize it.

*   Improved `tiny_int` class:
      
    - Turned it into a template parameterized on the value type so
      you can get both signed and unsigned TINYINTs

    - Changed the `sql_tinyint` and `sql_tinyint_unsigned` typedefs to
      use `mysqlpp::tiny_int<VT>` instead of raw chars

    - Added a bool conversion ctor and operator, and typedef'd it
      to sql_bool and sql_boolean to match MySQL server behavior

*   Added many more `sql_*` typedefs.  We now have a typedef for every
    type the MySQL server knows about, including those it supports
    just for compatibility with other database servers.

*   Changed the `sql_*int*` typedefs to use integer types of the same
    size as the MySQL server.  (Run test/inttypes.cpp to test it.)

*   Added copy ctor and assignment operator to Row.

*   Row::operator[]() takes int now instead of unsigned int.
    This finally (!) makes it possible to say row[0] without the
    compiler giving an ambiguous overload error.

*   Changed all uses of row.at(0) in the examples to row[0]

*   Added operator[] to all classes that only had at().

*   Query now automatically resets itself unless the query fails
    or you're using template queries.  In either case, the contents
    of the query's internal buffers are considered precious,
    either for debugging, or future queries.  Except when using
    template queries, this means you may be able to avoid calling
    Query::reset() entirely.  It's still safe to call reset()
    as before, just unnecessary most of the time.

*   Removed `reset_query` parameter from all Query methods.  It was
    almost completely broken before, and above change does what
    was really wanted to begin with.

*   `Query::store_next()` and `Result::fetch_row()` no longer throw
    the EndOfResults and EndOfResultSets exceptions; these are not
    exceptional conditions!  These methods simply return false now.

*   Removed examples/usequery.cpp: there's no essential difference
    between what it did and what examples/simple3.cpp does now as
    a result of the previous change.

*   Added Query::exec(void), paralleling Query::execute(void).

*   Removed Query::preview().  The most direct replacement is str(),
    which has always done the same thing.

*   You can now insert a Query object into an ostream to get a copy
    of the built query.  This means Query::str() is only necessary
    when using template queries.

*   Removed overloads of Query::execute(), store(), and use()
    that take const char*.  It was redundant because const char*
    converts implicitly to STA, for which overloads already exist.

*   Renamed Query::def to `Query::template_defaults` to make its
    purpose clearer.

*   `Query::error()` now returns const `char*`, not a `std::string` by
    value.  There's no point in making a copy of the error string.
    The method is now `const` as well, as it doesn't change the
    `Query` object.

*   Added Query::errnum(), which just wraps Connection::errnum().

*   Added error number parameters and accessor functions to BadQuery,
    ConnectionFailed and DBSelectionFailed exceptions, to preserve
    the state of Connection::errnum() at the point of the exception,
    so you don't have to rely on this value remaining unchanged
    during the exception throw process.  All places that use these
    exceptions now include this value where possible.  Thanks for the
    initial patch go to Jim Wallace.

*   Removed Lockable mechanism from Connection and Query; it was
    conceptually flawed.  See the new user manual chapter on
    threading for advice on using MySQL++ safely without locking.
    There is mutex locking now in ConnectionPool, but that's it.

*   Connection::query() now takes an optional query string, allowing
    the returned Query object to start off with a value.  Especially
    useful when the query string is static, either because it's
    a simple query or because it's a template.  You can now build
    chains like "if (conn.query("CREATE INDEX ...").exec()) { ..."

*   Added `Connection::thread_aware()`, `thread_end()`, `thread_id()`
    and thread_safe().  See user manual's threading chapter for
    explanations.

*   Renamed "success" data members in Connection, Query and
    SimpleResult (neé ResNSel) to "copacetic_", making them private
    if they weren't before.  This better reflects their actual
    use, which isn't to say that there has necessarily been actual
    success, but rather that everything's okay with the object.

*   Removed success() member functions from above classes.  All can
    be tested in bool context to get the same information.

*   Replaced all operator bool()s in MySQL++ classes with safer
    alternatives.  See http://www.artima.com/cppsource/safebool.html
    Thanks to Jonathan Wakely for much helpful commentary, advice,
    and code used in these mechanisms.

*   Decoupled `Connection::copacetic_` from `Connection::is_connected_`.
    It is now possible for the object to be copacetic without being
    connected.  However, if it tries to connect and fails, then
    it is not copacetic.  If it is copacetic and not connected, it
    means we haven't even tried to connect yet, a useful distinction.

*   Collapsed `Connection`'s host, port, and socket name down into
    a new combined `server` parameter which is parsed to determine
    what kind of connection you mean.  These interfaces are still
    compatible with v2.3 and earlier up through the port parameter.
    There are differences beyond this.

*   Added TCPConnection, UnixDomainSocketConnection and
    WindowsNamedPipeConnection subclasses for Connection giving
    simpler construction and connect() method interfaces for
    instances where you know what kind of connection you want at
    compile time.

*   Changed Connection::ping() return value from int to bool.

*   Renamed NullisNull to NullIsNull -- capital I -- and similar for
    NullisZero and NullisBlank.

*   It's now a compile-time error to try to convert a MySQL++
    representation of a SQL null to any other data type, rather
    than a run-time error as in previous versions.  Removed
    BadNullConversion exception as a result.

*   Way back in v1.7.x we used the BadQuery exception for all kinds
    of exceptional conditions, not just bad queries.  Replaced
    most of these in v2.0.0 with new dedicated exceptions, but a
    few remained:
    
    - Errors that occur during the processing of a "use" query after
      the query itself executes correctly now throw UseQueryError.
      It's not a "bad query", because the query executed
      successfully.  It just choked during subsequent processing,
      so it's a different exception.  Thanks for this patch go to
      Jim Wallace.

    - Replaced BadQuery exceptions thrown in Row constructor due
      to bad ctor parameters with new ObjectNotInitialized exception
      This is also Jim Wallace's work.

*   The examples now all use getopt() type command line options
    instead of positional options.  This makes it possible to
    pass options in any order, leave at their default options that
    used to be in the middle of the sequence, and offer different
    subsets of options for different programs.  Also allows for
    special internal-use options, like -D passed by dtest to let
    examples change their behavior when run under dtest to produce
    only predictable output.

*   Split old libutil functionality into two modules, one holding
    all the "print data" functions, and another holding all the
    command line parsing stuff.  This makes it easier for newbies
    to ignore the command line stuff, treating it like a black box.
    The wish to understand the "print data" routines is much more
    common, so the two needed to be disentangled.

*   Renamed examples' libutil to libexcommon.

*   Removed `connect_to_db()` `libutil` function.  It combined command
    line parsing, which users don't care about, with database
    connection establishment, which they do care about.  Now the
    examples just call out to libexcommon to parse the command
    line, and use the values they get back to explicitly make the
    connection, so it isn't hidden.

*   Removed cchar and uint typedefs.

*   Redesigned dbinfo example's output to be easier to read.

*   Fixed an output formatting bug created in 2.3.0 that caused the
    tabular output from examples to not line up.

*   Renamed examples/tquery.cpp to tquery1.cpp.  Created tquery2.cpp
    to demonstrate passing parameters via a SQLQueryParametrs object
    instead of discretely.  Created tquery3.cpp for testing unquoted
    template parameters, such as integers.

*   Renamed fieldinf1.cpp example to fieldinf.cpp, and simplified
    its output so it can be part of the dtest sequence.

*   Renamed examples/xaction.cpp to transaction.cpp.  It created too
    much cognotive dissonance whenever thinking about both it and
    lib/transaction.cpp.

*   Added examples/deadlock.cpp, to test handling of exceptions due
    to server-side transaction deadlock detection.  Also added
    code to resetdb to create a table needed to test this.
    Initial version created by Jim Wallace to test the value of
    all his BadQuery exception work, with reworking by me.

*   Greatly expanded dtest suite.  Primary change is that we now
    have a handful of unit tests, where in v2.3.2 we only tested
    a subset of the examples.  Still very low coverage ratio,
    but it's a big improvement.

*   Optimized #includes, especially in `lib/*.h` to reduce
    dependencies and thus compile time when one of these changes.

*   Fixed a typo in RPM filename generation that prevented -devel
    RPM from recognizing that the corresponding MySQL++ library
    RPM was installed.

*   RPM spec file improvements by Remi Collet.

*   Renamed `NO_LONG_LONGS` to `MYSQLPP_NO_LONG_LONGS` to avoid a risk
    of collision in the global macro namespace.
    
*   First cut at Xcode2 project support.  Testing needed!

*   Debug build of library on VC++ and Xcode have a `_d` suffix now
    so you can have both versions of the library installed without
    conflict.

*   Moved the VC++ 2003 project files into a new vs2003 subdirectory
    because there are so many of them.  Also created vs2005
    subdirectory for VC++ 2005 and 2008 compatible project files.
    2005 makes an even bigger mess of the directory containing
    the .sln file, so the incentive is bigger.  Plus, we have to
    disable several things to get VC++ 2003 to build MySQL++ now,
    so we need a special 2005+ version of the project files for a
    complete build, if the user has one of the newer compilers.

*   ...plus dozens of small bug fixes and internal enhancements,
    many documentation improvements, and expansion of support for
    newer operating systems and compilers.


## 2.3.2, 2007.07.11

**NOTE:** Because this is the last release in the 2.x line, and because
I have no plans to make another formal 2.x release, there is now a
[branch][m232] with minimal changes to allow this version of the
software to build on modern systems.

[m232]: https://tangentsoft.com/mysqlpp/timeline?r=v2.3.2-modern

*   Previous release's `const_string` change caused more problems
    than it fixed.  This release contains the real fix. :)

*   New `Connection::set_option()` handling deals with the multi
    statements option correctly again.  examples/multiquery now
    runs again as a result.

*   Added new unit testing script, called dtest.  See the
    HACKERS file for details.  (This tool caught the previous
    two problems!)

*   Squished a GCC pedantic warning.  Thanks for the patch go to
    Andrew Sayers.


## 2.3.1, 2007.07.10 (r1659) The "After the Fireworks" release

*   `const_string` objects now keep a copy of their data, not
    just a pointer to it.  This is less efficient, but necessary
    to allow SSQLS to work with BLOBs.  Without this, we were
    seeing segfaults due to accessing freed memory pointed to
    by the const_string, because the underlying object went
    out of scope.

*   Fixed many more potential embedded null handling problems
    in manip.h.

*   MySQL++ can now optionally reference MySQL C API headers as
    being in a mysql subdirectory, a common thing on *ix systems,
    by defining MYSQLPP_MYSQL_HEADERS_BURIED before #including
    mysql++.h.

*   Restored `ColData_Tmpl<T>::get_string()`, removed in v2.3.0,
    along with warnings in the docs saying why you don't want
    to use it, and what your alternatives are.

*   VC++ and MinGW builds now define the `HAVE_MYSQL_SSL_SET`
    macro, which lets you use the C API's SSL features.
    This assumes your C API library does actually have these
    features enabled, which is the case with the official binary
    releases on Windows.  (Builds on *ix systems continue to
    test for these features at configure time.)

*   Fixed simple examples-only Makefile generation, for RPMs.


## 2.3.0, 2007.07.02

*   Added `Query::for_each()` and `Query::store_if()` methods
    proposed by Joel Fielder, and added examples for each.

*   It's now possible to store BLOB data in an SSQLS.  It's not
    foolproof, so added a section to the user manual (5.9) to
    document the method.  Also, changed examples/cgi_jpeg to use
    this new mechanism, instead of the ugly "raw row data" method
    it used to use.

*   Revamped `Connection::set_option()` handling.  These options
    used to be queued up, and applied only just before actually
    establishing the connection.  This made error reporting less
    helpful because the diagnostic was separated from the cause.
    Plus, the error messages were misleading to begin with.  Now,
    set_option() takes effect immediately if the connection is not
    yet up (excepting one special option that can actually be set
    after the connection is up) and issues better diagnostics when
    it detects errors.

*   Connection::connect() used to set a few options in such a
    way that the user couldn't override them.  Now it's smart enough
    to set them with the desired default values only when we see
    that the user hasn't given them other values.

*   SQLString can now be initialized from a mysqlpp::null,
    giving a "NULL" string.  This is useful for template queries.
    Patch by Michael Hanselmann.

*   resetdb error message about mixing library and header version
    numbers is now more explicit.

*   Changed BadConversion exception's "what" message text to be
    more like the other exceptions.  The inconsistency lead one
    to incorrectly copy-paste code from another exception handler,
    expecting it to behave the same way.  Now it does.

*   Added `Row::raw_size()`, as a shortcut for Row::at().size().

*   ssqls-pretty now detects when it's being run from within
    the MySQL++ distribution tree and gives a different -I flag
    to the compiler, so that it picks up the distribution headers
    instead of those that may be on the system already.

*   The quote manipulator now works for char[] correctly.
    Thanks for this patch go to Andrew Sayers.  (It's always
    worked for char*, but C++ doesn't consider that to be the
    same type, so it used the generic quote handling path,
    which doesn't do anything for char[].)

*   Fixed a build bug on older Solaris versions where the
    test for the C API library was erroneously failing, stopping
    the configuration process.

*   Simplified `mysql_shutdown()` level argument detection.
    Already had to do a version number ifdef check for the
    Windows case, so there's really no point to doing it with
    autoconf on Unixy platforms.  Moved version number check
    into lib/connection.cpp, and nuked the separate autoconf and
    Windows tests.

*   Removed dependency of `sql_types.h` on myset.h and (indirectly)
    datetime.h.  Now we only define sql_* typedef aliases for those
    MySQL++ types if the headers are included before sql_types.h.

*   Fixed a typo in one of the `storein_sequence()` template
    overloads, which is apparently rarely (or never?) used, because
    no one reported the compiler error you'd get if you tried.

*   Fixed a few more embedded null handling problems.

*   ColData used to keep two copies of all data it held.
    Now it keeps just one.

*   Fixed install.bat script to track the unified Bakefile change
    and the lack of separate debug and release builds under MinGW.

*   Yet another STLport + Query memory leak fix.

*   Squished a warning in newer GCCs having to do with identifier
    shadowing.  Patch by Jonathan Wakely.

*   Fixed a null-termination bug in Query::parse().  If you
    somehow constructed a query string without a terminating null
    character, then tried to parse it as a template query, it could
    walk off the end of the string.  Patch by Worster Chen.

*   Removed `MYSQLPP_EXPORT` tag from FieldNames and FieldTypes
    class declarations, as this can cause problems in programs
    that use vector<string> in VC++.  It has to do with multiply
    defined templates, since these classes derive from that
    template, and VC++ can't resolve the conflict without help.
    Since these classes aren't actually used outside the library,
    this shouldn't cause a problem.  Patch by Nils Woetzel.

*   Partial fix to Doxygen PDF build on RHEL4 and 5.  Needs
    hand-coaxing to complete successfully on RHEL4, and doesn't
    yet work at all on RHEL5.

*   Shortened the `no*` options to the bootstrap script, so that
    the usage message fits on a single line.

*   Added "nodoc" bootstrap script option, for disabling the
    documentation build during the dist target build.  Allows for
    building binary RPMs on CentOS 5.0, where doc building is
    currently broken.

*   Removed the updel example program.  It was kind of silly,
    and if you were to rewrite it today, you'd use for_each() anyway.

*   Lots of documentation improvements.


## 2.2.3, 2007.04.17 (r1538) The "Tax Day" release

*   Previous version left `examples/vstudio/*` out of the tarball
    by accident.

*   Improved generation of RPM temporary build directory path
    name generation.  Was using a hacked variant of the Fedora
    Packaging Guidelines' second best choice.  Now we're using
    the choice they recommend most highly, without changes.

*   Removed unnecessary resources from vstudio/wforms example.

*   Minor URL fix in refman


## 2.2.2, 2007.04.13 (r1526) The "Nervousmaking Friday the 13th" release

*   More small fixes to embedded null handling in Query.

*   Fixed a bug in single-parameter template query handling.

*   Added tquery example, to demonstrate proper use of template
    queries.  Previously, resetdb was the only exemplar, and
    it wasn't really suited for that.  This example also tests
    the previous item.

*   Added examples/vstudio/mfc, allowing us to improve the way
    we demonstrate Unicode handling.  Old way wasn't realistic.
    On *ix, people will depend on the terminal code to handle
    UTF-8.  On Windows, users are almost certain to be writing
    a GUI program, which requires different Unicode handling
    than the old examples showed.

*   Removed explicit Unicode conversion stuff from command line
    examples, and reworked the Unicode chapter in the user
    manual.
    
*   Added examples/vstudio/wforms to show integration with
    C++/CLI and Windows Forms.  Documented this in README.vc.

*   Rewrote `load_file` and `cgi_image` examples to be more
    useful, renaming them to load_jpeg and cgi_jpeg along
    the way.  Also, resetdb now creates a second table in the
    sample database for these two examples' use.  Also, added
    examples/logo.jpg to the distribution as sample data for
    these examples.

*   Limited the ostream base class casting stuff in Query to
    VC++ 2003, which is the only platform that really needed it.
    VC++ 2005 emits a warning with that hack in place, and on
    other platforms it's just replicating work that the compiler
    does already.

*   Added library version information to main library target
    so that systems that version shared libraries work as
    expected.  Thanks for this patch go to Jack Eidsness.

*   Merged much of the diffs between Remi Collet's RPM spec file
    into the official one.

*   Reorganized the doc subdir a bit.  Generated HTML is now all
    under doc/html instead of scattered under other subdirs,
    and renamed doc/README.mysql++ to doc/README.manuals.

*   Improvements to top-level manual building make targets:
    manuals now only rebuild at need, it's easier to request
    a rebuild of all manuals, and we force a rebuild attempt
    before building the distribution tarball so we don't ship
    outdated manuals.

*   Added ability to run examples under gdb using exrun,
    using same mechanism as we currently have for valgrind.
    Thanks for this patch go to Michael Hanselmann.

*   Added "Important Underlying C API Limitations" chapter to the
    user manual, to cover problems we keep seeing on the
    mailing list that are the result of ignorance of the way
    libmysqlclient behaves, not bugs MySQL++ is really in a
    position to fix.


## 2.2.1, 2007.02.28

*   Fixed the new localtime() alternative selection code
    for VS2003 and various uses of STLport.

*   No longer inserting a null character into the query stream
    on calling one of the preview() functions.  This was harmless
    in v2.1, which used C strings more extensively, but began
    causing problems in v2.2 due to its wider use of C++ strings.

*   Fixed a bug in the Connection copy ctor where it didn't
    completely initialize the object.

*   Optimized `Query::preview_char()` a bit.  Patch by Jonathan
    Wakely.

*   Reordered directory list used by autconf when locating the
    MySQL C API library.  The list is now ordered with the
    most likely locations for the library first, so we're less
    distracted by incorrect libraries.  This fixes a specific
    build error under RHEL4 with recent versions of MySQL 5.0.


## 2.2.0, 2007.01.23

*   `ColData`, `const_string`, and `SQLString` can now be constructed
    with an explicit length parameter.  Furthermore, Query
    class's `execute()`, `store()` and `use()` call chains terminate
    in a version taking an explicit length parameter, instead
    of one taking a simple C string.  Together, this means
    that it's now easier to handle data from the SQL server
    containing nulls.  The library is almost certainly not yet
    capable of handling embedded nulls in all cases, but this
    is a big first step towards that.

*   Can now construct a DateTime object from a `time_t`, and
    convert a DateTime back to a time_t.  Patch by Korolyov Ilya.

*   Changed the way we're handling exported functions in the
    Windows DLL case so that it works more reliably under MinGW.

*   Added proper copy semantics to Connection, so that you get a
    new connection with the same parameters, not just a bitwise
    copy of the object.

*   Using an explicitly thread-safe variant of localtime() for
    time conversions where one is available.

*   Removed ListInsert template from myset.h.  This wasn't used
    within the library, and was never documented, so I'm betting
    that no one actually uses it.

*   Result::copy() was not copying the exception flag in
    all cases.  Fix by Steven Van Ingelgem.

*   Added exrun shell script and exrun.bat files to distribution,
    to avoid linkage errors when running the examples while
    you still have an older version of MySQL++ installed.

*   Renamed `MYSQLPP_LIB_VERSION` to `MYSQLPP_HEADER_VERSION`, as
    what it really encodes is the version number in the `mysql++.h`
    file you're using, not the actual library version number.

*   Added `mysqlpp::get_library_version()`, which returns the
    library version number at build time.  Between this and
    the header version constant, you can check that you're not
    mixing MySQL++ header and library versions.

*   resetdb example uses these new version number affordances to
    double-check that you're not mixing libraries and headers
    from different versions.  This happens easily unless you
    take care of it (such as by using exrun) when you have one
    version of MySQL++ installed and you're trying to build and
    test a new version without blowing away the old one first
    or overwriting it.

*   No longer using recursive Makefiles on Unixy platforms
    or split lib + examples project files on VC++.  Everything is
    handled by a single top-level Makefile or project file, which
    is simpler for the end user, and makes better dependency
    management possible.

*   When looking for the MySQL C library on systems using
    autoconf, looking in .../lib64 wherever we are also looking
    in .../lib.

*   RPM build process no longer depends on Bakefile.  It means
    you have to build the examples when building an RPM even
    though they're never used within the RPM, but it's a better
    tradeoff in my opinion.

*   Updated include and library paths on Windows to reflect
    changes in the most recent MySQL installers.

*   Merged lib/defs.h and lib/platform.h into new file,
    lib/common.h.  Just cleans up the library internals.

*   Fixed build errors on Windows due to recent changes in MySQL.

*   Fixed a few memory leaks and double-deletes in Query class.

*   Fixed compatibility with STLPort's string implementation.
    Patch by dengxy at cse.buaa.edu.cn.

*   Fixed a compatibility problem between Set<> template and
    SSQLS.  Patch by Korolyov Ilya.

*   Fixed build bug in SQLQueryParms due to a character
    signedness issue on PowerPC with GCC.  Patch by Michael
    Hanselmann.

*   ~Transaction() can no longer throw exceptions.  It'll just
    quietly eat them, to avoid program termination.  Fix
    suggested by Alex Burton.

*   Fixed thread safety testing in autoconf case, accidentally
    broken during v2.1.0 development cycle.

*   Using Doxygen 1.5.1 to generate documentation.


## 2.1.1, 2006.04.04

*   MinGW and Cygwin will now build and link to mysqlpp DLLs.

*   Fixed bug in Query, causing it to initialize the "throw
    exceptions" flag incorrectly.  Thanks for this patch go to
    Joel Fielder.

*   Added -v flag for custom.pl script, which turns off the
    multiply-defined static variable fix.  Needed for VS 2003,
    which doesn't support variadic macros.  Also, added
    a diagnostic to detect the need for the -v flag, and
    suppressed the test for this feature in examples/util.cpp.


## 2.1.0, 2006.03.24

*   Converted automake and makemake files to their equivalents in
    Bakefile format.

*   Added the Transaction class, which makes it easy to use
    transaction sets in MySQL++.

*   Added xaction example to test new Transaction class.

*   Resetdb example now creates its example table using the
    InnoDB storage engine, in order to test the new transaction
    support.  Resetdb also declares the table as using UTF-8
    text; this doesn't change anything, but it does correctly
    document what we're doing.

*   Added `sql_types.h` header, containing C++ typedefs
    corresponding to each MySQL column type.  Using those new
    types in the type_info module, and in the SSQLS examples.

*   Replaced the way we were handling the template query
    version of Query member functions, to allow an arbitrary
    number of template query parameters.  By default, we
    now support 25 parameters, up from the old limit of 12.
    It's now possible to change just one number, run a script,
    and have a new limit.

*   Connection class does a better job of returning error
    messages if you call certain member functions that depend
    on a connection to the server before the connection is
    established.

*   Updated libmysqlclient.def for newer versions of MySQL.  (Fixes
    build errors having to do with mysql_more_results() and
    mysql_next_result().

*   Replaced final use of strcpy() with strncpy().

*   custom.pl now runs without complaint in strict mode, with
    warnings turned on.  Thanks for this patch go to "Waba".

*   Fixed a bug in custom.pl where incorrect code would be
    generated for some SSQLS set() methods.  Thanks for this
    patch go to "Waba".

*   SSQLS structures now support long and unsigned long fields.
    Thanks for this patch go to "Waba".

*   It's now possible to put SSQLS definitions in a header
    file used by multiple modules in a program without
    getting multiple static member definition errors.  See the
    documentation for details.  Thanks for this patch go to
    Viktor Stark.

*   Moved the definition of the 'stock' SSQLS out of the
    custom*.cpp example files and into a new stock.h file.
    Also, #including that file in the util module to test out
    the new SSQLS multiple static definition fix.

*   Using all of the digits of precision guaranteed by the
    IEEE 754 spec when stringizing floating point numbers
    to build queries.  Previously, we would use the platform
    default, which can be as few as 6 digits.

*   Removed lib/compare.h.  Not used within the library, never
    documented, and nobody seems to want to defend it.


## 2.0.7, 2005.11.23

*   Added explicit mysqlpp namespace qualifiers to generated code in
    custom*.h so you can use SSQLS in places where it doesn't make
    sense to say "using namespace mysqlpp" before the declaration.
    Also updated some of the examples to not have this "using"
    declaration to make it clear to users that it isn't needed, if you
    want to use explicit namespace qualifiers as well.  Thanks for
    this patch to Chris Frey.

*   Removed an apparently useless unlock() call from ResUse; there is
    no nearby lock() call, so if this unlock() is in fact necessary,
    it shouldn't be here anyway, because the two calls should be
    nearby each other.  Thanks for this patch to Chris Frey.

*   Fixed Query ostream initialization bug affecting SunPro CC (at
    least).  While this bug violates the Standard, it doesn't affect
    many real compilers because they don't enforce this rule.  Fixed
    by Chris Frey.

*   Previously, we only used the C99 style "long long" support when
    building under GNU CC.  This is now the default.  This should
    allow the code to work under SunPro CC.

*   Added another dynamic cast needed for proper Query ostream
    subclass overloading under VC++.  (7.1 at least...)

*   Detecting whether MySQL is built with SSL support on platforms
    using autotools.  Needed on some old Sun systems, for instance.
    Thanks for this patch to Ovidiu Bivolaru.

*   Fixed a potential memory bug in ColData's conversion to SQL null.

*   Many minor packaging tweaks.  (README clarifications, file
    permission fixes, better adherence to GNU packaging standards,
    etc.)

    
## 2.0.6, 2005.09.28

*   Fixed makemake.bat so it works on cmd.exe, not just 4NT.

*   Documentation fixes.


## 2.0.5, 2005.09.13

*   Visual C++ build now requires GNU make.  It is tested to work
    with either the Cygwin or the MinGW versions.  The previous
    version of MySQL++ used nmake.  This change enabled the
    following features:

      o Debug and Release versions are both built into
        separate subdirectories.

      o Dependency tracking for release version works
        correctly now.  (Previously dependencies worked
        only for debug version.)

      o 'make clean' removes release version binaries
        in addition to debug versions.

*   MinGW makemake support updated to support new release/debug
    subdirectory system.  This is probationary support, since
    this code currently can't be built as a DLL.  As a result,
    it is no more useful than the Cygwin version, for licensing
    reasons.

*   Several fixes to allow building on Solaris 8.  These fixes may
    also help on other SVR4-derived systems.

*   Removed Borland C++ makemake support, because this version
    of the library does not work completely, and there seems
    to be almost no user interest in fixing it.
    
*   Clarified "Handling SQL Nulls" section of user manual's
    Tutorial chapter.


## 2.0.4, 2005.08.29

*   Made `mysql_shutdown()` second parameter autoconf check less
    sensitive to compiler pedantry.

*   VC++ library Makefile is now smart enough to re-create the
    import library, if it is deleted while leaving the DLL alone.

*   Added libmysqlclient.def to tarball.

*   Reworked most of the top-level `README*` files.

*   Renamed LGPL file to LICENSE.


## 2.0.3, 2005.08.25

*   Visual C++ makemake system updated to build both debug and
    release versions of library DLL.

*   Fixed bug in simple1 example that caused crashes on Windows.

*   Doing UTF-8 to ANSI text translation in simple examples now.

*   Previous two releases built libmysqlpp with wrong soname on
    autotools-based systems.  Fixed.


## 2.0.2, 2005.08.18

*   Fixes to makemake system for cmd.exe.

*   Fixed the case where the system's C++ library includes an slist
    implementation in namespace std.


## 2.0.1, 2005.08.17

*   Added new simple1 example, showing how to retrieve just one
    column from a table.  Old simple1 is now called simple2, and
    simple2 is likewise shifted to simple3.

*   Added custom6 example, showing how to do the same thing with
    SSQLS.

*   Updated user manual to cover new examples.

*   Was accidentally shipping Subversion crap with tarball.  Fixed.


## 2.0.0, 2005.08.16 (r1031) The "Excess Hair Removal" release

**WARNING:** THIS IS NOT A DROP-IN REPLACEMENT FOR MySQL++ v1.7!

At minimum, you will have to recompile your program against this
library.  You may also have to make code changes.  Please see the
"[Incompatible Library Changes][ilc]" chapter of the user manual for a
guide to migrating your code to this new version.

[ilc]: https://tangentsoft.com/mysqlpp/doc/html/userman/breakages.html

*   The library's shared object file name (soname) scheme has
    changed.  (This mainly affects POSIX systems.)
    
    The soname for the last 1.7.x releases of MySQL++ was
    libmysqlpp.so.4, meaning the fourth version of the library's
    application binary interface (ABI).  (The first ABI version
    in this scheme was that provided by 1.7.9.)  MySQL++
    2.0.0's soname is libmysqlpp.so.2.0.0.  Since the dynamic
    linker setup on some systems will create a symlink to
    that file called libmysqlpp.so.2, it's possible that this
    library could be confused with that for MySQL++ 1.7.19
    through .21, which also used this number.  Do not install
    this library on a system which still has binaries linked
    against that version of the library!

    The new scheme is {ABI}.{feature}.{bug fix}.  That is,
    the first number changes whenever we break the library's
    binary interface; the second changes when adding features
    that do not break the ABI; and the last changes when the
    release contains only internal bug fixes.  This means
    that we will probably end up with MySQL++ 3.0 and 4.0 at
    some point, so there will be further soname conflicts.
    Hopefully we can put these ABI changes off long enough
    to avoid any real problems.

*   autoconf now installs headers into $prefix/include/mysql++,
    instead of $prefix/include.  If you were using the
    --includedir configure script option to get this behavior
    before, you no longer need it.

*   Linux binary RPMs will henceforth include only the
    libmysqlpp.so.X.Y.Z file, and create any short names
    required, to allow multiple versions to be installed at
    once.  Currently, you cannot install two MySQL++ library
    RPMs at once, because they both have /usr/lib/libmysqlpp.so.X,
    for instance.

*   Replaced the Visual C++ and Borland C++ project files with
    a new "makemake" system, which creates Makefiles specific
    to a particular toolchain.  This new mechanism also supports
    MinGW and generic GCC-on-*ix.  This was done partly to reduce
    the number of places we have to change when changing the
    file names in MySQL++ or adding new ones, and partly so we're
    not tied to one particular version of each of these tools.

*   VC++ Makefiles create a DLL version of the library only
    now, so there's no excuse for LGPL violations now.
    This same mechanism should make DLL builds under other
    Windows compilers easy.

*   Added `Connection::enable_ssl()`, which enables encrypted
    connections to the database server using SSL.

*   `Connection::create_db()` and `drop_db()` now return true on
    success, not false.

*   `Connection::create_db()` and `drop_db()` use Query::exec()
    now, for efficiency, rather than Query::execute().

*   Removed `Connection::infoo()`.  Apparently just there to
    save you from a typo when calling the info() method, since
    it was a mere alias.

*   Renamed `Connection::real_connect()` to `connect()`, gave
    several more of its parameters defaults, and removed old
    connect() function.  Then changed user manual and examples
    to use new APIs.

*   Replaced `Connection::read_option()` with new `set_option()`
    mechanism.  The name change matches the method's purpose
    better.  Functional changes are that it returns true on
    success instead of 0, it supports a broader set of options
    than read_option() did, and it enforces the correct option
    argument type.

*   You can now call `Connection::set_option()` before the
    connection is established, which will simply queue the option
    request up until the connection comes up.  If you use this
    feature, you should use exceptions, because that's the only
    way an option setting failure can be signalled in this case.

*   Removed query-building functions (`exec*()`, `store*()`,
    `use()`) from class `Connection`, and moved all the implementation
    code to `class Query`.  `Query` no longer delegates the final
    step of sending the query to the database server to
    `Connection()`.

*   Added `Connection::enable_ssl()`, for turning on SSL support on
    a connection.

*   Extracted exception disabling mechanism out of the many
    classes that had the feature into a new OptionalExceptions
    base class, which all classes having this feature now
    derive from.  Also, removed all per-method exception
    handling flags.  Finally, added NoExceptions class.  With
    all of these changes, there is now a common way to disable
    exceptions with fine granularity on all objects that
    support the feature.

*   All custom MySQL++ exceptions now derive from the new
    Exceptions class.  This regularizes the exception interface
    and allows you to use a single catch() block if you want.

*   The "throw exceptions" flag is passed from parent to child
    in all situations now.  (Or if not, please report it as
    a bug.) This fulfills a promise made in the v1.7.9 user
    manual, with the cost being that some programs will see
    new exceptions thrown that they're not expecting.

*   Added a bunch of new exception types: BadOption,
    ConnectionFailed, DBSelectionFailed, EndOfResults,
    EndOfResultSets, LockFailed, and ObjectNotInitialized.
    Some of these replace the use of BadQuery, which in v1.7.x
    was a kind of generic exception, thrown when something more
    specific wasn't available.  Beware, this means that programs
    may start crashing after recompiling them under v2.0 due to
    uncaught exceptions, if they were only trying to catch BadQuery.

    There are additional instances where the library will
    throw new exceptions.  One is when calling a method that
    forces the internals to use an out-of-bounds index on a
    vector; previously, this would just make the program
    likely to crash.  Another is that the library uses the
    BadFieldName exception -- created in v1.7.30 -- in more
    apropos situations.

*   Renamed SQLQueryNEParms to BadParamCount, to match naming
    style of other concrete exception types.

*   Extracted lock()/unlock() functions from Connection and
    Query classes into a new Lockable interface class.  Locking
    is implemented in terms of a different class hierarchy, Lock,
    which allows multiple locking strategies with a single ABI.

*   Removed ResUse::eof().  It's based on a deprecated MySQL
    C API feature, and it isn't needed anyway.

*   Removed arrow operator (->) for iterator returned by Fields,
    Result and Row containers.  It was inherently buggy, because
    a correct arrow operator must return the address of an
    object, but the underlying element access functions in these
    classes (e.g. at()) return objects by value, of necessity.
    Therefore, this operator could only return the address of
    a temporary, which cannot be safely dereferenced.

*   Returned Row subscripting to something more like the
    v1.7.9 scheme: there are two operator[] overloads, one for an
    integer (field by index) and another for const char* (field
    by name).  lookup_by_name() has been removed.  Because row[0]
    is ambiguous again, added Row::at() (by analogy with STL
    sequence containers), which always works.

*   Collapsed two of the `Row::value_list*()` overloads into
    two other similar functions using default parameters.
    This changes the API, but the removed functions aren't
    used within the library, and I doubt they are used outside,
    either.

*   Merged RowTemplate into Row.

*   Merged SQLQuery class into Query class.

*   Query is now derived from std::ostream instead of
    std::stringstream, and we manage our own internal string
    buffer.

*   Moved SQLParseElement and SQLQueryParms into their own
    module, qparms.

*   Added multiple result set handling to Query.  MySQL 4.1
    and higher allow you to give multiple SQL statements in a
    single "store" call, which requires extensions to MySQL++
    so you can iterate through the multiple result sets.  Also,
    stored procedures in MySQL 5.0 reportedly return multiple
    result sets.  Thanks for the initial patch go to Arnon Jalon;
    I reworked it quite a bit.

*   `Query::storein*()` now supports more varieties of the
    nonstandard slist comtainer.  (Singly-linked version of
    STL `std::list`.)

*   Template query mechanism and user manual had several
    mismatches.  Made manual match actual behavior, or
    made library match documented behavior, as apropriate.
    Initial patch by Jürgen MF Gleiss, with corrections and
    enhancements by Warren Young.

*   Collapsed `mysql_*` date and time base classes' methods and
    data into the subclasses.  Also, DateTime no longer derives
    from Date and Time; you could get away with that in the
    old hierarchy, but now it creates an inheritance diamond,
    and allows unsupported concepts like comparing a Time to
    a DateTime.

*   Removed "field name" form of `Row::field_list()`.  It was
    pretty much redundant -- if you have the field names, why
    do you need a list of field names?

*   ColData can convert itself to bool now.  Thanks for this
    patch go to Byrial Jensen.

*   Removed `simp_list_b` type; wasn't being used, and doesn't
    look to be useful for end-user code.

*   Several methods that used to take objects by value now
    do so by const reference, for efficiency.

*   Several variable and function renamings so that MySQL++
    isn't needlessly tied to MySQL.  Even if we never make
    the library work with other database servers, there's
    little point in tying this library to MySQL blindly.

*   Renamed all private data members of MySQL++ classes to
    have trailing underscores.

*   'private' section follows 'public' section in all classes
    now.

*   Removed mysql++.hh and sqlplus.hh backwards-compatibility
    headers.

*   Added copy ctors to Date/Time classes so that they will
    work in SSQLS under GCC 4.0.0.  Without these, the compiler
    couldn't make the conversion from raw MySQL row data.

*   Fixed a bunch of GCC 4.0 pedantic warnings: added virtual
    dtors to all base classes, calling base class ctors from leaf
    classes, etc.

*   All warnings fixed under VC++ at warning level 3.  (Mostly
    harmless signedness and integer conversion stuff.)

*   Updated LGPL license/copyright comments at the top of
    several files to use FSF's new physical address.

*   Relicensed user manual under a close variant of the Linux
    Documentation Project License, as it's designed for
    documentation, which the LGPL is not.  Permission for this
    received from Kevin Atkinson and MySQL AB.

*   Added ABI and API breakages chapter to user manual.  It
    is basically a subset of this change log, with only the
    information an end-user must know when migrating between
    versions.

*   Reworked user manual's DocBook code quite a bit after
    reading Bob Stayton's book "DocBook XSL" 3/e.  Better handling
    of stylesheets, taking advantage of some superior DocBook
    features, prettier output (especially the HTML version), etc.

*   Rewrote doc/userman/README to make it clearer how to get
    started contributing to the user manual.  It's essentially a
    "getting started with DocBook" guide now!

*   Lots of small text improvements to user and reference
    manuals.  Aside from the obvious tracking of library changes,
    made a bunch of minor style and clarity improvements.

*   Added CSS stylesheets for userman and refman to make the HTML
    versions of each a) not ugly; and b) match `tangentsoft.com`.  (Yes,
    some may say that these are incompatible goals....)

*   Standardized exception handling code in the examples that
    use it.

*   Fixed a potential memory leak due to exceptions thrown from
    ResUse.  Thanks for this patch go to Chris Frey.

*   Using new "no exceptions" feature of library in simple1
    example, so it is now truly simple.

*   simple1 example no longer depends as much on util module, so
    that all of the important code is in one place.  Makes
    learning MySQL++ a little less intimidating.

*   Added new simple2 and usequery examples, to demonstrate
    the proper way to handle a "use" query, with exceptions
    disabled, and not, respectively.  Added them to the user
    manual, in the appropriate place.

*   Refactored the "print stock table" example functions
    again, to make code using them clearer.

*   UTF-8 to UCS-2 handling in examples is now automatic on
    Windows.

*   Removed debug code from Windows Unicode output examples
    that slipped into previous release.

*   resetdb example is now clearer, and more robust in the
    face of database errors.

*   Simplified `connect_to_db()` in examples' util module.

*   Added sample autoconf macro for finding MySQL++ libraries, for
    people to use in their own autotools-based projects.

*   Lots and lots of minor cleanups not worth mentioning
    individually...


## 1.7.40, 2005.05.26

*   Multiple item form of insert() now works if you're using the
    SQLQuery class, or its derivative, Query.  Thanks to Mark
    Meredino for this patch.

*   Fixed a bug in `const_string::compare()`, in which MySQL++
    would walk off the end of the shorter of the two strings.
    All was well if the two were the same length.

*   ResUse::operator=() now fully updates the object, so it's more
    like the behavior of the full ctor.

*   All source files now contain a license and copyright statement
    somewhere within them.

*   Optimized mysql++.h a bit: it now #includes only the minimum set
    of files required, and there is now an idempotency guard.
    This improves compile times a smidge, but mainly it was
    done to clean up the generated #include file graph in the
    reference manual.  Before, it was a frightful tangle because
    we #included everything except custom*.h.

*   Constness fix in MySQL++ date/time classes to avoid compiler
    warnings with SSQLS.  Thanks to Wolfram Arnold for this patch.

*   Fixed some compiler warnings in `custom*.h`.  Thanks to Chris Frey
    for this patch.

*   Added "Submitting Patches" and "Maintaining a Private CVS
    Repository" sections to the HACKERS file.  Thanks to Chris
    Frey for the source material for these sections.  The HACKERS
    file was improved in several other ways at the same time.

*   PDF version of user manual no longer has links to the reference
    manual.  They were ugly, and they were broken anyway due to the
    way we move the PDFs after generating them.  If you want
    interlinked manuals, use the HTML version.

*   PDF version of user manual now has hard page breaks between
    chapters.

*   Removed complic1 example.  Wasn't pulling its own weight.
    Everything it is supposed to demonstrate is shown in other
    examples already.

*   Refactored `print_stock_table()` in examples/util module to be
    four functions, and made all the examples use various of
    these functions where appropriate.  Before, several of
    the examples had one-off stock table printing code because
    print_stock_table() wasn't exactly the right thing, for one
    reason or another.  One practical problem with this is that
    some of the examples missed out on the recent Unicode updates;
    now such a change affects all examples the same way.

*   Since so many of the examples rely on the util module, the user
    manual now covers it.  The simple1 example in the user manual
    didn't make much sense before, in particular, because it's
    really just a driver for the util module.

*   Added custom5 example.  It shows how to use the `equal_list()`
    functionality of SSQLS.  Thanks to Chris Frey for the original
    version of this program.  (I simplified it quite a bit after
    accepting it.)

*   New user manual now covers the `value_list()`, `equal_list()` and
    `field_list()` stuff that the old manual covered but which was
    left out in previous versions of the new manaul.  Most of the
    examples are the same, but the prose is almost completely new.
    This new section includes the custom5 example.

*   Every declaration in MySQL++ is now documented in the reference
    manual, or explicitly treated as "internal only".

*   Improved docs for MySQL++'s mechanism to map between MySQL
    server types and C++ types.  Initial doc patch by Chris Frey,
    which I greatly reworked.

*   Improved a lot of existing reference manual documentation while
    adding the new stuff.

*   Expanded greatly on the exception handling discussion in the user
    manual.

*   Added all-new "Quoting and Escaping" section to the user
    manual's Tutorial chapter.  Moved some existing comments on
    quoting and escaping around and added some new ones to other
    sections as a result.

*   Added all-new "Handling SQL Nulls" section to the user manual's
    Tutorial chapter.

*   Many improvements to the Overview section of the user manual.

*   Row::operator[] reference now explains the right and wrong way to
    use the values it returns.  This is in response to a mailing list
    post where someone was incorrectly using this feature and getting
    a bunch of dangling pointers.

*   Updated Doxyfile so 1.3.19.1 parses it without warnings.  Still
    works with versions back to 1.2.18, at least.  (These are
    the versions shipped with Fedora Core 3 and Red Hat Linux 9,
    respectively.)

*   Using a superior method to make Doxygen ignore certain sections
    of the source code.  Between this change and the fact that
    everything not so ignored is documented, Doxygen no longer
    generates any warnings.

*   Lots of code style updates.  Everything should now be consistently
    formatted.


## 1.7.35, 2005.05.05 (r601) The "Cinco de Mayo" release

*   Added a "how to use Unicode with MySQL++" chapter to the user
    manual.  (Too bad "Cinco de Mayo" doesn't have any accented
    characters.  That would be just _too_ precious.)

*   VC++ examples now use the Unicode Win32 APIs, so they can display
    Unicode data from MySQL++.

*   Added an optional conversion function to examples/util.cpp to
    handle the conversion from UTF-8 to UCS-2 on Win32.

*   Moved "brief history of MySQL++" from intro section of refman to
    intro section of userman.

*   Lots of small bits of documentation polishing.
    
*   Made some minor constness fixes.  Thanks to Erwin van Eijk
    for this patch.

*   Made some warning fixes for GCC 4.0.  Not all warnings are
    fixed, because some of the needed changes would break the ABI.
    Thanks to Chris Frey for this patch.

*   Added lib/Doxyfile to distribution.
    

## 1.7.34, 2005.04.30

*   Added a multiple-insert method for Query, which lets you insert
    a range of records from an STL container (or the whole thing,
    if you like) in a single SQL query.  This is faster, and it
    reduces coding errors due to less repetition.  Thanks to Mark
    Meredino for the patch.

*   Reference and user manual now get rebuilt automatically when
    required.  (E.g. on 'make dist', or explicitly now through 'make
    docs'.)

*   Made it easier to change the maximum number of SSQLS data members
    in generated custom-macros.h file.  It used to be hard-coded in
    several places in lib/custom.pl; now it's a variable at the top of
    the file.

*   Changed default SSQLS data member limit to 25, which is what it
    has been documented as for a long time now.  It was actually 26
    within custom.pl.

*   Fixed a regression in previous version.

*   Trimmed some fat from the distribution packages.

*   Some more small doucmentation improvements.


## 1.7.33, 2005.04.29

*   Worked around an overloaded operator lookup bug in VC++ 7.1 that
    caused SSQLS insert, replace and update queries to get mangled.
    (Symptom was that custom2 and custom3 examples didn't work right.)
    Thanks to Mark Meredino for digging up [the problem's characterization
    and its solution](https://groups.google.com/forum/#!topic/microsoft.public.vc.stl/mmjYRkTmTxU).

*   Some VC++ warning fixes.

*   Major documentation improvements:
    
      o Using DocBook for user manual and Doxygen for reference
        manual.  The former now references the latter where
        useful.

      o Split out HACKERS and CREDITS files from main README,
        and improved remaining bits of README.

      o Moved the text from the old v1.7.9 LaTeX-based
        documentation over into the new systems, and reworked
        it to more closely resemble English.

      o Added a lot of new material to documentation, and
        simplified a lot of what already existed.

      o Documentation is now being built in HTML and PDF forms.

*   ebuild file updated to take advantage of recent configure script
    features.  Thanks to Chris Frey for this patch.



## 1.7.32, 2005.03.10

*   Example building may now be skipped with --disable-examples
    configure script flag.

*   Changed stock items added in resetdb.  One is now UTF-8 encoded,
    to show that basic use of Unicode with MySQL++ is easy, yet not
    foolproof.  (See formatting of table on systems where cout isn't
    UTF-8 aware!)  Other stock items now follow a theme, for your
    amusement.  :)

*   custom3 example now changes UTF-8 item's name to the 7-bit ASCII
    equivalent.  Previously, this example would fix a spelling error
    in the table.

*   resetdb example now says 'why' when it is unable to create the
    sample database.

*   Small formatting change to `print_stock_table()`, used by several
    examples.

*   Was issuing a VC++-specific warning-disable pragma when built by
    any Windows compiler.  Fixed.
    

## 1.7.31, 2005.03.05 (r462) The "Inevitable Point-one Followup" release

*   Check for threads support must now be explicitly requested via
    configure script's new --enable-thread-check flag.

*   Fix for contacting MySQL server on a nonstandard port number.
    Thanks to Chris Frey for this patch.

*   Example programs using standard command line format now accept a
    fourth optional parameter, a port number for the server.  Thanks
    to Chris Frey for this patch.

*   One more g++ 3.4 pedantic warning fix by Chris Frey.

*   Exception handling in resetdb is no longer nested, because you'd
    get a segfault on some systems when an exception was thrown from
    one of the inner try blocks.

*   Improvements to Connection class's handling of locking mechanism.
    Concept based on patches by Rongjun Mu.

*   Implemented the declared-but-never-defined Query::lock().  Thanks
    to Rongjun Mu for this patch.
    
*   Cleaned up some unclear if/else blocks in connection.cpp by
    adding explicit braces, correct indenting and putting normal
    code path in the if side instead of the else.


## 1.7.30, 2005.02.28 (r443) The "Power of Round Numbers" release

*   bootstrap script now accepts a 'pedantic' argument, which sets a
    bunch of CFLAGS that make g++ very picky about the code it
    accepts without warnings.
    
*   Fixed a bunch of things that generated warnings with g++ in
    pedantic mode. Only two warnings remain, having to do with
    floating point comparisons.  (See Wishlist for plans on how to
    deal with these.)  Thanks to Chris Frey for this patch.

*   Split long tests out of configure.in into M4 files in new config
    subdir.  This makes configure.in easier to read.

*   Added preliminary thread support.  Currently, this just means that
    we detect the required compiler and linker thread flags, and link
    against the proper thread-safe libraries.  THERE MAY BE
    UN-THREAD-SAFE CODE IN MYSQL++ STILL!

*   Standard C++ exceptions are the default now.  Old pre-Standard
    exception stuff removed.

*   `Row::lookup_by_name()` will throw the new BadFieldName exception if
    you pass a bad field name.  Thanks for this patch to Chris Frey.

*   Row::operator[] will throw a Standard C++ out of bounds exception
    by way of std::vector::at() if you pass it a bad index.  Thanks
    for this patch to Chris Frey.

*   Setting `Connection::is_connected` flag to false on close().
    Previously, is_connected() would continue to return true after
    close() was called.

*   All number-to-string conversion ctors in SQLString class now use
    ostringstream to do the conversion.  Previously, we used
    snprintf(), which isn't available on all systems.  Also, we used a
    C99 format specifier for the "long long" conversion, which is also
    not available on all systems.  This new ostringstream code should
    be platform-independent, finally.


## 1.7.28, 2005.02.04

*  ` --with-mysql*` flags to configure script now try the given
    directory explicitly, and only if that fails do they try
    variations, like tacking '/lib' and such onto it to try and find
    the MySQL includes and libraries.  Thanks to Matthew Walton for
    the patch.

*   Finally removed `sql_quote.h`'s dependence on `custom.h` by moving
    the one definition it needed from `custom.h` to `deps.h`.  This will
    help portability to compilers that can't handle the SSQLS macros
    by making that part of the library truly optional.


## 1.7.27, 2005.01.12

*   configure check for libmysqlclient now halts configuration if the
    library isn't found.  Previously, it would just be flagged as
    missing, and MySQL++ would fail to build.

*   Added `sql_string.cpp` to VC++ and BCBuilder project files.

*   Removed Totte Karlsson's 'populate' example, which never made it
    into the distribution anyway.

*   Removed last vestiges of 'dummy.cpp'.

*   Renamed `*.cc` to `*.cpp` in BCBuilder project files.

*   Worked around a BCBuilder C++ syntax processing bug in row.h.


## 1.7.26, 2004.12.17

*   Moved all of the SQLString definitions out of the header and into
    a new .cpp file, reformatted it all, and made the integer
    conversion functions use snprintf() or _snprintf() instead of
    sprintf().  Also, widened some of the buffers for 64-bit systems.

*   Using quoted #include form for internal library headers, to avoid
    some problems with file name clashes.  (The headers should still
    be installed in their own separate directory for best results,
    however.)  Thanks to Chris Frey and Evan Wies for the patch and
    the discussion that lead to it.

*   Removed unnecessary semicolons on namespace block closures.
    Thanks to Evan Wies for this patch.

*   Fixed namespace handling in the legacy headers mysql++.hh and
    sqlplus.hh.  Thanks to Chris Frey for this patch.

*   #including iostream instead of ostream in lib/null.h for
    broader C++ compatibility.  (This may allow MySQL++ to work on GCC
    2.95.2 again, but this is unconfirmed.)

*   Detecting proper `mysql_shutdown()` argument handling automatically
    in platform.h for the Windows compiler case instead of making the
    user edit the file.  Thanks to Evan Wies for this patch.
    
*   Fixed examples/Makefile.simple to use new `*.cpp` file naming.

*   Fix to Gentoo ebuild file's exception configure switch handling.
    Thanks to Chris Frey for this patch.

*   Rebuilding `lib/custom*.h` intelligently now, to avoid unnecessary
    recompiles after running bootstrap script.


## 1.7.25, 2004.12.09

*   Yet more fixes to the --with-mysql-lib and --with-mysql-include
    flags.

*   Added DLLEXPORT stuff to platform.h, hopefully so that someone
    can figure out how to make VC++ make a DLL version of MySQL++.

*   Renamed `*.cc` to `*.cpp`.

*   Made 'set -> myset' change in VC++ project files.

*   Some style changes (mostly whitespace) in header files.


## 1.7.24, 2004.12.08

*   Fixed the --with-mysql-lib and --with-mysql-include flags'
    behavior, and extended their search ability to handle one other
    common case.  (Fixed by Steve Roberts)

*   Fixes to put freestanding functions in namespace mysqlpp.  (They
    weren't in the namespace, while all the class member functions
    were.)  This required bumping the ABI version number to 4.
    
*   Renamed set module to myset, to avoid conflicts with Standard C++
    Library's set.h when MySQL++ headers were installed into one of
    the standard system include directories.

*   Renamed all the idempotency guards to make them consistent in
    style and unique to MySQL++.

*   Reformatted all of `lib/*.cc`.


## 1.7.23, 2004.11.20

*   Query::reset() now empties the stored query string.  If you
    subsequently stored a longer query in the object, you'd overwrite
    the previous query, but otherwise the longer part of the previous
    one would stick out past the new query.

*   We now look to the `NO_LONG_LONGS` macro only to decide whether to
    fake 64-bit integer support using 32-bit integers.

*   64-bit integer support under Visual C++ may be working now, using
    that platform's __int64_t type.  This has not been tested.

*   Removed 64-bit integer support for Codewarrior on Mac OS 9 and
    earlier.  OS X uses GCC, so it requires no special support.

*   Added MinGW detection in platform.h.
    
*   If you pass a flag (-X) to the examples that take the standard
    parameters (resetdb, simple1, etc.), it prints a usage message.
    
*   Better error handling in resetdb example, where errors are the
    most critical.  (If that one runs without errors, the others
    probably will, too, and you have to run that one first.)

*   resetdb now reports success, rather than succeeding silently.

*   Removed the code in sample1 example that duplicated util module's
    print_stock_table(), and called that function instead.

*   Moved the preview() calls in the example programs to before the
    query execution calls, because execution modifies the query.

*   All examples that take the standard command line parameters now 
    exit when connect_to_db() fails in one of the ways that don't
    throw an exception, rather than bulling onward until the next
    MySQL database call fails because the connection isn't up.

*   dbinfo example now takes the standard command line parameters.

*   Much better output formatting in dbinfo example.

*   Calling reset() where appropriate in the various example programs.
    Before, the programs may have worked, but not for the right
    reason.  This lead some people to believe that calling reset()
    was not necessary.

*   Fixed an incorrect use of row["string"] in complic1 example.

*   Lots of code style improvements to the examples.

*   Some VC++ type warnings squished.  Some remain.
    

## 1.7.22, 2004.11.17

*   Applied patches by Zahroof Mohammed to allow it to build under GCC
    3.4.2.  Tested on MinGW and Fedora Core 3 systems.

*   Removed all the forward declarations in defs.h, and added
    forward declarations where necessary in individual header files.
    #including defs.h in fewer locations as a result.

*   Legacy headers sqlplus.hh and mysql++.hh now declare they are
    using namespace mysqlpp, to allow old code to compile against the
    new library without changes.

*   Removed `query_reset` parameter from several class Query member
    functions.  In the implementation, these parameters were always
    overridden!  No sense pretending that we pay attention to these
    parameters.  This changes the ABI version to 3.

*   `#including custom.h` in `sql_query.h` again...it's necessary on GCC
    3.4.
    
*   bootstrap script runs lib/config.pl after configure.  This is
    just a nicety for those running in 'maintainer mode'.


## 1.7.21, 2004.11.05

*   Generating a main mysql++ RPM containing just the library files
    and basic documentation, and the -devel package containing
    everything else.

*   Devel package contains examples now, along with a new Makefile
    that uses the system include and library files, rather than the
    automake-based Makefile.am we currently have which uses the files
    in the mysql++ source directory.

*   Renamed sqlplusint subdirectory in the package to lib.

*   Removed the obsolete lib/README file.

*   `lib/sql_query.h` no longer #includes custom.h, simplifying
    build-time dependencies and shortening compile times.


## 1.7.20, 2004.11.03

*   Collapsed all numbered `*.hh` headers into a single `*.h` file.  For
    example, the contents of row1.hh, row2.hh and row3.hh are now in
    row.h.

*   While doing the previous change, broke several circular
    dependencies.  (The numbered file scheme was probably partly done
    to avoid this problem.)  The practical upshot of most of these
    changes is that some functions are no longer inline.

*   Removed `define_short.hh` and everything associated with it.  The
    library now uses the short names exclusively (e.g. Row instead of
    MysqlRow).

*   Put all definitions into namespace mysqlpp.  For most programs,
    simply adding a 'using namespace mysqlpp' near the top of the
    program will suffice to convert to this version.

*   Once again, the main include file was renamed, this time to
    mysql++.h.  Hopefully this is the last renaming!

*   mysql++.hh still exists.  It emits a compiler warning that the
    file is obsolete, then it #includes mysql++.h for you.

*   sqlplus.hh is back, being a copy of the new mysql++.hh.  Both of
    these files may go away at any time.  They exist simply to help
    people transition to the new file naming scheme.

*   Renamed mysql++-windows.hh to platform.h, and added code to it to
    handle #inclusion of config.h on autotools-based systems
    intelligently.  This fixes the config.h error when building under
    Visual C++.

*   There is now only one place where conditional inclusion of
    winsock.h happens: platform.h.

*   Beautified the example programs.


## 1.7.19, 2004.10.25

*   Fixed an infinite loop in the query mechanism resulting from the
    strstream change in the previous version.  There is an overloaded
    set of str() member functions that weren't a problem when query
    objects were based on strstream.
     
*   Query mechanism had a bunch of const-incorrectness: there were
    several function parameters and functions that were const for
    the convenience of other parts of the code, but within these
    functions the constness was const_cast away!  This was evil
    and wrong; now there are fewer const promises, and only one is
    still quietly broken within the code.  (It's in the SQLQuery
    copy ctor implementation; it should be harmless.)

*   Removed operator=() in Query and SQLQuery classes.  It cannot take
    a const argument for the same reason we have to cast away const
    in the SQLQuery copy ctor.  It's tolerable to do this in the copy
    ctor, but intolerable in an operator.  Since the copy ctor is good
    enough for all code within the library and within my own code, I'm
    removing the operator.

*   Above changes required bumping the ABI to version 2.

*   Visual C++ projects now look for MySQL build files in c:\mysql,
    since that's the default install location.  (Previously, it was
    c:\program files\mysql.)


## 1.7.18, 2004.10.01

*   Changed all the strstream (and friends) stuff to stringstream type
    classes.  Let there be much rejoicing.

*   Query object now lets you use store() even when the SQL query
    cannot return a result, such as a DROP TABLE command.  This is
    useful for sending arbitrary SQL to the server.  Thanks to
    Jose Mortensen for the patch.

*   Quote fix in configure.in, thanks to David Sward.

*   Renamed `undef_short` file to `undef_short.hh`.

*   Gentoo ebuild file is actually being shipped with the tarball,
    instead of just sitting in my private CVS tree since 1.7.14 was
    current.  Ooops....


## 1.7.17, 2004.09.16

*   Reverted one of the VC++ warning fix changes from 1.7.16 that
    caused crashes on Linux.

*   Added a configure test that conditionally adds the extra 'level'
    parameter to mysql_shutdown() that was added in MySQL 4.1.3 and
    5.0.1.


## 1.7.16, 2004.09.13

*   Building VC++ version with DLL version of C runtime libraries, and
    at warning level 3 with no warnings emitted.

*   VC++ build no longer attempts to fake "long long" support.  See
    the Wishlist for further thoughts on this.


## 1.7.15, 2004.09.02

*   Renamed Configure file to common.am, to avoid file name conflict
    with configure script on case-sensitive file systems.

*   Added ebuild file and ebuild target to top-level Makefile for
    Gentoo systems.  Thanks to Chris Frey for this.

*   Small efficiency improvements to BadQuery exception handling.
    Initial idea by Chris Frey, improvements by Warren Young.


## 1.7.14, 2004.08.26

*   Builds with Visual C++ 7.1.

*   Fixed a bug in custom macro generation that caused problems with
    GCC 3.4.  (X_cus_value_list ctor definition was broken.)


## 1.7.13, 2004.08.23

*   Removed USL CC support.  (System V stock system compiler.)  Use
    GCC on these platforms instead.

*   Added examples/README, explaining how to use the examples, and
    what they all do.

*   Most of the example programs now accept command line arguments for
    host name, user name and password, like resetdb does.

*   Renamed `sinisa_ex` example to `dbinfo`.

*   Several Standard C++ syntax fixes to quash errors emitted by
    GCC 3.4 and Borland C++ Builder 6.  Thanks to Steffen Schumacher
    and Totte Karlsson for their testing and help with these.

*   Added proper #includes for BCBuilder, plus project files for same.
    Thanks to Totte Karlsson for these.


## 1.7.12, 2004.08.19

*   Many Standard C++ fixes, most from the GCC 3.4 patch by
    Rune Kleveland.

*   Added Wishlist file to distribution.

*   Fixed a problem in the bootstrap script that caused complaints
    from the autotools on some systems.

*   RPM building is working properly now.

*   Fixed the idempotency guard in datetime1.hh.


## 1.7.11, 2004.08.17

*   Renamed `mysql++`, `defs` and `define_short` files, adding `.hh` to the
    end of each.  (They're header files!)  This shouldn't impact
    library users, since these are hopefully used internal to the
    library only.

*   Removed `sqlplus.hh` file.  Use `mysql++.hh` instead.
    
*   Added mysql++.spec, extracted from contributed 1.7.9 source RPM, 
    and updated it significantly.  Also, added an 'rpm' target to
    Makefile.am to automate the process of building RPMs.

*   Added bootstrap and LGPL files to distribution tarball.

*   Added pre-1.7.10 history to this file.

*   Removed .version file.  Apparently it's something required by old
    versions of libtool.


## 1.7.10, 2004.08.16

Maintenance taken over by Warren Young.  See [the mailing list
post](http://lists.mysql.com/plusplus/3326) for rationale.

*   Applied many of the GCC 3.x patches submitted for 1.7.9 over
    the years.  This allows it to build on everything from 3.0 to
    3.3.3, at least.  Because so many patches are rolled up in one
    big jump, it's difficult to describe all the changes and where
    they came from.  Mostly they're Standard C++ fixes, as GCC
    has become more strict in the source code that it will accept.

*   MysqlRow used to overload operator[] for string types as well as
    integers so you could look up a field by its name, rather than by
    its index.  GCC 3.3 says this is illegal C++ due to ambiguities in
    resolving which overload should be used in various situations.
    operator[] is now overloaded only for one integer type, and a
    new member function lookup_by_name() was added to maintain the old
    by-field-name functionality.

*   Fixed another operator overloading problem in SSQLS macro
    generation with GCC 3.3.

*   The` _table` member of SSQLS-defined structures is now `const char*`,
    so you can assign to it from a `const char*` string.

*   Got autoconf/automake build system working with current versions
    of those tools again.  Removed the generated autotools files from
    CVS.

*   Renamed library file from libsqlplus to libmysqlpp.


## 1.7.9, 2001.05.01

*   Fixed a serious bug in Connection constructor when reading MySQL
    options

*   Improved copy constructor and some other methods in Result / ResUse

*   Many other minor improvements

*   Produced a complete manual with chapter 5 included

*   Updated documentation, including a Postscript format


## 1.7.8, 2000.11.14

*   Introduced a new, standard way of dealing with C++ exceptions.

*   MySQL++ now supports two different methods of tracing exceptions.
    One is by the fixed type (the old one) and one is standard C++ type
    by the usage of what() method. A choice of methods has to be done in
    building a library. If configure script is run with
    `-enable-exception` option , then new method will be used. If no
    option is provided, or `-disable-exception` is used, old MySQL++
    exceptions will be enforced. This innovation is a contribution of
    Mr. Ben Johnson <ben@blarg.net>

*   MySQL++ now automatically reads at connection all standard MySQL
    configuration files

*   Fixed a bug in `sql_query::parse` to enable it to parse more then 99 char's

*   Added an optional client flag in connect, which will enable usage of
    this option, e.g. for getting matched and not just affected rows.
    This change does not require any changes in existing programs.

*   Fixed some smaller bugs

*   Added better handling of NULL's. Programmers will get a NULL string in
    result set and should use `is_null()` method in `ColData` to check if
    value is NULL

*   Further improved configuration

*   Updated documentation, including a Postscript format


## 1.7.6, 2000.09.22

This release contains some C++ coherency improvements and scripts
enhacements.

*   `result_id()` is made available to programmers to fetch
    `LAST_INSERT_ID()` value

*   `Connection` ctor ambiguity resolved, thanks to marc@mit.edu

*   Improved cnnfigure for better finding out MySQL libraries and includes

*   Updated documentation, including a Postscript format


## 1.7.5, 2000.07.30

This release has mainl bug fixes and code improvements:

*   A bug in FieldNames::init has been fixed, enabling a bug free usage
    of this class with in what ever a mixture of cases that is required

*   Changed behaviour of ResUse, Result and Row classes, so that they
    could be re-used as much as necessary, without any memory leaks, nor
    with any re-initializations necessary

*   Fixed all potential leaks that could have been caused by usage of
    delete instead of delete[] after memory has been allocated with new[]

*   Deleted all unused classes and macros. This led to a reduction of
    library size to one half of the original size. This has furthermore
    brought improvements in compilation speed

*   Moved all string manipulation from system libraries to
    libmysqlclient, thus enabling uniformity of code and usage of 64 bit
    integers on all platforms, including Windows, without reverting to
    conditional compilation. This changes now requires usage of mysql
    3.23 client libraries, as mandatory

*   Changed examples to reflect above changes

*   Configuration scripts have been largely changed and further changes
    shall appear in consecutive sub-releases. This changes have been
    done and shall be done by our MySQL developer Thimble Smith
    <tim@mysql.com>

*   Changed README, TODO and text version of manual. Other versions of
    manual have not been updated

*   Fixed .version ``bug''. This is only partially fixed and version
    remains 1.7.0 due to some problems in current versions of libtool.
    This shall be finally fixed in a near future.

*   Several smaller fixes and improvements

*   Added build.sh script to point to the correct procedure of building
    of this library. Edit it to add configure options of your choice.


## 1.7, 2000.05.17

This is mainly a release dealing with bug fixes, consistency
improvements and easier configure on some platforms.

*   A bug in fetch_row() method of ResUse class has been fixed. Beside
    changes that existed in a distributed patch, some additional error
    checking has been introduced.

*   A bug in escape manipulator has been fixed that could cause an error
    if all characters had to be escaped.

*   An inconsistency in column indexing has been fixed. Before this
    version, column names in row indexing with strings, i.e.
    row[<string>] , has been case sensitive, which was inconsistent with
    MySQL server handling of column names.

*   An inconsistency in conversion from strings to integers or floats
    has been fixed. In prior version a space found in data would cause a
    BadConversion exception. This has been fixed, but 100% consistency
    with MySQL server has not been targeted, so that other non-numeric
    characters in data will still cause BadConversion exception or
    error. As this API is used in applications, users should provide
    feedback if full compatibility with MySQL server is desired, in
    which case BadConversion exception or error would be abolished in
    some of future versions.

*   A new method in ColData class has been introduced. is_null() method
    returns a boolean to denote if a column in a row is NULL.

*   Finally, as of this release, testing for NULL values is possible.

*   Those are columns with empty strings for which is_null() returns
    true.

*   Some SPARC Solaris installations had C++ exception problems with g++
    2.95.2 This was a bug that was fixed in GNU gcc, as from release
    2.95 19990728. This version was thoroughly tested and is fully
    functional on SPARC Solaris 2.6 with the above version of gcc.

*   A 'virtual destructor ' warning for Result class has been fixed

*   Several new functions for STL strings have been added. Those
    functions (see string_util.hh) add some of the functionality missing
    in existing STL libraries

*   Conversion for 64 bit integers on FreeBSD systems has been added.
    On those systems _FIX_FOR_BSD_ should be defined in CXXFLAGS prior
    to configuring.  Complete conversion to the usage of functions for
    integer conversion found in mysqlclient library is planned for one
    of the next releases.

*   A completely new, fully dynamic, dramatic and fully mutable result
    set has been designed and will be implemented in some of 2.x
    releases.

*   Several smaller fixes and improvements, including defaulting
    exceptions to true, instead of false, as of this version.

*   An up-to-date and complete Postscript version of documentation is
    included in this distribution.

*   Large chunks of this manual are changed, as well as README and TODO
    files.


## 1.6, 2000.02.03

*   This is a major release as it includes new features and major
    rewrites.

*   Automatic quoting and escaping with streams. It works automatically
    , depending on the column type. It will work with << on all ostream
    derived types. it is paricularly handy with query objects and
    strstreams. Automatic quoting and escaping on cout, cerr and clog
    stream objects is intentionally left out, as quoting / escaping on
    those stream objects is not necessary. This feature can be turned of
    by setting global boolean dont_quote_auto to true.

*   Made some major changes in code, so that now execute method should
    be used only with SSQL and template queries, while for all other
    query execution of UPDATE's, INSERT's, DELETE's, new method exec()
    should be used. It is also faster.

*   New method `get_string` is inroduced for easier handling / casting
    `ColData` into C++ strings.

*   Major rewrite of entire code, which led to it's reduction and speed
    improvement. This also led to removal of several source files.

*   Handling of binary data is introduced. No application program
    changes are required. One of new example programs demonstrates
    handling of binary data.

*   Three new example programs have been written and thoroughly tested.
    Their intention is to solve some problems addressed by MySQL users.

*   Thorough changes is Makefile system has been made

*   Better configuration scripts are written, thanks to D.Hawkins
    <dhawkins@cdrgts.com>

*   Added several bug fixes

*   Changed Manual and Changelog


## 1.5, 1999.12.01

*   Fixed bug in template queries, introduced in 1.4 (!)

*   Fixed connect bug

*   Fixed several bug in `type_info` classes

*   Added additional robustness in classes

*   Added additional methods for SQL type info

*   Changed Changelog and README


## 1.4, 1999.11.25

*   Fixed bug in store and storein methods

*   Fixed one serious memory leak

*   Fixed a very serious bug generated by gcc 2.95.xx !!

*   Added robustness in classes, so that e.g. same query and row objects
    can be re-used.  Changed `sinisa_ex` example to reflect and
    demonstrate this ability.

*   Changed Changelog and README

*   Few other bug fixes and small improvements and speed-ups


## 1.3, 1999.11.10

*   Fixed several erronous definitions

*   Further changed source to be 2.95.2 compatible

*   Expunged unused statements, especially dubious ones, like use of
    `pointer_tracker`

*   Corrected bug in example file `fieldinf1`

*   Finally fixed `mysql_init` in `Connection` constructor, which
    provided much greater stability!

*   Added read and get options, so that clients, like mysqlgui can use
    it

*   Changed Changelog and README

*   Many other bug fixes.


## 1.2, 1999.10.15

First offical release by [Sinisa Milivojevic](mailto:sinisa@mysql.com)
after Kevin Atkinson made him the offical maintainer,

*   Many manual fixes.

*   Changed README and Changelog

*   Changed source to be compilable by gcc 2.95.xx, tribute to Kevin
    Atkinson <kevinatk@home.com>

*   Added methods in Connection class which are necessary for
    fullfilling administrative functions with MySQL

*   Added many bug fixes in code pertaining to missing class
    initializers, as notified by Michael Rendell <michael@cs.mun.ca>


## 1.1, 1999.08.02

This release is the first one made by Sinisa Milivojevic.

*   Added several bug fixes

*   Fixed memory leak problems and variables overlapping problems.

*   Added automake and autoconf support by loic@ceic.com

*   Added Makefile for manual

*   Added support for cygwin

*   Added example `sinisa_ex` (let modesty prevail) which used to crash a
    lot when memory allocation, memory leak and overlap problems were
    present. Smooth running of this example proves that all those bugs
    are fixed

*   Corrected bugs in `sql_query.cc` regarding delete versus `delete[]`
    and string length in `manip.cc`

*   Changed manual

*   Changed README

*   Many other smaller things


## 1.0, 1999.06.09

All prior versions were released by Kevin Atkinson.  This release was
made by [Monty Widenius](mailto:monty@monty.pp.sci.fi)

*   Added patches from Orion Poplawski <orion@bvt.com> to support the
    UnixWare 7.0 compiler


## .64.1.1a, 1998.09.27

*   Fixed several bugs that caused my library to fail to compile with
    egcs 1.1. Hopefully it will still compile with egcs 1.0 however I
    have not been able to test it with egcs 1.0.

*   Removed some problem causing debug output in sql++pretty.


## .64.1a, 1998.08.01

*   Added an (almost) full guide to using Template Queries.

*   Fixed it so the SQLQuery will throw an exception when all the
    template parameters are not provided.

*   Proofread and speedchecked the manual (it really needed it).

*   Other minor document fixes.


## .64.0.1a, 1998.07.31

*   Reworked the Class Reference section a bit.

*   Minor document fixes

*   Added more examples for SSQLS.

*   Changed the syntax of `equal_list` for SSQLS from `equal_list(cchar
    *, Manip, cchar *)` to `(cchar *, cchar *, Manip)`.

*   Added set methods to SSQLS. These new methods do the same thing as
    there corresponding constructors.

*   Added methods for creating a `mysql_type_info` from a C++ `type_info`.


## .64.a, 1998.07.24

*   Changed the names of all the classes so they no longer have to have
    Mysql in the begging of it. However if this creates a problem you
    can define a macro to only use the old names instead.

*   The Specialized SQL Structures (formally known as Custom Mysql
    Structures) changed from mysql_ to sql_.

*   Added the option of using exceptions thoughout the API.

*   ColData (formally known as MysqlStrings) will now throw an exception
    if there is a problem in the conversion.

*   Added a null adapter.

*   Added Mutable Result Sets

*   Added a very basic runtime type identification for SQL types

*   Changed the document format from POD to LYX .

*   Am now using a modified version of Perceps to extract the class
    information directly from the code to make my life easier.

*   Added an option of defining a macro to avoid using the automatic
    conversion with binary operators.

*   Other small fixed I probully forgot to mentune.


## .63.1.a

*   Added Custom Mysql Structures.

*   Fixed the Copy constructor of class Mysql

*   Started adding code so that class Mysql lets it children now when it
    is leaving.

*   Attempted to compile it into a library but still need help. As
    default it will compile as a regular program.

*   Other small fixes.


## .62.a, 1998.05.03

*   Added Template Queries

*   Created a separate SQLQuery object that is independent of an SQL
    connection.

*   You no longer have to import the data for the test program as the
    program creates the database and tables it needs.

*   Many small bug fixes.


## .61.1.a 1998.04.28

*   Cleaned up the example code in test.cc and included it in the
    manual.

*   Added an interface layout plan to the manual.

*   Added a reverse iterator.

*   Fixed a bug with row.hh (It wasn't being included because of a
    typo).


## .61.0.a

*   Major interface changes. I warned you that the interface may change
    while it is in pre-alpha state and I wasn't kidding.

*   Created a new and Separate Query Object. You can no longer execute
    queries from the Mysql object instead you have to create a query
    object with Mysql::query() and use it to execute queries.

*   Added the comparison operators to MysqlDate, MysqlTime and
    MysqlDateTime. Fixed a few bugs in the MysqlDate... that effected
    the stream output and the conversion of them to strings.

*   Reflected the MysqlDate... changes in the manual.

*   Added a new MysqlSet object and a bunch of functions for working
    with mysql set strings.


## .60.3a, 1998.04.24

*   Changed strtoq and strtouq to strtoll and strtull for metter
    compatibility Minor Manual fix.

*   Changed makefile to make it more compatible with Solaris (Thanks
    Chris H)

*   Fixed bug in comparison functions so that they would compare in he
    right direction.

*   Added some items to the to do list be sure to have a look.

