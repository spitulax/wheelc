TARGET_NAME := wheelc

SRC := $(shell find ./src -type f -name "*.c")
OBJ := $(SRC:./src/%.c=./build/%.o)

LDFLAGS += -lraylib -lm

build/$(TARGET_NAME): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

$(OBJ): build/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all run clean
all: build/$(TARGET_NAME)
run: build/$(TARGET_NAME)
	./build/$(TARGET_NAME)
clean:
	rm -r ./build
