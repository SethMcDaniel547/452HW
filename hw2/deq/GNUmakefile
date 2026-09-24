prog=deq

include ../GNUmakefile

libdeq.so: deq.c
	gcc -shared -fPIC -o libdeq.so deq.c

try: main.o libdeq.so
	gcc -o $@ $< -L. -ldeq -Wl,-rpath=.