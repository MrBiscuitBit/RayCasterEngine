CC = gcc

SRC_DIR = src
BUILD_DIR = build
OUT_DIR = bin
TARGET = RayCaster

INCLUDES = -Iinclude -Iinclude/raycaster -Ilib/SDL3/include
LIBS = -Llib/SDL3/lib
CFLAGS = -g -std=c99 -Wall
LDFLAGS = -lSDL3

SRC := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

.PHONY: all clean rebuild run build_dirs

all: build_dirs $(OUT_DIR)/$(TARGET).exe

$(OUT_DIR)/$(TARGET).exe: $(OBJ)
	$(CC) -o $@ $^ $(LIBS) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build_dirs:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(OUT_DIR)

clean:
	rm -f $(OUT_DIR)/$(TARGET).exe
	rm -rf $(BUILD_DIR)

rebuild: clean all

run: all
	$(OUT_DIR)/$(TARGET).exe