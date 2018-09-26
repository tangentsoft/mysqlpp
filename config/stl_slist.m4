The user manual is written in XML DocBook format, version 4.2. We
restrict ourselves to Simplified DocBook 1.1 as much as possible.
(Why these versions? They're the newest supported on the oldest system
I still use, Red Hat Linux 9.)

If you're looking to hack on the manual, here are some helpful resources
for getting up to speed on DocBook:


Mills' "Installing And Using An XML/SGML DocBook Editing Suite" article:

	http://supportweb.cs.bham.ac.uk/documentation/tutorials/docsystem/build/tutorials/docbooksys/docbooksys.html

	This is the best tutorial I've found.
	

Walsh and Muellner's _Simplified DocBook: The Definitive Guide_ book:

	http://www.docbook.org/tdg/simple/en/html/sdocbook.html

	This is the most accessible reference.


Walsh and Muellner's _DocBook: The Definitive Guide_ book, second
edition, online version:

	http://www.docbook.org/tdg/en/html/docbook.html

	This is the official DocBook referece; the "Simplified" guide is a
	subset of this book.


DocBook FAQ:

	http://www.dpawson.co.uk/docbook/

	Go here when you have a question that the tutorials and references
	do not answer.


xsltproc:

	http://xmlsoft.org/XSLT/

	This is the XSL processor that takes the DocBook file and an XSL
	stylesheet and produces either HTML or XSL+FO. (The latter is an
	intermediate step towards any of several print-oriented formats,
	such as PDF or TeX.) If your system has GNOME on it, you probably
	have this installed already.

	You will also need the standard set of DocBook style sheets.  On
	Red Hat Linux and Fedora Core, these are in the docbook-style-xsl
	package.


FOP:

	http://xml.apache.org/fop/

	This is the XSL+FO document processor we use to build the PDF
	files. ('make pdf')

	If for some reason you have trouble getting FOP working, there's
	an alternate method available as 'make oldpdf' which uses xsltproc
	and pdfxmltex. It produces a pretty ugly PDF, but you probaly
	already have the tools installed, if you have any of them at all.
	pdfxmltex is in the xmltex package on Fedora Core 3.

	If neither of those work for you, you could use the OpenJade
	tool chain instead. (http://openjade.org/) This is SGML DocBook
	rather than the more modern XML DocBook I favor, but the only
	XML-specific thing we are using is local XSL stylesheets. You
	could ignore them and use the standard DSSSL stylesheets
	instead. I avoid the SGML tool chain only because I've had
	trouble with it in the past.


The official DocBook site:

	http://docbook.org/

