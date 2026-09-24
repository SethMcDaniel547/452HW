CC = gcc
CFLAGS = -Wall -Wextra -g -I.. -Iunity

# Source files from parent directory
SRC = ../src/balloc.c ../src/freelist.c ../src/bbm.c ../src/bm.c ../src/utils.c
# Unity source
UNITY = unity/unity.c
# Test runner source
TEST_SRC = test_balloc.c

TARGET = runner

all: $(TARGET)

$(TARGET): $(TEST_SRC) $(UNITY) $(SRC)
	$(CC) $(CFLAGS) $(TEST_SRC) $(UNITY) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean