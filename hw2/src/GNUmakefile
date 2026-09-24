CC = gcc
CFLAGS = -Wall -Wextra -g -I../deq -I.
TARGET = test_deq

DEQ_DIR = ../deq

OBJS = main.o wrapper.o balloc.o freelist.o bbm.o bm.o utils.o deq.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

deq.o: $(DEQ_DIR)/deq.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all run clean