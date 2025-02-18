# Define the compiler and assembler
CC = g++
AS = nasm

# Define the source directories
SRC_DIR = Source
BOOT_DIR = $(SRC_DIR)/Boot
INCLUDE_DIR = $(SRC_DIR)/Include
KERNEL_DIR = $(SRC_DIR)/Kernel

# Define the output directory
BUILD_DIR = Build

# Define the output binary
OUTPUT = kernel.bin

# Define the source files
BOOT_SRC = $(BOOT_DIR)/boot.asm
KERNEL_SRC = $(KERNEL_DIR)/kernel.cpp $(KERNEL_DIR)/framebuffer.cpp

# Define the include files
INCLUDES = -I$(INCLUDE_DIR)

# Define the object files
BOOT_OBJ = $(BUILD_DIR)/boot.o
KERNEL_OBJS = $(BUILD_DIR)/kernel.o $(BUILD_DIR)/framebuffer.o

# Define the linker script
LINKER_SCRIPT = $(BOOT_DIR)/linker.ld

# Define the build targets
all: $(BUILD_DIR) $(OUTPUT)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BOOT_OBJ): $(BOOT_SRC)
	$(AS) -f elf32 $(BOOT_SRC) -o $(BOOT_OBJ)

$(BUILD_DIR)/kernel.o: $(KERNEL_DIR)/kernel.cpp
	$(CC) -m32 -nostdlib -nodefaultlibs -c $(KERNEL_DIR)/kernel.cpp $(INCLUDES) -o $(BUILD_DIR)/kernel.o

$(BUILD_DIR)/framebuffer.o: $(KERNEL_DIR)/framebuffer.cpp
	$(CC) -m32 -nostdlib -nodefaultlibs -c $(KERNEL_DIR)/framebuffer.cpp $(INCLUDES) -o $(BUILD_DIR)/framebuffer.o

$(OUTPUT): $(BOOT_OBJ) $(KERNEL_OBJS)
	$(CC) -m32 -T $(LINKER_SCRIPT) -nostdlib -nodefaultlibs -o $(OUTPUT) $(BOOT_OBJ) $(KERNEL_OBJS)

clean:
	rm -rf $(BUILD_DIR) $(OUTPUT)

.PHONY: all clean
