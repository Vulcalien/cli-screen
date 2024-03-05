DOC_DIR := doc

ADOC_TO_MANPAGES := asciidoctor -b manpage

.PHONY: manpages

manpages:
	$(ADOC_TO_MANPAGES) $(wildcard $(DOC_DIR)/*.adoc)
