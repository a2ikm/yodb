.PHONY: all
all: yodb

main.o: main.c
	clang -c main.c

yodb: main.o
	clang -o yodb main.o
