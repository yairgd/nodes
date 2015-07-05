
#INCLUDES = @GLIB_CFLAGS@ -I$(top_srcdir)/includ



# what flags you want to pass to the C compiler & linker
AM_CFLAGS =  -Wall -std=c99  
AM_LDFLAGS = -lpthread 
AM_CPPFLAGS = -I$(top_srcdir)/src 


if UNIT_TEST
test_bins = simple_parser node_ex1 node_ex2 link_list
endif
bin_PROGRAMS = tree_build_with_parser ${test_bins}

simple_parser_CFLAGS = -Wl,--wrap=ex
simple_parser_LDADD = $(top_srcdir)/src/libnode.a -lfl
simple_parser_LDFLAGS= -lpthread #-ldl -lc -lssl -lcrypto
simple_parser_SOURCES= \
simple_parser.c

node_ex1_LDADD = $(top_srcdir)/src/libnode.a 
node_ex1_LDFLAGS= -lpthread
node_ex1_SOURCES= \
node_ex1.c

node_ex2_LDADD = $(top_srcdir)/src/libnode.a 
node_ex2_LDFLAGS= -lpthread
node_ex2_SOURCES= \
node_ex2.c


link_list_LDADD = $(top_srcdir)/src/libnode.a
link_list_LDFLAGS=
link_SOURCES= \
link_list.c


tree_build_with_parser_CFLAGS = -std=c99
tree_build_with_parser_LDADD = $(top_srcdir)/src/libnode.a -lfl 
tree_build_with_parser_LDFLAGS= -lpthread
tree_build_with_parser_SOURCES= \
tree_build_with_parser.c




#test2_LDADD = $(top_srcdir)/src/libccoin.a #$(top_srcdir)/src/libmalloc_la-malloc.lo
#test2_LDFLAGS=-ldl -lc -lssl -lcrypto
#test2_SOURCES= \
#test2.c
