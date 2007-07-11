## ------------------------
## Things you may need to diddle for your local system
## ------------------------

FOPDIR=/usr/local/fop


## ------------------------
## Input files
## ------------------------

HTML_DIR=../html/userman
BASENAME=userman
DOCFILE=$(BASENAME).dbx
PDFFILE=$(BASENAME).pdf
FOFILE=$(BASENAME).fo
COMMON_SS=common.xsl
FO_SS=fo.xsl
HTML_SS=html.xsl
EX_TXT=cgi_jpeg.txt custom1.txt custom2.txt custom3.txt custom4.txt \
		custom5.txt custom6.txt fieldinf1.txt for_each.txt load_jpeg.txt \
		resetdb.txt simple1.txt simple2.txt simple3.txt stock.txt \
		store_if.txt tquery.txt xaction.txt


## ------------------------
## Major output rules
## ------------------------

html: $(HTML_DIR)/index.html

pdf: $(PDFFILE)


## ------------------------
## Standard Makefile targets
## ------------------------

# Notice that this is not the first target in the file, as is standard.
# PDF generation takes longer than HTML generation, so to keep the code-
# test-debug-rebuild cycle short, we generate only the HTML manual by
# default.  You can explicitly say "make pdf" or "make all" when you're
# sure the DocBook file's contents are correct.
all: html pdf

clean:
	rm -f tags *.fo $(HTML_DIR)/*.html *.log *.out *.pdf *.txt


## ------------------------
## How to make output files
## ------------------------

$(PDFFILE): $(DOCFILE) $(FO_SS) $(COMMON_SS)
	xsltproc --xinclude $(FO_SS) $(DOCFILE) > $(FOFILE)
	$(FOPDIR)/fop -q -fo $(FOFILE) $(PDFFILE)
	cp $(PDFFILE) ../pdf

$(HTML_DIR)/index.html: $(DOCFILE) $(EX_TXT) $(HTML_SS) $(COMMON_SS)
	@xmllint --xinclude --postvalid --noent --noout $(DOCFILE)
	xsltproc --xinclude -o $(HTML_DIR)/ $(HTML_SS) $(DOCFILE)


## ------------------------
## Dependency rules
## ------------------------

$(EX_TXT):
	@./mktxt $@
