include $(top_srcdir)/common.am

LDFLAGS = 
LDADD = $(MYSQLPP_LIB)

EXTRA_DIST = *.vcproj *.bpf *.bpr *.bpg dummy.cpp Makefile.simple

noinst_PROGRAMS = resetdb simple1 custom1 custom2 custom3 custom4 \
		complic1 fieldinf1 dbinfo updel load_file cgi_image

noinst_HEADERS = util.h

simple1_SOURCES = simple1.cc util.cc
simple1_DEPENDENCIES = $(MYSQLPP_LIB)

dbinfo_SOURCES = dbinfo.cc util.cc
dbinfo_DEPENDENCIES = $(MYSQLPP_LIB)

cgi_image_SOURCES = cgi_image.cc
cgi_image_DEPENDENCIES = $(MYSQLPP_LIB)

resetdb_SOURCES = resetdb.cc util.cc
resetdb_DEPENDENCIES = $(MYSQLPP_LIB)

custom1_SOURCES = custom1.cc util.cc
custom1_DEPENDENCIES = $(MYSQLPP_LIB)

custom2_SOURCES = custom2.cc util.cc
custom2_DEPENDENCIES = $(MYSQLPP_LIB)

custom3_SOURCES = custom3.cc util.cc
custom3_DEPENDENCIES = $(MYSQLPP_LIB)

custom4_SOURCES = custom4.cc util.cc
custom4_DEPENDENCIES = $(MYSQLPP_LIB)

complic1_SOURCES = complic1.cc util.cc
complic1_DEPENDENCIES = $(MYSQLPP_LIB)

fieldinf1_SOURCES = fieldinf1.cc util.cc
fieldinf1_DEPENDENCIES = $(MYSQLPP_LIB)

updel_SOURCES = updel.cc
updel_DEPENDENCIES = $(MYSQLPP_LIB)

load_file_SOURCES = load_file.cc
load_file_DEPENDENCIES = $(MYSQLPP_LIB)

