## ------------------------
## Things you may need to diddle for your local system
## ------------------------

FOPDIR=/usr/local/fop


## ------------------------
## Input files
## ------------------------

BASENAME=userman
DOCFILE=$(BASENAME).dbx
PDFFILE=$(BASENAME).pdf
FOFILE=$(BASENAME).fo
COMMON_SS=common.xsl
FO_SS=fo.xsl
HTML_SS=html.xsl
EX_TXT=cgi_image.txt custom1.txt custom2.txt custom3.txt custom4.txt \
		custom5.txt custom6.txt fieldinf1.txt load_file.txt \
		resetdb.txt simple1.txt simple2.txt simple3.txt stock.txt \
		updel.txt util.txt xaction.txt


## ------------------------
## Major output rules
## ------------------------

html: html/index.html

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
	rm -f tags *.fo html/*.html *.log *.out *.pdf *.txt


## ------------------------
## How to make output files
## ------------------------

$(PDFFILE): $(DOCFILE) $(FO_SS) $(COMMON_SS)
	xsltproc --xinclude $(FO_SS) $(DOCFILE) > $(FOFILE)
	$(FOPDIR)/fop -q -fo $(FOFILE) $(PDFFILE)
	cp $(PDFFILE) ../mysqlpp-$(PDFFILE)

html/index.html: $(DOCFILE) $(EX_TXT) $(HTML_SS) $(COMMON_SS)
	@mkdir -p html
	@xmllint --xinclude --postvalid --noent --noout $(DOCFILE)
	xsltproc --xinclude -o html/ $(HTML_SS) $(DOCFILE)


## ------------------------
## Dependency rules
## ------------------------

$(EX_TXT):
	@./mktxt $@
