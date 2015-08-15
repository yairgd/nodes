
#INCLUDES = @GLIB_CFLAGS@ -I$(top_srcdir)/include


#BUILT_SOURCES = parser.h
#PARSER=parser.y parser.l

#AM_YFLAGS = -d


# what flags you want to pass to the C compiler & linker
AM_CFLAGS =  -Wall -std=c99 
AM_LDFLAGS =

	
libjson_a_CFLAGS =  -fPIC -std=c99 #-include libnode_a-yparser.h -fPIC # -DDEBUG_PARSER 
libjson_a_YFLAGS = -d 
libjson_a_LFLAGS =  
libjson_a_SOURCES= \
yjson.y\
ljson.l\	
json.c\
json.h

noinst_LIBRARIES= libjson.a 

#libccoin_a_LIBADD = $(top_srcdir)/src/libmalloc_la-malloc.lo
 




#-version-info 1:2:3


#lib_LTLIBRARIES=libmalloc.ld
#libmalloc_la_LDFLAGS=-fPIC -shared -o libsample.so -lc -ldl
#libmalloc_la_SOURCES = libmalloc.c

clean:
	rm libjson*

