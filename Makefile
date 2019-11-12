.PHONY: all
all: yodb

main.o: yodb.h main.c
	clang -c main.c

backend.o: yodb.h backend.c
	clang -c backend.c

yodb: yodb.h main.o backend.o
	clang -o yodb main.o backend.o

.PHONY: test
test: yodb Gemfile.lock
	bundle exec rspec

Gemfile.lock: Gemfile
	bundle install

.PHONY: clean
clean:
	git clean -dfx
