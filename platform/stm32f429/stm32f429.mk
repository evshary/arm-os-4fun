CUR_DIR = platform/stm32f429

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
	-I$(HAL)/TARGET_STM/TARGET_STM32F4/TARGET_STM32F429xI \
    -I$(HAL)/TARGET_STM/TARGET_STM32F4/TARGET_STM32F429xI/device \
    -I$(HAL)/TARGET_STM/TARGET_STM32F4/TARGET_STM32F429xI/TARGET_DISCO_F429ZI

SRC += $(HAL)/TARGET_STM/TARGET_STM32F4/device/system_stm32f4xx.c
SRC += $(HAL)/TARGET_STM/TARGET_STM32F4/device/stm32f4xx_hal_rcc.c
SRC += $(HAL)/TARGET_STM/TARGET_STM32F4/device/stm32f4xx_hal_rcc_ex.c
SRC += $(HAL)/TARGET_STM/TARGET_STM32F4/device/stm32f4xx_hal_gpio.c
SRC += $(HAL)/TARGET_STM/TARGET_STM32F4/device/stm32f4xx_hal_uart.c
