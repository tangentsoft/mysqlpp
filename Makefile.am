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
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
# USA.

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
	doc/refman/html \
	doc/userman/*.in \
	doc/userman/*.xml \
	doc/userman/html \
	doc/userman/findxsl \
	doc/userman/Makefile \
	doc/userman/README \
	*.ebuild \
	*.spec \
	*.sln \
	*.vcproj

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

