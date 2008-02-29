Building the Examples
~~~~~~~~~~~~~~~~~~~~~
    If you're installing MySQL++ from the source tarball, the example
    programs get built when you build the library.  If you change
    any example code, just say 'make' to rebuild the examples.
    The examples are built against the headers and library in the
    lib subdirectory, not against the ones you may have installed
    elsewhere on the system.

    If these example files were installed on your system as part of
    the -devel RPM, copy all the files to a directory you can write
    to, then say 'make' in that directory.  This uses a simplified
    Makefile, which builds the examples against the headers and
    libraries installed in the system directories.


Getting Started with the Examples
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    MySQL++ is built as a shared library on most systems, and a DLL
    on Windows.  Since it isn't built in the same directory as the
    examples, this means that your system won't be able to find the
    library without help until you install it.  Since you generally
    want to run the examples _before_ installing the library, to test
    that the library actually works, we need a workaround.

    That workaround is the exrun script.  There are two versions,
    a Bourne shell script called just exrun for POSIX systems, and
    exrun.bat for Windows.

    Before running the other examples, you must first create the
    sample database.  On POSIX systems, you do that like so:

        $ ./exrun resetdb [-s server_addr] [-u user] [-p password]

    On Windows, that would instead be:

        C:\mysql++\> exrun.bat resetdb [-s server] [-u user] [-p pass]

    You don't have to give any of these options.  If you don't pass -s,
    it assumes the database server is running on the same machine,
    and so tries to contact the server over some form of local IPC.
    If you don't pass -u, it uses your own user name when logging
    into to the database server.  If you don't pass -p, it assumes the
    database user has an empty password, which hopefully is not true.

    The -s option accepts many different forms of address.  The main
    one is some sort of TCP/IP address, with an optional port number
    or service name.  On Unixy systems, you can give a Unix domain
    socket name.  On Windows, you can give just a period to use named
    pipes, if the server supports it.  All of these are legal:

        .
        localhost
        172.20.0.252:12345
        /var/run/mysqld.sock
        my.server.name.com:mysql

    If you give -s but don't give a port number or service name with
    it, it assumes the default, port 3306.


Running the Other Command Line Examples
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    The following examples use the database set up by resetdb, and
    have the same command line format as resetdb:

        simple1: Shows the item names for all records in the sample
            stock table.  Try this one if you do nothing else.

        simple2: Similar to simple1, but it displays all columns
            from the sample stock table, not just the item name.

        simple3: Same as simple2, except that it retrieves the data
            with a "use" query instead of a "store" query.  See the
            user manual for the difference between these methods.

        ssqls1-5: These demonstrate the SSQLS features.  Read the
            SSQLS sections in the user manual for details about
            these examples.

        multiquery: MySQL++ allows you to issue multiple queries at
            once, and get the results as separate sets.  This shows
            that, and also how to use stored procedures, which return
            their results in the same way as a multiquery.

        tquery1-3: Shows how to use the template query facility.

        transaction: Shows how to use the Transaction class to create
            transaction sets which automatically roll back if not
            explicitly committed.

        deadlock: Shows how to handle errors that happen when the
            database server detects a deadlock in a transaction set.
            Also demonstrates the need for BadQuery::errnum()

        store_if: Demonstrates the Query::store_if() method, which
            allows you to store the results of a query in an STL
            container conditionally.  Think of it as a way to express
            rules for selecting records in C++, to be used when SQL's
            WHERE clause isn't powerful enough.

        for_each: Demonstrates the Query::for_each() method, which
            allows you to execute a query and call a functor on each
            returned row.  This example uses this to gather statistics
            on the sample table's contents.

        cpoolp, cpoolw: Respectively, the POSIX and Windows threads
            variants of the ConnectionPool class demonstration.

        load_jpeg: Inserts a JPEG file into the sample database,
            for use by the cgi_jpeg example.  (See below.)  Unlike the
            other examples, this one takes anything given on the
            command line that isn't a switch to be a JPEG file name.
            We've included examples/logo.jpg as a sample, if you want
            to use that.

        fieldinf: Shows how to get information about the fields in
            a result set.  (Types, etc.)

        dbinfo: Dumps a bunch of information about the database
            server and some of the data it's managing.

    If you run the load_jpeg example, you should consider also
    playing with the other half of the demonstration, cgi_jpeg.
    To run it, you'll need to install MySQL++ on a machine with
    a web server, then copy the cgi_jpeg program to the server's
    CGI directory.  For example, on a stock Red Hat type box,
    that would be /var/www/cgi-bin.  At that point, a request like
    http://my.server.com/cgi-bin/cgi_jpeg?id=1 should show the JPEG you
    loaded.  The ID value to use will be that reported by load_jpeg.


Dedicated Windows Examples
~~~~~~~~~~~~~~~~~~~~~~~~~~
    If you're a Visual C++ user, there are two examples specially
    created for you:

        examples\vstudio\mfc.vcproj: A GUI replacement for the simple2
            example, it simply logs in and displays the stock table's
            contents.  Uses good old C++ and MFC.

        examples\vstudio\wforms.vcproj: The same thing, only written in
            C++/CLI and using Windows Forms.  We needed a second
            example for two reasons.  First, string handling is quite
            different under .NET than in unmanaged C++.  Second, many
            developers are now using Visual Studio Express, which
            doesn't include MFC.  Please read README-Visual-C++.txt
            before trying to run this example.  The MySQL++ build
            settings must be modified before it will run correctly.

    Before trying to run these examples, run the updexdll.bat file
    in the top MySQL++ directory.  This copies the built mysqlpp.dll
    files into the Windows examples' EXE output directories so you
    can run them before you're ready to formally install the DLL on
    your system.

    Open examples\vstudio\mysqlpp_gui_examples.sln to begin working
    with these examples.  They only work correctly in Visual Studio
    2005 right now.  If you want to backport them to VS 2003, it's
    probably not hard, and we will accept patches for this.  The main
    difficulty is that VS 2003 supports Managed C++, which isn't the
    same thing as C++/CLI.

    The main justification for these examples is that Unicode is
    handled differently in Windows GUI programs than on the POSIX
    systems where MySQL++ was born and raised.  In earlier versions
    of MySQL++, the command line examples had very limited Unicode
    support on Windows, but it was unrealistic and confusing.  Now the
    command line examples don't even try to handle Unicode on Windows,
    leaving that up to these new GUI examples.


Special exrun Capabilities
~~~~~~~~~~~~~~~~~~~~~~~~~~
    The Bourne shell version of the exrun script has a few features
    not avaiable in the Windows batch file version.  These features
    let you run the examples under various debugging tools.

    You can get simple gdb debugging if you run an example like this:

        $ ./exrun gdb simple1 foo bar qux

    The script also supports valgrind, in memory leak testing mode:

        $ ./exrun valgrind simple1 foo bar qux
