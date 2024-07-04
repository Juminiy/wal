# template notmodify
objs: $(patsubst %.c,%.o,$(wildcard *.c))

%.o: %.c
	$(CC) -c $(c_args) -o $@ $<