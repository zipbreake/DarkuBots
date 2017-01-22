# Makefile for language module

include ../Makefile.inc


LANGOBJS = es
LANGSRCS = es.l

LANGCOMP = ./langcomp
#LANGCOMP = ./langcomp -w


all: $(LANGOBJS)


install: all
	mkdir -p $(DATDEST)/languages
ifdef RUNGROUP
	chgrp $(RUNGROUP) $(DATDEST)/languages
	chmod 770 $(DATDEST)/languages
else
	chmod 700 $(DATDEST)/languages
endif
	cp $(LANGOBJS) $(DATDEST)/languages
ifdef RUNGROUP
	chgrp $(RUNGROUP) $(DATDEST)/languages/*
	chmod 660 $(DATDEST)/languages/*
else
	chmod 600 $(DATDEST)/languages/*
endif


clean:
	rm -f $(LANGOBJS) langcomp

spotless: clean
	rm -f language.h



en_us:    en_us.l    langcomp index
	./langcomp $@.l
es:       es.l       langcomp index
	./langcomp $@.l
it:       it.l       langcomp index
	./langcomp $@.l
ja_euc:   ja_euc.l   langcomp index
	./langcomp $@.l
ja_jis:   ja_jis.l   langcomp index
	./langcomp $@.l
ja_sjis:  ja_sjis.l  langcomp index
	./langcomp $@.l
pt:       pt.l       langcomp index
	./langcomp $@.l
tr:       tr.l       langcomp index
	./langcomp $@.l
ca:       ca.l       langcomp index
	./langcomp $@.l
ga:       ga.l       langcomp index
	./langcomp $@.l

ja_jis.l: ja_euc.l jconv.pl
	perl jconv.pl -jis <ja_euc.l >$@
ja_sjis.l: ja_euc.l jconv.pl
	perl jconv.pl -sjis <ja_euc.l >$@

langcomp: langcomp.c
	$(CC) $(CFLAGS) langcomp.c -o $@


language.h: index Makefile
	@perl -e <index >$@ '\
		print STDERR "Generando language.h... "; \
		$$i=0; \
		while (<>) { \
			chop; \
			printf "#define %-32s %d\n", $$_, $$i++; \
		} \
		print "\n#define NUM_STRINGS $$i\n"; \
		print STDERR "$$i strings\n";'


