EXTRA_DIST = bootstrap Configure doc devel LGPL mysql++.spec Wishlist \
		*.vcproj *.sln README.*

#SUBDIRS = sqlplusint examples doc
SUBDIRS = sqlplusint examples

rpm-setup: dist
	cp $(distdir).tar.gz /usr/src/redhat/SOURCES
	cp mysql++.spec /usr/src/redhat/SPECS

rpm: rpm-setup
	( cd /usr/src/redhat/SPECS ; rpmbuild -bb mysql++.spec )

srpm: rpm-setup
	( cd /usr/src/redhat/SPECS ; rpmbuild -bs mysql++.spec )
