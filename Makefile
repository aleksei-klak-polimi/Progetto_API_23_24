# Define the compiler
CC = gcc

# Define the source file
SRC = main/Main.c

# Define the output directory
OUT_DIR = target

# Define the output binary name
OUT_BIN = $(OUT_DIR)/Main.out

# Define compiler and linker flags
CFLAGS += -Wall -Werror -std=gnu11 -g3
LDFLAGS += -lm

# Default target
all: $(OUT_DIR) $(OUT_BIN)

# Create the output directory if it doesn't exist
$(OUT_DIR):
	mkdir -p $(OUT_DIR)

# Compile the source file into the output binary
$(OUT_BIN): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT_BIN) $(SRC) $(LDFLAGS)

# Clean up the output directory
clean:
	rm -rf $(OUT_DIR)