-   Update `ChangeLog.md`.

-   Change version number in `configure.ac`, `mysql++.bkl`, and
    `~/tangentsoft.com/mysqlpp/GNUmakefile`

    All other places the version number occurs are generated from the
    first two. If there’s a corresponding `*.in` file for the one you’re
    looking at, the version number was substituted in by autoconf from
    `configure.ac`. Otherwise, the file was most likely created by the
    build system using the version number in `mysql++.bkl`.

-   Run “`make abicheck`”. There should be no changes.

    You may have to run the following command in the current “stable”
    directory before this will succeed, since it depends on there being
    an ACC dump file in place already.

        $ abi-compliance-checker -lib mysqlpp -dump abi.xml

    (“Stable” is assumed to be in ../3.3.0 relative to the trunk
    checkout, as I write this.)

    This dependence on an existing ABI dump file is deemed reasonable
    since the ABI of the stable version had better not be changing!
    Plus, it saves some processing time, since ACC can load the stable
    ABI info without re-parsing its headers and library file.

-   Re-bootstrap the system in pedantic mode, then do a clean rebuild.
    Fix any new errors and warnings.

    Known bogus warnings:

    -   The base class `std::basic_ios<>` for `class Query` is not being
        initialized.  Yes, we know.  We don’t care.  The same basic
        thing occurs in a few other places where we derive from core C++
        classes.  The problem is, how do we call these parent class ctors
        portably?  It isn’t the same everywhere!

    -   The float equality comparisons in `lib/stadapter.cpp` are
        harmless.  They’re comparisons against special NaN and infinity
        constants, which are safe.

    -   If building in C++98 mode (which includes `-Wpedantic` on some
        compilers) the argument-less calls to `MAY_THROW()` are
        harmless.  All still-supported C++98 compilers cope with this
        usage.

-   Re-bootstrap it again without “pedantic” to avoid shipping the
    pedantic build files.

-   Run `tools/mkrel` to check the new `ChangeLog.md`, `configure.ac`, and
    `mysql++.bkl` files in with the proper version tag and to merge it into
    the release branch. This also creates `mysql++-$(VERSION).tar.gz` and
    synchs it up to tangentsoft.com in the MySQL++ releases directory.

-   On a CentOS 8 box, say “`make rpm`” from the “release” branch tree.
    Copy the resulting `~/rpmbuild/SRPMS/mysql++-$(VERSION)-1.el8.src.rpm`
    to that same releases directory.

-   Update “MySQL++” wiki document to point to new release packages.
