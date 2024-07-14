export

.PHONY:
	all main build test clean ssrc yyjson

c_args= -std=gnu99 -pedantic -Wall $(c_release) $(c_mt) $(c_with_sc) $(c_with_yyjson) $(c_with_mimalloc)
c_performance= -O3 -DPERFORMANCE
c_release= -O2 -DRELEASE
c_debug= -O0 -g -DDEBUG
c_deep_stack= -Wl,-stack,536870912
c_mt= -lpthread -DWITH_MT
c_dll= -fPIC -shared -DWITH_DYLIB
c_debug_compile_args= -v

c_with_json_flatten= -I/usr/local/include/json_flatten -L/usr/local/lib -ljson_flatten
c_with_sc= -I/usr/local/include/sc -L/usr/local/lib -lsc_map -DWITH_SC
c_with_yyjson= -I/usr/local/include/yyjson -L/usr/local/lib -lyyjson -DWITH_YYJSON
c_with_mimalloc= -I/usr/local/include/mimalloc -L/usr/local/lib -lmimalloc -DWITH_MIMALLOC

_ARCH = $(shell uname -m)
_OS   = $(shell uname -s)
_CC   = $(shell $(CC) --version | head -n 1)

ifeq ($(findstring Darwin,$(_OS)),Darwin)
c_run_opt += env DYLD_INSERT_LIBRARIES=/usr/local/lib/libmimalloc.dylib
c_dll_suffix= .dylib
os_install= brew install
prerun= install_name_tool -change @rpath/libmimalloc.1.dylib /usr/local/lib/libmimalloc.1.dylib main
work_dir= ~/code/c/wal/
endif
ifeq ($(findstring Linux,$(_OS)),Linux)
c_run_opt += env MIMALLOC_VERBOSE=1 LD_PRELOAD=/usr/lib/libmimalloc.so
c_dll_suffix= .so
os_install= sudo apt-get install
prerun= export LD_LIBRARY_PATH=/usr/local/lib
work_dir= ~/wal/
endif

app_log= app.log
bin_file= *.o *.ko *.obj *.elf *.so.* *.so *.dylib *.dll *.lo *.la *.a *.lib main *.exe *.out *.app
bin_dir= bin
build_dir= build
include_dir= include
src_dir= src
thirdparty_dir= thirdparty
dst_objs= src/*.o
exe_of= main

all: clean build main
	$(prerun)
	./$(exe_of)

go:
	cd golang && make

build: ssrc
	rm -rf $(build_dir) && mkdir -p $(build_dir)
	cp $(dst_objs) $(build_dir)

ssrc:
	$(MAKE) -C src

main: $(build_dir)/*.o
	$(CC) $(c_args) -o $@ $^

clean:
	rm -rf $(bin_dir) $(build_dir)
	rm -f $(app_log) $(bin_file)
	cd $(src_dir) && rm -f $(bin_file)
	cd golang && rm -f $(bin_file)
	sudo rm -rf $(thirdparty_dir)

json_flatten: src/json_flatten.c
	sudo rm -rf /usr/local/include/json_flatten
	sudo mkdir -p /usr/local/include/json_flatten
	sudo cp $(include_dir)/json_flatten.h /usr/local/include/json_flatten
	sudo $(CC) -c $(c_args) $(c_dll) -o lib$@$(c_dll_suffix) $<
	sudo mv lib$@$(c_dll_suffix) /usr/local/lib

# before run please change your work_dir
install_thirdparty:
	sudo rm -rf $(thirdparty_dir) && sudo mkdir -p $(thirdparty_dir)
	cd $(thirdparty_dir) && sudo git clone https://github.com/tezc/sc.git
	cd $(thirdparty_dir) && sudo git clone https://github.com/ibireme/yyjson.git
	cd $(thirdparty_dir) && sudo git clone https://github.com/microsoft/mimalloc.git
	cd $(thirdparty_dir)/sc 	    && sudo rm -rf /usr/local/include/sc 	  && sudo mkdir -p /usr/local/include/sc 	 && sudo cp array/sc_array.h map/sc_map.h /usr/local/include/sc && cd map && sudo rm -f map_example.c map_test.c && sudo cp $(work_dir)/Makefile.dylib.tpl Makefile && sudo make && sudo mv libsc_map.so /usr/local/lib
	cd $(thirdparty_dir)/yyjson     && sudo rm -rf /usr/local/include/yyjson  && sudo mkdir -p /usr/local/include/yyjson && sudo cp src/yyjson.h /usr/local/include/yyjson 			    && cd src 								    	 && sudo cp $(work_dir)/Makefile.dylib.tpl Makefile && sudo make && sudo mv libyyjson.so /usr/local/lib
	cd $(thirdparty_dir)/mimalloc   && sudo mkdir -p out/release && cd out/release && $(os_install) cmake make gcc && sudo cmake ../.. && sudo make -j8 && sudo make install && [ -d "/usr/local/include/mimalloc-1.8" ] && sudo cp -r /usr/local/include/mimalloc-1.8 /usr/local/include/mimalloc && [ -d "/usr/local/include/mimalloc-2.1" ] && sudo cp -r /usr/local/include/mimalloc-2.1 /usr/local/include/mimalloc 
