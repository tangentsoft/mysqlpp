EXTRA_DIST = bootstrap common.am doc devel LGPL *.spec *.ebuild \
		Wishlist *.vcproj *.sln README.* config/*.m4

#SUBDIRS = lib examples doc
SUBDIRS = lib examples

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
