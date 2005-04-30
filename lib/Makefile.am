include $(top_srcdir)/common.am

AM_CXXFLAGS = @PTHREAD_CFLAGS@
LDADD = $(MYSQLPP_LIB) @PTHREAD_LIBS@

EXTRA_DIST = custom.pl *.bpf *.bpr *.bpg

lib_LTLIBRARIES=libmysqlpp.la

# Interface version, bug fix revision, and 'age' -- number of previous
# interfaces supported.  Bump the last only if explicit backwards-
# compatibility is added -- like two different versions of a single 
# function.  'age' must be less than the current interface version.
libmysqlpp_la_LDFLAGS=-version-info 4:0:0

libmysqlpp_la_SOURCES = \
	coldata.cpp connection.cpp datetime.cpp fields.cpp field_names.cpp \
	field_types.cpp manip.cpp myset.cpp query.cpp result.cpp row.cpp \
	sql_query.cpp sql_string.cpp type_info.cpp vallist.cpp \
	string_util.cpp

include_HEADERS = \
	coldata.h compare.h connection.h const_string.h convert.h custom.h \
	custom-macros.h defs.h datetime.h exceptions.h field_names.h \
	field_types.h fields.h manip.h myset.h mysql++.h mysql++.hh null.h \
	platform.h query.h resiter.h result.h row.h sqlplus.hh sql_query.h \
	sql_string.h stream2string.h string_util.h tiny_int.h type_info.h \
	vallist.h 

all-local: custom.h

custom.h custom-macros.h: custom.pl
	./custom.pl

distclean-local:
	rm -f custom.h custom-macros.h

ctags:
	ctags *.cpp *.h

tags: ctags

refman:
	rm -rf ../doc/refman/html ../doc/refman/latex
	doxygen
	( cd ../doc/refman/latex && make pdf && mv refman.pdf ../../mysqlpp-refman.pdf ) 

