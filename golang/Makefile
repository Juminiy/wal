main: main.go
	env CGO_ENABLE=1 GO111MODULE=on go build -o $@ $<
	sudo chmod 777 $@