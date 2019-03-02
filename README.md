# arm-os-4fun
This is OS based on ARM for fun.

We can learn OS concept and ARM architecture by creating the OS.

Some of the functions are from jserv's [mini-arm-os](https://github.com/jserv/mini-arm-os).

# Build Environment
## QEMU
1. Install necessary package.
  * Ubuntu
```
sudo apt-get install zlib1g-dev libsdl1.2-dev automake autoconf libtool libpixman-1-dev bison flex
# For 32 bit, lib32gcc1 is necessary
sudo apt-get install lib32gcc1 lib32ncurses5
# Install cross compiler
sudo apt-get install binutils-arm-none-eabi libnewlib-arm-none-eabi gcc-arm-none-eabi
# install gdb
sudo apt-get install gdb-multiarch
```
   * MAC
```
# Install cross compiler
brew install gcc-arm-none-eabi
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
5. If you want to run gdb
```
cd arm-os-4fun
make DEBUG=1
# start to run gdb
make qemu_gdb
# run in different terminal
make qemu_connect
```
## STM32F4
1. Install necessary package.
```
sudo apt-get install cmake libtool libusb-1.0-0-dev libgtk-3-dev
```
2. Clone stlink and build.
  * Ubuntu
```
git clone http://github.com/texane/stlink.git
cd stlink
cmake .
make
```
  * MAC
```
brew install stlink
```
3. clone the source code.
```
git clone https://github.com/evshary/arm-os-4fun.git
```
4. Build and run.
```
cd arm-os-4fun
# F407
make BOARD=STM32F407
# F429
make BOARD=STM32F429
make flash
```
5. If you want to run gdb
```
cd arm-os-4fun
# F407
make DEBUG=1 BOARD=STM32F407
# F429
make DEBUG=1 BOARD=STM32F429
# start to run gdb
make st-util_gdb
# run in different terminal
make st-util_connect
```

# Serial Console
We can connect the UART to see the debug message.

The GPIO for UART is different by models.

| model | GPIO |
| --- | --- |
| F429 | PA9:TX, PA10:RX |

You can use serial application, like minicom to see the message.

Serial Settings is 8N1, 115200.

```
# Ubuntu
sudo apt-get install minicom
# MAC
brew install minicom
# Usage
minicom -s
# ESC-Z for leaving minicom
```

# System
arm-os-4fun now can run on qemu, which emulates [STM32-P103](https://www.olimex.com/Products/ARM/ST/STM32-P103/). The supported CPU is stm32f103, which is ARM Cortex M3.

The OS also supports [STM32F429I](https://www.st.com/en/evaluation-tools/32f429idiscovery.html), which is ARM Cortex M4.

You can reference the link below.

* [The Definitive Guide to ARM® Cortex®-M3 and Cortex®-M4 Processors](https://www.amazon.com/Definitive-Guide-Cortex%C2%AE-M3-Cortex%C2%AE-M4-Processors/dp/0124080820)
* [RM0008 Reference manual for STM32F10xxx/20xxx/21xxx/L1xxxx](https://www.st.com/content/ccc/resource/technical/document/reference_manual/59/b9/ba/7f/11/af/43/d5/CD00171190.pdf/files/CD00171190.pdf/jcr:content/translations/en.CD00171190.pdf)
* [PM0056 Programming manual for STM32F101xx, STM32F102xx, STM32F103xx, STM32F105xx and STM32F107xx](https://www.st.com/content/ccc/resource/technical/document/programming_manual/5b/ca/8d/83/56/7f/40/08/CD00228163.pdf/files/CD00228163.pdf/jcr:content/translations/en.CD00228163.pdf)

# Features
## Done
* Context Switch (Use SysTick)
* System Call
* malloc/free

## Future
* basic shell
* message queue/IPC
* resource protection
* file system
