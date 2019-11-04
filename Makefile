.PHONY: all
all: yodb

main.o: main.c
	clang -c main.c

yodb: main.o
	clang -o yodb main.o

.PHONY: test
test: yodb Gemfile.lock
	bundle exec rspec

Gemfile.lock: Gemfile
	bundle install
