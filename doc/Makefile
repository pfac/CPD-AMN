PDFTEX	=	pdflatex
BIBTEX	=	bibtex

DOCS	=	report.pdf

%.pdf: %.tex
	$(PDFTEX) $<
	$(BIBTEX) $(*F)
	$(PDFTEX) $<
	$(PDFTEX) $<

.PHONY: clean clean-pdf clean-bib view check

all: $(DOCS)

report.pdf: references.bib
report.pdf: report/01-intro.tex
report.pdf: report/02-problem.tex

check:
	hunspell $(DOCS:%.pdf=%.tex)

view: all
	open $(DOCS)	

clean:	clean-pdf clean-bib

clean-pdf:
	$(RM) *.{aux,out,log,synctex.gz,pdf}

clean-bib:
	$(RM) *.{bbl,blg}
