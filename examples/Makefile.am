include $(top_srcdir)/common.am

AM_CXXFLAGS = @PTHREAD_CFLAGS@
LDADD = $(MYSQLPP_LIB) @PTHREAD_LIBS@

EXTRA_DIST = *.vcproj *.bpf *.bpr *.bpg Makefile.simple

noinst_PROGRAMS = resetdb simple1 custom1 custom2 custom3 custom4 \
		complic1 fieldinf1 dbinfo updel load_file cgi_image

noinst_HEADERS = util.h

simple1_SOURCES = simple1.cpp util.cpp
simple1_DEPENDENCIES = $(MYSQLPP_LIB)

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

complic1_SOURCES = complic1.cpp util.cpp
complic1_DEPENDENCIES = $(MYSQLPP_LIB)

fieldinf1_SOURCES = fieldinf1.cpp util.cpp
fieldinf1_DEPENDENCIES = $(MYSQLPP_LIB)

updel_SOURCES = updel.cpp
updel_DEPENDENCIES = $(MYSQLPP_LIB)

load_file_SOURCES = load_file.cpp
load_file_DEPENDENCIES = $(MYSQLPP_LIB)

