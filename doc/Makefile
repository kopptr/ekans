TEX = pdflatex
BIB = bibtex

DOC = spec

PDF = $(patsubst %, %.pdf, $(DOC))

spec.pdf: spec.tex
	- $(TEX) $<
	- $(TEX) $<
	- $(TEX) $<
	- $(TEX) $<
	cleantex