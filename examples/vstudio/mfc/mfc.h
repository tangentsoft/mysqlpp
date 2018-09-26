Patches for any of these thoughtfully considered!  See the HACKERS.txt
file for instructions on sending patches.


Any Version
-----------
    o Any time you must hand-roll some SQL code in your program,
      consider whether it could be generalized to a widely-useful
      API feature.


v3.1 Tentative Plan
-------------------
    o SSQLS v2:

      - Switch from C macros to a DSL that is translated to .cpp and
        .h files by a tool built along with MySQL++ library.  Design:

        http://lists.mysql.com/plusplus/6929

      - Switch per SSQLS that makes it throw an exception when it sees
        a mismatch between DB schema and SSQLS definition.  Harkens
        back to pre v3.0 SSQLS behavior, where you'd get a crash as
        it tried to fill out fields by position, and fail.  For the
        B&D folk who don't like the new "just cope" behavior.

      - Try to design it so as much code as possible resides in
        a common base class (SsqlsBase) that all SSQLSes derive from.
        Can't do it in v1 because virtually everything is specific
        to the structure's type.  Study equal_list(), for example.
        Do it with an eye toward replacing Query's template methods
        taking SSQLSes with concrete methods taking SsqlsBase&.

      - Add truthiness operator to SSQLS to detect an incompletely-
        populated object?  Or maybe an incomplete() method?

      - Add features to ssqlsxlat to write SSQLSv2 declaration files
        from existing schemas extracted from CREATE TABLE statements,
        from running databases, and from C++ files containing old
        SSQLS v1 declarations.

      - Add table creation ability to SSQLS.  It has the schema...

      - Support per-instance table name overrides, instead of just
        per SSQLS?  Needed if you're going to use a single SSQLS for
        many tables with the same structure in a multithreaded program,
        so changing it statically isn't safe.

    o Chris Frey's packarray class

    o Create adaptors for std::bitset, for storing binary data in a
      MySQL table.  Make two options available, one for storing the
      return from bitset::to_ulong() in an UNSIGNED INTEGER column,
      and another for storing a larger set of bits in a more flexible
      way, perhaps as a BLOB.

    o field_list should use backticks to quote its items to handle
      spaces and other special characters.  Probably also remove all
      the manipulator stuff associated with these: no reason to make
      it user-settable, as there's only one right way to do it.  See:

         http://dev.mysql.com/doc/refman/5.0/en/identifiers.html

    o Add a general-purpose backtick manipulator as well.

    o Define operator<< for Fields, Row, StoreQueryResult, etc.  In other
      words, there should be a way to get a user-readable version of
      received data without a lot of code.  CSV format by default, and
      mysql(1)-like ASCII grid optionally, perhaps with a manipulator?
      There is grid code in examples/multiquery.cpp which we can use
      and then make multiquery.cpp a demonstration platform for it.
      Maybe dbinfo and fieldinf, too?

    o Has experience with new thread awareness changed our mind on
      atomic inc/dec of reference counts in RefCounted*?

    o Create a fixed-point data type for use with SQL's DECIMAL and
      related types.  Right now, sql_decimal is a typedef for double,
      so you lose accuracy in the fractional part.

    o Optional checked conversions in String for numerics: throw
      BadConversion on range overflow?


v4.0 or Later
-------------
    o Database independence:

      - Make DBDriver class purely abstract; move its entire functional
        contents to new MysqlDriver.

      - Must create at least two other DBDriver subclasses to
        ensure base class is reusable before releasing v4.0.
        PostgresDriver and SqlLiteDriver?

      - Templatize all classes that use DBDriver interface with the
        DB driver type.  This lets you specify the driver type to use
        with a Connection and all its children without modifying the
        existing method parameter lists.  This also lets us worry less
        about C API types, as they can be hidden away behind typedefs:

        class MysqlDriver : public DBDriver { ...
           typedef MYSQL_ROW row_type;
           ...
        }

        template <class DBD = MysqlDriver>
        class Connection ... { ...
           Query<DBD> query();
           ...
        }

        template <class DBD = MysqlDriver>
        class UseQueryResult { ...
            DBD::row_type fetch_raw_row();
        }

      - Tricky bits:
    
        - Initializing result set objects.

        - type_info module.  Extremely closely tied to MySQL C API
          right now.  Will probably have to turn it into a parallel
          class hierarchy to DBDriver, or fold it in with same.

        - Building MySQL++ on systems without autoconf.  How to
          specify what DB engines are available?  Probably default to
          supporting MySQL only, and let people turn things on manually
          as they need them.  Or, maybe make them use Bakefile so they
          can fiddle with the options if they want something atypical.

    o Some sort of support for prepared statements.  Can we hijack
      the template query mechanism?

    o If SSQLSv2 does use a common base class, change Query template
      methods taking SSQLS into concrete methods taking SsqlsBase&.

    o Make Query::insert(), replace() and update() execute their
      queries immediately.  Requires an ABI break, because they'll
      have to return SimpleResult.
