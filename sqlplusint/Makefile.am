include $(top_srcdir)/common.am

EXTRA_DIST = custom.pl *.bpf *.bpr *.bpg

lib_LTLIBRARIES=libmysqlpp.la

# Interface version, bug fix revision, and 'age' -- number of previous
# interfaces supported.  Bump the last only if explicit
# backwards-compatibility is added -- like two different versions of a
# single function exist.  'age' must be less than the current interface
# version.
libmysqlpp_la_LDFLAGS=-version-info 1:0:0

libmysqlpp_la_SOURCES = coldata.cc connection.cc datetime.cc field_names.cc \
	field_types.cc manip.cc query.cc result.cc  set.cc sql_query.cc \
	type_info.cc vallist.cc string_util.cc

include_HEADERS = \
	coldata1.hh coldata2.hh coldata3.hh coldata4.hh \
	compare1.hh compare2.hh compare3.hh \
	connection0.hh connection1.hh connection2.hh connection3.hh \
	const_string1.hh const_string2.hh const_string3.hh \
	convert1.hh convert2.hh convert3.hh \
	custom.hh \
	custom-macros.hh \
	defs.hh \
	define_short.hh \
	datetime1.hh datetime2.hh datetime3.hh \
	exceptions.hh \
	field_names1.hh field_names2.hh field_names3.hh \
	field_types1.hh field_types2.hh field_types3.hh \
	fields1.hh fields2.hh \
	manip1.hh manip2.hh manip3.hh \
	mysql++.hh mysql++-windows.hh \
	null1.hh null2.hh null3.hh \
	query1.hh query2.hh query3.hh \
	resiter1.hh \
	result1.hh result2.hh result3.hh \
	row1.hh row2.hh row3.hh \
	set1.hh set2.hh set3.hh \
	sql_query0.hh sql_query1.hh sql_query2.hh sql_query3.hh \
	sql_string1.hh sql_string2.hh sql_string3.hh \
	stream2string1.hh stream2string2.hh stream2string3.hh \
	tiny_int1.hh tiny_int2.hh tiny_int3.hh \
	type_info1.hh type_info2.hh type_info3.hh \
	string_util.hh \
	vallist1.hh vallist2.hh vallist3.hh

all-local: custom.hh custom-macros.hh

custom.hh custom-macros.hh: custom.pl
	perl custom.pl

distclean-local:
	rm -f custom.hh custom-macros.hh
