include $(top_srcdir)/common.am

EXTRA_DIST = custom.pl *.bpf *.bpr *.bpg

lib_LTLIBRARIES=libmysqlpp.la

# Interface version, bug fix revision, and 'age' -- number of previous
# interfaces supported.  Bump the last only if explicit
# backwards-compatibility is added -- like two different versions of a
# single function exist.  'age' must be less than the current interface
# version.
libmysqlpp_la_LDFLAGS=-version-info 2:0:0

libmysqlpp_la_SOURCES = coldata.cc connection.cc datetime.cc fields.cc \
	field_names.cc field_types.cc manip.cc query.cc result.cc row.cc \
	set.cc sql_query.cc type_info.cc vallist.cc string_util.cc

include_HEADERS = \
	bad_query.h coldata.h compare.h connection.h const_string.h \
	convert.h custom.h custom-macros.h defs.h datetime.h exceptions.h \
	field_names.h field_types.h fields.h manip.h mysql++.h mysql++.hh \
	null.h platform.h query.h resiter.h result.h row.h set.h \
	sqlplus.hh sql_query.h sql_string.h stream2string.h tiny_int.h \
	type_info.h string_util.h vallist.h

all-local: custom.h custom-macros.h

custom.h custom-macros.h: custom.pl
	./custom.pl

distclean-local:
	rm -f custom.h custom-macros.h
