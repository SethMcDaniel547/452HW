CC ?= gcc
prog := shell
objs := $(addsuffix .o,$(basename $(wildcard *.c)))

CPPFLAGS += -D_GNU_SOURCE $(shell pkg-config --cflags readline ncurses 2>/dev/null)
CFLAGS += -g -Wall -MMD
LDFLAGS += -g
readline_libs := $(shell pkg-config --libs readline ncurses 2>/dev/null)
LDLIBS += $(if $(readline_libs),$(readline_libs),-lreadline -lncurses)

.SUFFIXES:
%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

shell: $(objs)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

test: shell
	./Test/test_commands.sh $(CURDIR)/$<

Parser.o: CFLAGS += -Wno-unused-function

clean:
	rm -f $(prog) *.o *.d libdeq.so

sinclude *.d
