EXTRA_DIST = bootstrap common.am doc devel LGPL *.spec *.ebuild \
		Wishlist *.vcproj *.sln README.*

#SUBDIRS = lib examples doc
SUBDIRS = lib examples

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
