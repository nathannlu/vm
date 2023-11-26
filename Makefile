CC = gcc
CFLAGS = -Iinclude -Iobjects -Ivm -Iparser -g

SRC_DIR = .
OBJ_DIR = objects
BUILD_DIR = build
PARSER_DIR = parser
VM_DIR = vm

# Find .c files in SRC_DIR, OBJ_DIR, and VM_DIR
SRC_FILES = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(OBJ_DIR)/*.c) $(wildcard $(PARSER_DIR)/*.c) $(wildcard $(VM_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

TARGET = run

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^
	rm -rf $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: clean

clean:
	rm -rf $(BUILD_DIR) $(OBJ_DIR)/*.o $(TARGET)


