PDFTEX	=	pdflatex -halt-on-error
PDFOPN	=	open -a /Applications/Preview.app
BIBTEX	=	bibtex

COMPILE.tex	=	$(PDFTEX) $(*F).tex > /dev/null

COMPILE.bib =	$(BIBTEX) $(*F) > /dev/null

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

all: $(DOCS)

report.bbl: references.bib
report.pdf: report/10-intro.tex
report.pdf: report/20-problem.tex
report.pdf: report/30-implementation.tex
report.pdf: report/31-parallelism.tex
report.pdf: report/40-comparison.tex
report.pdf: report/41-environment.tex
report.pdf: report/42-methodology.tex
report.pdf: report/43-results.tex
report.pdf: report/80-conclusion.tex

check:
	for d in $(DIRS);
	do
		for f in `ls "$d" | grep "tex"`;
		do
			hunspell "$f";
		done;
	done;

view: all
	open $(DOCS)	

clean:	clean-pdf clean-bib

clean-pdf:
	$(RM) *.{aux,out,log,synctex.gz,pdf}

clean-bib:
	$(RM) *.{bbl,blg}
