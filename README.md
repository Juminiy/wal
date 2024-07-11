# wal
> all components either memory/disk 
> 1. copied
> 2. duplicated
> 3. mutex, rwmutex protected
> 4. not in resource raced status
> ...
> anyway must be MTS
> MTS: multiple threads safe
1. log
   1. macro log type: info, warn, error, debug, fatal
   2. SAFE: fmt log msg length is restricted
   3. BUG: msg truncated and overflow
      1. (buffer BUG in windows msys2 BUG, not mine)
      2. MacOS-Arm64 and Ubuntu-Amd64 is OK
      3. has been solved
   4. TODO: enable color
   5. hasDone: ILOG, MTS, Buffer Not Safe
2. coworker
   1. concurrency in multithreads
   2. sync, async, time record
   3. mutex, cond signal, progress record
   4. param_list, return values set
   5. TODO: error cancel, timeout control
3. wal
   1. normal log in app
   2. fd,write fd,fsync
   3. write ahead logging
4. wal in coworker
5. yyjson parse json to accelerate json parser
   1. yyjson read jsonstring not need to be escaping, only json format is ok
   2. struct of binding, serialize, unserialize
   3. easy search key: full_path, short_key, value
   4. json flatten: full_path, short_key, array_value
   5. TODO: how to switch tag to get type and value
6. TODO: find a c lib of map any_val -> any_val
7. all thirdparty library can be found on Github search by package name LIKE: yyjson
8. TODO: micorsoft/mimalloc
9. report BUG of opensource lib: uthash, c_hashmap
10. anyway: the API design is:
    1.  Go Server Loading a plugin of gnuc99 target .o file called plugin_json_flatten.
    2.  go program send a original json binary bytestream to plugin_json_flatten.
    3.  gnuc99 parse and flatten json to new json format, write a new json to a new bytestream.
    4.  go receive a flatten json binary bytestream to parse it use predefined format in go.
    5.  now go only to recycle garbage of original bytestream and new bytestream and new struct of.
11. internal:
    1.  use sc_map to store string to array of value by map[char*]void* -> void* reinterpret to array of real value.
    2.  ~~use bidirectional linked list to store the order of key.~~
    3.  map[char*]void* to map full_path -> void* reinterpret to {uint64_t tag; void * arr;}
    4.  write back to json_string use of mut_json
    5.  receive from Go bytestream to char *
    6.  send char * to Go bytestream
    7.  free gnuc99 alloc to use mimalloc
12. memory leak saved method:
    1.  record for all alloc but maynot be freed memory
    2.  use freelist or mimalloc in application level mimalloc and mifree for reused and preheet
    3.  memory leak tools like: valgrind 
13. performance-test:
    1. 100MB JSON: https://github.com/seductiveapps/largeJSON/blob/master/100mb.json
    2. 15GB JSON: curl -L http://openlibrary.org/data/ol_cdump_latest.txt.gz  | gunzip | cut -f 5 > ol_cdump.json
14. load of .so module plugin in golang