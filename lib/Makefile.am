# This file is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation; either version 2.1 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with MySQL++; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
# USA

include $(top_srcdir)/common.am

AM_CXXFLAGS = @PTHREAD_CFLAGS@
LDADD = $(MYSQLPP_LIB) @PTHREAD_LIBS@

fooincludedir = $(pkgincludedir)/foo

EXTRA_DIST = \
		custom.pl \
		Doxyfile \
		Makefile.base \
		Makefile.gcc \
		Makefile.mingw \
		Makefile.simple \
		Makefile.vc

lib_LTLIBRARIES=libmysqlpp.la

libmysqlpp_la_LDFLAGS=-version-info @MYSQLPP_LIBTOOL_VERSION@

libmysqlpp_la_SOURCES = \
	coldata.cpp connection.cpp datetime.cpp fields.cpp field_names.cpp \
	field_types.cpp manip.cpp myset.cpp qparms.cpp query.cpp \
	result.cpp row.cpp sql_string.cpp string_util.cpp type_info.cpp \
	vallist.cpp

pkginclude_HEADERS = \
	coldata.h compare.h connection.h const_string.h convert.h custom.h \
	custom-macros.h defs.h datetime.h exceptions.h field_names.h \
	field_types.h fields.h lockable.h manip.h myset.h mysql++.h \
	noexceptions.h null.h platform.h qparms.h query.h resiter.h \
	result.h row.h sql_string.h stream2string.h string_util.h \
	tiny_int.h type_info.h vallist.h 

all-local: custom.h

custom.h custom-macros.h: custom.pl
	./custom.pl

distclean-local:
	rm -f custom.h custom-macros.h

tags: ctags

ctags:
	( cd .. && make ctags )

clean-docs:
	( cd ../doc/refman/html ; rm -f *.dot [a-z]*.html *.png doxygen.css )
	rm -rf ../doc/refman/latex

refman:
	doxygen
	( cd ../doc/refman/latex && make pdf && mv refman.pdf ../../mysqlpp-refman.pdf ) 

