# this is a makefile for making a manual

default: manual

perlclean: 
	-rm -f manual.txt manual.html
	$(MAKE) -C ../sqlplusint/ perlclean

manual:
#	$(MAKE) -C ../sqlplusint/  classref
#	perl insert-classref.pl
#	-rm -f -r man-html
#	-mkdir man-html
#	perl examples-proc.pl
#	latex manual.tex
#	latex manual.tex
	latex2html \
	  -split 3 \
	  -toc_depth 3 \
	  -dir man-html \
	  -long_titles 2 \
	  -local_icons \
	  -show_section_numbers \
		-image_type gif \
	   manual.tex

	-rm -f -r man-text
	-mkdir man-text
	perl html2txt.pl

