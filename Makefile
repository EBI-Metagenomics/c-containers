.POSIX:

CCO_VERSION := 0.0.1

LIB      = cco.h
HDR      = cco_compiler.h cco_of.h cco_hash_support.h cco_node.h cco_hnode.h cco_hlist.h cco_hash.h cco_iter.h cco_queue.h cco_stack.h
TEST_SRC = test_hash.c test_queue.c test_stack.c
TEST_OBJ = $(TEST_SRC:.c=.o)
TEST_EXE = $(TEST_SRC:.c=)

$(LIB): $(HDR) meld.sh
	./meld.sh $(HDR) > $@

%.o: %.c $(LIB)
	$(CC) $(CFLAGS) -c $<

%: %.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $^

all: $(LIB)

test: $(TEST_EXE) $(LIB)
	./test_hash
	./test_queue
	./test_stack

clean:
	rm -f $(LIB) $(TEST_OBJ) $(TEST_EXE)

dist:
	mkdir -p cco-$(CCO_VERSION)
	cp -R Makefile README.md LICENSE $(LIB) cco-$(CCO_VERSION)
	tar -cf - cco-$(CCO_VERSION) | gzip > cco-$(CCO_VERSION).tar.gz
	rm -rf cco-$(CCO_VERSION)

.PHONY: all test clean dist
