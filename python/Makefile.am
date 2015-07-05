AM_CFLAGS =  -Wall -std=c99 
AM_LDFLAGS =
all:	py

py: 
	@if [  -d ./build ]; then \
		rm build -R ; \
	fi
	@python setup.py install --user
clean:
	@if [  -d ./build ]; then \
		rm build -R ; \
	fi
