INCLUDES += -I$(ROOT)/y86lib

LDLIBS += -L$(OBJ_DIR) -ly86

SRC_DIR = $(ROOT)/$(TARGET)
SRCS = $(wildcard $(SRC_DIR)/*.c)

TARGET = y86run

#TARGET_EXE = $(TARGET).exe


all: $(TARGET)



include $(ROOT)/makefile.common



$(info ********)
$(info SRCS = $(SRCS))
$(info OBJ_DIR = $(OBJ_DIR))
$(info OBJS = $(OBJS))

#$(TARGET_EXE): $(OBJS) | $(OBJ_DIR)
$(TARGET): $(OBJS)
