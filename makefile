CFLAGS += -Wall
CFLAGS += -Os
CFLAGS += -MMD -MP

INCLUDES += $(SRC_DIR)

SRC_DIR = ../source
SRCS := $(wildcard $(SRC_DIR)/*.c)

OBJ_DIR := build-$(shell $(CC) -dumpmachine)
OBJS := $(patsubst $(SRC_DIR)/%.c, %.o, $(SRCS))

TARGET := y86

VPATH += $(SRC_DIR)



all: $(OBJ_DIR)



clean:
	rm -rf $(OBJ_DIR)

.PHONY: $(OBJ_DIR)
$(OBJ_DIR):
	@[ -d $@ ] || mkdir -p $@
	make -C $(OBJ_DIR) -f ../makefile $(TARGET)

$(TARGET): $(OBJS)



-include *.d
