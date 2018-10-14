# arm-os-4fun
This is OS based on ARM for fun.

We can learn OS concept and ARM architecture by creating the OS.

Some of the functions are from jserv's [mini-arm-os](https://github.com/jserv/mini-arm-os).

# Build Environment
## QEMU
1. Install necessary package.
```
sudo apt-get install zlib1g-dev libsdl1.2-dev automake autoconf libtool libpixman-1-dev bison flex
# For 32 bit, lib32gcc1 is necessary
sudo apt-get install lib32gcc1 lib32ncurses5
# Install cross compiler
sudo apt-get install binutils-arm-none-eabi libnewlib-arm-none-eabi gcc-arm-none-eabi
```
2. We run the environment on qemu, so you should build the qemu first.
```
git clone https://github.com/beckus/qemu_stm32.git
cd qemu_stm32
./configure --disable-werror --enable-debug \
    --target-list="arm-softmmu" \
    --extra-cflags=-DSTM32_UART_NO_BAUD_DELAY \
    --extra-cflags=-DSTM32_UART_ENABLE_OVERRUN \
    --disable-gtk
make
```
3. clone the source code.
```
git clone https://github.com/evshary/arm-os-4fun.git
```
4. Build and run.
```
cd arm-os-4fun
make
make qemu
```
## STM32F4
1. Install necessary package.
```
sudo apt-get install cmake libtool libusb-1.0-0-dev libgtk-3-dev
```
2. Clone stlink and build.
```
git clone http://github.com/texane/stlink.git
cd stlink
cmake .
make
```
3. clone the source code.
```
git clone https://github.com/evshary/arm-os-4fun.git
```
4. Build and run.
```
cd arm-os-4fun
make BOARD=STM32F407
make flash
```

# System
Currently, the OS runs on qemu, which emulates [STM32-P103](https://www.olimex.com/Products/ARM/ST/STM32-P103/). The supported CPU is stm32f103, which is ARM Cortex M3.
Therefore, you can reference the data below.

* [The Definitive Guide to ARM® Cortex®-M3 and Cortex®-M4 Processors](https://www.amazon.com/Definitive-Guide-Cortex%C2%AE-M3-Cortex%C2%AE-M4-Processors/dp/0124080820)
* [RM0008 Reference manual for STM32F10xxx/20xxx/21xxx/L1xxxx](https://www.st.com/content/ccc/resource/technical/document/reference_manual/59/b9/ba/7f/11/af/43/d5/CD00171190.pdf/files/CD00171190.pdf/jcr:content/translations/en.CD00171190.pdf)
* [PM0056 Programming manual for STM32F101xx, STM32F102xx, STM32F103xx, STM32F105xx and STM32F107xx](https://www.st.com/content/ccc/resource/technical/document/programming_manual/5b/ca/8d/83/56/7f/40/08/CD00228163.pdf/files/CD00228163.pdf/jcr:content/translations/en.CD00228163.pdf)
