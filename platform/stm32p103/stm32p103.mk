CUR_DIR = platform/stm32p103

CPU = cortex-m3

SRC += $(CUR_DIR)/start.c \
	   $(CUR_DIR)/usart.c \
	   $(CUR_DIR)/systick.c

CFLAGS += \
    -I$(CUR_DIR) \
    -I$(HAL)/util \
    -I$(HAL)/cmsis \
    -I$(HAL)/cmsis/TARGET_CORTEX_M \
    -I$(HAL)/cmsis/TARGET_CORTEX_M/TOOLCHAIN_GCC \
    -I$(HAL)/TARGET_STM \
    -I$(HAL)/TARGET_STM/TARGET_STM32F1 \
    -I$(HAL)/TARGET_STM/TARGET_STM32F1/device \
    -I$(HAL)/TARGET_STM/TARGET_STM32F1/TARGET_NUCLEO_F103RB \
    -I$(HAL)/TARGET_STM/TARGET_STM32F1/TARGET_NUCLEO_F103RB/device
