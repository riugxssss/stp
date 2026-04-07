CC      := gcc
CFLAGS  := -std=gnu2x -Wcpp -Wall -Wextra -g -O2 -Iinclude

SRC_DIR  := src
OBJ_DIR  := obj
BIN_DIR  := bin
TEST_DIR := test

TARGET        := $(BIN_DIR)/main
TEST_TARGET   := $(BIN_DIR)/test_runner

# Sorgenti principali (escludo main.c per riusarli nei test)
SRC      := $(filter-out $(SRC_DIR)/main.c,$(wildcard $(SRC_DIR)/*.c))
OBJ      := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

# Sorgenti test
TEST_SRC := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ := $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/%.test.o,$(TEST_SRC))

# ----------- TARGET PRINCIPALE -----------

all: $(BIN_DIR) $(TARGET)

$(TARGET): $(OBJ) $(OBJ_DIR)/main.o | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# Compilazione main separata
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# ----------- TEST -----------

test: $(BIN_DIR) $(TEST_TARGET)

$(TEST_TARGET): $(OBJ) $(TEST_OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# Compilazione file src
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compilazione file test
$(OBJ_DIR)/%.test.o: $(TEST_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# ----------- DIRECTORY -----------

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# ----------- CLEAN -----------

clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/*

distclean: clean
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# ----------- UTILITY -----------

run: all
	./$(TARGET)

run-test: test
	./$(TEST_TARGET)

.PHONY: all test clean distclean run run-test
