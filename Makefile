.PHONY:
	all main build test clean

c_args= -std=gnu99 -Wall -pedantic -O0 -g -DDEBUG

build: $(patsubst %.c,%.o,$(wildcard *.c))

main: *.o
	$(CC) $(c_args) -o $@ $^

%.o: %.c
	$(CC) -c $(c_args) -o $@ $<

clean:
	rm -f main *.o *.exe