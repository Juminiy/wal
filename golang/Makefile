main: main.go
	env CGO_ENABLED=1 GO111MODULE=on go build -o $@ $<
	$(prerun)
	./$@

clean:
	rm -f main