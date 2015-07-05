
#INCLUDES = @GLIB_CFLAGS@ -I$(top_srcdir)/include


#BUILT_SOURCES = parser.h
#PARSER=parser.y parser.l

#AM_YFLAGS = -d


# what flags you want to pass to the C compiler & linker
AM_CFLAGS =  -Wall -std=c99 
AM_LDFLAGS =

	
libnode_a_CFLAGS =  -fPIC -std=c99 #-include libnode_a-yparser.h -fPIC # -DDEBUG_PARSER 
libnode_a_YFLAGS = -d 
libnode_a_LFLAGS =  
libnode_a_SOURCES= \
node.c\
node.h\
thread.c\
thread.h\
list.c\
list.h\
yparser.y \
scanner.l\
parser.h\
parser.c\
stack.c\
stack.h\
link_list.c\
link_list.h\
python_maker.c\
python_maker.h


noinst_LIBRARIES= libnode.a 

#libccoin_a_LIBADD = $(top_srcdir)/src/libmalloc_la-malloc.lo
 




#-version-info 1:2:3


#lib_LTLIBRARIES=libmalloc.ld
#libmalloc_la_LDFLAGS=-fPIC -shared -o libsample.so -lc -ldl
#libmalloc_la_SOURCES = libmalloc.c


