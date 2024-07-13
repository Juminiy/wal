main: main.go
	env CGO_ENABLED=1 GO111MODULE=on go build -o $@ $<
	sudo chmod 777 $@
	$(prerun)
	./$@

clean:
	rm -f libjson_flatten.so main