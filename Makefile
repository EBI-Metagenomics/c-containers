.POSIX:

HEADERS = cco_of.h cco_hash_support.h cco_node.h cco_hnode.h cco_hlist.h cco_hash.h cco_iter.h cco_queue.h cco_stack.h

cco.h: $(HEADERS)
	./meld.sh $^ > $@

all: cco.h

clean:
	rm -f cco.h

.PHONY: all clean
