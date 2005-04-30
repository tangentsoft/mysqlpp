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
EX_TXT=cgi_image.txt complic1.txt custom1.txt custom2.txt \
		custom3.txt custom4.txt fieldinf1.txt load_file.txt \
		resetdb.txt simple1.txt updel.txt 
EX_SRC=$(EX_TXT:.txt=.cpp)


## ------------------------
## Standard Makefile targets
## ------------------------

all: $(EX_TXT) html/index.html

clean:
	rm -rf html *.aux *.cpp *.fo *.log *.out *.pdf *.txt


## ------------------------
## Major output rules
## ------------------------

pdf:
	$(FOPDIR)/fop.sh -q -xml $(DOCFILE) -xsl $(FO_SS) $(BASENAME).pdf
	mv $(BASENAME).pdf ../mysqlpp-$(BASENAME).pdf

oldpdf:
	xsltproc --xinclude $(FO_SS) $(DOCFILE) > $(BASENAME).fo
	pdfxmltex $(BASENAME).fo 2>&1 > /dev/null
	mv $(BASENAME).pdf ../mysqlpp-$(BASENAME).pdf

html/index.html: $(DOCFILE) $(HTML_SS)
	@xmllint --valid --noout $(DOCFILE)
	@mkdir -p html
	xsltproc -o html/ $(HTML_SS) $(DOCFILE)


## ------------------------
## Dependency rules
## ------------------------

.SUFFIXES: .txt

.cpp.txt:
	cat $< | sed -e 's/&/\&amp;/g' | sed -e 's/</\&lt;/g' | sed -e 's/>/\&gt;/g' > $@

$(EX_SRC):
	expand -t4 < ../../examples/$@ > $@

$(HTML_SS): $(HTML_SS).in
	./findxsl
