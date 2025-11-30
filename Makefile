# Toolchain
CC = i386-elf-gcc
LD = i386-elf-ld
ASM = nasm
GRUB_MKRESCUE = grub-mkrescue
# Directories
CORE_DIR = kernel/core
APPLETS_DIR = kernel/applets
BUILD_DIR = build
ISO_DIR = $(BUILD_DIR)/iso_root

C_SOURCES = $(shell find $(CORE_DIR) $(DRIVERS_DIR) $(APPLETS_DIR) -name "*.c")

OBJ = $(addprefix $(BUILD_DIR)/,$(C_SOURCES:.c=.o))

KERNEL_ELF = $(BUILD_DIR)/kernel.elf
OS_IMAGE = badapple.iso

BASE_CFLAGS = -m32 -fno-exceptions -fno-asynchronous-unwind-tables -mgeneral-regs-only -ffreestanding -Ikernel -Wall -Wextra -nostdlib -fno-stack-protector
LDFLAGS = -m elf_i386 -nostdlib -T $(CORE_DIR)/linker.ld

.PHONY: all clean run release dev

all: dev

release:
	@$(MAKE) BUILD_TYPE=release $(OS_IMAGE)

# Dev build  
dev:
	@$(MAKE) BUILD_TYPE=dev $(OS_IMAGE)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(BASE_CFLAGS) -DVERSION=\"$(VERSION)\" -c $< -o $@

$(BUILD_DIR)/%.o: %.asm | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(ASM) -f elf $< -o $@

$(KERNEL_ELF): $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^
	@echo "=== Kernel built successfully ==="
	@i386-elf-readelf -h $(KERNEL_ELF) | grep Entry

$(OS_IMAGE): $(KERNEL_ELF) grub.cfg
	# Create ISO directory structure
	mkdir -p $(ISO_DIR)/boot/grub
	# Copy kernel and GRUB config
	cp $(KERNEL_ELF) $(ISO_DIR)/boot/
	cp grub.cfg $(ISO_DIR)/boot/grub/
	# Create bootable ISO
	$(GRUB_MKRESCUE) -o $(OS_IMAGE) $(ISO_DIR)
	@echo "=== ISO created successfully ==="

run: $(OS_IMAGE)
	qemu-system-i386 -cdrom $(OS_IMAGE) -net none -m 512M

debug: $(OS_IMAGE)
	qemu-system-i386 -cdrom $(OS_IMAGE) -net none -m 512M -s

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(OS_IMAGE)

kernel-info: $(KERNEL_ELF)
	@echo "=== Kernel Information ==="
	@i386-elf-readelf -h $(KERNEL_ELF)

.PHONY: all run clean debug kernel-info
