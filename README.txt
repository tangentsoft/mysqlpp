Visual C++ Version Compatibility
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    MySQL++ is fully functional with Visual C++ 2005 and 2008.

    MySQL++ also works with VC++ 2003 (a.k.a. VC++ 7.1), with the
    exception of the SSQLS feature.  There was partial support for
    SSQLS with VC++ 2003 in MySQL++ v2, but a feature we added in
    MySQL++ v3.0 crashes this version of the compiler, so we had to
    remove support for it entirely.  MySQL++ automatically disables
    SSQLS when you build it with VC++ 2003.  As long as you avoid
    using that feature of the library, you should be fine.

    Older versions of Visual C++ are basically hopeless when it
    comes to building current versions of MySQL++.  They have too
    many weaknesses in their Standard C++ implementation to build a
    modern library like MySQL++.  If you cannot upgrade your compiler,
    my advice is that you're best off programming straight to the
    MySQL C API rather than try to make MySQL++ build.

    There are two sets of .sln and .vcproj files shipped with MySQL++:
    one for Visual C++ 2003 in the vc2003 subdirectory, and another
    set for VC++ 2005 and newer in vc2005.  The only difference
    between them is that the VC++ 2003 versions omit SSQLS related
    files from the build.  If you're using VC++ 2008, use the vc2005
    project files; Visual Studio will walk you through the conversion
    process, which will do the right thing.


Prerequisites
~~~~~~~~~~~~~
    You need to have the Windows version of the MySQL server installed
    on your development system, even if you always access a MySQL
    server on a different machine.  This is because in addition to
    installing the server itself, the official MySQL Windows binaries
    also install the client-side development files that MySQL++
    needs in order to communicate with a MySQL server.

    You have to do a Custom install to enable installation of these
    development files.  If you get an error about mysql-version.h or
    mysql.h when building MySQL++, go back and reinstall the MySQL
    server, paying careful attention to the options.

    If you've installed the development files and are still getting
    header file include errors, read on.


Building the Library and Example Programs
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    If you installed the MySQL server somewhere other than 
    
        C:\Program Files\MySQL\MySQL Server 5.0\
        
    you need to change the project file settings.  If you're willing to
    install Bakefile (see below), you can do this quickly by changing
    the variable MYSQL_WIN_DIR at the top of the file mysql++.bkl,
    then regenerating the project files by running rebake.bat.
    Otherwise, you'll need to change the include and library paths
    in all of the project files by hand.

    You must build both the Debug and Release versions of the library,
    because a release build of your program won't work with a Debug
    version of the MySQL++ DLL.  Since version 3.0, the VC++ build
    of MySQL++ names these two DLLs differently: mysqlpp_d.dll for
    the Debug version, and mysqlpp.dll for the Release version.
    This lets you keep them in the same PATH directory, without a
    concern as to whether the correct one will be used.

    With the library built, run at least the resetdb and simple1
    examples to ensure that the library is working correctly.
    In addition to the other generic examples, there are a few
    Visual C++ specific examples that you might want to look at in
    examples\vstudio.  See README-examples.txt for further details.

    Once you're sure the library is working correctly, you can run
    the install.hta file at the project root to install the library
    files and headers in a directory of your choosing.
    
    (Aside: You may not have come across the .hta extension before.
    It's for a rarely-used feature of Microsoft's Internet Explorer,
    called HTML Applications.  Know what Adobe AIR is?  Kinda like
    that, only without the compilation into a single binary blob,
    the cross-platform compatibility, and the power of Flash and
    ActionScript 3.  These limitations don't cause a problem here.
    In fact, AIR would be vast overkill here.  Just open install.hta
    in a text editor to see how it works.)


Using MySQL++ in an MFC Project
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    If you don't already have a project set up, open Visual Studio,
    say File > New > Project, then choose Visual C++ > MFC > MFC
    Application.  Go through the wizard setting up the project as
    you see fit.

    Once you have your project open, right click on your top-level
    executable in the Solution Explorer, choose Properties, and make
    the following changes.  (Where it doesn't specify Debug or Release,
    make the same change to both configurations.)

        o Append the following to C/C++ > General > Additional Include
          Directories:

            C:\Program Files\MySQL\MySQL Server 5.0\include,
            C:\mysql++\include

        o Under C/C++ > Code Generation change "Runtime Library" to
          "Multi-threaded Debug DLL (/MDd)" for the Debug
          configuration.  For the Release configuration, make it
          "Multi-threaded DLL (/MD)".

        o Append the following to Linker > General > Additional Library
          Directories for the Debug configuration:

            C:\Program Files\MySQL\MySQL Server 5.0\lib\debug,
            C:\mysql++\vc\debug

          For the Release configuration, make it the same, but
          change the 'debug' directory names to 'opt'.

        o Under Linker > Input add the following to "Additional
          Dependencies" for the Debug configuration:

            libmysql.lib wsock32.lib mysqlpp_d.lib

          ...and then for the Release configuration:
          
            libmysql.lib wsock32.lib mysqlpp.lib

          This difference is because MySQL++'s Debug DLL and import
          library have a _d suffix so you can have both in the same
          directory without conflicts.

    You may want to study examples\vstudio\mfc\mfc.vcproj to see
    this in action.  Note that some of the paths will be different,
    because it can use relative paths for mysqlpp.dll.


Using MySQL++ in a Windows Forms C++/CLI Project
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Before you start work on getting MySQL++ working with your own
    program, you need to make some changes to the MySQL++ build
    settings.  Open mysqlpp.sln, then right-click on the mysqlpp
    target and select Properties.  Make the following changes for
    both the Debug and Release configurations:

        o Under Configuration Properties > General, change "Common
          Language Runtime support" to the /clr setting.

        o Under C/C++ > Code Generation, change "Enable C++ Exceptions"
          from "Yes (/EHsc)" to "Yes With SEH Exceptions (/EHa)"

    If you have already built MySQL++, be sure to perform a complete
    rebuild after changing these options.  The compiler will emit
    several C4835 warnings after making those changes, which are
    harmless when using the DLL with a C++/CLI program, but which
    warn of real problems when using it with unmanaged C++.  This is
    why MySQL++'s Windows installer (install.hta) offers the option
    to install the CLR version into a separate directory; use it if
    you need both managed and unmanaged versions installed!

    For the same reason, you might give some thought about where you
    install mysqlpp.dll on your end user's machines when distributing
    your program.  My recommendation is to install it in the same
    directory as the .exe file that uses it, rather than installing
    into a system directory where it could conflict with a mysqlpp.dll
    built with different settings.

    Once you have MySQL++ built with CLR support, open your program's
    project.  If you don't already have a project set up, open Visual
    Studio, say File > New > Project, then choose Visual C++ > CLR >
    Windows Forms Application.  Go through the wizard setting up the
    project as you see fit.

    The configuration process isn't much different from that for an
    MFC project, so go through the list above first.  Then, make the
    following changes particular to .NET and C++/CLI:

        o Under Configuration Properties > General change the setting
          from /clr:pure to /clr.  (You need mixed assembly support
          to allow a C++/CLI program to use a plain C++ library
          like MySQL++.)

        o For the Linker > Input settings, you don't need wsock32.lib.
          The mere fact that you're using .NET takes care of that
          dependency for you.

    In the MFC instructions above, it said that you need to build it
    using the Multi-threaded DLL version of the C++ Runtime Library.
    That's not strictly true for MFC, but it's an absolute requirement
    for C++/CLI.  See the Remarks in this MSDN article for details:

        http://msdn2.microsoft.com/en-us/library/k8d11d4s.aspx

    You may want to study examples\vstudio\wforms\wforms.vcproj to
    see all this in action.  Note that some of the paths will be
    different, because it can use relative paths for mysqlpp_d.dll
    and mysqlpp.dll.


Working With Bakefile
~~~~~~~~~~~~~~~~~~~~~
    MySQL++'s top-level Visual Studio project files aren't
    maintained directly.  Instead, we use a tool called Bakefile
    (http://bakefile.org/) to generate many different project file
    and Makefile types from a single set of source files.  There is
    a native Windows version of Bakefile up on that web site.
    Download that and put the directory containing bakefile_gen.exe
    in your Windows PATH.

    Bakefile generates the various project files and Makefiles from
    a single source file, mysql++.bkl.  This is usually the file you
    need to change when you want to make some change to the MySQL++
    build system.

    While Bakefile's documentation isn't as comprehensive as it
    ought to be, you can at least count on it to list all of the
    available features.  So, if you can't see a way to make Bakefile
    do something, it's likely it just can't do it.  Bakefile is a
    high-level abstraction of build systems in general, so it'll never
    support all the particulars of every odd build system out there.

    Once you've made your changes, you can generate the Visual C++
    project files by running rebake.bat, which you can find in the
    same directory as this file.


If You Run Into Problems...
~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Especially if you have linking problems, make sure your project
    settings match the above.  Visual C++ is very picky about things
    like run time library settings.  When in doubt, try running one
    of the example programs.  If it works, the problem is likely in
    your project settings, not in MySQL++.

