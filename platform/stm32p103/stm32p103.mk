CUR_DIR = platform/stm32p103

CPU = cortex-m3

SRC += $(CUR_DIR)/start.c \
	   $(CUR_DIR)/usart.c \
	   $(CUR_DIR)/systick.c \
       $(HAL)/TARGET_STM/TARGET_STM32F1/device/system_stm32f1xx.c   \
       $(HAL)/TARGET_STM/TARGET_STM32F1/device/stm32f1xx_hal_rcc.c  \
       $(HAL)/TARGET_STM/TARGET_STM32F1/device/stm32f1xx_hal_gpio.c \
       $(HAL)/TARGET_STM/TARGET_STM32F1/device/stm32f1xx_hal_uart.c

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
