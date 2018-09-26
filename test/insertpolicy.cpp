3.2.1, 2013.12.10 (r2756)

    o Connection copy ctor wasn't copying the "throw exceptions"
      flag.  The copy always had exceptions enabled.

    o Fixed resetdb's first output message when you run it without
      arguments.  It was giving a misleading message that could
      confuse someone.

    o Previous release tarball was missing a few text files present in
      the source repository.

    o Updated build paths for VC++ and MinGW to track a file layout
      change in Connector/C 6.1.2.


3.2.0, 2013.06.20 (r2747)

    o References to MySQL 5.1 have been replaced with 5.6, that
      being the current GA release.

    o All Windows builds now assume you're using Connector/C 6.1
      instead of having installed the MySQL server on your machine
      just to get development files.  If you are running a MySQL
      server on your development machine and don't want to install
      Connector/C just to make MySQL++ happy, see the README-*.txt
      file for your platform to get instructions on fixing the paths
      so the build system finds the development files under the MySQL
      server directory.

    o The generated VC++ 2008 project files now assume you want 64-bit
      executables.  (It is 2013 now, after all.)

      Since the VC++ 2005 project files continue to be configured for
      32-bit builds, the easiest way to get a 32-bit project in VC++
      2008+ is to import the VC++ *2005* project files, rather than
      adjust the build target settings of all 44 vc2008/*.vcproj
      files within the IDE.

      See README-Visual-C++.txt for more on this change.

    o Added Query::replacefrom(): like insertfrom() but it uses
      REPLACE statements instead of INSERT.  Patch by Adrian Cornish.

    o Added support for SQL "unsigned float" columns.  Patch by
      Leonardo Lang.

    o Added "abicheck" top-level build target, which runs the
      ISPRAS ABI checker tool (http://goo.gl/e19lD) against
      a predecided "ABI-stable" version of MySQL++, comparing it
      against the current version.  This should help avoid future
      unplanned ABI breakages, as happened between 3.0.9 and 3.1.0.

      We have chosen not to revert those changes that broke the ABI,
      but instead have chosen to accept the 3.1.0 ABI as the new
      stable ABI for the rest of the 3.x series.  Running the ABI
      checker is now part of the release process, so we should not
      change the ABI again until 4.0!

    o The Query::insert() and replace() overloads that take a pair
      of iterators now work with containers that only provide forward
      iterators.  Idea by Adrian Cornish.

    o Using libtool again on Autotools systems.  It gives us
      relocatable libraries, which is needed in some linking
      situations.  Patch by Adrian Cornish.

    o VC++ release builds now have optimization enabled.  This might
      also affect other toolchains that create separate Debug and
      Release builds, if Bakefile obeys the directive for those
      systems.  (Such as, perhaps, Xcode.)  Initial patch by Matthew
      Bradbury.

    o Fixed a crash in CommandLine mechanism when the usage message is
      called up for programs that have no "extra" parameter and
      corresponding usage appendage.  Initial patch by "Crazy Pete".

    o Query::storein() now tolerates empty result sets, due either to
      DBMS failure or passing a query that never returns results,
      such as INSERT.

    o Exposed DBDriver::result_empty() as Query::result_empty().  We
      did this primarily because it was needed as a helper for the
      previous fix, but it could also be useful more broadly.

    o Added -Wextra to the pedantic build flags.

    o Fixed the "escape" manipulator so it will work with Null<>
      wrapped types.  Patch by "Kemet".

    o ssqls2parse now depends on libmysqlpp.  (It wasn't getting
      relinked when you changed the library.)

    o The configure script's test for the existence of mysql_ssl_set()
      got broken by a change in the mysql_loc.m4 test, causing it to
      always return "no" even if the platform's MySQL C API library did
      have the function.  Therefore, the SslOption could never be set.
      Fixes this bug: stackoverflow.com/questions/7794408

    o Fixed a missing a "buried headers" ifdef wrapper for
      type_info.cpp.  Patch provided by Xavier Richez.

    o We now export the DBDriver class from the MySQL++ DLL when
      building on Windows.  Client programs can now access it directly,
      instead of being able to access only the fields and members
      exposed through Connection and Query.  Fix suggested by Xavier
      Richez.

    o MinGW builds no longer require that you manually create an import
      library for the MySQL C API DLL from a .def file we provide,
      which would inevitably get outdated.  We can now link directly to
      the DLL, and rely on the MinGW linker to figure out the imports.

    o Split the -l flags out of LDFLAGS in the userman's example
      Makefiles.  They're now in LDLIBS, which causes them to be
      put after -o $(EXE) by the default GNU make rules, where they
      should be.

    o Assorted documentation and build system improvements.

    o Assorted fixes for newer GCCs.


3.1.0, 2010.06.03 (r2670)

    o Default MySQL version now 5.1, having entered GA since 3.0.9.

    o Lots of platform updates tracked, lots of warnings from newer
      compilers squished.  Most by me, some by Adrian Cornish.

    o Added Query::insertfrom(), plus associated classes SQLStream,
      NoTransactions, and the InsertPolicy hierarchy.  Also adds
      examples/ssqls6.cpp, which shows how to use this new feature.
      Thanks for this feature go to Rick Gutleber, except for
      RowCountInsertPolicy, by me.

    o Added comparison operators to tiny_int<T>.  Thanks for this patch
      go to Edward Diener.

    o Added SQLTypeAdapter::is_null().  This lets you retrieve a
      nullable column from the DB, then re-insert a value from that
      column back into the DB via a template query without explicit
      checks for null-ness; it will do the right thing now.

    o Added -f flags to lib/*.pl header file generating scripts to
      allow overriding the default limit of 25 fields for tqueries
      and SSQLSes without having to edit the file, as in previous
      versions of MySQL++.  Also added --with-field-limit option to
      the configure script so you can give both -f options at once
      during the build process.  This latter is especially useful
      for automated binary package builders.

    o The file lib/querydef.h (generated by lib/querydef.pl) now
      defines the MYSQLPP_TQUERY_MAX_PARAMETERS macro, giving the
      maximum number of parameters the functions in the generated file
      allow for template queries.  Similarly, lib/ssqls.h (generated
      by lib/ssqls.pl) defines MYSQLPP_SSQLS_MAX_MEMBERS.

    o Added ConnectionPool::safe_grab(), which pings the grabbed
      connection before returning it.  If the ping fails, we remove
      the dead connection from the pool and try again.  This doesn't
      eliminate the possibility of returning a dead connection;
      there is no protection against race conditions.  It is merely
      a best-effort attempt.

    o Added ConnectionPool::remove(Connection*), which removes a
      connection from the pool.

    o Added ConnectionPool::exchange(), which wraps remove()
      and grab().  It works like a customer support counter in a
      store: "I'm sorry, but this Connection is defective.  Can I
      have another, please?"

    o Added a feature to Transaction, where it can send a SET
      TRANSACTION ISOLATION LEVEL query before it starts the
      transaction proper, affecting the isolation level and the scope
      of that change.

    o Added ScopedConnection class.  Initial version by Joel Fielder,
      lots of changes by me.  Using it instead of explicit safe_grab()
      and release() calls on the pool in examples/cpool.cpp.

    o Added FieldNames::operator[](size_type) overloads.  This is
      necessary to correctly overload the base class versions inherited
      from std::vector.

    o FieldNames wasn't doing field-by-name matches case insentitively,
      most likely since 3.0.0 when we changed it to derive from
      std::vector.  This caused SSQLS (at least) to be case sensitive.
      Fixed this and renamed the stock.sdate field in the SSQLS
      examples to stock.sDate to test this.

    o Added Query::replace(iter, iter), on the model of existing
      insert(iter, iter).  Thanks for this patch go to David Walthour.

    o Added Query::operator!(), overriding basic_ios version so that
      "if (!query)" does the right thing.  Fixes problem report in
      thread http://lists.mysql.com/plusplus/8594

    o Query copy ctor and assignment operator now perform deeper
      copies, so you can pass Query objects around in certain ways
      that used to cause crashes.  Thanks for this patch go to Steven
      Van Ingelgem.

    o Defined *_null variants for every sql_* typedef, wrapping
      all those earlier types in Null<>.  All example programs now use
      these types instead of explicit Null<>-wrapped types.

    o Created a C++ command line parsing mechanism for the examples
      today, and for programs coming in the future.  It uses the
      platform version of getopt() if available, substituting a
      public-domain implementation that gets built into MySQL++
      itself otherwise.  It's documented in the refman -- see
      the CommandLineBase class -- but it's in the newly created
      mysqlpp::internal namespace, which means we don't promise any
      sort of ABI or API stability for it.  You're welcome to use it
      in your own programs, but expect it to change without warning.

    o Added a few missing MYSQLPP_EXPORTs to operator<<()s for stream
      manipulators, to allow their use when MySQL++ is built as a DLL.

    o Added backticks around table and column identifiers in all
      code paths exercised by dtest.  There are some others remaining,
      particularly things like Row::field_list().  Thanks for this
      patch go to Adrian Cornish.

    o Added mysqlpp::NOW() which returns a value that, when inserted
      into a SQL query, results in a call to SQL's NOW() function.
      DateTime::now() -- added in 3.0.0 -- does the same thing,
      but this is shorter and matches the SQL syntax.

    o Calling DBDriver::disconnect() twice no longer calls
      mysql_close() twice.

    o Worked around a weakness on Windows that caused problems in code
      with a connect(), disconnect(), connect() call pattern, when
      also using a named pipe to connect to the DB.  (There never
      has been a problem with this on other platforms or with other
      IPC mechanisms.)

    o Initializing a SQLTypeAdapter with a floating-point value
      equal to infinity or NaN results in SQL null if the type
      is also nullable, or 0 otherwise.  Previously, we got an
      implementation-defined string, which could give invalid SQL,
      or could give 0, a different value from NaN, infinity, or
      SQL null.  This does collapse the notions of NaN and infinity,
      but the MySQL reference manual doesn't say how you're supposed
      to preserve these differences.

    o Removed the dependency of stadapter.h on sql_types.h, just to get
      sql_tinyint*.  #including tinyint.h instead, and using equivalent
      types to what sql_types.h defines.  This is necessary because
      sql_types.h should be one of the last things included from
      mysql++.h, so that it can define sql_* equivalents for everything
      else in MySQL++, which means all those other things need to be
      #included by that point.

    o The "find MySQL++" autoconf macro (config/mysql++.m4) now
      searches for the library in lib64 subdirectories everywhere it
      also looks in lib.  We did this for the C API finder macro way
      back in MySQL++ 2.2.0, and only neglected to do it for this
      macro because it's not used by MySQL++ itself, just provided
      as a courtesy to MySQL++ users that also use autoconf.

    o Also improved the "find C API" macro (config/mysql_loc.m4)
      so it, too, is more useful in third-party projects.

    o Moved private FieldNames::str_to_lwr() method out into its
      own module, in the new mysqlpp::internal namespace, so other
      modules can use it.  Also added an overload for this new global
      method taking std::string instead of char*.

    o userman build process can now find FOP when installed from
      the standard Ubuntu/Debian package repository.

    o No longer generating refman.pdf.  It's difficult to generate
      properly on some platforms, and isn't all that useful anyway
      compared to the HTML version, which builds properly everywhere.

    o Dropped custom.h backwards-compatibility header.  (Deprecated
      over a year ago in MySQL++ 3.0.0.)

    o Fixed a bad pointer access crash in Connection::set_option()
      triggered when the option set call fails.  Thanks for this
      patch go to Kevin Regan.

    o ReconnectOption behavior now knows about a fix in MySQL 5.1.6 and
      newer that lets you set it either before the connection is up,
      or after.  If you try to set this option when MySQL++ is built
      against earlier versions of the MySQL C API, it will refuse to
      set the option because it knows the option will be reset when
      the connection comes up.

    o No longer trying to make the C API library read from a "my"
      options file before connect.  It does that by default, and
      the file is supposed to be .my.cnf anyway.

    o Reworked the load_jpeg example code for clarity, extracting
      load_jpeg_file() routine as an implementation detail and
      making that use the C++ stringstream "slurp into string" idiom
      instead of allocating a C char buffer and copying that into
      a std::string.

    o Restored support for repeating query parameters.  Apparently
      you could say things like "stuff %0 morestuff %0" in v2.1,
      but this was broken by initial 1-parameter fixes in v2.2.
      This patch reportedly lets us have both things at once, and
      improves handling of 1-parameter template queries in general.
      Thanks for this patch go to Martin Gallwey.

    o Added examples/tquery4.cpp based on test code and feedback
      from Martin.

    o Put Comparable class in namespace mysqlpp, where it should always
      have been.  Aside from being just plain wrong, it caused a
      conflict with Boost if you used its Comparable class and hoist
      them both into the global namespace.  Thanks for this patch go
      to Michael Radzewitz.

    o Fixed all the known problems with the Xcode project files.

    o Skeletal support for SSQLS v2.  Doesn't do anything useful
      yet, it just got caught up in the need to release 3.1 as-is.

    o Various enhancements to the test suite.

    o Many, many documentation enhancements.


3.0.9, 2009.02.04 (r2442)

    o It is now possible to use Null<T> objects in const context.

    o Query was emitting thousands separators when used in some
      locales.  Thanks for the fix go to Tomaž Šolc.

    o Restored support for using Query::storein() with template
      queries.  Was mistakenly removed in the 3.0.0 development
      process.

    o GCC 4.4 build fixes.  Thanks for this go to Remi Collet.


3.0.8, 2008.11.27 (r2432) The "Thanksgiving" release

    o Fixed a VC++ build error introduced in 3.0.7 in the Sun CC fix.

    o Fixed the Makefile we generate from Makefile.simple for the
      devel RPM.  The recent Bakefile upgrade changed the output so
      our munger stopped munging.

    o Changed all instances of "CREDITS" in header comments to
      "CREDITS.txt".

    o Brought ebuild file up to date with what Gentoo has been
      using for 2.3.2.


3.0.7, 2008.11.21 (r2423)

    o Fixed bug where initting a Query with a query string and then
      appending more to it with operator <<() would start overwriting
      the beginning of the buffer.  Thanks for the fix go to Chris
      Frey.

    o Added String::empty(), to parallel std::string better.

    o Added SQLTypeAdapter ctor for Null<String>, to allow SSQLS with
      NULL BLOB columns.  Thanks for this patch go to Russell Robinson.

    o Passing on "throw exceptions" flag in Query::storein() to the
      temporary Row objects it creates.  (Should have done this
      all along.)

    o All MySQL++ classes with operator[]() and at() methods now
      throw the new BadIndex exception on out of range indices now,
      unconditionally.  Before, it was a mishmash of MySQL++-specific
      mechanisms, std::range_error, and unchecked access.

    o Fixed some bugs in load_jpeg and cgi_jpeg examples that prevented
      them from working correctly on Windows.

    o Squished some warnings and incompatibilities that caused
      difficulty with Solaris CC.

    o Several minor fixes to allow building on Cygwin again.

    o Dedicated VC++ 2008 support.  (Not just a clone of VC++ 2005
      project files in a different directory.)

    o Lots of documentation improvements.


3.0.6, 2008.08.16 (r2350)

    o Several fixes to allow thread-aware build on Solaris

    o More improvements to search for Fink version of C API library.

    o Various improvements to Windows installer (install.hta)


3.0.5, 2008.08.06 (r2332)

    o Replaced install.bat with new install.hta, which has a GUI and a
      lot of embeded logic for doing The Right Thing, which we couldn't
      do in a dumb batch file.

    o Several fixes to allow it to build on Solaris 10.

    o Fixed a bug in comparison of Null<T> to T: wouldn't always give
      the right result for one particular combination of argument
      values and order of parameters to operator <().  It wouldn't
      fail all the time, either; it depended on the way the system's
      memory allocator dealt with newly allocated RAM.  The bug was
      discovered during the Solaris 10 porting effort, but it is not
      a Solaris-specific bug.

    o Split Linux-specific material out of README-Unix.txt into
      README-Linux.txt, and created README-Solaris.txt.

    o Shipping a vc2008 directory.  Populated by bootstrap script with
      copies of vc2005 files when those are newer, with the idea being
      to update them by hand by running them through VC++2008 before
      release.

    o Several fixes to VS-only examples. They hadn't been updated to
      track several of the changes in MySQL++ v3, so they wouldn't
      build OOTB at all, crashed when you did get them building, and
      emitted a lot of warnings during the build.  Cleaned it all up.

    o Autoconf now explicitly checks whether we need to link to zlib
      to link to MySQL C API library.  It used to be required, but
      lately, MySQL has been shipping the library with zlib stuff
      embedded, causing multiply defined symbol errors on some systems.

    o configure script now looks in more locations for the MySQL C API
      library, adding default locations for Fink and Solaris.


3.0.4, 2008.07.02 (r2303)

    o Fixed a few bugs in SslOption that prevented it from actually
      working.  If you've been having SSL problems since upgrading
      to MySQL++ v3, this is why!  Thanks for these patches go to
      Petteri Kiiskinen.

    o Changed type of String's internal "size" typedefs so it'll
      build without errors on 64-bit systems where these ints are
      different sizes.

    o Many user manual improvements.


3.0.3, 2008.05.11 (r2284)

    o Fixed query well-formedness handling in Query::store() and use().
      It was quietly eating these errors, confusing them with the
      no-results case.

    o Fixed examples/cpool.cpp to build without thread support.

    o No longer hiding all stdint.h typedefs inside namespace mysqlpp.

    o Fixed mysqlpp::String comparisons of empty strings against
      nonempty strings; if the empty string was on the left hand side,
      it would succeed because it was only comparing against characters
      in the empty side, which matches everything.  (Fixes bug 11588.)


3.0.2, 2008.04.13 (r2275)

    o Increased float to string conversion precision from 7 to 9 and
      double from 16 to 17 digits.  Previous values weren't enough
      near the maximum values that each can hold.

    o Replaced Query's safe bool mechanism with an override of
      basic_ios::operator void*() to avoid a conflict between the
      two mechanisms.  As the base class version isn't virtual,
      this is arguably evil, but it should be harmless in typical use.
      Besides, testing Query in bool context usually isn't the right
      thing anyway: test the result set instead.

    o Made ConnectionPool::grab() virtual.

    o Overriding ConnectionPool::grab() and release() in
      examples/cpool.cpp to show how to do connection-in-use count
      limiting.  Also, added a few more output indicator states to
      allow one to better understand program flow.


3.0.1, 2008.03.23 (r2263)

    o String objects can now be compared to mysqlpp::null directly.

    o Added a template operator== and operator!= to String, syntactic
      sugar for the existing String::compare() methods.

    o String::compare() now returns 0 ("equal") when one of the
      strings is an uninitialized String() (no refcounted buffer)
      and the other is empty.  It used to consider any initialized
      string greater than an uninitted one.  An uninitialized String
      appears empty, though, so this was incorrect.

    o Made Connection::thread_aware(), thread_start() and thread_end()
      static methods, so they can be called before you create your
      first connection.  Ditto for DBDriver versions of these methods.

    o Calling Connection::thread_start() and thread_end() in
      examples/cpool.cpp, as appropriate.  Above changes were necessary
      to make this work sensibly.

    o Made ConnectionPool::release() virtual, so your pool subclass can
      override it.

    o Added ConnectionPool::size(), so a subclass can know the current
      number of extant connections.

    o No longer single-quoting NOW() call generated for default
      init of DateTime type when building queries from SSQLS objects
      in Query::insert(), replace() and update().  The template query
      and stream interfaces of Query treated NOW() correctly already.

    o Fixed a bug that left SSQLS::table_override_ uninitted if
      you used certain of the generated ctors or set() member
      functions instead of others used by the examples.  This could
      cause a crash any time you caused SSQLS.table() to be called,
      such as when passing the SSQLS to Query::insert().

    o Minor memset bug fixed in test/uds.cpp.  Patch by Dave Jones.


3.0.0, 2008.02.29 (r2236) The "Giant Leap Forward" release

    THIS IS NOT A DROP-IN REPLACEMENT FOR MySQL++ v2.x!

    You will have to recompile your program against this version
    of the library, and you will almost certainly have to make code
    changes as well.  Please see these sections in the user manual
    for information on migrating your code to this new version:

    http://tangentsoft.net/mysql++/doc/html/userman/breakages.html#api-3.0.0
    http://tangentsoft.net/mysql++/doc/html/userman/breakages.html#abi-3.0.0

    o Added ConnectionPool class, primarily to let multithreaded
      programs share a set of Connection objects safely in situations
      where it isn't acceptable to have a Connection per thread.

    o Created examples/cpool.cpp to demonstrate this new class.

    o Added RefCountedPointer template, which provides automatic
      memory management and data sharing.  It's not intended
      for use outside of MySQL++ itself, but it's the mechanism
      behind everything below where reference counting is mentioned.
      I created the initial version of it, but Jonathan Wakely almost
      completely rebuilt it, and Joseph Artsimovich provided helpful
      commentary and advice as well.

    o Many improvements to Specialized SQL Structures (SSQLS):

      - Renamed custom* to ssqls*.  There's still a custom.h which
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

      - As a result of previous, removed sql_create_c_order_* macros;
        they have no purpose now.

      - Removed order parameters from sql_create_complete_*, which now
        gives it the same functionality as sql_create_c_names_* so
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

      - Renamed _table static member variable in each SSQLS to table_
        and made it private.  There are now public setter and getter
        methods, table().

      - Added per-instance table name overriding via instance_table()
        setter.  table() getter returns static version if this is not
        set, so it's still a global setting by default.

    o You can now use mysqlpp::null as a template query parameter to
      get a SQL null.

    o Replaced template ColData_Tmpl<T>:

      - Used to have typedef ColData_Tmpl<std::string> MutableColData.
        It was used only once within MySQL++ itself, and was never
        documented as a class for end users.  This one use within
        the library was a crock, so we just replaced this use with
        std::string and removed the typedef.

      - This left just one use of ColData_Tmpl<T>, instantiating it
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

    o Redesigned SQLString:

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

      - Added mysqlpp::tiny_int interfaces to STA to replace the
        former char interfaces for those needing one-byte integers.

    o As a result of the ColData -> String redesign, removed
      Row::raw_*().  Before String copies were efficient, this
      was helpful in accessing BLOB data efficiently.  It was also
      required back when ColData didn't deal correctly with embedded
      null characters, but that reason is gone now, too.

    o Row::operator[](const char*) no longer unconditionally throws the
      BadFieldName exception when you ask for a field that doesn't
      exist.  It will still throw it if exceptions are enabled, but if
      not, it'll just return an empty String.  This was necessary to
      make the SSQLS subset and field order independence features work.

    o Similarly, Result::field_num() returns -1 when exceptions are
      disabled and you ask for a field that doesn't exist.

    o You can now use the OptionalExceptions mechanism to disable
      exceptions on const MySQL++ objects.

    o Redesigned query result classes:

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

      - IMPORTANT COMPATIBILITY BREAK: because we used fetch_row()
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

    o Field class used to just be a typedef for the corresponding C
      API class.  Now it's a real C++ class providing a more MySQL++
      sort of interface, plus good OO things like information hiding
      and implementation detail abstraction.  This changes several
      things about the interface.

    o Fields class was basically a specialized std::vector work-alike
      for dealing with the C API to get access to MYSQL_FIELD objects
      and present them as contained Field objects.  New Field type
      let us replace it with "typedef std::vector<Field> Fields"

    o Major improvements to the quoting and escaping mechanisms:

      - Replaced almost all of the type-specific interfaces in manip.h
        with a single version taking STA.  The compiler can convert
        almost anything to STA without losing any information we need
        for correct quoting and escaping.  This has the side benefit
        that we can now do correct quoting and escaping for more data
        types now, including plain C and C++ string types.

      - Fixed a bug in quote_double_only manipulator for String: was
        using single quotes by mistake.

      - Escaping and quoting only works in instances where MySQL++
        can tell you're building a SQL query and are using a data type
        that requires it.  This affects many things, but the one most
        likely to cause trouble is that inserting MySQL++'s quoting
        and escaping manipulators in non-Query ostreams is now a no-op.

      - Added escape_string() member functions to Query and
        SQLQueryParms::escape_string(), and removed the global function
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

    o Created DBDriver class from code previously in Connection and
      Query to almost completely wrap the low-level MySQL C API:

      - Connection creates a DBDriver object upon connection and
        passes a pointer to it down to Query objects it creates.
        In turn, they pass the pointer on to any of their children
        that need access to the C API.

      - Nothing outside DBDriver calls the C API directly now, though
        DBDriver leaks C API data structures quite a lot, so this
        feature doesn't constitute "database independence."  See the
        Wishlist for what must be done to get to that point.

    o Completely redesigned the connection option setting mechanism:

      - There's now just a single Connection::set_option() method that
        takes a pointer to the abstract Option base class, and there is
        an Option subclass for every connection option we understand.
        Thus, type errors are now caught at compile time instead of
        at run time.

      - Replaced Connection::enable_ssl() with SslOption class.

      - Enabling data compression and setting the connection timeout
        are no longer set via parameters to Connection interfaces.
        These are now set with CompressOption and ConnectTimeoutOption.

      - Similarly, removed client_flag parameters from Connection's
        ctor and connect() method and added corresponding Option
        subclasses.  There's about a dozen, so rather than list them
        here, look for similarly-named classes in lib/options.h.

    o Added Connection::count_rows() to execute "SELECT COUNT(*) FROM
      tablename" queries for you.

    o Moved Connection::affected_rows(), info() and insert_id() methods
      to class Query, as they relate to the most recently-executed
      query, not to the connection.

    o Several method name changes in Connection:

      - client_info()   -> client_version()
      - host_info()     -> ipc_info()
      - proto_info()    -> protocol_version()
      - server_info()   -> server_version()
      - stat()          -> status()

    o Removed Connection::api_version().  It does the same thing as
      client_version().

    o Lots of changes to Date, DateTime, and Time classes:

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

      - The sql_timestamp typedef now aliases DateTime instead of Time.

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

      - Added time_t conversion to Date and Time classes.  DateTime
        already had it, since it's more legitimate to convert time_t
        to DateTime, but you could already "slice" it with something
        like Time(DateTime(time(0))) so there's no point pretending
        you can't get from time_t to Date or Time.  Might as well
        legitimize it.

    o Improved tiny_int class:
        
      - Turned it into a template parameterized on the value type so
        you can get both signed and unsigned TINYINTs

      - Changed the sql_tinyint and sql_tinyint_unsigned typedefs to
        use mysqlpp::tiny_int<VT> instead of raw chars

      - Added a bool conversion ctor and operator, and typedef'd it
        to sql_bool and sql_boolean to match MySQL server behavior

    o Added many more sql_* typedefs.  We now have a typedef for every
      type the MySQL server knows about, including those it supports
      just for compatibility with other database servers.

    o Changed the sql_*int* typedefs to use integer types of the same
      size as the MySQL server.  (Run test/inttypes.cpp to test it.)

    o Added copy ctor and assignment operator to Row.

    o Row::operator[]() takes int now instead of unsigned int.
      This finally (!) makes it possible to say row[0] without the
      compiler giving an ambiguous overload error.

    o Changed all uses of row.at(0) in the examples to row[0]

    o Added operator[] to all classes that only had at().

    o Query now automatically resets itself unless the query fails
      or you're using template queries.  In either case, the contents
      of the query's internal buffers are considered precious,
      either for debugging, or future queries.  Except when using
      template queries, this means you may be able to avoid calling
      Query::reset() entirely.  It's still safe to call reset()
      as before, just unnecessary most of the time.

    o Removed reset_query parameter from all Query methods.  It was
      almost completely broken before, and above change does what
      was really wanted to begin with.

    o Query::store_next() and Result::fetch_row() no longer throw
      the EndOfResults and EndOfResultSets exceptions; these are not
      exceptional conditions!  These methods simply return false now.

    o Removed examples/usequery.cpp: there's no essential difference
      between what it did and what examples/simple3.cpp does now as
      a result of the previous change.

    o Added Query::exec(void), paralleling Query::execute(void).

    o Removed Query::preview().  The most direct replacement is str(),
      which has always done the same thing.

    o You can now insert a Query object into an ostream to get a copy
      of the built query.  This means Query::str() is only necessary
      when using template queries.

    o Removed overloads of Query::execute(), store(), and use()
      that take const char*.  It was redundant because const char*
      converts implicitly to STA, for which overloads already exist.

    o Renamed Query::def to Query::template_defaults to make its
      purpose clearer.

    o Query::error() now returns const char*, not a std::string by
      value.  There's no point in making a copy of the error string.
      The method is now const as well, as it doesn't change the
      Query object.

    o Added Query::errnum(), which just wraps Connection::errnum().

    o Added error number parameters and accessor functions to BadQuery,
      ConnectionFailed and DBSelectionFailed exceptions, to preserve
      the state of Connection::errnum() at the point of the exception,
      so you don't have to rely on this value remaining unchanged
      during the exception throw process.  All places that use these
      exceptions now include this value where possible.  Thanks for the
      initial patch go to Jim Wallace.

    o Removed Lockable mechanism from Connection and Query; it was
      conceptually flawed.  See the new user manual chapter on
      threading for advice on using MySQL++ safely without locking.
      There is mutex locking now in ConnectionPool, but that's it.

    o Connection::query() now takes an optional query string, allowing
      the returned Query object to start off with a value.  Especially
      useful when the query string is static, either because it's
      a simple query or because it's a template.  You can now build
      chains like "if (conn.query("CREATE INDEX ...").exec()) { ..."

    o Added Connection::thread_aware(), thread_end(), thread_id()
      and thread_safe().  See user manual's threading chapter for
      explanations.

    o Renamed "success" data members in Connection, Query and
      SimpleResult (neé ResNSel) to "copacetic_", making them private
      if they weren't before.  This better reflects their actual
      use, which isn't to say that there has necessarily been actual
      success, but rather that everything's okay with the object.

    o Removed success() member functions from above classes.  All can
      be tested in bool context to get the same information.

    o Replaced all operator bool()s in MySQL++ classes with safer
      alternatives.  See http://www.artima.com/cppsource/safebool.html
      Thanks to Jonathan Wakely for much helpful commentary, advice,
      and code used in these mechanisms.

    o Decoupled Connection::copacetic_ from Connection::is_connected_.
      It is now possible for the object to be copacetic without being
      connected.  However, if it tries to connect and fails, then
      it is not copacetic.  If it is copacetic and not connected, it
      means we haven't even tried to connect yet, a useful distinction.

    o Collapsed Connection's host, port, and socket_name down into
      a new combined 'server' parameter which is parsed to determine
      what kind of connection you mean.  These interfaces are still
      compatible with v2.3 and earlier up through the port parameter.
      There are differences beyond this.

    o Added TCPConnection, UnixDomainSocketConnection and
      WindowsNamedPipeConnection subclasses for Connection giving
      simpler construction and connect() method interfaces for
      instances where you know what kind of connection you want at
      compile time.

    o Changed Connection::ping() return value from int to bool.

    o Renamed NullisNull to NullIsNull -- capital I -- and similar for
      NullisZero and NullisBlank.

    o It's now a compile-time error to try to convert a MySQL++
      representation of a SQL null to any other data type, rather
      than a run-time error as in previous versions.  Removed
      BadNullConversion exception as a result.

    o Way back in v1.7.x we used the BadQuery exception for all kinds
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

    o The examples now all use getopt() type command line options
      instead of positional options.  This makes it possible to
      pass options in any order, leave at their default options that
      used to be in the middle of the sequence, and offer different
      subsets of options for different programs.  Also allows for
      special internal-use options, like -D passed by dtest to let
      examples change their behavior when run under dtest to produce
      only predictable output.

    o Split old libutil functionality into two modules, one holding
      all the "print data" functions, and another holding all the
      command line parsing stuff.  This makes it easier for newbies
      to ignore the command line stuff, treating it like a black box.
      The wish to understand the "print data" routines is much more
      common, so the two needed to be disentangled.

    o Renamed examples' libutil to libexcommon.

    o Removed connect_to_db() libutil function.  It combined command
      line parsing, which users don't care about, with database
      connection establishment, which they do care about.  Now the
      examples just call out to libexcommon to parse the command
      line, and use the values they get back to explicitly make the
      connection, so it isn't hidden.

    o Removed cchar and uint typedefs.

    o Redesigned dbinfo example's output to be easier to read.

    o Fixed an output formatting bug created in 2.3.0 that caused the
      tabular output from examples to not line up.

    o Renamed examples/tquery.cpp to tquery1.cpp.  Created tquery2.cpp
      to demonstrate passing parameters via a SQLQueryParametrs object
      instead of discretely.  Created tquery3.cpp for testing unquoted
      template parameters, such as integers.

    o Renamed fieldinf1.cpp example to fieldinf.cpp, and simplified
      its output so it can be part of the dtest sequence.

    o Renamed examples/xaction.cpp to transaction.cpp.  It created too
      much cognotive dissonance whenever thinking about both it and
      lib/transaction.cpp.

    o Added examples/deadlock.cpp, to test handling of exceptions due
      to server-side transaction deadlock detection.  Also added
      code to resetdb to create a table needed to test this.
      Initial version created by Jim Wallace to test the value of
      all his BadQuery exception work, with reworking by me.

    o Greatly expanded dtest suite.  Primary change is that we now
      have a handful of unit tests, where in v2.3.2 we only tested
      a subset of the examples.  Still very low coverage ratio,
      but it's a big improvement.

    o Optimized #includes, especially in lib/*.h to reduce
      dependencies and thus compile time when one of these changes.

    o Fixed a typo in RPM filename generation that prevented -devel
      RPM from recognizing that the corresponding MySQL++ library
      RPM was installed.

    o RPM spec file improvements by Remi Collet.

    o Renamed NO_LONG_LONGS to MYSQLPP_NO_LONG_LONGS to avoid a risk
      of collision in the global macro namespace.
    
    o First cut at Xcode2 project support.  Testing needed!

    o Debug build of library on VC++ and Xcode have a _d suffix now
      so you can have both versions of the library installed without
      conflict.

    o Moved the VC++ 2003 project files into a new vs2003 subdirectory
      because there are so many of them.  Also created vs2005
      subdirectory for VC++ 2005 and 2008 compatible project files.
      2005 makes an even bigger mess of the directory containing
      the .sln file, so the incentive is bigger.  Plus, we have to
      disable several things to get VC++ 2003 to build MySQL++ now,
      so we need a special 2005+ version of the project files for a
      complete build, if the user has one of the newer compilers.

    o ...plus dozens of small bug fixes and internal enhancements,
      many documentation improvements, and expansion of support for
      newer operating systems and compilers.


2.3.2, 2007.07.11 (r1669)

    o Previous release's const_string change caused more problems
      than it fixed.  This release contains the real fix. :)

    o New Connection::set_option() handling deals with the multi
      statements option correctly again.  examples/multiquery now
      runs again as a result.

    o Added new unit testing script, called dtest.  See the
      HACKERS file for details.  (This tool caught the previous
      two problems!)

    o Squished a GCC pedantic warning.  Thanks for the patch go to
      Andrew Sayers.


2.3.1, 2007.07.10 (r1659) The "After the Fireworks" release

    o const_string objects now keep a copy of their data, not
      just a pointer to it.  This is less efficient, but necessary
      to allow SSQLS to work with BLOBs.  Without this, we were
      seeing segfaults due to accessing freed memory pointed to
      by the const_string, because the underlying object went
      out of scope.

    o Fixed many more potential embedded null handling problems
      in manip.h.

    o MySQL++ can now optionally reference MySQL C API headers as
      being in a mysql subdirectory, a common thing on *ix systems,
      by defining MYSQLPP_MYSQL_HEADERS_BURIED before #including
      mysql++.h.

    o Restored ColData_Tmpl<T>::get_string(), removed in v2.3.0,
      along with warnings in the docs saying why you don't want
      to use it, and what your alternatives are.

    o VC++ and MinGW builds now define the HAVE_MYSQL_SSL_SET
      macro, which lets you use the C API's SSL features.
      This assumes your C API library does actually have these
      features enabled, which is the case with the official binary
      releases on Windows.  (Builds on *ix systems continue to
      test for these features at configure time.)

    o Fixed simple examples-only Makefile generation, for RPMs.


2.3.0, 2007.07.02 (r1645)

    o Added Query::for_each() and Query::store_if() methods
      proposed by Joel Fielder, and added examples for each.

    o It's now possible to store BLOB data in an SSQLS.  It's not
      foolproof, so added a section to the user manual (5.9) to
      document the method.  Also, changed examples/cgi_jpeg to use
      this new mechanism, instead of the ugly "raw row data" method
      it used to use.

    o Revamped Connection::set_option() handling.  These options
      used to be queued up, and applied only just before actually
      establishing the connection.  This made error reporting less
      helpful because the diagnostic was separated from the cause.
      Plus, the error messages were misleading to begin with.  Now,
      set_option() takes effect immediately if the connection is not
      yet up (excepting one special option that can actually be set
      after the connection is up) and issues better diagnostics when
      it detects errors.

    o Connection::connect() used to set a few options in such a
      way that the user couldn't override them.  Now it's smart enough
      to set them with the desired default values only when we see
      that the user hasn't given them other values.

    o SQLString can now be initialized from a mysqlpp::null,
      giving a "NULL" string.  This is useful for template queries.
      Patch by Michael Hanselmann.

    o resetdb error message about mixing library and header version
      numbers is now more explicit.

    o Changed BadConversion exception's "what" message text to be
      more like the other exceptions.  The inconsistency lead one
      to incorrectly copy-paste code from another exception handler,
      expecting it to behave the same way.  Now it does.

    o Added Row::raw_size(), as a shortcut for Row::at().size().

    o ssqls-pretty now detects when it's being run from within
      the MySQL++ distribution tree and gives a different -I flag
      to the compiler, so that it picks up the distribution headers
      instead of those that may be on the system already.

    o The quote manipulator now works for char[] correctly.
      Thanks for this patch go to Andrew Sayers.  (It's always
      worked for char*, but C++ doesn't consider that to be the
      same type, so it used the generic quote handling path,
      which doesn't do anything for char[].)

    o Fixed a build bug on older Solaris versions where the
      test for the C API library was erroneously failing, stopping
      the configuration process.

    o Simplified mysql_shutdown() level argument detection.
      Already had to do a version number ifdef check for the
      Windows case, so there's really no point to doing it with
      autoconf on Unixy platforms.  Moved version number check
      into lib/connection.cpp, and nuked the separate autoconf and
      Windows tests.

    o Removed dependency of sql_types.h on myset.h and (indirectly)
      datetime.h.  Now we only define sql_* typedef aliases for those
      MySQL++ types if the headers are included before sql_types.h.

    o Fixed a typo in one of the storein_sequence() template
      overloads, which is apparently rarely (or never?) used, because
      no one reported the compiler error you'd get if you tried.

    o Fixed a few more embedded null handling problems.

    o ColData used to keep two copies of all data it held.
      Now it keeps just one.

    o Fixed install.bat script to track the unified Bakefile change
      and the lack of separate debug and release builds under MinGW.

    o Yet another STLport + Query memory leak fix.

    o Squished a warning in newer GCCs having to do with identifier
      shadowing.  Patch by Jonathan Wakely.

    o Fixed a null-termination bug in Query::parse().  If you
      somehow constructed a query string without a terminating null
      character, then tried to parse it as a template query, it could
      walk off the end of the string.  Patch by Worster Chen.

    o Removed MYSQLPP_EXPORT tag from FieldNames and FieldTypes
      class declarations, as this can cause problems in programs
      that use vector<string> in VC++.  It has to do with multiply
      defined templates, since these classes derive from that
      template, and VC++ can't resolve the conflict without help.
      Since these classes aren't actually used outside the library,
      this shouldn't cause a problem.  Patch by Nils Woetzel.

    o Partial fix to Doxygen PDF build on RHEL4 and 5.  Needs
      hand-coaxing to complete successfully on RHEL4, and doesn't
      yet work at all on RHEL5.

    o Shortened the "no*" options to the bootstrap script, so that
      the usage message fits on a single line.

    o Added "nodoc" bootstrap script option, for disabling the
      documentation build during the dist target build.  Allows for
      building binary RPMs on CentOS 5.0, where doc building is
      currently broken.

    o Removed the updel example program.  It was kind of silly,
      and if you were to rewrite it today, you'd use for_each() anyway.

    o Lots of documentation improvements.


2.2.3, 2007.04.17 (r1538) The "Tax Day" release

    o Previous version left examples/vstudio/* out of the tarball
      by accident.

    o Improved generation of RPM temporary build directory path
      name generation.  Was using a hacked variant of the Fedora
      Packaging Guidelines' second best choice.  Now we're using
      the choice they recommend most highly, without changes.

    o Removed unnecessary resources from vstudio/wforms example.

    o Minor URL fix in refman


2.2.2, 2007.04.13 (r1526) The "Nervousmaking Friday the 13th" release

    o More small fixes to embedded null handling in Query.

    o Fixed a bug in single-parameter template query handling.

    o Added tquery example, to demonstrate proper use of template
      queries.  Previously, resetdb was the only exemplar, and
      it wasn't really suited for that.  This example also tests
      the previous item.

    o Added examples/vstudio/mfc, allowing us to improve the way
      we demonstrate Unicode handling.  Old way wasn't realistic.
      On *ix, people will depend on the terminal code to handle
      UTF-8.  On Windows, users are almost certain to be writing
      a GUI program, which requires different Unicode handling
      than the old examples showed.

    o Removed explicit Unicode conversion stuff from command line
      examples, and reworked the Unicode chapter in the user
      manual.
    
    o Added examples/vstudio/wforms to show integration with
      C++/CLI and Windows Forms.  Documented this in README.vc.

    o Rewrote load_file and cgi_image examples to be more
      useful, renaming them to load_jpeg and cgi_jpeg along
      the way.  Also, resetdb now creates a second table in the
      sample database for these two examples' use.  Also, added
      examples/logo.jpg to the distribution as sample data for
      these examples.

    o Limited the ostream base class casting stuff in Query to
      VC++ 2003, which is the only platform that really needed it.
      VC++ 2005 emits a warning with that hack in place, and on
      other platforms it's just replicating work that the compiler
      does already.

    o Added library version information to main library target
      so that systems that version shared libraries work as
      expected.  Thanks for this patch go to Jack Eidsness.

    o Merged much of the diffs between Remi Collet's RPM spec file
      into the official one.

    o Reorganized the doc subdir a bit.  Generated HTML is now all
      under doc/html instead of scattered under other subdirs,
      and renamed doc/README.mysql++ to doc/README.manuals.

    o Improvements to top-level manual building make targets:
      manuals now only rebuild at need, it's easier to request
      a rebuild of all manuals, and we force a rebuild attempt
      before building the distribution tarball so we don't ship
      outdated manuals.

    o Added ability to run examples under gdb using exrun,
      using same mechanism as we currently have for valgrind.
      Thanks for this patch go to Michael Hanselmann.

    o Added "Important Underlying C API Limitations" chapter to the
      user manual, to cover problems we keep seeing on the
      mailing list that are the result of ignorance of the way
      libmysqlclient behaves, not bugs MySQL++ is really in a
      position to fix.


2.2.1, 2007.02.28 (r1433)

    o Fixed the new localtime() alternative selection code
      for VS2003 and various uses of STLport.

    o No longer inserting a null character into the query stream
      on calling one of the preview() functions.  This was harmless
      in v2.1, which used C strings more extensively, but began
      causing problems in v2.2 due to its wider use of C++ strings.

    o Fixed a bug in the Connection copy ctor where it didn't
      completely initialize the object.

    o Optimized Query::preview_char() a bit.  Patch by Jonathan
      Wakely.

    o Reordered directory list used by autconf when locating the
      MySQL C API library.  The list is now ordered with the
      most likely locations for the library first, so we're less
      distracted by incorrect libraries.  This fixes a specific
      build error under RHEL4 with recent versions of MySQL 5.0.


2.2.0, 2007.01.23 (r1417)

    o ColData, const_string, and SQLString can now be constructed
      with an explicit length parameter.  Furthermore, Query
      class's execute(), store() and use() call chains terminate
      in a version taking an explicit length parameter, instead
      of one taking a simple C string.  Together, this means
      that it's now easier to handle data from the SQL server
      containing nulls.  The library is almost certainly not yet
      capable of handling embedded nulls in all cases, but this
      is a big first step towards that.

    o Can now construct a DateTime object from a time_t, and
      convert a DateTime back to a time_t.  Patch by Korolyov Ilya.

    o Changed the way we're handling exported functions in the
      Windows DLL case so that it works more reliably under MinGW.

    o Added proper copy semantics to Connection, so that you get a
      new connection with the same parameters, not just a bitwise
      copy of the object.

    o Using an explicitly thread-safe variant of localtime() for
      time conversions where one is available.

    o Removed ListInsert template from myset.h.  This wasn't used
      within the library, and was never documented, so I'm betting
      that no one actually uses it.

    o Result::copy() was not copying the exception flag in
      all cases.  Fix by Steven Van Ingelgem.

    o Added exrun shell script and exrun.bat files to distribution,
      to avoid linkage errors when running the examples while
      you still have an older version of MySQL++ installed.

    o Renamed MYSQLPP_LIB_VERSION to MYSQLPP_HEADER_VERSION, as
      what it really encodes is the version number in the mysql++.h
      file you're using, not the actual library version number.

    o Added mysqlpp::get_library_version(), which returns the
      library version number at build time.  Between this and
      the header version constant, you can check that you're not
      mixing MySQL++ header and library versions.

    o resetdb example uses these new version number affordances to
      double-check that you're not mixing libraries and headers
      from different versions.  This happens easily unless you
      take care of it (such as by using exrun) when you have one
      version of MySQL++ installed and you're trying to build and
      test a new version without blowing away the old one first
      or overwriting it.

    o No longer using recursive Makefiles on Unixy platforms
      or split lib + examples project files on VC++.  Everything is
      handled by a single top-level Makefile or project file, which
      is simpler for the end user, and makes better dependency
      management possible.

    o When looking for the MySQL C library on systems using
      autoconf, looking in .../lib64 wherever we are also looking
      in .../lib.

    o RPM build process no longer depends on Bakefile.  It means
      you have to build the examples when building an RPM even
      though they're never used within the RPM, but it's a better
      tradeoff in my opinion.

    o Updated include and library paths on Windows to reflect
      changes in the most recent MySQL installers.

    o Merged lib/defs.h and lib/platform.h into new file,
      lib/common.h.  Just cleans up the library internals.

    o Fixed build errors on Windows due to recent changes in MySQL.

    o Fixed a few memory leaks and double-deletes in Query class.

    o Fixed compatibility with STLPort's string implementation.
      Patch by dengxy at cse.buaa.edu.cn.

    o Fixed a compatibility problem between Set<> template and
      SSQLS.  Patch by Korolyov Ilya.

    o Fixed build bug in SQLQueryParms due to a character
      signedness issue on PowerPC with GCC.  Patch by Michael
      Hanselmann.

    o ~Transaction() can no longer throw exceptions.  It'll just
      quietly eat them, to avoid program termination.  Fix
      suggested by Alex Burton.

    o Fixed thread safety testing in autoconf case, accidentally
      broken during v2.1.0 development cycle.

    o Using Doxygen 1.5.1 to generate documentation.


2.1.1, 2006.04.04 (r1289)

    o MinGW and Cygwin will now build and link to mysqlpp DLLs.

    o Fixed bug in Query, causing it to initialize the "throw
      exceptions" flag incorrectly.  Thanks for this patch go to
      Joel Fielder.

    o Added -v flag for custom.pl script, which turns off the
      multiply-defined static variable fix.  Needed for VS 2003,
      which doesn't support variadic macros.  Also, added
      a diagnostic to detect the need for the -v flag, and
      suppressed the test for this feature in examples/util.cpp.


2.1.0, 2006.03.24 (r1269)

    o Converted automake and makemake files to their equivalents in
      Bakefile format.

    o Added the Transaction class, which makes it easy to use
      transaction sets in MySQL++.

    o Added xaction example to test new Transaction class.

    o Resetdb example now creates its example table using the
      InnoDB storage engine, in order to test the new transaction
      support.  Resetdb also declares the table as using UTF-8
      text; this doesn't change anything, but it does correctly
      document what we're doing.

    o Added sql_types.h header, containing C++ typedefs
      corresponding to each MySQL column type.  Using those new
      types in the type_info module, and in the SSQLS examples.

    o Replaced the way we were handling the template query
      version of Query member functions, to allow an arbitrary
      number of template query parameters.  By default, we
      now support 25 parameters, up from the old limit of 12.
      It's now possible to change just one number, run a script,
      and have a new limit.

    o Connection class does a better job of returning error
      messages if you call certain member functions that depend
      on a connection to the server before the connection is
      established.

    o Updated libmysqlclient.def for newer versions of MySQL.  (Fixes
      build errors having to do with mysql_more_results() and
      mysql_next_result().

    o Replaced final use of strcpy() with strncpy().

    o custom.pl now runs without complaint in strict mode, with
      warnings turned on.  Thanks for this patch go to "Waba".

    o Fixed a bug in custom.pl where incorrect code would be
      generated for some SSQLS set() methods.  Thanks for this
      patch go to "Waba".

    o SSQLS structures now support long and unsigned long fields.
      Thanks for this patch go to "Waba".

    o It's now possible to put SSQLS definitions in a header
      file used by multiple modules in a program without
      getting multiple static member definition errors.  See the
      documentation for details.  Thanks for this patch go to
      Viktor Stark.

    o Moved the definition of the 'stock' SSQLS out of the
      custom*.cpp example files and into a new stock.h file.
      Also, #including that file in the util module to test out
      the new SSQLS multiple static definition fix.

    o Using all of the digits of precision guaranteed by the
      IEEE 754 spec when stringizing floating point numbers
      to build queries.  Previously, we would use the platform
      default, which can be as few as 6 digits.

    o Removed lib/compare.h.  Not used within the library, never
      documented, and nobody seems to want to defend it.


2.0.7, 2005.11.23 (r1147)

    o Added explicit mysqlpp namespace qualifiers to generated code in
      custom*.h so you can use SSQLS in places where it doesn't make
      sense to say "using namespace mysqlpp" before the declaration.
      Also updated some of the examples to not have this "using"
      declaration to make it clear to users that it isn't needed, if you
      want to use explicit namespace qualifiers as well.  Thanks for
      this patch to Chris Frey.

    o Removed an apparently useless unlock() call from ResUse; there is
      no nearby lock() call, so if this unlock() is in fact necessary,
      it shouldn't be here anyway, because the two calls should be
      nearby each other.  Thanks for this patch to Chris Frey.

    o Fixed Query ostream initialization bug affecting SunPro CC (at
      least).  While this bug violates the Standard, it doesn't affect
      many real compilers because they don't enforce this rule.  Fixed
      by Chris Frey.

    o Previously, we only used the C99 style "long long" support when
      building under GNU CC.  This is now the default.  This should
      allow the code to work under SunPro CC.

    o Added another dynamic cast needed for proper Query ostream
      subclass overloading under VC++.  (7.1 at least...)

    o Detecting whether MySQL is built with SSL support on platforms
      using autotools.  Needed on some old Sun systems, for instance.
      Thanks for this patch to Ovidiu Bivolaru.

    o Fixed a potential memory bug in ColData's conversion to SQL null.

    o Many minor packaging tweaks.  (README clarifications, file
      permission fixes, better adherence to GNU packaging standards,
      etc.)

    
2.0.6, 2005.09.28 (r1123)

    o Fixed makemake.bat so it works on cmd.exe, not just 4NT.

    o Documentation fixes.


2.0.5, 2005.09.13 (r1114)

    o Visual C++ build now requires GNU make.  It is tested to work
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

    o MinGW makemake support updated to support new release/debug
      subdirectory system.  This is probationary support, since
      this code currently can't be built as a DLL.  As a result,
      it is no more useful than the Cygwin version, for licensing
      reasons.

    o Several fixes to allow building on Solaris 8.  These fixes may
      also help on other SVR4-derived systems.

    o Removed Borland C++ makemake support, because this version
      of the library does not work completely, and there seems
      to be almost no user interest in fixing it.
      
    o Clarified "Handling SQL Nulls" section of user manual's
      Tutorial chapter.


2.0.4, 2005.08.29 (r1076)

    o Made mysql_shutdown() second parameter autoconf check less
      sensitive to compiler pedantry.

    o VC++ library Makefile is now smart enough to re-create the
      import library, if it is deleted while leaving the DLL alone.

    o Added libmysqlclient.def to tarball.

    o Reworked most of the top-level README* files.

    o Renamed LGPL file to LICENSE.


2.0.3, 2005.08.25 (r1060)

    o Visual C++ makemake system updated to build both debug and
      release versions of library DLL.

    o Fixed bug in simple1 example that caused crashes on Windows.

    o Doing UTF-8 to ANSI text translation in simple examples now.

    o Previous two releases built libmysqlpp with wrong soname on
      autotools-based systems.  Fixed.


2.0.2, 2005.08.18 (r1050)

    o Fixes to makemake system for cmd.exe.

    o Fixed the case where the system's C++ library includes an slist
      implementation in namespace std.


2.0.1, 2005.08.17 (r1046)

    o Added new simple1 example, showing how to retrieve just one
      column from a table.  Old simple1 is now called simple2, and
      simple2 is likewise shifted to simple3.

    o Added custom6 example, showing how to do the same thing with
      SSQLS.

    o Updated user manual to cover new examples.

    o Was accidentally shipping Subversion crap with tarball.  Fixed.


2.0.0, 2005.08.16 (r1031) The "Excess Hair Removal" release

    THIS IS NOT A DROP-IN REPLACEMENT FOR MySQL++ v1.7!

    At minimum, you will have to recompile your program against
    this library.  You may also have to make code changes.
    Please see the "Incompatible Library Changes" chapter of
    the user manual for a guide to migrating your code to this
    new version:

        http://tangentsoft.net/mysql++/doc/html/userman/breakages.html

    o The library's shared object file name (soname) scheme has
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

    o autoconf now installs headers into $prefix/include/mysql++,
      instead of $prefix/include.  If you were using the
      --includedir configure script option to get this behavior
      before, you no longer need it.

    o Linux binary RPMs will henceforth include only the
      libmysqlpp.so.X.Y.Z file, and create any short names
      required, to allow multiple versions to be installed at
      once.  Currently, you cannot install two MySQL++ library
      RPMs at once, because they both have /usr/lib/libmysqlpp.so.X,
      for instance.

    o Replaced the Visual C++ and Borland C++ project files with
      a new "makemake" system, which creates Makefiles specific
      to a particular toolchain.  This new mechanism also supports
      MinGW and generic GCC-on-*ix.  This was done partly to reduce
      the number of places we have to change when changing the
      file names in MySQL++ or adding new ones, and partly so we're
      not tied to one particular version of each of these tools.

    o VC++ Makefiles create a DLL version of the library only
      now, so there's no excuse for LGPL violations now.
      This same mechanism should make DLL builds under other
      Windows compilers easy.

    o Added Connection::enable_ssl(), which enables encrypted
      connections to the database server using SSL.

    o Connection::create_db() and drop_db() now return true on
      success, not false.

    o Connection::create_db() and drop_db() use Query::exec()
      now, for efficiency, rather than Query::execute().

    o Removed Connection::infoo().  Apparently just there to
      save you from a typo when calling the info() method, since
      it was a mere alias.

    o Renamed Connection::real_connect() to connect(), gave
      several more of its parameters defaults, and removed old
      connect() function.  Then changed user manual and examples
      to use new APIs.

    o Replaced Connection::read_option() with new set_option()
      mechanism.  The name change matches the method's purpose
      better.  Functional changes are that it returns true on
      success instead of 0, it supports a broader set of options
      than read_option() did, and it enforces the correct option
      argument type.

    o You can now call Connection::set_option() before the
      connection is established, which will simply queue the option
      request up until the connection comes up.  If you use this
      feature, you should use exceptions, because that's the only
      way an option setting failure can be signalled in this case.

    o Removed query-building functions (exec*(), store*(),
      use()) from class Connection, and moved all the implementation
      code to class Query.  Query no longer delegates the final
      step of sending the query to the database server to
      Connection().

    o Added Connection::enable_ssl(), for turning on SSL support on
      a connection.

    o Extracted exception disabling mechanism out of the many
      classes that had the feature into a new OptionalExceptions
      base class, which all classes having this feature now
      derive from.  Also, removed all per-method exception
      handling flags.  Finally, added NoExceptions class.  With
      all of these changes, there is now a common way to disable
      exceptions with fine granularity on all objects that
      support the feature.

    o All custom MySQL++ exceptions now derive from the new
      Exceptions class.  This regularizes the exception interface
      and allows you to use a single catch() block if you want.

    o The "throw exceptions" flag is passed from parent to child
      in all situations now.  (Or if not, please report it as
      a bug.) This fulfills a promise made in the v1.7.9 user
      manual, with the cost being that some programs will see
      new exceptions thrown that they're not expecting.

    o Added a bunch of new exception types: BadOption,
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

    o Renamed SQLQueryNEParms to BadParamCount, to match naming
      style of other concrete exception types.

    o Extracted lock()/unlock() functions from Connection and
      Query classes into a new Lockable interface class.  Locking
      is implemented in terms of a different class hierarchy, Lock,
      which allows multiple locking strategies with a single ABI.

    o Removed ResUse::eof().  It's based on a deprecated MySQL
      C API feature, and it isn't needed anyway.

    o Removed arrow operator (->) for iterator returned by Fields,
      Result and Row containers.  It was inherently buggy, because
      a correct arrow operator must return the address of an
      object, but the underlying element access functions in these
      classes (e.g. at()) return objects by value, of necessity.
      Therefore, this operator could only return the address of
      a temporary, which cannot be safely dereferenced.

    o Returned Row subscripting to something more like the
      v1.7.9 scheme: there are two operator[] overloads, one for an
      integer (field by index) and another for const char* (field
      by name).  lookup_by_name() has been removed.  Because row[0]
      is ambiguous again, added Row::at() (by analogy with STL
      sequence containers), which always works.

    o Collapsed two of the Row::value_list*() overloads into
      two other similar functions using default parameters.
      This changes the API, but the removed functions aren't
      used within the library, and I doubt they are used outside,
      either.

    o Merged RowTemplate into Row.

    o Merged SQLQuery class into Query class.

    o Query is now derived from std::ostream instead of
      std::stringstream, and we manage our own internal string
      buffer.

    o Moved SQLParseElement and SQLQueryParms into their own
      module, qparms.

    o Added multiple result set handling to Query.  MySQL 4.1
      and higher allow you to give multiple SQL statements in a
      single "store" call, which requires extensions to MySQL++
      so you can iterate through the multiple result sets.  Also,
      stored procedures in MySQL 5.0 reportedly return multiple
      result sets.  Thanks for the initial patch go to Arnon Jalon;
      I reworked it quite a bit.

    o Query::storein*() now supports more varieties of the
      nonstandard slist comtainer.  (Singly-linked version of
      STL std::list.)

    o Template query mechanism and user manual had several
      mismatches.  Made manual match actual behavior, or
      made library match documented behavior, as apropriate.
      Initial patch by Jürgen MF Gleiss, with corrections and
      enhancements by Warren Young.

    o Collapsed mysql_* date and time base classes' methods and
      data into the subclasses.  Also, DateTime no longer derives
      from Date and Time; you could get away with that in the
      old hierarchy, but now it creates an inheritance diamond,
      and allows unsupported concepts like comparing a Time to
      a DateTime.

    o Removed "field name" form of Row::field_list().  It was
      pretty much redundant -- if you have the field names, why
      do you need a list of field names?

    o ColData can convert itself to bool now.  Thanks for this
      patch go to Byrial Jensen.

    o Removed simp_list_b type; wasn't being used, and doesn't
      look to be useful for end-user code.

    o Several methods that used to take objects by value now
      do so by const reference, for efficiency.

    o Several variable and function renamings so that MySQL++
      isn't needlessly tied to MySQL.  Even if we never make
      the library work with other database servers, there's
      little point in tying this library to MySQL blindly.

    o Renamed all private data members of MySQL++ classes to
      have trailing underscores.

    o 'private' section follows 'public' section in all classes
      now.

    o Removed mysql++.hh and sqlplus.hh backwards-compatibility
      headers.

    o Added copy ctors to Date/Time classes so that they will
      work in SSQLS under GCC 4.0.0.  Without these, the compiler
      couldn't make the conversion from raw MySQL row data.

    o Fixed a bunch of GCC 4.0 pedantic warnings: added virtual
      dtors to all base classes, calling base class ctors from leaf
      classes, etc.

    o All warnings fixed under VC++ at warning level 3.  (Mostly
      harmless signedness and integer conversion stuff.)

    o Updated LGPL license/copyright comments at the top of
      several files to use FSF's new physical address.

    o Relicensed user manual under a close variant of the Linux
      Documentation Project License, as it's designed for
      documentation, which the LGPL is not.  Permission for this
      received from Kevin Atkinson and MySQL AB.

    o Added ABI and API breakages chapter to user manual.  It
      is basically a subset of this ChangeLog, with only the
      information an end-user must know when migrating between
      versions.

    o Reworked user manual's DocBook code quite a bit after
      reading Bob Stayton's book "DocBook XSL" 3/e.  Better handling
      of stylesheets, taking advantage of some superior DocBook
      features, prettier output (especially the HTML version), etc.

    o Rewrote doc/userman/README to make it clearer how to get
      started contributing to the user manual.  It's essentially a
      "getting started with DocBook" guide now!

    o Lots of small text improvements to user and reference
      manuals.  Aside from the obvious tracking of library changes,
      made a bunch of minor style and clarity improvements.

    o Added CSS stylesheets for userman and refman to
      make the HTML versions of each a) not ugly; and b) match
      tangentsoft.net.  (Yes, some may say that these are incompatible
      goals....)

    o Standardized exception handling code in the examples that
      use it.

    o Fixed a potential memory leak due to exceptions thrown from
      ResUse.  Thanks for this patch go to Chris Frey.

    o Using new "no exceptions" feature of library in simple1
      example, so it is now truly simple.

    o simple1 example no longer depends as much on util module, so
      that all of the important code is in one place.  Makes
      learning MySQL++ a little less intimidating.

    o Added new simple2 and usequery examples, to demonstrate
      the proper way to handle a "use" query, with exceptions
      disabled, and not, respectively.  Added them to the user
      manual, in the appropriate place.

    o Refactored the "print stock table" example functions
      again, to make code using them clearer.

    o UTF-8 to UCS-2 handling in examples is now automatic on
      Windows.

    o Removed debug code from Windows Unicode output examples
      that slipped into previous release.

    o resetdb example is now clearer, and more robust in the
      face of database errors.

    o Simplified connect_to_db() in examples' util module.

    o Added sample autoconf macro for finding MySQL++ libraries, for
      people to use in their own autotools-based projects.

    o Lots and lots of minor cleanups not worth mentioning
      individually...


1.7.40, 2005.05.26 (r719)

    o Multiple item form of insert() now works if you're using the
      SQLQuery class, or its derivative, Query.  Thanks to Mark
      Meredino for this patch.

    o Fixed a bug in const_string::compare(), in which MySQL++
      would walk off the end of the shorter of the two strings.
      All was well if the two were the same length.

    o ResUse::operator=() now fully updates the object, so it's more
      like the behavior of the full ctor.

    o All source files now contain a license and copyright statement
      somewhere within them.

    o Optimized mysql++.h a bit: it now #includes only the minimum set
      of files required, and there is now an idempotency guard.
      This improves compile times a smidge, but mainly it was
      done to clean up the generated #include file graph in the
      reference manual.  Before, it was a frightful tangle because
      we #included everything except custom*.h.

    o Constness fix in MySQL++ date/time classes to avoid compiler
      warnings with SSQLS.  Thanks to Wolfram Arnold for this patch.

    o Fixed some compiler warnings in custom*.h.  Thanks to Chris Frey
      for this patch.

    o Added "Submitting Patches" and "Maintaining a Private CVS
      Repository" sections to the HACKERS file.  Thanks to Chris
      Frey for the source material for these sections.  The HACKERS
      file was improved in several other ways at the same time.

    o PDF version of user manual no longer has links to the reference
      manual.  They were ugly, and they were broken anyway due to the
      way we move the PDFs after generating them.  If you want
      interlinked manuals, use the HTML version.

    o PDF version of user manual now has hard page breaks between
      chapters.

    o Removed complic1 example.  Wasn't pulling its own weight.
      Everything it is supposed to demonstrate is shown in other
      examples already.

    o Refactored print_stock_table() in examples/util module to be
      four functions, and made all the examples use various of
      these functions where appropriate.  Before, several of
      the examples had one-off stock table printing code because
      print_stock_table() wasn't exactly the right thing, for one
      reason or another.  One practical problem with this is that
      some of the examples missed out on the recent Unicode updates;
      now such a change affects all examples the same way.

    o Since so many of the examples rely on the util module, the user
      manual now covers it.  The simple1 example in the user manual
      didn't make much sense before, in particular, because it's
      really just a driver for the util module.

    o Added custom5 example.  It shows how to use the equal_list()
      functionality of SSQLS.  Thanks to Chris Frey for the original
      version of this program.  (I simplified it quite a bit after
      accepting it.)

    o New user manual now covers the value_list(), equal_list() and
      field_list() stuff that the old manual covered but which was
      left out in previous versions of the new manaul.  Most of the
      examples are the same, but the prose is almost completely new.
      This new section includes the custom5 example.

    o Every declaration in MySQL++ is now documented in the reference
      manual, or explicitly treated as "internal only".

    o Improved docs for MySQL++'s mechanism to map between MySQL
      server types and C++ types.  Initial doc patch by Chris Frey,
      which I greatly reworked.

    o Improved a lot of existing reference manual documentation while
      adding the new stuff.

    o Expanded greatly on the exception handling discussion in the user
      manual.

    o Added all-new "Quoting and Escaping" section to the user
      manual's Tutorial chapter.  Moved some existing comments on
      quoting and escaping around and added some new ones to other
      sections as a result.

    o Added all-new "Handling SQL Nulls" section to the user manual's
      Tutorial chapter.

    o Many improvements to the Overview section of the user manual.

    o Row::operator[] reference now explains the right and wrong way to
      use the values it returns.  This is in response to a mailing list
      post where someone was incorrectly using this feature and getting
      a bunch of dangling pointers.

    o Updated Doxyfile so 1.3.19.1 parses it without warnings.  Still
      works with versions back to 1.2.18, at least.  (These are
      the versions shipped with Fedora Core 3 and Red Hat Linux 9,
      respectively.)

    o Using a superior method to make Doxygen ignore certain sections
      of the source code.  Between this change and the fact that
      everything not so ignored is documented, Doxygen no longer
      generates any warnings.

    o Lots of code style updates.  Everything should now be consistently
      formatted.


1.7.35, 2005.05.05 (r601) The "Cinco de Mayo" release

    o Added a "how to use Unicode with MySQL++" chapter to the user
      manual.  (Too bad "Cinco de Mayo" doesn't have any accented
      characters.  That would be just _too_ precious.)

    o VC++ examples now use the Unicode Win32 APIs, so they can display
      Unicode data from MySQL++.

    o Added an optional conversion function to examples/util.cpp to
      handle the conversion from UTF-8 to UCS-2 on Win32.

    o Moved "brief history of MySQL++" from intro section of refman to
      intro section of userman.

    o Lots of small bits of documentation polishing.
    
    o Made some minor constness fixes.  Thanks to Erwin van Eijk
      for this patch.

    o Made some warning fixes for GCC 4.0.  Not all warnings are
      fixed, because some of the needed changes would break the ABI.
      Thanks to Chris Frey for this patch.

    o Added lib/Doxyfile to distribution.
    

1.7.34, 2005.04.30 (r573)

    o Added a multiple-insert method for Query, which lets you insert
      a range of records from an STL container (or the whole thing,
      if you like) in a single SQL query.  This is faster, and it
      reduces coding errors due to less repetition.  Thanks to Mark
      Meredino for the patch.

    o Reference and user manual now get rebuilt automatically when
      required.  (E.g. on 'make dist', or explicitly now through 'make
      docs'.)

    o Made it easier to change the maximum number of SSQLS data members
      in generated custom-macros.h file.  It used to be hard-coded in
      several places in lib/custom.pl; now it's a variable at the top of
      the file.

    o Changed default SSQLS data member limit to 25, which is what it
      has been documented as for a long time now.  It was actually 26
      within custom.pl.

    o Fixed a regression in previous version.

    o Trimmed some fat from the distribution packages.

    o Some more small doucmentation improvements.


1.7.33, 2005.04.29 (r555)

    o Worked around an overloaded operator lookup bug in VC++ 7.1
      that caused SSQLS insert, replace and update queries to get
      mangled.  (Symptom was that custom2 and custom3 examples didn't
      work right.)  Thanks to Mark Meredino for digging up the
      following, which explains the problem and gives the solution:

      http://groups-beta.google.com/group/microsoft.public.vc.stl/browse_thread/thread/9a68d84644e64f15

    o Some VC++ warning fixes.

    o Major documentation improvements:
    
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

    o ebuild file updated to take advantage of recent configure script
      features.  Thanks to Chris Frey for this patch.



1.7.32, 2005.03.10 (r479)

    o Example building may now be skipped with --disable-examples
      configure script flag.

    o Changed stock items added in resetdb.  One is now UTF-8 encoded,
      to show that basic use of Unicode with MySQL++ is easy, yet not
      foolproof.  (See formatting of table on systems where cout isn't
      UTF-8 aware!)  Other stock items now follow a theme, for your
      amusement.  :)

    o custom3 example now changes UTF-8 item's name to the 7-bit ASCII
      equivalent.  Previously, this example would fix a spelling error
      in the table.

    o resetdb example now says 'why' when it is unable to create the
      sample database.

    o Small formatting change to print_stock_table(), used by several
      examples.

    o Was issuing a VC++-specific warning-disable pragma when built by
      any Windows compiler.  Fixed.
    

1.7.31, 2005.03.05 (r462) The "Inevitable Point-one Followup" release

    o Check for threads support must now be explicitly requested via
      configure script's new --enable-thread-check flag.

    o Fix for contacting MySQL server on a nonstandard port number.
      Thanks to Chris Frey for this patch.

    o Example programs using standard command line format now accept a
      fourth optional parameter, a port number for the server.  Thanks
      to Chris Frey for this patch.

    o One more g++ 3.4 pedantic warning fix by Chris Frey.

    o Exception handling in resetdb is no longer nested, because you'd
      get a segfault on some systems when an exception was thrown from
      one of the inner try blocks.

    o Improvements to Connection class's handling of locking mechanism.
      Concept based on patches by Rongjun Mu.

    o Implemented the declared-but-never-defined Query::lock().  Thanks
      to Rongjun Mu for this patch.
    
    o Cleaned up some unclear if/else blocks in connection.cpp by
      adding explicit braces, correct indenting and putting normal
      code path in the if side instead of the else.


1.7.30, 2005.02.28 (r443) The "Power of Round Numbers" release

    o bootstrap script now accepts a 'pedantic' argument, which sets a
      bunch of CFLAGS that make g++ very picky about the code it
      accepts without warnings.
      
    o Fixed a bunch of things that generated warnings with g++ in
      pedantic mode. Only two warnings remain, having to do with
      floating point comparisons.  (See Wishlist for plans on how to
      deal with these.)  Thanks to Chris Frey for this patch.

    o Split long tests out of configure.in into M4 files in new config
      subdir.  This makes configure.in easier to read.

    o Added preliminary thread support.  Currently, this just means that
      we detect the required compiler and linker thread flags, and link
      against the proper thread-safe libraries.  THERE MAY BE
      UN-THREAD-SAFE CODE IN MYSQL++ STILL!

    o Standard C++ exceptions are the default now.  Old pre-Standard
      exception stuff removed.

    o Row::lookup_by_name() will throw the new BadFieldName exception if
      you pass a bad field name.  Thanks for this patch to Chris Frey.

    o Row::operator[] will throw a Standard C++ out of bounds exception
      by way of std::vector::at() if you pass it a bad index.  Thanks
      for this patch to Chris Frey.

    o Setting Connection::is_connected flag to false on close().
      Previously, is_connected() would continue to return true after
      close() was called.

    o All number-to-string conversion ctors in SQLString class now use
      ostringstream to do the conversion.  Previously, we used
      snprintf(), which isn't available on all systems.  Also, we used a
      C99 format specifier for the "long long" conversion, which is also
      not available on all systems.  This new ostringstream code should
      be platform-independent, finally.


1.7.28, 2005.02.04 (r403)

    o --with-mysql* flags to configure script now try the given
      directory explicitly, and only if that fails do they try
      variations, like tacking '/lib' and such onto it to try and find
      the MySQL includes and libraries.  Thanks to Matthew Walton for
      the patch.

    o Finally removed sql_quote.h's dependence on custom.h, by moving
      the one definition it needed from custom.h to deps.h.  This will
      help portability to compilers that can't handle the SSQLS macros,
      by making that part of the library truly optional.


1.7.27, 2005.01.12 (r395)

    o configure check for libmysqlclient now halts configuration if the
      library isn't found.  Previously, it would just be flagged as
      missing, and MySQL++ would fail to build.

    o Added sql_string.cpp to VC++ and BCBuilder project files.

    o Removed Totte Karlsson's 'populate' example, which never made it
      into the distribution anyway.

    o Removed last vestiges of 'dummy.cpp'.

    o Renamed *.cc to *.cpp in BCBuilder project files.

    o Worked around a BCBuilder C++ syntax processing bug in row.h.


1.7.26, 2004.12.17 (r382)

    o Moved all of the SQLString definitions out of the header and into
      a new .cpp file, reformatted it all, and made the integer
      conversion functions use snprintf() or _snprintf() instead of
      sprintf().  Also, widened some of the buffers for 64-bit systems.

    o Using quoted #include form for internal library headers, to avoid
      some problems with file name clashes.  (The headers should still
      be installed in their own separate directory for best results,
      however.)  Thanks to Chris Frey and Evan Wies for the patch and
      the discussion that lead to it.

    o Removed unnecessary semicolons on namespace block closures.
      Thanks to Evan Wies for this patch.

    o Fixed namespace handling in the legacy headers mysql++.hh and
      sqlplus.hh.  Thanks to Chris Frey for this patch.

    o #including iostream instead of ostream in lib/null.h for
      broader C++ compatibility.  (This may allow MySQL++ to work on GCC
      2.95.2 again, but this is unconfirmed.)

    o Detecting proper mysql_shutdown() argument handling automatically
      in platform.h for the Windows compiler case instead of making the
      user edit the file.  Thanks to Evan Wies for this patch.
    
    o Fixed examples/Makefile.simple to use new *.cpp file naming.

    o Fix to Gentoo ebuild file's exception configure switch handling.
      Thanks to Chris Frey for this patch.

    o Rebuilding lib/custom*.h intelligently now, to avoid unnecessary
      recompiles after running bootstrap script.


1.7.25, 2004.12.09 (r360)

    o Yet more fixes to the --with-mysql-lib and --with-mysql-include
      flags.

    o Added DLLEXPORT stuff to platform.h, hopefully so that someone
      can figure out how to make VC++ make a DLL version of MySQL++.

    o Renamed *.cc to *.cpp.

    o Made 'set -> myset' change in VC++ project files.

    o Some style changes (mostly whitespace) in header files.


1.7.24, 2004.12.08 (r343)

    o Fixed the --with-mysql-lib and --with-mysql-include flags'
      behavior, and extended their search ability to handle one other
      common case.  (Fixed by Steve Roberts)

    o Fixes to put freestanding functions in namespace mysqlpp.  (They
      weren't in the namespace, while all the class member functions
      were.)  This required bumping the ABI version number to 4.
      
    o Renamed set module to myset, to avoid conflicts with Standard C++
      Library's set.h when MySQL++ headers were installed into one of
      the standard system include directories.

    o Renamed all the idempotency guards to make them consistent in
      style and unique to MySQL++.

    o Reformatted all of lib/*.cc.


1.7.23, 2004.11.20 (r333)

    o Query::reset() now empties the stored query string.  If you
      subsequently stored a longer query in the object, you'd overwrite
      the previous query, but otherwise the longer part of the previous
      one would stick out past the new query.

    o We now look to the NO_LONG_LONGS macro only to decide whether to
      fake 64-bit integer support using 32-bit integers.

    o 64-bit integer support under Visual C++ may be working now, using
      that platform's __int64_t type.  This has not been tested.

    o Removed 64-bit integer support for Codewarrior on Mac OS 9 and
      earlier.  OS X uses GCC, so it requires no special support.

    o Added MinGW detection in platform.h.
    
    o If you pass a flag (-X) to the examples that take the standard
      parameters (resetdb, simple1, etc.), it prints a usage message.
    
    o Better error handling in resetdb example, where errors are the
      most critical.  (If that one runs without errors, the others
      probably will, too, and you have to run that one first.)

    o resetdb now reports success, rather than succeeding silently.

    o Removed the code in sample1 example that duplicated util module's
      print_stock_table(), and called that function instead.

    o Moved the preview() calls in the example programs to before the
      query execution calls, because execution modifies the query.

    o All examples that take the standard command line parameters now 
      exit when connect_to_db() fails in one of the ways that don't
      throw an exception, rather than bulling onward until the next
      MySQL database call fails because the connection isn't up.

    o dbinfo example now takes the standard command line parameters.

    o Much better output formatting in dbinfo example.

    o Calling reset() where appropriate in the various example programs.
      Before, the programs may have worked, but not for the right
      reason.  This lead some people to believe that calling reset()
      was not necessary.

    o Fixed an incorrect use of row["string"] in complic1 example.

    o Lots of code style improvements to the examples.

    o Some VC++ type warnings squished.  Some remain.
    

1.7.22, 2004.11.17 (r302)

    o Applied patches by Zahroof Mohammed to allow it to build under GCC
      3.4.2.  Tested on MinGW and Fedora Core 3 systems.

    o Removed all the forward declarations in defs.h, and added
      forward declarations where necessary in individual header files.
      #including defs.h in fewer locations as a result.

    o Legacy headers sqlplus.hh and mysql++.hh now declare they are
      using namespace mysqlpp, to allow old code to compile against the
      new library without changes.

    o Removed query_reset parameter from several class Query member
      functions.  In the implementation, these parameters were always
      overridden!  No sense pretending that we pay attention to these
      parameters.  This changes the ABI version to 3.

    o #including custom.h in sql_query.h again...it's necessary on GCC
      3.4.
    
    o bootstrap script runs lib/config.pl after configure.  This is
      just a nicety for those running in 'maintainer mode'.


1.7.21, 2004.11.05 (r273)

    o Generating a main mysql++ RPM containing just the library files
      and basic documentation, and the -devel package containing
      everything else.

    o Devel package contains examples now, along with a new Makefile
      that uses the system include and library files, rather than the
      automake-based Makefile.am we currently have which uses the files
      in the mysql++ source directory.

    o Renamed sqlplusint subdirectory in the package to lib.

    o Removed the obsolete lib/README file.

    o lib/sql_query.h no longer #includes custom.h, simplifying
      build-time dependencies and shortening compile times.


1.7.20, 2004.11.03 (r258)

    o Collapsed all numbered *.hh headers into a single *.h file.  For
      example, the contents of row1.hh, row2.hh and row3.hh are now in
      row.h.

    o While doing the previous change, broke several circular
      dependencies.  (The numbered file scheme was probably partly done
      to avoid this problem.)  The practical upshot of most of these
      changes is that some functions are no longer inline.

    o Removed define_short.hh and everything associated with it.  The
      library now uses the short names exclusively (e.g. Row instead of
      MysqlRow).

    o Put all definitions into namespace mysqlpp.  For most programs,
      simply adding a 'using namespace mysqlpp' near the top of the
      program will suffice to convert to this version.

    o Once again, the main include file was renamed, this time to
      mysql++.h.  Hopefully this is the last renaming!

    o mysql++.hh still exists.  It emits a compiler warning that the
      file is obsolete, then it #includes mysql++.h for you.

    o sqlplus.hh is back, being a copy of the new mysql++.hh.  Both of
      these files may go away at any time.  They exist simply to help
      people transition to the new file naming scheme.

    o Renamed mysql++-windows.hh to platform.h, and added code to it to
      handle #inclusion of config.h on autotools-based systems
      intelligently.  This fixes the config.h error when building under
      Visual C++.

    o There is now only one place where conditional inclusion of
      winsock.h happens: platform.h.

    o Beautified the example programs.


1.7.19, 2004.10.25 (r186)

    o Fixed an infinite loop in the query mechanism resulting from the
      strstream change in the previous version.  There is an overloaded
      set of str() member functions that weren't a problem when query
      objects were based on strstream.
     
    o Query mechanism had a bunch of const-incorrectness: there were
      several function parameters and functions that were const for
      the convenience of other parts of the code, but within these
      functions the constness was const_cast away!  This was evil
      and wrong; now there are fewer const promises, and only one is
      still quietly broken within the code.  (It's in the SQLQuery
      copy ctor implementation; it should be harmless.)

    o Removed operator=() in Query and SQLQuery classes.  It cannot take
      a const argument for the same reason we have to cast away const
      in the SQLQuery copy ctor.  It's tolerable to do this in the copy
      ctor, but intolerable in an operator.  Since the copy ctor is good
      enough for all code within the library and within my own code, I'm
      removing the operator.

    o Above changes required bumping the ABI to version 2.

    o Visual C++ projects now look for MySQL build files in c:\mysql,
      since that's the default install location.  (Previously, it was
      c:\program files\mysql.)


1.7.18, 2004.10.01 (r177)

    o Changed all the strstream (and friends) stuff to stringstream type
      classes.  Let there be much rejoicing.

    o Query object now lets you use store() even when the SQL query
      cannot return a result, such as a DROP TABLE command.  This is
      useful for sending arbitrary SQL to the server.  Thanks to
      Jose Mortensen for the patch.

    o Quote fix in configure.in, thanks to David Sward.

    o Renamed undef_short file to undef_short.hh.

    o Gentoo ebuild file is actually being shipped with the tarball,
      instead of just sitting in my private CVS tree since 1.7.14 was
      current.  Ooops....


1.7.17, 2004.09.16 (r170)

    o Reverted one of the VC++ warning fix changes from 1.7.16 that
      caused crashes on Linux.

    o Added a configure test that conditionally adds the extra 'level'
      parameter to mysql_shutdown() that was added in MySQL 4.1.3 and
      5.0.1.


1.7.16, 2004.09.13 (r160)

    o Building VC++ version with DLL version of C runtime libraries, and
      at warning level 3 with no warnings emitted.

    o VC++ build no longer attempts to fake "long long" support.  See
      the Wishlist for further thoughts on this.


1.7.15, 2004.09.02 (r144)

    o Renamed Configure file to common.am, to avoid file name conflict
      with configure script on case-sensitive file systems.

    o Added ebuild file and ebuild target to top-level Makefile for
      Gentoo systems.  Thanks to Chris Frey for this.

    o Small efficiency improvements to BadQuery exception handling.
      Initial idea by Chris Frey, improvements by Warren Young.


1.7.14, 2004.08.26 (r130)

    o Builds with Visual C++ 7.1.

    o Fixed a bug in custom macro generation that caused problems with
      GCC 3.4.  (X_cus_value_list ctor definition was broken.)


1.7.13, 2004.08.23 (r92)

    o Removed USL CC support.  (System V stock system compiler.)  Use
      GCC on these platforms instead.

    o Added examples/README, explaining how to use the examples, and
      what they all do.

    o Most of the example programs now accept command line arguments for
      host name, user name and password, like resetdb does.

    o Renamed sinisa_ex example to dbinfo.

    o Several Standard C++ syntax fixes to quash errors emitted by
      GCC 3.4 and Borland C++ Builder 6.  Thanks to Steffen Schumacher
      and Totte Karlsson for their testing and help with these.

    o Added proper #includes for BCBuilder, plus project files for same.
      Thanks to Totte Karlsson for these.


1.7.12, 2004.08.19 (r63)

    o Many Standard C++ fixes, most from the GCC 3.4 patch by
      Rune Kleveland.

    o Added Wishlist file to distribution.

    o Fixed a problem in the bootstrap script that caused complaints
      from the autotools on some systems.

    o RPM building is working properly now.

    o Fixed the idempotency guard in datetime1.hh.


1.7.11, 2004.08.17 (r50)

    o Renamed mysql++, defs and define_short files, adding .hh to the
      end of each.  (They're header files!)  This shouldn't impact
      library users, since these are hopefully used internal to the
      library only.

    o Removed sqlplus.hh file.  Use mysql++.hh instead.
    
    o Added mysql++.spec, extracted from contributed 1.7.9 source RPM, 
      and updated it significantly.  Also, added an 'rpm' target to
      Makefile.am to automate the process of building RPMs.

    o Added bootstrap and LGPL files to distribution tarball.

    o Added pre-1.7.10 history to this file.

    o Removed .version file.  Apparently it's something required by old
      versions of libtool.


1.7.10, 2004.08.16 (r27)

    o Maintenance taken over by Warren Young (mysqlpp at etr dash usa
      dot com.) See http://lists.mysql.com/plusplus/3326 for details.

    o Applied many of the GCC 3.x patches submitted for 1.7.9 over
      the years.  This allows it to build on everything from 3.0 to
      3.3.3, at least.  Because so many patches are rolled up in one
      big jump, it's difficult to describe all the changes and where
      they came from.  Mostly they're Standard C++ fixes, as GCC
      has become more strict in the source code that it will accept.

    o MysqlRow used to overload operator[] for string types as well as
      integers so you could look up a field by its name, rather than by
      its index.  GCC 3.3 says this is illegal C++ due to ambiguities in
      resolving which overload should be used in various situations.
      operator[] is now overloaded only for one integer type, and a
      new member function lookup_by_name() was added to maintain the old
      by-field-name functionality.

    o Fixed another operator overloading problem in SSQLS macro
      generation with GCC 3.3.

    o The _table member of SSQLS-defined structures is now const char*,
      so you can assign to it from a const char* string.

    o Got autoconf/automake build system working with current versions
      of those tools again.  Removed the generated autotools files from
      CVS.

    o Renamed library file from libsqlplus to libmysqlpp.


1.7.9 (May 1 2001) Sinisa Milivojevic <sinisa@mysql.com>

    * Fixed a serious bug in Connection constructor when reading MySQL
    * options
    * Improved copy constructor and some other methods in Result /
    * ResUse
    * Many other minor improvements
    * Produced a complete manual with chapter 5 included
    * Updated documentation, including a Postscript format

1.7.8 (November 14 2000) Sinisa Milivojevic <sinisa@mysql.com>

    * Introduced a new, standard way of dealing with C++ exceptions.
    * MySQL++ now supports two different methods of tracing exceptions.
    * One is by the fixed type (the old one) and one is standard C++
    * type by the usage of what() method. A choice of methods has to be
    * done in building a library. If configure script is run with
    * -enable-exception option , then new method will be used. If no
    * option is provided, or -disable-exception is used, old MySQL++
    * exceptions will be enforced. This innovation is a contribution of
    * Mr. Ben Johnson <ben@blarg.net>
    * MySQL++ now automatically reads at connection all standard MySQL
    * configuration files
    * Fixed a bug in sql_query::parse to enable it to parse more then 99
    * char's
    * Added an optional client flag in connect, which will enable usage
    * of this option, e.g. for getting matched and not just affected
    * rows. This change does not require any changes in existing
    * programs
    * Fixed some smaller bugs
    * Added better handling of NULL's. Programmers will get a NULL
    * string in result set and should use is_null() method in ColData to
    * check if value is NULL
    * Further improved configuration
    * Updated documentation, including a Postscript format

1.7.6 (September 22 2000) Sinisa Milivojevic <sinisa@mysql.com>

    * This release contains some C++ coherency improvements and scripts
    * enhacements
    * result_id() is made available to programmers to fetch
    * LAST_INSERT_ID() value
    * Connection constroctur ambiguity resolved, thanks to marc@mit.edu
    * Improved cnnfigure for better finding out MySQL libraries and
    * includes
    * Updated documentation, including a Postscript format

1.7.5 (July 30 2000) Sinisa Milivojevic <sinisa@mysql.com>

    * This release has mainl bug fixes  and code improvements
    * A bug in FieldNames::init has been fixed, enabling a bug free
    * usage of this class with in what ever a mixture of cases that is
    * required
    * Changed behaviour of ResUse, Result and Row classes, so that they
    * could be re-used as much as necessary, without any memory leaks,
    * nor with any re-initializations necessary
    * Fixed all potential leaks that could have been caused by usage of
    * delete instead of delete[] after memory has been allocated with
    * new[]
    * Deleted all unused classes and macros. This led to a reduction of
    * library size to one half of the original size. This has
    * furthermore brought improvements in compilation speed
    * Moved all string manipulation from system libraries to
    * libmysqlclient, thus enabling uniformity of code and usage of 64
    * bit integers on all platforms, including Windows, without
    * reverting to conditional compilation. This changes now requires
    * usage of mysql 3.23 client libraries, as mandatory
    * Changed examples to reflect above changes
    * Configuration scripts have been largely changed and further
    * changes shall appear in consecutive sub-releases. This changes
    * have been done and shall be done by our MySQL developer Thimble
    * Smith <tim@mysql.com>
    * Changed README, TODO and text version of manual. Other versions of
    * manual have not been updated
    * Fixed .version ``bug''. This is only partially fixed and version
    * remains 1.7.0 due to some problems in current versions of libtool.
    * This shall be finally fixed in a near future
    * Several smaller fixes and improvements
    * Added build.sh script to point to the correct procedure of
    * building of this library. Edit it to add configure options of your
    * choice

1.7 (May17 2000) Sinisa Milivojevic <sinisa@mysql.com>

    * This is mainly a release dealing with bug fixes, consistency
    * improvements and easier configure on some platforms
    * A bug in fetch_row() method of ResUse class has been fixed. Beside
    * changes that existed in a distributed patch, some additional error
    * checking has been introduced
    * A bug in escape manipulator has been fixed that could cause an
    * error if all characters had to be escaped
    * An inconsistency in column indexing has been fixed. Before this
    * version, column names in row indexing with strings, i.e.
    * row[<string>] , has been case sensitive, which was inconsistent
    * with MySQL server handling of column names
    * An inconsistency in conversion from strings to integers or floats
    * has been fixed. In prior version a space found in data would cause
    * a BadConversion exception. This has been fixed, but 100%
    * consistency with MySQL server has not been targeted, so that other
    * non-numeric characters in data will still cause BadConversion
    * exception or error. As this API is used in applications, users
    * should provide feedback if full compatibility with MySQL server is
    * desired, in which case BadConversion exception or error would be
    * abolished in some of future versions
    * A new method in ColData class has been introduced. is_null()
    * method returns a boolean to denote if a column in a row is NULL.
    * Finally, as of this release, testing for NULL values is possible.
    * Those are columns with empty strings for which is_null() returns
    * true.
    * Some SPARC Solaris installations had C++ exception problems with
    * g++ 2.95.2 This was a bug that was fixed in GNU gcc, as from
    * release 2.95 19990728. This version was thoroughly tested and is
    * fully functional on SPARC Solaris 2.6 with the above version of
    * gcc.
    * A 'virtual destructor ' warning for Result class has been fixed
    * Several new functions for STL strings have been added. Those
    * functions (see string_util.hh) add some of the functionality
    * missing in existing STL libraries
    * Conversion for 64 bit integers on FreeBSD systems has been added.
    * On those systems _FIX_FOR_BSD_ should be defined in CXXFLAGS prior
    * to configuring. Complete conversion to the usage of functions for
    * integer conversion found in mysqlclient library is planned for one
    * of the next releases
    * A completely new, fully dynamic, dramatic and fully mutable result
    * set has been designed and will be implemented in some of 2.x
    * releases
    * Several smaller fixes and improvements, including defaulting
    * exceptions to true, instead of false, as of this version
    * An up-to-date and complete Postscript version of documentation is
    * included in this distribution
    * Large chunks of this manual are changed, as well as README and
    * TODO files.

1.6 (Feb 3 2000) Sinisa Milivojevic <sinisa@mysql.com>

    * This is a major release as it includes new features and major
    * rewrites
    * Automatic quoting and escaping with streams. It works
    * automatically , depending on the column type. It will work with <<
    * on all ostream derived types. it is paricularly handy with query
    * objects and strstreams. Automatic quoting and escaping on cout,
    * cerr and clog stream objects is intentionally left out, as quoting
    * / escaping on those stream objects is not necessary. This feature
    * can be turned of by setting global boolean dont_quote_auto to
    * true.
    * Made some major changes in code, so that now execute method should
    * be used only with SSQL and template queries, while for all other
    * query execution of UPDATE's, INSERT's, DELETE's, new method exec()
    * should be used. It is also faster.
    * New method get_string is inroduced for easier handling / casting
    * ColData into C++ strings.
    * Major rewrite of entire code, which led to it's reduction and
    * speed improvement. This also led to removal of several source
    * files.
    * Handling of binary data is introduced. No application program
    * changes are required. One of new example programs demonstrates
    * handling of binary data
    * Three new example programs have been written and thoroughly
    * tested. Their intention is to solve some problems addressed by
    * MySQL users.
    * Thorough changes is Makefile system has been made
    * Better configuration scripts are written, thanks to D.Hawkins
    * <dhawkins@cdrgts.com>
    * Added several bug fixes
    * Changed Manual and Changelog

1.5 (Dec 1 1999) Sinisa Milivojevic <sinisa@mysql.com>

    * Fixed bug in template queries, introduced in 1.4 (!)
    * Fixed connect bug
    * Fixed several bug in type_info classes
    * Added additional robustness in classes
    * Added additional methods for SQL type info
    * Changed Changelog and README

1.4 (Nov 25 1999) Sinisa Milivojevic <sinisa@mysql.com>

    * Fixed bug in store and storein methods
    * Fixed one serious memory leak
    * Fixed a very serious bug generated by gcc 2.95.xx !!
    * Added robustness in classes, so that e.g. same query and row
    * objects can be re-used
    * Changed sinisa_ex example to reflect and demonstrate this
    * stability
    * Changed Changelog and README
    * Few other bug fixes and small improvements and speed-ups

1.3 (Nov 10 1999) Sinisa Milivojevic <sinisa@mysql.com>

    * Fixed several erronous definitions
    * Further changed source to be 2.95.2 compatible
    * Expunged unused statements, especially dubious ones, like use of
    * pointer_tracker
    * Corrected bug in example file fieldinf1
    * Finally fixed mysql_init in Connection constructor, which provided
    * much greater stability !
    * Added read and get options, so that clients, like mysqlgui can use
    * it
    * Changed Changelog and README
    * Many other bug fixes.

1.2 (Oct 15 1999) Sinisa Milivojevic <sinisa@mysql.com>

    * First offical release. Version 1.0 and 1.1 were releases by Sinisa
    * before I (Kevin Atkinson) made him the offical maintainer,
    * Many manual fixes.
    * Changed README and Changelog
    * Changed source to be compilable by gcc 2.95.xx, tribute to Kevin
    * Atkinson <kevinatk@home.com>
    * Added methods in Connection class which are necessary for
    * fullfilling administrative functions with MySQL
    * Added many bug fixes in code pertaining to missing class
    * initializers , as notified by Michael Rendell <michael@cs.mun.ca>
    * Sinisa Milivojevic <sinisa@mysql.com> is now the offical
    * maintainer.

1.1 (Aug 2 1999) Sinisa Milivojevic <sinisa@mysql.com>

    * Added several bug fixes
    * Fixed memory leak problems and variables overlapping problems.
    * Added automake and autoconf support by loic@ceic.com
    * Added Makefile for manual
    * Added support for cygwin
    * Added example sinisa_ex (let modesty prevail) which used to crash
    * a lot when memory allocation, memory leak and overlap problems
    * were present. Smooth running of this example proves that all those
    * bugs are fixed
    * Corrected bugs in sql_query.cc regarding delete versus delete[]
    * and string length in manip.cc
    * Changed manual
    * Changed README
    * Many other smaller things

1.0 (June 9 1999) Michael Widenius <monty@monty.pp.sci.fi>

    * Added patches from Orion Poplawski <orion@bvt.com> to support the
    * UnixWare 7.0 compiler

.64.1.1a (Sep 27 1998)

    * Fixed several bugs that caused my library to fail to compile with
    * egcs 1.1. Hopefully it will still compile with egcs 1.0 however I
    * have not been able to test it with egcs 1.0.
    * Removed some problem causing debug output in sql++pretty.

.64.1a (Aug 1 1998)

    * Added an (almost) full guide to using Template Queries.
    * Fixed it so the SQLQuery will throw an exception when all the
    * template parameters are not provided.
    * Proofread and speedchecked the manual (it really needed it).
    * Other minor document fixes.

.64.0.1a (July 31 1998)

    * Reworked the Class Reference section a bit.
    * Minor document fixes
    * Added more examples for SSQLS.
    * Changed the syntax of equal_list for SSQLS from equal_list (cchar
    * *, Manip, cchar *) to (cchar *, cchar *, Manip).
    * Added set methods to SSQLS. These new methods do the same thing as
    * there corresponding constructors.
    * Added methods for creating a mysql_type_info from a C++ type_info.

.64.a (July 24 1998)

    * Changed the names of all the classes so they no longer have to
    * have Mysql in the begging of it. However if this creates a problem
    * you can define a macro to only use the old names instead.
    * The Specialized SQL Structures (formally known as Custom Mysql
    * Structures) changed from mysql_ to sql_.
    * Added the option of using exceptions thoughout the API.
    * ColData (formally known as MysqlStrings) will now throw an
    * exception if there is a problem in the conversion.
    * Added a null adapter.
    * Added Mutable Result Sets
    * Added a very basic runtime type identification for SQL types
    * Changed the document format from POD to LYX .
    * Am now using a modified version of Perceps to extract the class
    * information directly from the code to make my life easier.
    * Added an option of defining a macro to avoid using the automatic
    * conversion with binary operators.
    * Other small fixed I probully forgot to mentune.

.63.1.a

    * Added Custom Mysql Structures.
    * Fixed the Copy constructor of class Mysql
    * Started adding code so that class Mysql lets it children now when
    * it is leaving
    * Attempted to compile it into a library but still need help. As
    * default it will compile as a regular program.
    * Other small fixes.

.62.a (May 3 1998)

    * Added Template Queries
    * Created s separate SQLQuery object that is independent of an SQL
    * connection.
    * You no longer have to import the data for the test program as the
    * program creates the database and tables it needs.
    * Many small bug fixes.

.61.1.a (April 28 1998)

    * Cleaned up the example code in test.cc and included it in the
    * manual.
    * Added an interface layout plan to the manual.
    * Added a reverse iterator.
    * Fixed a bug with row.hh (It wasn't being included because of a
    * typo).

.61.0.a

    * Major interface changes. I warned you that the interface may
    * change while it is in pre-alpha state and I wasn't kidding.
    * Created a new and Separate Query Object. You can no longer execute
    * queries from the Mysql object instead you have to create a query
    * object with Mysql::query() and use it to execute queries.
    * Added the comparison operators to MysqlDate, MysqlTime and
    * MysqlDateTime. Fixed a few bugs in the MysqlDate... that effected
    * the stream output and the conversion of them to strings.
    * Reflected the MysqlDate... changes in the manual.
    * Added a new MysqlSet object and a bunch of functions for working
    * with mysql set strings.

.60.3a (April 24 1998)

    * Changed strtoq and strtouq to strtoll and strtull for metter
    * compatibility Minor Manual fix.
    * Changed makefile to make it more compatible with Solaris (Thanks
    * Chris H)
    * Fixed bug in comparison functions so that they would compare in he
    * right direction.
    * Added some items to the to do list be sure to have a look.

