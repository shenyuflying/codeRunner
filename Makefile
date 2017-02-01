.PHONY: clean

CC = gcc
all:codeRunner

codeRunner:./src/main.c
	$(CC) -DREADLINE -g -O0 ./src/main.c -o ./bin/codeRunner -lreadline

no-readline:
	$(CC) -g -O0 ./src/main.c -o ./bin/codeRunner
clean:
	-rm ./bin/codeRunner
test:codeRunner
	./test.sh
install:codeRunner
	install ./bin/codeRunner /usr/local/bin/
