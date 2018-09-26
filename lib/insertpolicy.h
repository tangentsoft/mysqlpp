#!/bin/bash
AHCMD=/usr/XSLFormatterV42/run.sh
FOPCMD=$(type -p fop)
XEPCMD=/usr/local/xep/xep

FOFILE=$1
PDFFILE=$2
if [ -n "$FOFILE" -a -r "$FOFILE" -a -n "$PDFFILE" ]
then
	if [ -x "$XEPCMD" ]
	then
		echo Rendering $FOFILE to $PDFFILE with RenderX XEP...
		$XEPCMD -quiet -fo $FOFILE -pdf $PDFFILE
	elif [ -x "$AHCMD" ]
	then
		echo Rendering $FOFILE to $PDFFILE with Antenna House XSL Formatter...
		$AHCMD -silent -d $FOFILE -o $PDFFILE
	elif [ -x "$FOPCMD" ]
	then
		echo "Rendering $FOFILE to $PDFFILE with Apache FOP ($FOPCMD)..."
		$FOPCMD -q -fo $FOFILE $PDFFILE
	else
		echo 'Cannot find a working XSL-FO processor on your system!  See'
		echo 'doc/usrman/README.txt for instructions on installing one.'
		echo
		exit 1
	fi
else
	if [ -n "$FOFILE" ]
	then
		echo "XSL-FO file $FOFILE does not exist."
		echo
	fi
	echo "usage: $0 fofile pdffile"
	echo
	echo '    Translates XSL-FO in fofile to PDF output in pdffile.'
	echo
	exit 1
fi
