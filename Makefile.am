EXTRA_DIST = bootstrap Configure doc devel LGPL mysql++.spec Wishlist

#SUBDIRS = sqlplusint examples doc
SUBDIRS = sqlplusint examples

rpm: dist
	cp $(distdir).tar.gz /usr/src/redhat/SOURCES
	cp mysql++.spec /usr/src/redhat/SPECS
	( cd /usr/src/redhat/SPECS ; rpmbuild -bb mysql++.spec )
