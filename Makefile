.POSIX:

HDR = cco_compiler.h cco_of.h cco_hash_support.h cco_node.h cco_hnode.h cco_hlist.h cco_hash.h cco_iter.h cco_queue.h cco_stack.h
TEST_SRC = test_hash.c test_queue.c test_stack.c
TEST_OBJ = $(TEST_SRC:.c=.o)
TEST_EXE = $(TEST_SRC:.c=)

cco.h: $(HDR) meld.sh
	./meld.sh $(HDR) > $@

%.o: %.c cco.h
	$(CC) $(CFLAGS) -c $<

%: %.o cco.h
	$(CC) $(CFLAGS) -o $@ $^

all: cco.h

test: $(TEST_EXE) cco.h
	./test_hash
	./test_queue
	./test_stack

clean:
	rm -f cco.h $(TEST_OBJ) $(TEST_EXE)

.PHONY: all test clean
