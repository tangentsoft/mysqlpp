check_PROGRAMS = resetdb simple1 custom1 custom2 custom3 custom4 complic1 fieldinf1 cgi_image updel_ex load_file

noinst_HEADERS = util.hh

LDADD = $(top_builddir)/sqlplusint/libmysql++.la @MYSQL_LIBS@
INCLUDES = -I$(top_srcdir)/sqlplusint @MYSQL_CFLAGS@

simple1_SOURCES = simple1.cc

# sinisa_ex_SOURCES = sinisa_ex.cc

cgi_image_SOURCES = cgi_image.cc

resetdb_SOURCES = resetdb.cc util.cc

custom1_SOURCES = custom1.cc

custom2_SOURCES = custom2.cc util.cc

custom3_SOURCES = custom3.cc util.cc

custom4_SOURCES = custom4.cc

complic1_SOURCES = complic1.cc

fieldinf1_SOURCES = fieldinf1.cc

updel_ex_SOURCES = updel_x_.cc

load_file_SOURCES = load_file.cc

