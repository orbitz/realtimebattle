docsdir = $(DOCS_DIR)/German
docs_DATA = \
   RealTimeBattle-1.html RealTimeBattle-2.html RealTimeBattle-3.html \
   RealTimeBattle-4.html RealTimeBattle-5.html RealTimeBattle-6.html \
   RealTimeBattle.html next.gif prev.gif toc.gif next.png prev.png toc.png RealTimeBattle.dvi RealTimeBattle.sgml RealTimeBattle.txt RealTimeBattle.info

EXTRA_DIST = $(docs_DATA)

PAPER_SIZE = a4

all: docs

docs: RealTimeBattle-1.html RealTimeBattle-2.html RealTimeBattle-3.html \
      RealTimeBattle-4.html RealTimeBattle-5.html RealTimeBattle-6.html \
      RealTimeBattle.html RealTimeBattle.dvi RealTimeBattle.info RealTimeBattle.txt

html_docs: RealTimeBattle-1.html RealTimeBattle-2.html RealTimeBattle-3.html \
           RealTimeBattle-4.html RealTimeBattle-5.html RealTimeBattle-6.html \
           RealTimeBattle.html


# Targets to generate the files if the sgml source exists, otherwise nothing happens 

if SGML2HTML
%.html: %.sgml
	@if [ -f $< ]; then \
		echo "sgml2html --split=0 -c latin $<"; \
		sgml2html --split=0 -c latin $<; \
	fi
else
%.html: %.sgml
	@echo "Warning: $< cannot be generated, sgml2html is missing."; \
	touch $@
endif

#	elif [ ! -f $@ ]; then \
#		echo "Error: $@ is missing and cannot be generated"; \
#	fi


if SGML2HTML
RealTimeBattle%.html: RealTimeBattle.sgml
	@if [ -f $< ]; then \
		echo "sgml2html --imagebuttons -D table=real -c latin $<"; \
		sgml2html --imagebuttons -D table=real -c latin $<; \
	elif [ ! -f $@ ]; then \
		echo "Error: $@ is missing and cannot be generated"; \
	fi
else
RealTimeBattle%.html: RealTimeBattle.sgml
	@echo "Warning: $@ cannot be generated, sgml2html is missing."; \
	touch $@
endif



if SGML2LATEX
%.dvi: %.sgml
	@if [ -f $< ]; then \
		echo "sgml2latex  -p $(PAPER_SIZE) -D table=real -c latin -o dvi $<"; \
		sgml2latex  -p $(PAPER_SIZE) -D table=real -c latin  -o dvi $<; \
	elif [ ! -f $@ ]; then \
		echo "Error: $@ is missing and cannot be generated"; \
	fi
else
%dvi: %.sgml
	@echo "Warning: $@ cannot be generated, sgml2latex is missing."; \
	touch $@
endif



if SGML2LATEX
%.ps: %.sgml
	@if [ -f $< ]; then \
		echo "sgml2latex  -p $(PAPER_SIZE) -D table=real -c latin -o ps $<"; \
		sgml2latex  -p $(PAPER_SIZE)  -D table=real -c latin -o ps $<; \
	elif [ ! -f $@ ]; then \
		echo "Error: $@ is missing and cannot be generated"; \
	fi
else
%.ps: %.sgml
	@echo "Warning: $@ cannot be generated, sgml2latex is missing."; \
	touch $@
endif




if SGML2INFO
%.info: %.sgml
	@if [ -f $< ]; then \
		echo "sgml2info  -D table=char -c latin $<"; \
		sgml2info  -D table=char -c latin $<; \
	elif [ ! -f $@ ]; then \
		echo "Error: $@ is missing and cannot be generated"; \
	fi
else
%.info: %.sgml
	@echo "Warning: $@ cannot be generated, sgml2info is missing."; \
	touch $@
endif




if SGML2RTF
%.rtf: %.sgml
	@if [ -f $< ]; then \
		echo "sgml2rtf -D table=real -c latin $<"; \
		sgml2rtf  -D table=real -c latin $<; \
	elif [ ! -f $@ ]; then \
		echo "Error: $@ is missing and cannot be generated"; \
	fi
else
%.rtf: %.sgml
	@echo "Warning: $@ cannot be generated, sgml2rtf is missing."; \
	touch $@
endif



if SGML2LYX
%.lyx: %.sgml
	@if [ -f $< ]; then \
		echo "sgml2lyx -D table=real -c latin $<"; \
		sgml2lyx  -D table=real -c latin $<; \
	elif [ ! -f $@ ]; then \
		echo "Error: $@ is missing and cannot be generated"; \
	fi
else
%.lyx: %.sgml
	@echo "Warning: $@ cannot be generated, sgml2lyx is missing."; \
	touch $@
endif



if SGML2TXT
%.txt: %.sgml
	@if [ -f $< ]; then \
		echo "sgml2txt --filter -D table=char -c latin $<"; \
		sgml2txt --filter -D table=char -c latin $<; \
	fi
else
%.txt: %.sgml
	@echo "Warning: $@ cannot be generated, sgml2txt is missing."; \
	touch $@
endif



if SGML2TXT
../%: %.sgml
	@if [ -f $< ]; then \
		echo "sgml2txt --filter -D table=char -c latin $<"; \
		sgml2txt --filter -D table=char -c latin $<; \
		echo "mv $(subst ../$(subst ../,,$@),$(subst ../,,$@).txt,$@) $@"; \
		mv $(subst ../$(subst ../,,$@),$(subst ../,,$@).txt,$@) $@; \
	elif [ ! -f $@ ]; then \
		echo "Error: $@ is missing and cannot be generated"; \
	fi
else
../%: %.sgml
	@echo "Warning: $@ cannot be generated, sgml2txt is missing."; \
	touch $@
endif


#Just to allow for non-existing .sgml-file
%.sgml: 
	@true


