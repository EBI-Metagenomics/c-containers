.POSIX:

CCO_VERSION := 0.1.1

CC ?= gcc
CFLAGS := $(CFLAGS) -std=c99 -Wall -Wextra
HDR := cco_compiler.h cco_of.h cco_hash_support.h cco_node.h cco_hnode.h cco_hlist.h cco_hash.h cco_iter.h cco_queue.h cco_stack.h

all: cco.h

cco.h: $(HDR) | meld.sh
	./meld.sh $^ > $@

test_hash.o: test/hash.c | cco.h
	$(CC) $(CFLAGS) -I. -c $< -o $@

test_hash: test_hash.o | cco.h
	$(CC) $(CFLAGS) $< -o $@

test_queue.o: test/queue.c | cco.h
	$(CC) $(CFLAGS) -I. -c $< -o $@

test_queue: test_queue.o | cco.h
	$(CC) $(CFLAGS) $< -o $@

test_stack.o: test/stack.c | cco.h
	$(CC) $(CFLAGS) -I. -c $< -o $@

test_stack: test_stack.o | cco.h
	$(CC) $(CFLAGS) $< -o $@

check: test_hash test_queue test_stack
	./test_hash
	./test_queue
	./test_stack

test: check

dist: clean cco.h
	mkdir -p cco-$(CCO_VERSION)
	cp -R README.md LICENSE cco.h cco-$(CCO_VERSION)
	tar -cf - cco-$(CCO_VERSION) | gzip > cco-$(CCO_VERSION).tar.gz
	rm -rf cco-$(CCO_VERSION)

distclean:
	rm -f jx-$(CCO_VERSION).tar.gz

clean: distclean
	rm -f cco.h *.o test_hash test_queue test_stack

.PHONY: all check test dist distclean clean
