# Makefile template to compile dylib, do not modify

c_args= -std=gnu99 -pedantic -Wall $(c_release) $(c_mt)
c_performance= -O3 -DPERFORMANCE
c_release= -O2 -DRELEASE
c_debug= -O0 -g -DDEBUG
c_deep_stack= -Wl,-stack,536870912
c_mt= -lpthread -DWITH_MT
c_dll= -fPIC -shared -DWITH_DYLIB
c_debug_compile_args= -v

c_with_sc= -I/usr/local/include -L/usr/local/lib -lsc -DWITH_SC
c_with_yyjson= -I/usr/local/include -L/usr/local/lib -lyyjson -DWITH_YYJSON
c_with_mimalloc= -I/usr/local/include -L/usr/local/lib -lmimalloc -DWITH_MIMALLOC

objs: $(patsubst %.c,%.so,$(wildcard *.c))

%.so: %.c
	$(CC) -c $(c_args) $(c_dll) -o $@ $<
	mv $@ lib$@