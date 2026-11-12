CC ?= gcc
CXX ?= g++

prog := wam
.DEFAULT_GOAL := $(prog)
ccflags := -D_GNU_SOURCE -g -Wall -Wextra -MMD -pthread -Ideq
cxxflags := -D_GNU_SOURCE -g -Wall -Wextra -MMD -pthread -Ideq
ldflags := -g -pthread
ldlibs := -lX11 -lfltk

c_objects := main.o mole.o lawn.o mtq.o threads.o deq/deq.o
cpp_objects := lawnimp.o
objects := $(c_objects) $(cpp_objects)

.SUFFIXES:

%.o: %.c
	$(CC) $(ccflags) -o $@ -c $<

deq/deq.o: deq/deq.c
	$(CC) $(ccflags) -o $@ -c $<

%.o: %.cc
	$(CXX) $(cxxflags) -o $@ -c $<

$(prog): $(objects)
	$(CXX) $(ldflags) -o $@ $^ $(ldlibs)

check: $(prog)
	@set -eu; for run in 1 2 3; do \
	  output=$$(mktemp); \
	  DISPLAY= ./$(prog) >$$output; \
	  test "$$(grep -c 'creating' $$output)" -eq 10; \
	  test "$$(grep -c 'created' $$output)" -eq 10; \
	  test "$$(grep -c 'whacking' $$output)" -eq 10; \
	  test "$$(grep -c 'whacked' $$output)" -eq 10; \
	  test "$$(grep -c 'expired' $$output)" -eq 10; \
	  rm -f $$output; \
	done

clean:
	rm -f $(prog) $(objects) *.d deq/*.d

-include *.d
