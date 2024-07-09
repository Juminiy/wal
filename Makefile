export

.PHONY:
	all main build test clean ssrc yyjson

c_args= -std=gnu99 -Wall -pedantic $(c_debug) -lpthread
c_release= -O2 -DRELEASE
c_debug= -O0 -g -DDEBUG
c_deep_stack= -Wl,--stack=536870912

app_fd= app.log
build_dir= build
src_dir= src
include_dir= include
thirdparty_dir = thirdparty
dst_objs= src/*.o thirdparty/yyjson/*.o thirdparty/c_hashmap/*.o
exe_O= main

all: clean build main 
	./$(exe_O)

build: ssrc yyjson
	rm -rf $(build_dir) && mkdir -p $(build_dir)
	cp $(dst_objs) $(build_dir)

ssrc:
	$(MAKE) -C src

yyjson:
	$(MAKE) -C thirdparty/yyjson

c_hashmap:
	$(MAKE) -C thirdparty/c_hashmap

main: $(build_dir)/*.o
	$(CC) $(c_args) -o $@ $^

clean:
	rm -f main *.o *.exe
	rm -f $(app_fd)
	rm -f src/*.o && rm -rf $(build_dir)


# template notmodify
objs: $(patsubst %.c,%.o,$(wildcard *.c))

%.o: %.c
	$(CC) -c $(c_args) -o $@ $<

