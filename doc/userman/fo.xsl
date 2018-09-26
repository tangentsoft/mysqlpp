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

	http://tangentsoft.net/mysql++/doc/userman/html/breakages.html

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

	o Added multiple result set handling to Query.	MySQL 4.1
	  and higher allow you to give multiple SQL statements in a
	  single "store" call, which requires extensions to MySQL++
	  so you can iterate through the multiple result sets.	Also,
	  stored procedures in MySQL 5.0 reportedly return multiple
	  result sets.	Thanks for the initial patch go to Arnon Jalon;
	  I reworked it quite a bit.

	o Query::storein*() now supports more varieties of the
	  nonstandard slist comtainer.	(Singly-linked version of
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
	  warnings with SSQLS.	Thanks to Wolfram Arnold for this patch.

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

	o Made some warning fixes for GCC 4.0.	Not all warnings are
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

