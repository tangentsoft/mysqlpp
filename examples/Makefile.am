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

EXTRA_DIST = \
		Makefile.base \
		Makefile.bc \
		Makefile.gcc \
		Makefile.mingw \
		Makefile.simple \
		Makefile.vc

noinst_PROGRAMS = resetdb simple1 simple2 usequery multiquery custom1 custom2 \
		custom3 custom4 custom5 fieldinf1 dbinfo updel load_file \
		cgi_image

noinst_HEADERS = util.h

simple1_SOURCES = simple1.cpp util.cpp
simple1_DEPENDENCIES = $(MYSQLPP_LIB)

simple2_SOURCES = simple2.cpp util.cpp
simple2_DEPENDENCIES = $(MYSQLPP_LIB)

usequery_SOURCES = usequery.cpp util.cpp
usequery_DEPENDENCIES = $(MYSQLPP_LIB)

dbinfo_SOURCES = dbinfo.cpp util.cpp
dbinfo_DEPENDENCIES = $(MYSQLPP_LIB)

cgi_image_SOURCES = cgi_image.cpp
cgi_image_DEPENDENCIES = $(MYSQLPP_LIB)

resetdb_SOURCES = resetdb.cpp util.cpp
resetdb_DEPENDENCIES = $(MYSQLPP_LIB)

custom1_SOURCES = custom1.cpp util.cpp
custom1_DEPENDENCIES = $(MYSQLPP_LIB)

custom2_SOURCES = custom2.cpp util.cpp
custom2_DEPENDENCIES = $(MYSQLPP_LIB)

custom3_SOURCES = custom3.cpp util.cpp
custom3_DEPENDENCIES = $(MYSQLPP_LIB)

custom4_SOURCES = custom4.cpp util.cpp
custom4_DEPENDENCIES = $(MYSQLPP_LIB)

custom5_SOURCES = custom5.cpp util.cpp
custom5_DEPENDENCIES = $(MYSQLPP_LIB)

fieldinf1_SOURCES = fieldinf1.cpp util.cpp
fieldinf1_DEPENDENCIES = $(MYSQLPP_LIB)

updel_SOURCES = updel.cpp
updel_DEPENDENCIES = $(MYSQLPP_LIB)

load_file_SOURCES = load_file.cpp
load_file_DEPENDENCIES = $(MYSQLPP_LIB)

multiquery_SOURCES = multiquery.cpp util.cpp
multiquery_DEPENDENCIES = $(MYSQLPP_LIB)

tags: ctags

ctags:
	( cd .. && make ctags )

