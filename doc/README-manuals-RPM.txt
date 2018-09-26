Patches for any of these thoughtfully considered!  See the HACKERS file
for instructions on sending patches.

The items in the bug fix/maintenance section are the easiest to do
without breaking things, so if you're looking for a project....

Here's a meta-item that doesn't really fit into any of the categories
below: any time you must hand-roll some SQL code in your program,
consider whether it could be reduced to an API feature that would be
widely useful.  Patches or proposals of this sort are always welcome.


v2.1 Plan
---------

	This plan is not set in stone.	These are simply the features
	we want to try and tackle for the v2.1 release.

	Items in this plan may slip to a future release.  This
	typically happens when the proper solution is unclear,
	so the best way to prevent this is to get on the mailing
	list and help discuss it.  Or even better, provide a patch;
	we rarely reject working code outright.

	Items from the following sections may make it in, but if
	you don't help make that happen, this will just be on the
	whim of one of MySQL++'s developers.  Don't forget that it's
	possible to subclass yourself from the "MySQL++'s developers"
	base class.


	o Transaction support.	Create a "Transaction" class, an
	  object of which you create on the stack, giving it a
	  reference to the Connection object.  Transaction object's
	  ctor calls a function on the Connection object to start
	  a transaction set, and its dtor calls another function to
	  commit the transaction.  Also provide a "commit()" member
	  function, to commit before destruction.  This has a couple
	  of uses.  First, it's useful for avoiding exceptions coming
	  from ~Transaction(), if such a thing is possible.  Second,
	  sometimes it's inconvenient to wait until the end of a block
	  to commit the transaction, and adding artifical blocks is
	  somewhat ugly.

	o Add a configure script option to allow the new lock
	  mechanism to use platform mutexes via the Boost.Threads
	  library.

	  Mechanism must reflect these MySQL C API restrictions:

	  - Only one query executing at once per connection

	  - For "use" queries, Connection (and therefore Query) object must
	    remain locked until last row is consumed

	  - Safest to have one Connection per thread.  Rules for sharing:
	    http://dev.mysql.com/doc/mysql/en/threaded-clients.html

	  Need some way to call mysql_thread_init() and
	  mysql_thread_end() per thread.  Also, work in some way to
	  call mysql_thread_safe() automatically, perhaps the first
	  time through the function that calls mysql_thread_init().
	  If the C API library reports that it is not thread-safe,
	  report this to the caller, perhaps through an exception,
	  or simply by refusing to init more than one thread.

	o Currently, all overloads for Query's execute(), store()
	  and use() methods eventually call the const char*
	  version, which does the actual work of executing the query.
	  This rules out query strings with embedded nulls, as you're
	  likely to get with BLOB columns.  Also, it means MySQL++
	  must scan the string for length in a few places.  The C API
	  isn't limited in this way if you use mysql_real_query(),
	  but you need an accurate length value to call it.  We could
	  get that length with binary data if the end of the call
	  chain were a std::string overload, but we can't do that
	  easily because each of these functions has a version taking
	  a SQLString (a subclass of std:string) for template queries.

	  One way around this is to add a parallel set of functions
	  (e.g. do_execute(), or execute_(), or some such) that take
	  a single std::string, which are the new terminus of the call
	  chain.  Reimplement const char* versions in terms of these.

	  Another way is to rename the template query versions (e.g. to
	  execute_tq()) to avoid the overload conflict.  With that
	  done, we can use C API functions like mysql_real_query(),
	  which can take binary data.

	  Yet another way is to add a length parameter to the call
	  chain end functions.

	  And finally, we may be able to co-opt the first template
	  query version of each of these functions, as it takes a
	  single SQLString.

	o Date and time classes are pretty minimalistic; they could
	  be so much more powerful.  Some ideas:

	  - Add time_t conversion.

	  - Arithmetic features.  (See "Algorithms for Programmers"
	    by Binstock and Rex.)

	  - It may be possible to get some nice syntactic sugar,
	    such as a way to call SQL functions like NOW() when
	    inserting certain Date/Time objects into a Query stream.

	  Don't forget to write an example showing how to use these
	  new mechanisms.

	  It may be possible to find existing date and time classes
	  that can be extended, instead of reinventing the wheel.
	  Boost, perhaps?

	o When you create a Connection object with its default ctor and
	  don't .connect() it, several of its functions can fail to
	  work correctly.  ping(), for one, because the MYSQL handle
	  isn't initialized until the connection is established.
	  Decide how to cope: a) init the handle in all ctors; b)
	  throw ObjectNotInitialized when the library knows the
	  call will fail; or c) just return a failure code and hope
	  the user is checking it.  Could be a different answer for
	  each function.  Keep in mind the consequences for database-
	  independence here.

	o Build a forward iterator mechanism for ResUse.  Make it
	  general enough that you can use it with STL algorithms
	  like find_if().  Then make an example to demonstrate this
	  augmentation of SELECT.  Also, update usequery example
	  to use the iterator.	Tricky bit: how do we make it not
	  interfere with subclass Result's random-access iterator?

	o Write an example to demonstrate compare.h features, or throw
	  the header away.  It's not being used within the library.

	o Have resetdb create a second table containing a BLOB column
	  that load_file and cgi_image can use.  
	  
	  Rework load_image to take the standard command line
	  parameters, and load a JPEG or something into the BLOB table.
	  Include a suitable JPEG with the distribution.  (A cheesy
	  Photoshopped "MySQL++ Rocks!" thing should suffice.)

	  Rework cgi_image so that you can drop it into a cgi-image
	  directory and immediately use it to query the database and
	  return the image data in CGI format.

	o It may be possible to optimize the use of ColData in
	  the return from Row::operator[].  Currently, that operator
	  returns a temporary ColData object, which contains a
	  std::string buffer which is initialized by a const char*
	  pointer to data within the Row object.  Since the ColData
	  object is temporary, you currently must copy the data a
	  second time to store it when using Row::operator[].  If the
	  end user just wants a const char*, this double copy could
	  be prevented.  See http://lists.mysql.com/plusplus/4451
	  for the proposal.


Future Features
---------------

	These changes are planned for versions after v2.1.  If you
	need one of these changes to happen on some particular
	schedule, the best way to ensure it is to start coding and
	provide a patch!


	o Define operator<< for Fields, Row, ResUse, etc.  In other
	  words, there should be a way to get a user-readable version
	  of received data without a lot of code.  Perhaps use a CSV
	  output format, or a mysql(1) one (ASCII grid).

	o Abstract all uses of MySQL C API functions into a database
	  driver class with a generic interface.  This is a step
	  towards database-independence, without the parallel class
	  hierarchy required by the MySQL++ 1.7 design.  Also, it
	  will make it easier to make class Connection completely
	  friend-less.	Right now, the main reason it needs friends
	  is because these other classes make C API calls using its
	  private MYSQL data member.  The other reasons for it having
	  friends aren't nearly as compelling, so it wouldn't be
	  hard to justify redesigning Connection to eliminate these
	  final reasons.

	  While it would be easy to have just one global database
	  driver object, it's probably going to be necessary to have
	  one per Connection.  Consider what happens when you have one
	  program connected to two very different MySQL databases,
	  and you indirectly call C API functions that take MYSQL
	  parameters.  It's likely that those calls are supposed
	  to behave different, depending on the data in that MYSQL
	  object; for instance, different character encodings in the
	  selected databases.  So, there must somehow be a way to pass
	  the database driver's instance pointer down to all objects
	  that will need to use the driver.  A side benefit is that
	  a single program could talk to multiple different database
	  server types.  Imagine a program for importing data from
	  PostgreSQL and loading it into a MySQL table, for instance.

	o manip.cpp uses mysql_escape_string(), which doesn't take the
	  selected database's character set into account.  To do that,
	  you must use mysql_real_escape_string(), which differs
	  by taking a MYSQL instance as an additional parameter.
	  The problem is, Connection owns the relevant MYSQL instance,
	  and the manipulator functionality is implemented in global
	  functions (operator<<() and such) so they have no direct
	  access to the relevant Connection object.

	  The key question for all operator<<'s for manipulators
	  to ask is, "which Query object am I being inserted into?"
	  From there, you can look up the associated Connection object.

	  In some cases, this answer to the question is easy, because
	  the operator takes an ostream parameter, which can be
	  dynamically cast to Query.  From there, it's just a lookup
	  table problem.
	  
	  Other operator<<'s don't take an ostream, but they do take
	  a manipulator.  Right now, manipulators are just enum values,
	  but they could in fact be classes.  Classes can carry data,
	  so there may be a way to "bind" them to the appropriate
	  Connection object.  If not, then perhaps some other method
	  will pop out of the database driver class idea.  The driver
	  object may be able to look up a suitable Connection object
	  for the manipulators.

	o SSQLS structures include some static elements (_table and
	  names[]), which are defined within the macro.  If you
	  put an SSQLS declaration in a header file and #include
	  that from multiple locations, you get a multiply-defined
	  symbol warning.  Some ways to separate the definition from
	  the declaration:

		  o Give the SSQLS macros another parameter, to
		    suppress static definition.

		  o Put statics in a separate macro, which the
		    user must instantiate once in a .cpp file.
		    (Similar to the way MFC message maps work.)

		  o Put statics in a sub-macro, conditionally defined,
		    which SSQLS is implemented in terms of.  Define the
			condition macro in one module within your
			program.

		  o Redesign the SSQLS mechanism entirely.  Instead of
		    defining SSQLSes by instantiating macros, you could
		    declare the structure in, say, an XML format,
		    which could be tranformed (XSLT?  Perl + a DOM
		    parser?) into code very much like in the current
		    SSQLS macros, except that it would generate
		    separate .cpp and .h files for each structure.
		    In addition to solving the static member problem,
		    it would have other advantages, like side-stepping
		    the Borland C++ macro size limit.

	o Deprecate sql_create_basic_* ? They have less functionality
	  and they're no easier to use than sql_create and friends,
	  so why bother with them?  I suppose the code generated
	  is a bit smaller, but *really*....  Only possible saving
	  grace is if BC++ can compile them due to the macro code
	  being shorter.  They also don't have the static members,
	  mentioned in the previous item.

	o Consider whether some of the current boilerplate can be
	  made into a base class that all SSQLSes derive from.
	  This may have implications for some templates, like
	  Query::insert<T>...they might become regular member
	  functions, taking a reference to the base class.

	o MySQL++ handles automatic quoting and escaping differently
	  for cout and cerr than for Query streams.  This should
	  probably be simplified so that automatic quoting is only
	  done for Query streams.  No other stream type should be
	  treated specially.

	o Some field_list() functions use the do_nothing manipulator,
	  while others use the quote manipulator.  Need to pick one.
	  In the vast majority of cases, quoting won't be necessary,
	  so make that the default.  But, it should be possible to turn
	  it on, if needed.  If all uses of quoting are in template
	  code, this can be a #define, allowing different programs
	  built on the same system to get different quoting rules.
	  Otherwise, it will probably have to be a configure script
	  flag, which will "burn" the choice into the built binary.

	o User-settable floating-point comparison precisions?
	  Something like this: http://lists.mysql.com/plusplus/3984
	  As it currently stands, sql_cmp(double,double) is foolish.
	  One shouldn't do exact equality comparison on floating
	  point values.

	o Consider using MySQL C API enum constants in
	  mysql_type_info::types definition instead of hard-coded
	  values.  This could potentially break a lot of
	  infrastructure, though, so do it only with care.


Bug Fix/Maintenance Items
-------------------------

	These items could happen in any version.


	o 64-bit integer support has been reported to work, but more
	  confirmation is wanted.

	o Template ListInsert in lib/myset.h isn't being used within the
	  library.  It could probably be used in place of SetInsert
	  in the same file, which things like type_info.h do use now,
	  but it isn't clear how one would go about doing that without
	  changing the library code.  Document it or throw it away.
