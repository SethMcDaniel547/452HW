prog?=$(basename $(notdir $(PWD)))
objs+=$(addsuffix .o,$(basename $(wildcard *.c *.cc)))

defines+=-D_GNU_SOURCE
ccflags+=-g -Wall -MMD $(defines)
ldflags+=-g

.SUFFIXES:

%.o: %.c  ; gcc -o $@ -c $< $(ccflags)
%.i: %.c  ; gcc -o $@ -E $< $(defines)
%.s: %.c  ; gcc -o $@ -S $< $(defines)
%.o: %.cc ; g++ -o $@ -c $< $(ccflags)
%.i: %.cc ; g++ -o $@ -E $< $(defines)
%.s: %.cc ; g++ -o $@ -S $< $(defines)

ld?=gcc

$(prog): $(objs) ; $(ld) -o $@ $^ $(ldflags)

TEST_TARGET = tests/test_deq
TEST_OBJS = tests/test_deq.o tests/unity/unity.o src/deq.o

$(TEST_TARGET): $(TEST_OBJS)
	$(ld) -o $@ $^ $(ldflags)

tests/test_deq.o: tests/test_deq.c
	gcc -o $@ -c $< $(ccflags) -Itests/unity -I.

tests/unity/unity.o: tests/unity/unity.c
	gcc -o $@ -c $< $(ccflags) -Itests/unity

.PHONY: clean run valgrind

clean:: ; rm -f $(prog) *.o *.d *.i try tests/*.o tests/*.d tests/unity/*.o tests/unity/*.d $(TEST_TARGET)

run:      $(prog) ; ./$< $(args)
valgrind: $(prog) ; $@ --leak-check=full --show-leak-kinds=all ./$< $(args)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

sinclude *.d
sinclude tests/*.d
sinclude tests/unity/*.d
