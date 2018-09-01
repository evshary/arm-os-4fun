CROSS = arm-none-eabi
CC = $(CROSS)-gcc
OBJCOPY = $(CROSS)-objcopy
OBJDUMP = $(CROSS)-objdump
CFLAGS = -fno-common -ffreestanding \
		 -Wall -Werror \
		 -mcpu=cortex-m3 -mthumb \
		 -Wl,-Tarm_os.ld -nostartfiles

QEMU = ../qemu_stm32/arm-softmmu/qemu-system-arm

BINARY = arm_os.bin
all: $(BINARY)

$(BINARY): start.c arm_os.c
	$(CC) $(CFLAGS) $^ -o arm_os.elf
	$(OBJCOPY) -Obinary arm_os.elf arm_os.bin
	$(OBJDUMP) -S arm_os.elf > arm_os.list

clean:
	rm -rf *.bin *.elf *list

qemu:
	echo "Press Ctrl-A and then X to exit QEMU"
	$(QEMU) -M stm32-p103 -nographic -kernel arm_os.bin
