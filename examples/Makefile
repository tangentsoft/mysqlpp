# Generated automatically from Makefile.in by configure.
# Makefile.in generated automatically by automake 1.4 from Makefile.am

# Copyright (C) 1994, 1995-8, 1999 Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.



SHELL = /bin/sh

srcdir = .
top_srcdir = ..
prefix = /usr/local
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include

DESTDIR =

pkgdatadir = $(datadir)/sqlplus
pkglibdir = $(libdir)/sqlplus
pkgincludedir = $(includedir)/sqlplus

top_builddir = ..

ACLOCAL = aclocal
AUTOCONF = autoconf
AUTOMAKE = automake
AUTOHEADER = autoheader

INSTALL = /usr/bin/ginstall -c
INSTALL_PROGRAM = ${INSTALL} $(AM_INSTALL_PROGRAM_FLAGS)
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL_PROGRAM}
transform = s,x,x,

NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
host_alias = i686-pc-linux-gnu
host_triplet = i686-pc-linux-gnu
AS = @AS@
CC = gcc
CXX = c++
DLLTOOL = @DLLTOOL@
LD = /usr/bin/ld
LIBTOOL = $(SHELL) $(top_builddir)/libtool
LN_S = ln -s
MAINT = #
MAKEINFO = makeinfo
NM = /usr/bin/nm -B
OBJDUMP = @OBJDUMP@
PACKAGE = sqlplus
RANLIB = ranlib
SQLPLUS_MAJOR_VERSION = 1
SQLPLUS_MICRO_VERSION = 0
SQLPLUS_MINOR_VERSION = 7
VERSION = 1.7.0

INCLUDES = -I$(top_srcdir)/sqlplusint
LIBS = -lz -lmysqlclient

SQLPLUSLIBS = $(top_builddir)/sqlplusint/libsqlplus.la

noinst_PROGRAMS = resetdb simple1 custom1 custom2 custom3 custom4 complic1 fieldinf1 sinisa_ex updel_ex load_file cgi_image

noinst_HEADERS = util.hh

simple1_SOURCES = simple1.cc
simple1_LDFLAGS = 
simple1_LDADD = $(SQLPLUSLIBS)
simple1_DEPENDENCIES = $(SQLPLUSLIBS)

sinisa_ex_SOURCES = sinisa_ex.cc
sinisa_ex_LDFLAGS = 
sinisa_ex_LDADD = $(SQLPLUSLIBS)
sinisa_ex_DEPENDENCIES = $(SQLPLUSLIBS)

cgi_image_SOURCES = cgi_image.cc
cgi_image_LDFLAGS = 
cgi_image_LDADD = $(SQLPLUSLIBS)
cgi_image_DEPENDENCIES = $(SQLPLUSLIBS)

resetdb_SOURCES = resetdb.cc util.cc
resetdb_LDFLAGS = 
resetdb_LDADD = $(SQLPLUSLIBS)
resetdb_DEPENDENCIES = $(SQLPLUSLIBS)

custom1_SOURCES = custom1.cc
custom1_LDFLAGS = 
custom1_LDADD = $(SQLPLUSLIBS)
custom1_DEPENDENCIES = $(SQLPLUSLIBS)

custom2_SOURCES = custom2.cc util.cc
custom2_LDFLAGS = 
custom2_LDADD = $(SQLPLUSLIBS)
custom2_DEPENDENCIES = $(SQLPLUSLIBS)

custom3_SOURCES = custom3.cc util.cc
custom3_LDFLAGS = 
custom3_LDADD = $(SQLPLUSLIBS)
custom3_DEPENDENCIES = $(SQLPLUSLIBS)

custom4_SOURCES = custom4.cc
custom4_LDFLAGS = 
custom4_LDADD = $(SQLPLUSLIBS)
custom4_DEPENDENCIES = $(SQLPLUSLIBS)

complic1_SOURCES = complic1.cc
complic1_LDFLAGS = 
complic1_LDADD = $(SQLPLUSLIBS)
complic1_DEPENDENCIES = $(SQLPLUSLIBS)

fieldinf1_SOURCES = fieldinf1.cc
fieldinf1_LDFLAGS = 
fieldinf1_LDADD = $(SQLPLUSLIBS)
fieldinf1_DEPENDENCIES = $(SQLPLUSLIBS)

updel_ex_SOURCES = updel_x_.cc
updel_ex_LDFLAGS = 
updel_ex_LDADD = $(SQLPLUSLIBS)
updel_ex_DEPENDENCIES = $(SQLPLUSLIBS)

load_file_SOURCES = load_file.cc
load_file_LDFLAGS = 
load_file_LDADD = $(SQLPLUSLIBS)
load_file_DEPENDENCIES = $(SQLPLUSLIBS)
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_HEADER = ../config.h ../sqlplusint/exceptions.hh
CONFIG_CLEAN_FILES = 
PROGRAMS =  $(noinst_PROGRAMS)


DEFS = -DHAVE_CONFIG_H -I. -I$(srcdir) -I.. -I../sqlplusint
CPPFLAGS =  -I/usr/local/include/mysql
LDFLAGS =  -L/usr/local/lib/mysql
resetdb_OBJECTS =  resetdb.o util.o
simple1_OBJECTS =  simple1.o
custom1_OBJECTS =  custom1.o
custom2_OBJECTS =  custom2.o util.o
custom3_OBJECTS =  custom3.o util.o
custom4_OBJECTS =  custom4.o
complic1_OBJECTS =  complic1.o
fieldinf1_OBJECTS =  fieldinf1.o
sinisa_ex_OBJECTS =  sinisa_ex.o
updel_ex_OBJECTS =  updel_x_.o
load_file_OBJECTS =  load_file.o
cgi_image_OBJECTS =  cgi_image.o
CXXFLAGS = -g -O2
CXXCOMPILE = $(CXX) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS)
LTCXXCOMPILE = $(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS)
CXXLD = $(CXX)
CXXLINK = $(LIBTOOL) --mode=link $(CXXLD) $(AM_CXXFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $@
HEADERS =  $(noinst_HEADERS)

DIST_COMMON =  Makefile.am Makefile.in


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = tar
GZIP_ENV = --best
DEP_FILES =  .deps/cgi_image.P .deps/complic1.P .deps/custom1.P \
.deps/custom2.P .deps/custom3.P .deps/custom4.P .deps/fieldinf1.P \
.deps/load_file.P .deps/resetdb.P .deps/simple1.P .deps/sinisa_ex.P \
.deps/updel_x_.P .deps/util.P
SOURCES = $(resetdb_SOURCES) $(simple1_SOURCES) $(custom1_SOURCES) $(custom2_SOURCES) $(custom3_SOURCES) $(custom4_SOURCES) $(complic1_SOURCES) $(fieldinf1_SOURCES) $(sinisa_ex_SOURCES) $(updel_ex_SOURCES) $(load_file_SOURCES) $(cgi_image_SOURCES)
OBJECTS = $(resetdb_OBJECTS) $(simple1_OBJECTS) $(custom1_OBJECTS) $(custom2_OBJECTS) $(custom3_OBJECTS) $(custom4_OBJECTS) $(complic1_OBJECTS) $(fieldinf1_OBJECTS) $(sinisa_ex_OBJECTS) $(updel_ex_OBJECTS) $(load_file_OBJECTS) $(cgi_image_OBJECTS)

all: all-redirect
.SUFFIXES:
.SUFFIXES: .S .c .cc .lo .o .s
$(srcdir)/Makefile.in: # Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) $(srcdir)/../Configure
	cd $(top_srcdir) && $(AUTOMAKE) --gnu examples/Makefile

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status $(BUILT_SOURCES)
	cd $(top_builddir) \
	  && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= $(SHELL) ./config.status


mostlyclean-noinstPROGRAMS:

clean-noinstPROGRAMS:
	-test -z "$(noinst_PROGRAMS)" || rm -f $(noinst_PROGRAMS)

distclean-noinstPROGRAMS:

maintainer-clean-noinstPROGRAMS:

.s.o:
	$(COMPILE) -c $<

.S.o:
	$(COMPILE) -c $<

mostlyclean-compile:
	-rm -f *.o core *.core

clean-compile:

distclean-compile:
	-rm -f *.tab.c

maintainer-clean-compile:

.s.lo:
	$(LIBTOOL) --mode=compile $(COMPILE) -c $<

.S.lo:
	$(LIBTOOL) --mode=compile $(COMPILE) -c $<

mostlyclean-libtool:
	-rm -f *.lo

clean-libtool:
	-rm -rf .libs _libs

distclean-libtool:

maintainer-clean-libtool:

resetdb: $(resetdb_OBJECTS) $(resetdb_DEPENDENCIES)
	@rm -f resetdb
	$(CXXLINK) $(resetdb_LDFLAGS) $(resetdb_OBJECTS) $(resetdb_LDADD) $(LIBS)

simple1: $(simple1_OBJECTS) $(simple1_DEPENDENCIES)
	@rm -f simple1
	$(CXXLINK) $(simple1_LDFLAGS) $(simple1_OBJECTS) $(simple1_LDADD) $(LIBS)

custom1: $(custom1_OBJECTS) $(custom1_DEPENDENCIES)
	@rm -f custom1
	$(CXXLINK) $(custom1_LDFLAGS) $(custom1_OBJECTS) $(custom1_LDADD) $(LIBS)

custom2: $(custom2_OBJECTS) $(custom2_DEPENDENCIES)
	@rm -f custom2
	$(CXXLINK) $(custom2_LDFLAGS) $(custom2_OBJECTS) $(custom2_LDADD) $(LIBS)

custom3: $(custom3_OBJECTS) $(custom3_DEPENDENCIES)
	@rm -f custom3
	$(CXXLINK) $(custom3_LDFLAGS) $(custom3_OBJECTS) $(custom3_LDADD) $(LIBS)

custom4: $(custom4_OBJECTS) $(custom4_DEPENDENCIES)
	@rm -f custom4
	$(CXXLINK) $(custom4_LDFLAGS) $(custom4_OBJECTS) $(custom4_LDADD) $(LIBS)

complic1: $(complic1_OBJECTS) $(complic1_DEPENDENCIES)
	@rm -f complic1
	$(CXXLINK) $(complic1_LDFLAGS) $(complic1_OBJECTS) $(complic1_LDADD) $(LIBS)

fieldinf1: $(fieldinf1_OBJECTS) $(fieldinf1_DEPENDENCIES)
	@rm -f fieldinf1
	$(CXXLINK) $(fieldinf1_LDFLAGS) $(fieldinf1_OBJECTS) $(fieldinf1_LDADD) $(LIBS)

sinisa_ex: $(sinisa_ex_OBJECTS) $(sinisa_ex_DEPENDENCIES)
	@rm -f sinisa_ex
	$(CXXLINK) $(sinisa_ex_LDFLAGS) $(sinisa_ex_OBJECTS) $(sinisa_ex_LDADD) $(LIBS)

updel_ex: $(updel_ex_OBJECTS) $(updel_ex_DEPENDENCIES)
	@rm -f updel_ex
	$(CXXLINK) $(updel_ex_LDFLAGS) $(updel_ex_OBJECTS) $(updel_ex_LDADD) $(LIBS)

load_file: $(load_file_OBJECTS) $(load_file_DEPENDENCIES)
	@rm -f load_file
	$(CXXLINK) $(load_file_LDFLAGS) $(load_file_OBJECTS) $(load_file_LDADD) $(LIBS)

cgi_image: $(cgi_image_OBJECTS) $(cgi_image_DEPENDENCIES)
	@rm -f cgi_image
	$(CXXLINK) $(cgi_image_LDFLAGS) $(cgi_image_OBJECTS) $(cgi_image_LDADD) $(LIBS)
.cc.o:
	$(CXXCOMPILE) -c $<
.cc.lo:
	$(LTCXXCOMPILE) -c $<

tags: TAGS

ID: $(HEADERS) $(SOURCES) $(LISP)
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	here=`pwd` && cd $(srcdir) \
	  && mkid -f$$here/ID $$unique $(LISP)

TAGS:  $(HEADERS) $(SOURCES)  $(TAGS_DEPENDENCIES) $(LISP)
	tags=; \
	here=`pwd`; \
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	test -z "$(ETAGS_ARGS)$$unique$(LISP)$$tags" \
	  || (cd $(srcdir) && etags $(ETAGS_ARGS) $$tags  $$unique $(LISP) -o $$here/TAGS)

mostlyclean-tags:

clean-tags:

distclean-tags:
	-rm -f TAGS ID

maintainer-clean-tags:

distdir = $(top_builddir)/$(PACKAGE)-$(VERSION)/$(subdir)

subdir = examples

distdir: $(DISTFILES)
	here=`cd $(top_builddir) && pwd`; \
	top_distdir=`cd $(top_distdir) && pwd`; \
	distdir=`cd $(distdir) && pwd`; \
	cd $(top_srcdir) \
	  && $(AUTOMAKE) --include-deps --build-dir=$$here --srcdir-name=$(top_srcdir) --output-dir=$$top_distdir --gnu examples/Makefile
	@for file in $(DISTFILES); do \
	  d=$(srcdir); \
	  if test -d $$d/$$file; then \
	    cp -pr $$/$$file $(distdir)/$$file; \
	  else \
	    test -f $(distdir)/$$file \
	    || ln $$d/$$file $(distdir)/$$file 2> /dev/null \
	    || cp -p $$d/$$file $(distdir)/$$file || :; \
	  fi; \
	done

DEPS_MAGIC := $(shell mkdir .deps > /dev/null 2>&1 || :)

-include $(DEP_FILES)

mostlyclean-depend:

clean-depend:

distclean-depend:
	-rm -rf .deps

maintainer-clean-depend:

%.o: %.c
	@echo '$(COMPILE) -c $<'; \
	$(COMPILE) -Wp,-MD,.deps/$(*F).pp -c $<
	@-cp .deps/$(*F).pp .deps/$(*F).P; \
	tr ' ' '\012' < .deps/$(*F).pp \
	  | sed -e 's/^\\$$//' -e '/^$$/ d' -e '/:$$/ d' -e 's/$$/ :/' \
	    >> .deps/$(*F).P; \
	rm .deps/$(*F).pp

%.lo: %.c
	@echo '$(LTCOMPILE) -c $<'; \
	$(LTCOMPILE) -Wp,-MD,.deps/$(*F).pp -c $<
	@-sed -e 's/^\([^:]*\)\.o[ 	]*:/\1.lo \1.o :/' \
	  < .deps/$(*F).pp > .deps/$(*F).P; \
	tr ' ' '\012' < .deps/$(*F).pp \
	  | sed -e 's/^\\$$//' -e '/^$$/ d' -e '/:$$/ d' -e 's/$$/ :/' \
	    >> .deps/$(*F).P; \
	rm -f .deps/$(*F).pp

%.o: %.cc
	@echo '$(CXXCOMPILE) -c $<'; \
	$(CXXCOMPILE) -Wp,-MD,.deps/$(*F).pp -c $<
	@-cp .deps/$(*F).pp .deps/$(*F).P; \
	tr ' ' '\012' < .deps/$(*F).pp \
	  | sed -e 's/^\\$$//' -e '/^$$/ d' -e '/:$$/ d' -e 's/$$/ :/' \
	    >> .deps/$(*F).P; \
	rm .deps/$(*F).pp

%.lo: %.cc
	@echo '$(LTCXXCOMPILE) -c $<'; \
	$(LTCXXCOMPILE) -Wp,-MD,.deps/$(*F).pp -c $<
	@-sed -e 's/^\([^:]*\)\.o[ 	]*:/\1.lo \1.o :/' \
	  < .deps/$(*F).pp > .deps/$(*F).P; \
	tr ' ' '\012' < .deps/$(*F).pp \
	  | sed -e 's/^\\$$//' -e '/^$$/ d' -e '/:$$/ d' -e 's/$$/ :/' \
	    >> .deps/$(*F).P; \
	rm -f .deps/$(*F).pp
info-am:
info: info-am
dvi-am:
dvi: dvi-am
check-am: all-am
check: check-am
installcheck-am:
installcheck: installcheck-am
install-exec-am:
install-exec: install-exec-am

install-data-am:
install-data: install-data-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-am
uninstall-am:
uninstall: uninstall-am
all-am: Makefile $(PROGRAMS) $(HEADERS)
all-redirect: all-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) AM_INSTALL_PROGRAM_FLAGS=-s install
installdirs:


mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*

maintainer-clean-generic:
mostlyclean-am:  mostlyclean-noinstPROGRAMS mostlyclean-compile \
		mostlyclean-libtool mostlyclean-tags mostlyclean-depend \
		mostlyclean-generic

mostlyclean: mostlyclean-am

clean-am:  clean-noinstPROGRAMS clean-compile clean-libtool clean-tags \
		clean-depend clean-generic mostlyclean-am

clean: clean-am

distclean-am:  distclean-noinstPROGRAMS distclean-compile \
		distclean-libtool distclean-tags distclean-depend \
		distclean-generic clean-am
	-rm -f libtool

distclean: distclean-am

maintainer-clean-am:  maintainer-clean-noinstPROGRAMS \
		maintainer-clean-compile maintainer-clean-libtool \
		maintainer-clean-tags maintainer-clean-depend \
		maintainer-clean-generic distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-am

.PHONY: mostlyclean-noinstPROGRAMS distclean-noinstPROGRAMS \
clean-noinstPROGRAMS maintainer-clean-noinstPROGRAMS \
mostlyclean-compile distclean-compile clean-compile \
maintainer-clean-compile mostlyclean-libtool distclean-libtool \
clean-libtool maintainer-clean-libtool tags mostlyclean-tags \
distclean-tags clean-tags maintainer-clean-tags distdir \
mostlyclean-depend distclean-depend clean-depend \
maintainer-clean-depend info-am info dvi-am dvi check check-am \
installcheck-am installcheck install-exec-am install-exec \
install-data-am install-data install-am install uninstall-am uninstall \
all-redirect all-am all installdirs mostlyclean-generic \
distclean-generic clean-generic maintainer-clean-generic clean \
mostlyclean distclean maintainer-clean


# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
