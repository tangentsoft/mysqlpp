include ../common.am

LDFLAGS = 
LDADD = $(SQLPLUSLIBS)

EXTRA_DIST = *.vcproj *.bpf *.bpr *.bpg dummy.cpp

noinst_PROGRAMS = resetdb simple1 custom1 custom2 custom3 custom4 \
		complic1 fieldinf1 dbinfo updel_ex load_file cgi_image

noinst_HEADERS = util.hh

simple1_SOURCES = simple1.cc util.cc
simple1_DEPENDENCIES = $(SQLPLUSLIBS)

dbinfo_SOURCES = dbinfo.cc
dbinfo_DEPENDENCIES = $(SQLPLUSLIBS)

cgi_image_SOURCES = cgi_image.cc
cgi_image_DEPENDENCIES = $(SQLPLUSLIBS)

resetdb_SOURCES = resetdb.cc util.cc
resetdb_DEPENDENCIES = $(SQLPLUSLIBS)

custom1_SOURCES = custom1.cc util.cc
custom1_DEPENDENCIES = $(SQLPLUSLIBS)

custom2_SOURCES = custom2.cc util.cc
custom2_DEPENDENCIES = $(SQLPLUSLIBS)

custom3_SOURCES = custom3.cc util.cc
custom3_DEPENDENCIES = $(SQLPLUSLIBS)

custom4_SOURCES = custom4.cc util.cc
custom4_DEPENDENCIES = $(SQLPLUSLIBS)

complic1_SOURCES = complic1.cc util.cc
complic1_DEPENDENCIES = $(SQLPLUSLIBS)

fieldinf1_SOURCES = fieldinf1.cc util.cc
fieldinf1_DEPENDENCIES = $(SQLPLUSLIBS)

updel_ex_SOURCES = updel_x_.cc
updel_ex_DEPENDENCIES = $(SQLPLUSLIBS)

load_file_SOURCES = load_file.cc
load_file_DEPENDENCIES = $(SQLPLUSLIBS)

