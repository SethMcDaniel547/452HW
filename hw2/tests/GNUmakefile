CC = gcc
CFLAGS = -Wall -Wextra -g -I.. -Iunity

# Source files from parent directory
SRC = ../src/balloc.c ../src/freelist.c ../src/bbm.c ../src/bm.c ../src/utils.c
# Unity source
UNITY = unity/unity.c
# Test runner source
TEST_SRC = test_balloc.c

TARGET = runner
MODULE_TARGET = module_runner
MODULE_TEST_SRC = test_modules.c
MODULE_SRC = ../src/freelist.c ../src/bbm.c ../src/bm.c ../src/utils.c

all: $(TARGET)

modules: $(MODULE_TARGET)

$(TARGET): $(TEST_SRC) $(UNITY) $(SRC)
	$(CC) $(CFLAGS) $(TEST_SRC) $(UNITY) $(SRC) -o $(TARGET)

$(MODULE_TARGET): $(MODULE_TEST_SRC) $(UNITY) $(MODULE_SRC)
	$(CC) $(CFLAGS) $(MODULE_TEST_SRC) $(UNITY) $(MODULE_SRC) -o $(MODULE_TARGET)

run: $(TARGET)
	./$(TARGET)

run-modules: $(MODULE_TARGET)
	./$(MODULE_TARGET)

clean:
	rm -f $(TARGET) $(MODULE_TARGET)

.PHONY: all modules run run-modules clean