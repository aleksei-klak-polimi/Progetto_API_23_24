# Define the compiler
CC = gcc

# Define the source files
SRC = main/Main.c

# Define the libraries folder
LIB_DIR = main/libraries

# Define the output directory
OUT_DIR = target

# Define the output binary name
OUT_BIN = $(OUT_DIR)/Main.out

# List of library source files to be compiled
LIB_SRC = $(LIB_DIR)/Map.c

# Generate corresponding .o filenames for the object files
LIB_OBJ = $(patsubst $(LIB_DIR)/%.c, $(OUT_DIR)/%.o, $(LIB_SRC))

# Define compiler and linker flags
CFLAGS += -Wall -Werror -std=gnu11 -g3 -O2
LDFLAGS += -lm

# Default target
all: $(OUT_DIR) $(OUT_BIN)

# Create the output directory if it doesn't exist
$(OUT_DIR):
	mkdir -p $(OUT_DIR)

# Debugging step: Check that the source files exist
$(LIB_SRC):
	@echo "Checking for source file: $@"
	@test -f $@ || { echo "Source file $@ not found!"; exit 1; }

# Compile each library source file into an object file
$(OUT_DIR)/%.o: $(LIB_DIR)/%.c
	@echo "Compiling $< into $@ ..."
	$(CC) $(CFLAGS) -c $< -o $@ || { echo "Compilation of $< failed"; exit 1; }

# Compile the main source file and link with the library object files
$(OUT_BIN): $(SRC) $(LIB_OBJ)
	@echo "Linking $(OUT_BIN) ..."
	$(CC) $(CFLAGS) -o $(OUT_BIN) $(SRC) $(LIB_OBJ) $(LDFLAGS) || { echo "Linking failed"; exit 1; }

# Clean up the output directory
clean:
	rm -rf $(OUT_DIR)
