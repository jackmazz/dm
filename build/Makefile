BIN_DIR := bin
OBJ_DIR := obj
INC_DIR := ../include
SRC_DIR := ../src

OBJS = $(OBJ_DIR)/stage.o
OBJS += $(OBJ_DIR)/tile.o
OBJS += $(OBJ_DIR)/actor.o
OBJS += $(OBJ_DIR)/utils/asset.o
OBJS += $(OBJ_DIR)/utils/schema.o
OBJS += $(OBJ_DIR)/utils/strings.o

CC := g++
CFLAGS := -Wall -Wextra -Werror -I$(INC_DIR)
CLIBS := -lm

dm: $(BIN_DIR)/main.exe

test: $(BIN_DIR)/tests/test-$(TARGET).exe

clean:
	find $(OBJ_DIR) -type f -name "*.o" -delete
	find $(BIN_DIR) -type f -name "*.exe" -delete

$(BIN_DIR)/%.exe: $(OBJS) $(OBJ_DIR)/%.o
	$(CC) -o $@ $^ $(CLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

.PRECIOUS: $(OBJ_DIR)/%.o

