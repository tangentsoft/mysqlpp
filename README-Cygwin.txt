Prerequisite: Build MySQL
~~~~~~~~~~~~~~~~~~~~~~~~~
    Before you can build MySQL++, you need to build the MySQL client
    library from source.  This is necessary because Cygwin programs
    work best when linked to Cygwin libraries, even though it's
    possible to link to some native Windows libraries.  You do not
    need to build a Cygwin version of the MySQL server; it's still
    best to use the native Windows version of that.

    The MySQL client library does not build correctly out of the box.
    The best instructions I've found covering the necessary changes
    are here:

        http://cygwin.com/ml/cygwin/2004-11/msg00159.html

    I recommend that you use MySQL v4.1.x at this time.  Skip the
    step changing the pthread stuff in my_thr_init.c, as this change
    has already been made in recent versions of MySQL.

    With the changes applied, build and install the client library
    like so:

    $ autoreconf --install --verbose --force
    $ ./configure --prefix=/usr --sysconfdir=/etc --localstatedir=/var \
        --infodir=/usr/share/info --mandir=/usr/share/man \
        --disable-shared --without-{debug,readline,libedit,server}
    $ make
    $ make install


Building the Library and Example Programs
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    With that done, you can build MySQL++ just as you would any other
    typical Unix program.  See README-Unix.txt for details.
