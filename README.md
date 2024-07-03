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
   2. TODO: fmt log msg length is restricted
   3. BUG: msg truncated and overflow
      1. (buffer BUG in windows msys2 BUG, not mine)
      2. MacOS-Arm64 and Ubuntu-Amd64 is OK
   4. TODO: enable color
   5. hasDone: ILOG MTS
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
