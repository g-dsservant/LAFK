
CC = gcc

CFLAGS = -Wall -Wextra -Iinclude -Isrc -fPIC -g


INCLUDE_DIR = include
SRC_DIR = src
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)


LIB_NAME = libLAFK.a


SRCS = $(wildcard $(SRC_DIR)/*.c)


OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))


all: $(BUILD_DIR)/$(LIB_NAME)


$(BUILD_DIR)/$(LIB_NAME): $(OBJS) | $(BUILD_DIR)
	ar rcs $@ $(OBJS)
	@echo "Library built at $@"

# Compile Source Files into Object Files in the build directory
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/%.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
	@echo "Cleaned build artifacts."

.PHONY: all clean
