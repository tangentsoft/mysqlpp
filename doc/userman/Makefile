## ------------------------
## Things you may need to diddle for your local system
## ------------------------

FOPDIR=/usr/local/fop


## ------------------------
## Input files
## ------------------------

BASENAME=userman
DOCFILE=$(BASENAME).xml
HTML_SS=html.xsl
FO_SS=fo.xsl
EX_TXT=cgi_image.txt custom1.txt custom2.txt custom3.txt custom4.txt \
		custom5.txt fieldinf1.txt load_file.txt resetdb.txt \
		simple1.txt updel.txt util.txt


## ------------------------
## Standard Makefile targets
## ------------------------

all: $(EX_TXT) $(HTML_SS) $(FO_SS) html/index.html

clean:
	rm -rf html *.aux *.cpp *.fo *.log *.out *.pdf *.txt


## ------------------------
## Major output rules
## ------------------------

pdf:
	$(FOPDIR)/fop.sh -q -xml $(DOCFILE) -xsl $(FO_SS) $(BASENAME).pdf
	mv $(BASENAME).pdf ../mysqlpp-$(BASENAME).pdf

oldpdf:
	xsltproc $(FO_SS) $(DOCFILE) > $(BASENAME).fo
	pdfxmltex $(BASENAME).fo 2>&1 > /dev/null
	mv $(BASENAME).pdf ../mysqlpp-$(BASENAME).pdf

html/index.html: $(DOCFILE)
	@xmllint --valid --noout $(DOCFILE)
	@mkdir -p html
	xsltproc -o html/ $(HTML_SS) $(DOCFILE)


## ------------------------
## Dependency rules
## ------------------------

$(EX_TXT):
	expand -t4 < `echo ../../examples/$@ | sed -e s/txt/cpp/` | sed -e 's/&/\&amp;/g' | sed -e 's/</\&lt;/g' | sed -e 's/>/\&gt;/g' > $@

$(HTML_SS): $(HTML_SS).in
	./findxsl

$(FO_SS): $(FO_SS).in
	./findxsl

