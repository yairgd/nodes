#!/bin/bash
autoconf
libtoolize
aclocal
automake --add-missing
autoreconf --install
./configure  --enable-debug --enable-debug-parser --enable-unit-test


