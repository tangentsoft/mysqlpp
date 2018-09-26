<?xml version="1.0" encoding='UTF-8'?>
<!DOCTYPE sect1 PUBLIC "-//OASIS//DTD DocBook V4.2//EN"
    "http://www.oasis-open.org/docbook/xml/4.2/docbookx.dtd">

<sect1 id="incorporating">
  <title>Using MySQL++ in Your Own Project</title>

  <para>Up to now, this manual has only discussed MySQL++
  in conjunction with the example programs that come with the
  library. This chapter covers the steps you need to take to
  incorporate MySQL++ into your own projects.</para>

  <para>The first thing you have to do is include
  <filename>mysql++.h</filename> in each module
  that uses MySQL++. In modules that use <link
  linkend="ssqls">SSQLS v1</link>, you also need to include
  <filename>ssqls.h</filename>.<footnote><para>MySQL++ has many header
  files, but the only one that isn&#x2019;t intertwined with the rest is
  <filename>ssqls.h</filename>. <filename>mysql++.h</filename> brings
  in all of the others in the correct order. Some have tried to speed
  their build times by finding a subset of MySQL++ headers to include,
  but <filename>mysql++.h</filename> already does as much of this as
  is practical. MySQL++&#x2019;s monolithic nature rules out finding
  a true subset of the library headers.</para></footnote></para>

  <para>At this point, your project probably still won&#x2019;t compile,
  and it certainly won&#x2019;t link. The remaining steps are dependent
  on the operating system and tools you are using. The rest of this
  chapter is broken up into several sections, one for each major
  platform type. You can skip over the sections for platforms you
  don&#x2019;t use.</para>


  <sect2 id="inc-vstudio">
    <title>Visual C++</title>

    <sect3 id="inc-vstudio-mfc">
      <title>Using MySQL++ in an MFC Project</title>

      <para>If you don&#x2019;t already have a project set up, open
      Visual Studio, say File | New | Project, then choose Visual C++
      | MFC | MFC Application.  Go through the wizard setting up the
      project as you see fit.</para>

      <para>Once you have your project open, right click on your
      top-level executable in the Solution Explorer, choose Properties,
      and make the following changes.  (Where it doesn&#x2019;t
      specify Debug or Release, make the same change to both
      configurations.)</para>

      <itemizedlist>
        <listitem><para>Append the following to C/C++
        | General | Additional Include Directories:
        <filename>C:\Program Files\MySQL\MySQL Server 5.0\include,
        C:\mysql++\include</filename></para></listitem>

        <listitem><para>Under C/C++ | Code Generation change
        &#x201C;Runtime Library&#x201D; to &#x201C;Multi-threaded
        Debug DLL (/MDd)&#x201D; for the Debug configuration. For
        the Release configuration, make it &#x201C;Multi-threaded DLL
        (/MD)&#x201D;.</para></listitem>

        <listitem>
          <para>Append the following to Linker | General |
          Additional Library Directories for the Debug configuration:
          <filename>C:\Program Files\MySQL\MySQL Server 5.0\lib\debug,
          C:\mysql++\vc\debug</filename></para>

          <para>For the Release configuration, make it the same,
          but change the &#x201C;debug&#x201D; directory names to
          &#x201C;opt&#x201D;.</para>
        </listitem>

        <listitem>
          <para>Under Linker | Input add the following to
          &#x201C;Additional Dependencies&#x201D; for the Debug
          configuration: <filename>libmysql.lib wsock32.lib
          mysqlpp_d.lib</filename></para>

          <para>...and then for the Release configuration:
          <filename>libmysql.lib wsock32.lib
          mysqlpp.lib</filename></para>

          <para>This difference is because MySQL++&#x2019;s Debug
          DLL and import library have a <filename>_d</filename>
          suffix so you can have both in the same directory without
          conflicts.</para>
        </listitem>
      </itemizedlist>

      <para>You may want to study
      <filename>examples\vstudio\mfc\mfc.vcproj</filename>
      to see this in action. Note that some of the paths will
      be different, because it can use relative paths for
      <filename>mysqlpp.dll</filename>.</para>
    </sect3>


    <sect3 id="inc-vstudio-wforms">
      <title>Using MySQL++ in a Windows Forms C++/CLI Project</title>

      <para>Before you start work on getting MySQL++ working with your
      own program, you need to make some changes to the MySQL++ build
      settings. Open <filename>mysqlpp.sln</filename>, then right-click
      on the mysqlpp target and select Properties. Make the following
      changes for both the Debug and Release configurations:</para>

      <itemizedlist>
        <listitem><para>Under Configuration Properties | General,
        change &#x201C;Common Language Runtime support&#x201D; to the
        /clr setting.</para></listitem>

        <listitem><para>Under C/C++ | Code Generation, change
        &#x201C;Enable C++ Exceptions&#x201D; from &#x201C;Yes
        (/EHsc)&#x201D; to &#x201C;Yes With SEH Exceptions
        (/EHa)&#x201D;</para></listitem>
      </itemizedlist>

      <para>If you have already built MySQL++, be sure to perform a
      complete rebuild after changing these options.  The compiler
      will emit several C4835 warnings after making those changes,
      which are harmless when using the DLL with a C++/CLI
      program, but which warn of real problems when using it with
      unmanaged C++.  This is why MySQL++&#x2019;s Windows installer
      (<filename>install.hta</filename>) offers the option to install
      the CLR version into a separate directory; use it if you need
      both managed and unmanaged versions installed!</para>

      <para>For the same reason, you might give some thought about
      where you install <filename>mysqlpp.dll</filename> on your
      end user&#x2019;s machines when distributing your program.
      My recommendation is to install it in the same directory as
      the <filename>.exe</filename> file that uses it, rather than
      installing into a system directory where it could conflict
      with a <filename>mysqlpp.dll</filename> built with different
      settings.</para>

      <para>Once you have MySQL++ built with CLR support, open your
      program&#x2019;s project.  If you don&#x2019;t already have a
      project set up, open Visual Studio, say File | New | Project,
      then choose Visual C++ | CLR | Windows Forms Application.
      Go through the wizard setting up the project as you see
      fit.</para>

      <para>The configuration process isn&#x2019;t much different from
      that for an MFC project, so go through the list above first.
      Then, make the following changes particular to .NET and
      C++/CLI:</para>

      <itemizedlist>
        <listitem><para>Under Configuration Properties | General
        change the setting from /clr:pure to /clr.  (You need mixed
        assembly support to allow a C++/CLI program to use a plain C++
        library like MySQL++.)</para></listitem>

        <listitem><para>For the Linker | Input settings, you
        don&#x2019;t need <filename>wsock32.lib</filename>. The mere
        fact that you&#x2019;re using .NET takes care of that dependency
        for you.</para></listitem>
      </itemizedlist>

      <para>In the MFC instructions above, it said that you need to
      build it using the Multi-threaded DLL version of the C++ Runtime
      Library. That&#x2019;s not strictly true for MFC, but it&#x2019;s
      an absolute requirement for C++/CLI. See the Remarks in <ulink
      url="http://msdn.microsoft.com/en-us/library/k8d11d4s.aspx">the
      MSDN article on the /clr switch</ulink> for details.</para>

      <para>You may want to study
      <filename>examples\vstudio\wforms\wforms.vcproj</filename>
      to see all this in action. Note that some of the
      paths will be different, because it can use relative
      paths for <filename>mysqlpp_d.dll</filename> and
      <filename>mysqlpp.dll</filename>.</para>
    </sect3>
  </sect2>


  <sect2 id="inc-unix">
    <title>Unixy Platforms: Linux, *BSD, OS X, Cygwin, Solaris...</title>

    <para>There are lots of ways to build programs on Unixy
    platforms.  We&#x2019;ll cover just the most generic way
    here, <filename>Makefile</filename>s. We&#x2019;ll use a very
    simple example so it&#x2019;s clear how to translate this
    to more sophisticated build systems such as GNU Autotools or
    Bakefile.</para>

    <para>&#x201C;Hello, world!&#x201D; for MySQL++ might look something
    like this:</para>

    <programlisting><xi:include href="hello.txt" parse="text"
    xmlns:xi="http://www.w3.org/2001/XInclude"/></programlisting>

    <para>Here&#x2019;s a <filename>Makefile</filename> for building
    that program:</para>

    <programlisting><xi:include href="Makefile.hello.posix" parse="text"
    xmlns:xi="http://www.w3.org/2001/XInclude"/></programlisting>

    <para>The <varname>*FLAGS</varname> lines are where all of the
    assumptions about file and path names are laid out. Probably at
    least one of these assumptions isn&#x2019;t true for your system,
    and so will require changing.</para>

    <para>The trickiest line is the <varname>LDLIBS</varname>
    one. MySQL++ programs need to get built against both the MySQL
    and MySQL++ libraries, because MySQL++ is built on top of the
    MySQL C API library<footnote><para>The MySQL C API library
    is most commonly called <filename>libmysqlclient</filename>
    on Unixy systems, though it is also known as <ulink
    url="https://dev.mysql.com/downloads/connector/c/">Connector/C</ulink>.</para></footnote>
    If you&#x2019;re building a threaded program, use
    <filename>-lmysqlclient_r</filename> instead of
    <filename>-lmysqlclient</filename> here. (See <xref
    linkend="threads"/> for more details on building thread-aware
    programs.)</para>

    <para>On some systems, the order of libraries in the
    <varname>LDLIBS</varname> line is important: these linkers collect
    symbols from right to left, so the rightmost library needs to
    be the most generic. In this example, MySQL++ depends on MySQL,
    so the MySQL C API library is rightmost.</para>

    <para>You might need to add more libraries to the
    <varname>LDLIBS</varname> line. <filename>-lnsl</filename>,
    <filename>-lz</filename> and <filename>-lm</filename> are
    common. If you study how MySQL++ itself gets built on your system,
    you can see what it uses, and emulate that.</para>

    <para>You may be wondering why we have used both
    <varname>LDLIBS</varname> and <varname>LDFLAGS</varname>
    here. Some <filename>Makefiles</filename> you have seen probably
    try to collect both types of flags in a single variable. Whether
    that works or not depends on where on the command line those
    flags appear. Since we&rsquo;re depending on the standard
    <filename>make</filename> rules here, we know have to separate
    the <option>-l</option> and <option>-L</option> flags due to the
    place they&rsquo;re inserted into the link command. If you were
    writing your own compilation rules, you could write them in such
    a way that you didn&rsquo;t have to do this.</para>

    <para>Beyond that, we have a pretty vanilla
    <filename>Makefile</filename>, thanks in large part to the fact
    that the default <filename>make</filename> rules are fine for
    such a simple program.</para>
  </sect2>


  <sect2 id="inc-osx">
    <title>OS X</title>

    <sect3 id="inc-osx-makefile">
      <title>Makefiles</title>

      <para>The <link linkend="inc-unix">generic
      <filename>Makefile</filename> instructions above</link>
      cover most of what you need to know about using Makefiles on
      OS X.</para>

      <para>One thing that may trip you up on OS X is that it uses an
      uncommon dynamic linkage system. The easiest way to cope with
      this is to link your executables with the compiler, rather than
      call <filename>ld</filename> directly.</para>

      <para>Another tricky bit on OS X is the concept of Universal
      binaries. See <filename>README-Mac-OS-X.txt</filename> for
      details on building a Universal version of the MySQL++ library,
      if you need one. By default, you only get a version tuned for
      the system type you build it on.</para>
    </sect3>

    <sect3 id="inc-osx-xcode">
      <title>Xcode</title>

      <para>I have no information on how to incorporate MySQL++ in
      an Xcode project. Send a message to the MySQL++ mailing list
      if you can help out here.</para>
    </sect3>
  </sect2>


  <sect2 id="inc-mingw">
    <title>MinGW</title>

    <sect3 id="inc-mingw-makefile">
      <title>Makefiles</title>

      <para>The <link linkend="inc-unix">generic
      <filename>Makefile</filename> instructions above</link> apply
      to MinGW&#x2019;s version of GNU <filename>make</filename> as
      well. You will have some differences due to the platform, so
      here&#x2019;s the adjusted <filename>Makefile</filename>:</para>

      <programlisting><xi:include
      href="Makefile.hello.mingw" parse="text"
      xmlns:xi="http://www.w3.org/2001/XInclude"/></programlisting>

      <para>Note that I&#x2019;ve used <command>del</command>
      instead of <command>rm</command> in the clean target. In
      the past, at least, MinGW <filename>make</filename>
      had some funny rules about whether commands in target
      rules would get run with <filename>sh.exe</filename>
      or with <filename>cmd.exe</filename>. I can&rsquo;t
      currently get my installation of MinGW to do anything
      but use <filename>sh.exe</filename> by default, but that
      may be because I have Cygwin installed, which provides
      <filename>sh.exe</filename>.  This explains the first
      line in the file, which overrides the default shell with
      <filename>cmd.exe</filename>, purely to get consistent
      behavior across platforms. If you knew all your platforms
      would have a better shell, you&rsquo;d probably want to use
      that instead.</para>

      <para>Note the use of forward slashes in the path to the MySQL
      Connector/C development files. GNU <filename>make</filename>
      uses the backslash as an escape character, so you&#x2019;d
      have to double them if you&#x2019;re unwilling to use forward
      slashes.</para>
    </sect3>

    <sect3 id="inc-mingw-ide">
      <title>Third-Party MinGW IDEs (Dev-C++, Code::Blocks...)</title>

      <para>I have no information on how to do this. We&#x2019;ve
      received reports on the mailing list from people that have made
      it work, but no specifics on what all needs to be done. The
      <filename>Makefile</filename> discussion above should give you
      some hints.</para>
    </sect3>
  </sect2>


  <sect2 id="inc-eclipse">
    <title>Eclipse</title>

    <para>As far as I can tell, the simplest way to build a C++ project
    with Eclipse is to set up a <filename>Makefile</filename> for it
    as described <link linkend="inc-unix">above</link>, then add an
    external run configuration for your local <filename>make</filename>
    tool. Get the project building from the command line with
    <filename>make</filename>, then go to Run | External Tools | Open
    External Tools Dialog and add a new launch configuration.</para>

    <para>For example, on my OS X system I use
    <filename>/usr/bin/gnumake</filename> for the program location
    and pick the project root with the Browse Workspace button to
    set the working directory.</para>
  </sect2>
</sect1>
