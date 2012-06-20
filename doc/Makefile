PDFTEX	=	pdflatex -halt-on-error
PDFOPN	=	open -a /Applications/Preview.app
BIBTEX	=	bibtex

COMPILE.tex	=	$(PDFTEX) $(*F).tex > /dev/null

COMPILE.bib =	-$(BIBTEX) $(*F) > /dev/null

DIRS	=	. report
DOCS	=	report

%.aux: %.tex
	$(COMPILE.tex)

%.bbl: %.aux
	$(COMPILE.bib)
	$(COMPILE.tex)

%.pdf: %.bbl
	$(COMPILE.tex)

%: %.pdf
	$(PDFOPN) $<

.PHONY: clean clean-pdf clean-bib view check

default:	report.pdf

all: $(DOCS)

report.bbl: references.bib
report.aux: report/10-intro.tex
report.aux: report/20-problem.tex
report.aux: report/30-implementation.tex
report.aux: report/31-parallelism.tex
report.aux: report/40-comparison.tex
report.aux: report/41-environment.tex
report.aux: report/42-methodology.tex
report.aux: report/43-results.tex
report.aux: report/80-conclusion.tex

slides.aux: slides/10-problem.tex
slides.aux: slides/20-implementation.tex
slides.aux:	slides/30-experiment.tex
slides.aux: slides/images/rand-01.png
slides.aux: slides/images/rand-192.png
slides.aux:	slides/31-methodenv.tex
slides.aux: slides/90-conclusion.tex

view: all
	open $(DOCS)	

clean:	clean-pdf clean-bib

clean-pdf:
	$(RM) *.{aux,out,log,synctex.gz,pdf}

clean-bib:
	$(RM) *.{bbl,blg}
