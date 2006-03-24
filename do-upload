#!/bin/sh
CFG_FILE=~/.mysqlpp
if [ ! -f $CFG_FILE ]
then
	echo Create $CFG_FILE, please.
	echo
	exit 1
fi
. $CFG_FILE

if [ ! -f config.h ]
then
	echo Package hasn't yet been built, or you aren't in the top
	echo mysql++ directory.
	echo
	exit 1
fi

VERSION=`cat mysql++.spec | grep ^Version: | cut -f2 -d' '`
TMPDIR=`mktemp -d /tmp/mysqlpp-up.XXXXXXXXXX`

if [ "$1" = "src" ]
then
	if [ ! -e mysql++-$VERSION.tar.gz ]
	then
		make dist
	fi
	cp mysql++-$VERSION.tar.gz $TMPDIR

	if [ ! -e /usr/src/redhat/RPMS/i386/mysql++-manuals-$VERSION-1.i386.rpm ]
	then
		make rpm
	fi
	cp /usr/src/redhat/RPMS/i386/mysql++-manuals-$VERSION-1.i386.rpm $TMPDIR

	if [ ! -e /usr/src/redhat/SRPMS/mysql++-$VERSION-1.src.rpm ]
	then
		make srpm
	fi
	cp /usr/src/redhat/SRPMS/mysql++-$VERSION-1.src.rpm $TMPDIR
fi

cp /usr/src/redhat/RPMS/i386/mysql++-$VERSION-1.i386.rpm \
		$TMPDIR/mysql++-$VERSION-1.`./osver`.i386.rpm
cp /usr/src/redhat/RPMS/i386/mysql++-devel-$VERSION-1.i386.rpm \
		$TMPDIR/mysql++-devel-$VERSION-1.`./osver`.i386.rpm

scp $SSH_OPTIONS $TMPDIR/* $SSH_TARGET

rm -rf $TMPDIR
