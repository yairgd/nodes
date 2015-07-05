AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS=-I m4

SUBDIRS = src test python 

mydist: clean
	@tar cjf ../algotrade.tar.bz2 .
	@mv  ../algotrade.tar.bz2 .
