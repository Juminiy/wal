export

.PHONY:
	all main build test clean ssrc yyjson

c_args= -std=gnu99 -Wall -pedantic $(c_performance) $(c_mt) $(c_with_mimalloc)
c_performance= -O3 -DPERFORMANCE
c_release= -O2 -DRELEASE
c_debug= -O0 -g -DDEBUG
c_deep_stack= -Wl,--stack=536870912
c_mt= -lpthread -DWITH_MT
c_dll= -fPIC -shared -DWITH_DYLIB
c_debug_compile_args= -v
c_with_mimalloc= -I/usr/local/include/mimalloc-2.1 -L/usr/local/lib -lmimalloc -DWITH_MIMALLOC

_ARCH = $(shell uname -m)
_OS   = $(shell uname -s)
_CC   = $(shell $(CC) --version | head -n 1)

ifeq ($(findstring Darwin,$(_OS)),Darwin)
c_run_opt += env DYLD_INSERT_LIBRARIES=/usr/local/lib/libmimalloc.dylib
prerun= install_name_tool -change @rpath/libmimalloc.2.dylib /usr/local/lib/libmimalloc.2.dylib main
endif
ifeq ($(findstring Linux,$(_OS)),Linux)
c_run_opt += env MIMALLOC_VERBOSE=1 LD_PRELOAD=/usr/lib/libmimalloc.so
prerun=
endif

app_fd= app.log
build_dir= build
src_dir= src
include_dir= include
thirdparty_dir = thirdparty
dst_objs= src/*.o thirdparty/yyjson/*.o thirdparty/sc/map/*.o
exe_O= main

all: clean build main
	$(prerun)
	./$(exe_O)

build: ssrc yyjson sc_map
	rm -rf $(build_dir) && mkdir -p $(build_dir)
	cp $(dst_objs) $(build_dir)

ssrc:
	$(MAKE) -C src

yyjson:
	$(MAKE) -C thirdparty/yyjson

sc_map:
	$(MAKE) -C thirdparty/sc/map

go: libjson_flatten.so
	cd golang && make

main: $(build_dir)/*.o
	$(CC) $(c_args) -o $@ $^

main_use_dll:
# $(CC) $(c_args) -I../include -DWITH_DYLIB -o main.o src/main.c
	$(CC) $(c_args) -I../include -DWITH_DYLIB -L. build/main.o -ljson_flatten -o main
	$(prerun)

libjson_flatten.so: src/json_flatten.c thirdparty/yyjson/yyjson.c thirdparty/sc/map/sc_map.c
	$(CC) $(c_args) $(c_dll) -o $@ $^
	sudo chmod 777 $@
	cp $@ golang/$@

clean:
	rm -f main *.o *.exe
	rm -f $(app_fd)
	rm -f src/*.o && rm -rf $(build_dir)

clean_under_objs:
	rm -f $(dst_objs) libjson_flatten.so
	cd golang && make clean

install_thirdparty:
	cd thirdparty && git clone https://github.com/tezc/sc.git

# template not modify
objs: $(patsubst %.c,%.o,$(wildcard *.c))

%.o: %.c
	$(CC) -c $(c_args) -o $@ $<

