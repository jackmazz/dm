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
CLANG := clang-format -i --style=file --verbose

dm: $(BIN_DIR)/main.exe

format:
	find $(INC_DIR) -type f -name "*.hpp" -exec $(CLANG) {} +
	find $(SRC_DIR) -type f -name "*.cpp" -exec $(CLANG) {} +

clean:
	find $(OBJ_DIR) -type f -name "*.o" -delete
	find $(BIN_DIR) -type f -name "*.exe" -delete

$(BIN_DIR)/%.exe: $(OBJS) $(OBJ_DIR)/%.o
	$(CC) -o $@ $^ $(CLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

.PRECIOUS: $(OBJ_DIR)/%.o

