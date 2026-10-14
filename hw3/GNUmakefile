prog=shell
objs=$(addsuffix .o,$(basename $(wildcard *.c)))
ccflags=-g -Wall -MMD -D_GNU_SOURCE
ldflags=-g -lreadline -lncurses

.SUFFIXES:
%.o: %.c
	gcc -o $@ -c $< $(ccflags)

shell: $(objs)
	gcc -o $@ $^ $(ldflags)

test: shell
	Test/run $<

Parser.o: ccflags += -Wno-unused-function

clean:
	rm -f $(prog) *.o *.d libdeq.so

sinclude *.d
