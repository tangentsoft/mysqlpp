EXTRA_DIST = custom.pl

lib_LTLIBRARIES = libmysql++.la

libmysql___la_SOURCES = coldata.cc connection.cc datetime.cc field_names.cc \
	field_types.cc manip.cc query.cc result.cc row.cc set.cc sql_query.cc \
	type_info.cc vallist.cc

libmysql___la_LDFLAGS = \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) \
	-release $(LT_RELEASE)

# DKH -- tracker _does_ need to be installed since it appears in one
# of the header files (actually, two header files, but it is not used
# in connection1.hh, only referenced).
# noinst_HEADERS = magic.h tracker.h

# LDADD = @MYSQL_LIBS@
INCLUDES = -I$(top_srcdir)/sqlplusint @MYSQL_CFLAGS@

# a little automake magic here; puts the headers in
# $(prefix)/include/$(package) good for projects such as this.  Let
# sqlplus-config sort things out...
pkginclude_HEADERS = sqlplus.hh coldata1.hh coldata2.hh coldata3.hh coldata4.hh \
	compare1.hh compare2.hh compare3.hh connection0.hh connection1.hh \
	connection2.hh connection3.hh const_string1.hh const_string2.hh const_string3.hh \
	convert1.hh convert2.hh convert3.hh custom-macros.hh custom.hh datetime1.hh \
	datetime2.hh datetime3.hh field_names1.hh field_names2.hh field_names3.hh \
	field_types1.hh field_types2.hh field_types3.hh fields1.hh fields2.hh \
	manip1.hh manip2.hh manip3.hh null1.hh null2.hh null3.hh query1.hh query2.hh \
	query3.hh resiter1.hh result1.hh result2.hh result3.hh row1.hh row2.hh \
	row3.hh set1.hh set2.hh set3.hh sql_query0.hh sql_query1.hh sql_query2.hh \
	sql_query3.hh sql_string1.hh sql_string2.hh sql_string3.hh stream2string1.hh \
	stream2string2.hh stream2string3.hh tiny_int1.hh tiny_int2.hh tiny_int3.hh \
	type_info1.hh type_info2.hh type_info3.hh vallist1.hh vallist2.hh vallist3.hh \
	define_short defs undef_short \
	mysql++

BUILT_SOURCES = custom.hh custom-macros.hh

custom.hh custom-macros.hh: custom.pl
	@PERL@ custom.pl

# These files should be removed by invoking maintainer-clean

# all-local: custom.hh custom-macros.hh

# distclean-local:
# 	rm -f custom.hh custom-macros.hh
