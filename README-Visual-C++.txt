Visual C++ Version Compatibility
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    MySQL++ is fully-functional with Visual C++ 2005 and 2008.

    MySQL++ also works with VS 2003, with the exception of the SSQLS
    feature.  There was partial support for SSQLS in the MySQL++
    v2 series, but a feature we added in MySQL++ v3.0 crashes the
    compiler, so we had to remove support for it entirely.  You don't
    need to change anything to use MySQL++ v3 with Visual C++ 2003.
    The SSQLS parts of the library simply don't get built when you
    use this compiler.

    Older versions of Visual C++ are basically hopeless when it comes
    to having sufficient support for Standard C++ to build MySQL++.
    There is an ancient hacked version of MySQL++ on the official
    MySQL++ web site that works with Visual C++ 6 and up, but my
    advice if you're on so old a compiler is that you're better off
    programming straight to the C API, if only because you can still
    get support for it.

    There are two sets of .sln and .vcproj files shipped with MySQL++:
    one for Visual C++ 2003 in the vc2003 subdirectory, and another
    set for VC++ 2005 and newer in vc2005.  The only difference
    between them is that the VC++ 2003 versions leave out several
    things from the build for compatibility reasons.


Prerequisites
~~~~~~~~~~~~~
    You need to have the Windows version of the MySQL server installed
    on your development system, even if you always access a MySQL
    server on a different machine.  This is because in addition to
    installing the server itself, the official MySQL Windows binaries
    also install the client-side development files that MySQL++
    needs in order to communicate with a MySQL server.

    Historically, the Windows version of the MySQL server installer
    has sometimes installed the development files by default, and
    sometimes not.  If you get an error about mysql-version.h or
    mysql.h when building MySQL++, you might need to go back and
    reinstall the server and select a custom install to enable
    installation of the development files.

    If you're getting these header file errors and are certain the
    development files are indeed installed, read on in the following
    section.


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

    You must build both the Debug and Release versions of the library.
    You will need them both because when you build your program in
    Release mode, it won't work with a MySQL++ DLL built in Debug mode.
    The simplest way to ensure that you're running your program against
    the correct DLL is to copy the debug version of the MySQL++ DLL
    into your program's Debug build directory, and the same for the
    Release directory.

    With the library built, run at least the resetdb and simple1
    examples to ensure that the library is working correctly.
    In addition to the other generic examples, there are a few
    Visual C++ specific examples that you might want to look at in
    examples\vstudio.  See README-examples.txt for further details.

    Once you're sure the library is working correctly, you can run
    the install.bat file at the project root to automatically install
    the library files and headers in subdirectories under c:\mysql++.


Using MySQL++ in an MFC Project
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    If you don't already have a project set up, open Visual Studio, say
    File::New::Project, then choose Visual C++::MFC::MFC Application.
    Go through the wizard setting up the project as you see fit.

    Once you have your project open, right click on your top-level
    executable in the Solution Explorer, choose Properties, and make
    the following changes.  (Where it doesn't specify Debug or Release,
    make the change to both configurations.)

        o Append the following to C/C++::General::Additional Include
          Directories:

            C:\Program Files\MySQL\MySQL Server 5.0\include,
            C:\mysql++\include

        o Under C/C++::Code Generation change "Runtime Library" to
          "Multi-threaded Debug DLL (/MDd)" for the Debug
          configuration.  For the Release configuration, make it
          "Multi-threaded DLL (/MD)".

        o Append the following to Linker::General::Additional Library
          Directories for the Debug configuration:

            C:\Program Files\MySQL\MySQL Server 5.0\lib\debug,
            C:\mysql++\vc\debug

          For the Release configuration, make it the same, but
          change the 'debug' directory names to 'opt'.

        o Under Linker::Input add the following to "Additional
          Dependencies" for the Debug configuration:
          
            libmysql.lib wsock32.lib mysqlpp_d.lib

          ...and then for the Release configuration:
          
            libmysql.lib wsock32.lib mysqlpp.lib

          This difference is because MySQL++'s Debug DLL and import
          library have a _d suffix so you can have both installed
          without conflicts.

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

        o Under Configuration Properties::General, change "Common
          Language Runtime support" to the /clr setting.

        o Under C/C++::Command Line, remove the /EHsc from the
          Additional Options section.

    If you have already built MySQL++, be sure to perform a complete
    rebuild after changing these options.  The compiler will emit
    several C4835 warnings after making those changes, which are
    harmless when using the DLL with a C++/CLI program, but which warn
    of real problems when using it with unmanaged C++.  As a result,
    it's probably best if you don't install the resulting DLL in a
    system level directory.  I'd recommend copying it only into the
    same directory as the EXE.

    Once you have MySQL++ built with CLR support, open your
    program's project.  If you don't already have a project set up,
    open Visual Studio, say File::New::Project, then choose Visual
    C++::CLR::Windows Forms Application.  Go through the wizard
    setting up the project as you see fit.

    The configuration process isn't much different from that for an
    MFC project, so go through the list above first.  Then, make the
    following changes particular to .NET and C++/CLI:

        o Under Configuration Properties::General change the setting
          from /clr:pure to /clr.  (You need mixed assembly support
          to allow a C++/CLI program to use a plain C++ library
          like MySQL++.)

        o For the Linker::Input settings, you don't need wsock32.lib.
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
    MySQL++'s Visual Studio project files aren't maintained directly.
    Instead, we use a tool called Bakefile (http://bakefile.org/)
    to generate many different project file and Makefile types from a
    single set of source files.  There is a native Windows version of
    Bakefile up on that web site.  Download that and put the directory
    containing bakefile.exe in your Windows PATH.

    Bakefile generates the various project files and Makefiles from
    a single source file, mysql++.bkl.  This is usually the file you
    need to change when you want to submit some change to the MySQL++
    build system.

    While Bakefile's documentation isn't as comprehensive as it
    ought to be, you can at least count on it to list all of the
    available features.  So, if you can't see a way to make Bakefile
    do something, it's likely it just can't do it.  Bakefile is a
    high-level abstraction of build systems in general, so it'll never
    support all the particulars of every odd build system out there.

    Once you've made your changes, you can generate the Visual C++
    project files by running rebake.bat, which you can find in the
    same directory as this README file.


If You Run Into Problems...
~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Especially if you have linking problems, make sure your project
    settings match the above.  Visual C++ is very picky about things
    like run time library settings.  When in doubt, try running one
    of the example programs.  If it works, the problem is likely in
    your project settings, not in MySQL++.

