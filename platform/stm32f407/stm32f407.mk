CUR_DIR = platform/stm32f407

CPU = cortex-m4

SRC += $(CUR_DIR)/start.c \
	   $(CUR_DIR)/uart.c \
	   $(CUR_DIR)/systick.c

CFLAGS += \
    -I$(CUR_DIR) \
    -I$(HAL)/util \
    -I$(HAL)/cmsis \
    -I$(HAL)/cmsis/TARGET_CORTEX_M \
    -I$(HAL)/cmsis/TARGET_CORTEX_M/TOOLCHAIN_GCC \
    -I$(HAL)/TARGET_STM \
    -I$(HAL)/TARGET_STM/TARGET_STM32F4 \
    -I$(HAL)/TARGET_STM/TARGET_STM32F4/device \
	-I$(HAL)/TARGET_STM//TARGET_STM32F4/TARGET_STM32F407xG \
    -I$(HAL)/TARGET_STM//TARGET_STM32F4/TARGET_STM32F407xG/device \
    -I$(HAL)/TARGET_STM//TARGET_STM32F4/TARGET_STM32F407xG/TARGET_DISCO_F407VG
