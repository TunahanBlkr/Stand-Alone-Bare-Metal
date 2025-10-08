CC		= arm-none-eabi-gcc
CXX		= arm-none-eabi-g++
AS		= arm-none-eabi-as
LD		= arm-none-eabi-gcc
OBJCOPY	= arm-none-eabi-objcopy

BUILD_DIR	= build
OBJ_DIR		= $(BUILD_DIR)/Object_Files
SRC_DIR		= src
INC_DIRS	= include include/FreeRTOS_include include/CMSIS
LIB_DIR		= lib

MCU			= -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard
CFLAGS		= $(MCU) -Wall -Wextra -O2 -ffunction-sections -fdata-sections -DCPU_MIMXRT1062CVJ5A
CXXFLAGS	= $(CFLAGS) -fno-exceptions -fno-rtti
LDFLAGS		= $(MCU) -Wl,--gc-sections,--no-warn-rwx-segments --specs=nosys.specs -TMIMXRT1062xxxxx_ram.ld

INCLUDES	= $(addprefix -I, $(INC_DIRS))
LIBS		= -L$(LIB_DIR) -lfreertos

C_SOURCES	= $(wildcard $(SRC_DIR)/*.c)
CPP_SOURCES	= $(wildcard $(SRC_DIR)/*.cpp)
ASM_SOURCES = $(wildcard *.S)
OBJS 		= $(addprefix $(OBJ_DIR)/, $(notdir $(C_SOURCES:.c=.o)) \
        		$(notdir $(CPP_SOURCES:.cpp=.o)) \
        		$(notdir $(ASM_SOURCES:.S=.o)))

TARGET = firmware

###############################################################################
# Build rules
###############################################################################

all: $(LIB_DIR)/libfreertos.a $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex

$(LIB_DIR)/libfreertos.a:
	$(MAKE) -C External

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: %.S | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

###############################################################################
# Utility targets
###############################################################################

clean:
ifeq ($(OS),Windows_NT)
	del /Q $(BUILD_DIR)\*.elf
	del /Q $(BUILD_DIR)\*.hex
else
	rm -f $(BUILD_DIR)/*.elf
	rm -f $(BUILD_DIR)/*.hex
endif

.PHONY: all clean
