CROSS = arm-none-eabi
CC = $(CROSS)-gcc
OBJCOPY = $(CROSS)-objcopy
OBJDUMP = $(CROSS)-objdump
QEMU = ../qemu_stm32/arm-softmmu/qemu-system-arm
ST_FLASH = ../stlink/st-flash
# For extern project
EXTERN = extern
HAL = $(EXTERN)/hal
SVN_REV = 104397
# Compile option
CFLAGS = -fno-common -ffreestanding \
		 -Wall -Werror \
		 -mcpu=$(CPU) -mthumb \
		 -Wl,-Tarm_os.ld -nostartfiles \
		 -Iinclude -I.
# Source Code
SRC = arm_os.c run_proc.S tasks.c syscall.c output.c malloc/malloc.c

# Choose board
BOARD ?= STM32P103
ifeq ($(BOARD), STM32P103)
	include platform/stm32p103/stm32p103.mk
else ifeq ($(BOARD), STM32F407)
	include platform/stm32f407/stm32f407.mk
else
	include platform/stm32p103/stm32p103.mk
endif

BINARY = arm_os.bin
all: extern style $(BINARY)

extern: $(HAL)
$(HAL):
	mkdir -p $(HAL)
	svn export -r$(SVN_REV) -q --force https://github.com/ARMmbed/mbed-os/trunk/cmsis/ $(HAL)/cmsis
	svn export -r$(SVN_REV) -q --force https://github.com/ARMmbed/mbed-os/trunk/targets/TARGET_STM/ $(HAL)/TARGET_STM

$(BINARY): $(SRC)
	$(CC) $(CFLAGS) $^ -o arm_os.elf
	$(OBJCOPY) -Obinary arm_os.elf arm_os.bin
	$(OBJDUMP) -S arm_os.elf > arm_os.list

clean:
	rm -rf *.bin *.elf *list

distclean: clean
	rm -rf $(HAL)

qemu:
	echo "Press Ctrl-A and then X to exit QEMU"
	$(QEMU) -M stm32-p103 -nographic -kernel arm_os.bin

flash:
	$(ST_FLASH) write $(BINARY) 0x8000000

style:
	astyle --style=linux --exclude=$(EXTERN) -cnpswHUSR *.c,*.h
