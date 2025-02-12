#CFLAGS += -fpic

#LDFLAGS += -shared

TARGET = liby86

TARGET_LIB = $(TARGET)
TARGET_LIB_A = $(TARGET_LIB).a
TARGET_LIB_SO = $(TARGET_LIB).so.1

SRC_DIR = $(ROOT)/y86lib
SRCS = $(wildcard $(SRC_DIR)/*.c)



all: $(TARGET_LIB_SO)


include $(ROOT)/makefile.common


.PRECIOUS: $(OBJS)

$(info ********)
$(info OBJS = $(OBJS))
$(info OBJ_DIR = $(OBJ_DIR))
$(info SRCS = $(SRCS))


$(TARGET_LIB_A): $(TARGET_LIB_A)($(OBJS))

$(TARGET_LIB_SO): $(TARGET_LIB_SO)($(TARGET_LIB_A))
