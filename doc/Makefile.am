# This file should cause the manual to be a generated source (i.e., so
# that it is build at distribution time since its creation is
# non-standard) 

# This file still needs a lot of work...

# SUBDIRS = man-html man-text

EXTRA_DIST = \
  doc.tex.tmpl html.sty manual.aux manual.dvi manual.log manual.lyx \
  manual.ps manual.tex pretty.pl sql++pretty doc-fix.pl doc-trim.pl \
  examples-proc.pl html2txt.pl insert-classref.pl

default: manual

perlclean: 
	-rm -f manual.txt manual.html
	$(MAKE) -C ../sqlplusint/ perlclean

manual:
	$(MAKE) -C ../sqlplusint/  classref
	perl insert-classref.pl
	-rm -f -r man-html
	-mkdir man-html
	perl examples-proc.pl
	latex manual.tex
	latex manual.tex
	latex2html \
	  -split 3 \
	  -toc_depth 3 \
	  -dir man-html \
	  -long_titles 2 \
	  -local_icons \
	  -show_section_numbers \
	   manual.tex

	-rm -f -r man-text
	-mkdir man-text
	perl html2txt.pl

