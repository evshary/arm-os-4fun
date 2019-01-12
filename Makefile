CROSS = arm-none-eabi
CC = $(CROSS)-gcc
OBJCOPY = $(CROSS)-objcopy
OBJDUMP = $(CROSS)-objdump
QEMU = ../qemu_stm32/arm-softmmu/qemu-system-arm
ST_FLASH = ../stlink/st-flash
GDB = gdb-multiarch
# For extern project
EXTERN = extern
HAL = $(EXTERN)/hal
SVN_REV = 104397
# Compile option
CFLAGS = -fno-common -ffreestanding \
		 -Wall -Werror \
		 -mcpu=$(CPU) -mthumb \
		 -Wl,-Tarm_os.ld -nostartfiles \
		 -Wl,--gc-sections \
		 -ffunction-sections -fdata-sections \
		 -Iinclude -I.
# Source Code
SRC = arm_os.c run_proc.S tasks.c syscall.c output.c malloc/malloc.c

# Choose board
BOARD ?= STM32P103
ifeq ($(BOARD), STM32P103)
	include platform/stm32p103/stm32p103.mk
else ifeq ($(BOARD), STM32F407)
	include platform/stm32f407/stm32f407.mk
else ifeq ($(BOARD), STM32F429)
	include platform/stm32f429/stm32f429.mk
else
	include platform/stm32p103/stm32p103.mk
endif

# DEBUG mode or not
# Use DWARF format and level 3
DEBUG ?= 0
ifeq ($(DEBUG), 1)
CFLAGS += -gdwarf-2 -g3
endif

# OS name
OS_NAME = arm_os
OS_BINARY = $(OS_NAME).bin
OS_ELF = $(OS_NAME).elf
all: extern style $(OS_BINARY)

extern: $(HAL)
$(HAL):
	mkdir -p $(HAL)
	svn export -r$(SVN_REV) -q --force https://github.com/ARMmbed/mbed-os/trunk/cmsis/ $(HAL)/cmsis
	svn export -r$(SVN_REV) -q --force https://github.com/ARMmbed/mbed-os/trunk/targets/TARGET_STM/ $(HAL)/TARGET_STM

$(OS_BINARY): $(SRC)
	$(CC) $(CFLAGS) $^ -o $(OS_ELF)
	$(OBJCOPY) -Obinary $(OS_ELF) $(OS_BINARY)
	$(OBJDUMP) -S $(OS_ELF) > $(OS_NAME).list

clean:
	rm -rf *.bin *.elf *list

distclean: clean
	rm -rf $(HAL)

st-flash: $(OS_BINARY)
	../stlink/st-flash --reset write $(OS_BINARY) 0x8000000

st-erase:
	st-flash erase

st-util_gdb:
	echo "Open another terminal and type \"make st-util_connect\""
	../stlink/src/gdbserver/st-util

st-util_connect: $(OS_ELF)
	$(GDB) $^ -ex "target remote:4242"

qemu:
	echo "Press Ctrl-A and then X to exit QEMU"
	$(QEMU) -M stm32-p103 -nographic -kernel $(OS_BINARY)

qemu_gdb:
	echo "Open another terminal,and type \"make qeme_connect\""
	$(QEMU) -M stm32-p103 -nographic -kernel $(OS_BINARY) -s -S

qemu_connect:
	$(GDB) $(OS_ELF) -ex "target remote:1234"

flash:
	$(ST_FLASH) write $(OS_BINARY) 0x8000000

style:
	astyle --style=linux --exclude=$(EXTERN) -cnpswHUSR *.c,*.h

sum:
	cloc --exclude-dir=extern .
