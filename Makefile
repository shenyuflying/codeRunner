all:main

main:main.c
	gcc -g -O0 main.c -o codeRunner
clean:
	-rm codeRunner
test:
	./test.sh
install:main
	install codeRunner /usr/local/bin/
