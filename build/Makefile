BIN_DIR := bin
OBJ_DIR := obj
INC_DIR := ../include
SRC_DIR := ../src

OBJS := $(OBJ_DIR)/actor.o
OBJS += $(OBJ_DIR)/asset.o
OBJS += $(OBJ_DIR)/stage.o
OBJS += $(OBJ_DIR)/tile.o
OBJS += $(OBJ_DIR)/unit.o
OBJS += $(OBJ_DIR)/utils/dev-tools.o
OBJS += $(OBJ_DIR)/utils/schema.o
OBJS += $(OBJ_DIR)/utils/string-tools.o

TESTS := $(BIN_DIR)/tests/test-actor.exe
TESTS += $(BIN_DIR)/tests/test-cache.exe
TESTS += $(BIN_DIR)/tests/test-dev-tools.exe
TESTS += $(BIN_DIR)/tests/test-schema.exe
TESTS += $(BIN_DIR)/tests/test-stage.exe
TESTS += $(BIN_DIR)/tests/test-string-tools.exe
TESTS += $(BIN_DIR)/tests/test-tile.exe

CC := g++
CFLAGS := -Wall -Wextra -Werror -I$(INC_DIR)
CLIBS := -lm

TARGET ?= all
ARGS ?= all

dm: $(BIN_DIR)/main.exe

clean:
	find $(OBJ_DIR) -type f -name "*.o" -delete
	find $(BIN_DIR) -type f -name "*.exe" -delete

ifeq ($(TARGET),all)
test: $(TESTS)
	@for t in $(TESTS); do valgrind $$t; done
else
test: $(BIN_DIR)/tests/test-$(TARGET).exe
	valgrind $(BIN_DIR)/tests/test-$(TARGET).exe $(ARGS)
endif   

$(BIN_DIR)/%.exe: $(OBJS) $(OBJ_DIR)/%.o
	$(CC) -o $@ $^ $(CLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

.PRECIOUS: $(OBJ_DIR)/%.o

