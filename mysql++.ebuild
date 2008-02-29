# Copyright 1999-2004 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2

inherit gcc eutils gnuconfig

DESCRIPTION="C++ API interface to the MySQL database"
# This is the download page but includes links to other places
HOMEPAGE="http://tangentsoft.net/mysql++/"
SRC_URI_BASE="http://tangentsoft.net/mysql++/releases"
SRC_URI="${SRC_URI_BASE}/${P}.tar.gz"

LICENSE="LGPL-2"
SLOT="0"
KEYWORDS="x86 ~alpha ~hppa ~mips ~sparc ~ppc ~amd64"
IUSE=""

DEPEND=">=dev-db/mysql-3.23.49"

src_unpack() {
	unpack ${P}.tar.gz
}

src_compile() {
	gnuconfig_update
	# not including the directives to where MySQL is because it seems to find it
	# just fine without
	# force the cflags into place otherwise they get totally ignored by
	# configure
	CFLAGS="${CFLAGS}" CXXFLAGS="${CFLAGS} ${CXXFLAGS}" econf || \
			die "econf failed"

	emake || die "unable to make"
}

src_install() {
	make DESTDIR=${D} install || die
	# install the docs and HTML pages
	dodoc README* LICENSE.txt
	dodoc doc/*
	dohtml doc/html/*
	prepalldocs
}

