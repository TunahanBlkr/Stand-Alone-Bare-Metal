CC = arm-none-eabi-gcc
AR = arm-none-eabi-ar

SRC_DIR = FreeRTOS_src
BUILD_DIR = FreeRTOS_build
LIB_DIR = ../lib
INCLUDE_DIR = ../include/FreeRTOS_include

CFLAGS = -Wall -O2 -I$(INCLUDE_DIR) -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard

LIB_NAME = libfreertos.a

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

ifeq ($(OS),Windows_NT)
    RM = cmd /C "cd $(BUILD_DIR) && del /Q *.o"
else
    RM = rm -f $(BUILD_DIR)/*.o
endif

all: $(LIB_DIR)/$(LIB_NAME)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/*.h
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_DIR)/$(LIB_NAME): $(OBJS)
	$(AR) rcs $@ $(OBJS)

clean:
	ifeq ($(OS),Windows_NT)
		cmd /C "if exist $(BUILD_DIR) del /Q $(BUILD_DIR)\*.o & if exist $(LIB_DIR)\$(LIB_NAME) del /Q $(LIB_DIR)\$(LIB_NAME)"
	else
		rm -rf $(BUILD_DIR)/*.o $(LIB_DIR)/$(LIB_NAME)
	endif

.PHONY: all clean