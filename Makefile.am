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

EXTRA_DIST = \
	CREDITS \
	HACKERS \
	LGPL \
	README.* \
	Wishlist \
	bootstrap \
	common.am \
	config/*.m4 \
	doc/mysqlpp-refman.pdf \
	doc/mysqlpp-userman.pdf \
	doc/README* \
	doc/ssqls-pretty \
	doc/refman/html/*.css \
	doc/refman/html/*.html \
	doc/refman/html/*.png \
	doc/userman/*.xml \
	doc/userman/*.xsl \
	doc/userman/mktxt \
	doc/userman/html/*.css \
	doc/userman/html/*.html \
	doc/userman/Makefile \
	doc/userman/README \
	makemake.* \
	*.ebuild \
	*.spec

#SUBDIRS = lib examples doc
SUBDIRS = lib @OPTIONAL_EXAMPLES@

# Override default aclocal command, to force it to look in config subdir
# for *.m4 macros when automatically rebuilding configure script when
# configure.in file changes.
ACLOCAL=aclocal -I config


rpm-setup: dist
	cp $(distdir).tar.gz /usr/src/redhat/SOURCES
	cp mysql++.spec /usr/src/redhat/SPECS

rpm: rpm-setup
	( cd /usr/src/redhat/SPECS ; rpmbuild -bb mysql++.spec )

srpm: rpm-setup
	( cd /usr/src/redhat/SPECS ; rpmbuild -bs mysql++.spec )

ebuild: dist
	cp $(distdir).tar.gz /usr/portage/distfiles
	cp mysql++.ebuild /usr/portage/dev-db/mysql++/$(distdir).ebuild
	ebuild /usr/portage/dev-db/mysql++/$(distdir).ebuild digest

docs: doc/mysqlpp-refman.pdf doc/mysqlpp-userman.pdf


doc/mysqlpp-refman.pdf: lib/*.h
	( cd lib ; make refman )

doc/mysqlpp-userman.pdf: doc/userman/userman.xml
	( cd doc/userman ; make && make pdf )

tags: ctags

ctags:
	find . -name TAGS -o -name tags -exec rm {} \;
	ctags `pwd`/*/*.cpp `pwd`/*/*.h
	find . -type d -mindepth 1 -exec ln tags {} \;

clean-local:
	( cd lib ; make clean-docs )
	( cd doc/userman ; make clean )
	find . -name tags -exec rm {} \;
