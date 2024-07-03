# wal
1. log
   1. macro log type: info, warn, error, debug, fatal
   2. TODO: fmt log msg length is restricted
   3. BUG: msg truncated and overflow, (buffer BUG in windows)
   4. TODO: enable color
2. coworker
   1. concurrency in multithreads
   2. sync, async, time record
   3. mutex, cond signal, progress record
   4. param_list, return values set
3. wal
   1. normal log in app
   2. fd,write fd,fsync
   3. write ahead logging
4. wal in coworker
5. yyjson parse json to accelerate json parser
