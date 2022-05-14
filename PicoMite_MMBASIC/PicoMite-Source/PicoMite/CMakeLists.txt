cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(test_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()
add_executable(PicoMite
    PicoMite.c
    MMBasic.c
    Operators.c
    Custom.c
    Functions.c
    Commands.c
    Memory.c
    FileIO.c
    ff.c
    ffsystem.c
    ffunicode.c
    mmc_stm32.c
    Draw.c
    Editor.c
    XModem.c
    MM_Misc.c
    External.c
    MATHS.c
    Onewire.c
    I2C.c
    SPI.c
    Serial.c
    SPI-LCD.c
    BmpDecoder.c
    Touch.c
    GUI.c
    GPS.c
    Audio.c
    CFunction.c
    Keyboard.c
    SSD1963.c
    picojpeg.c
)
Pico_enable_stdio_usb(PicoMite 1)
pico_enable_stdio_uart(PicoMite 0)
pico_add_extra_outputs(PicoMite)
target_compile_options(PicoMite PRIVATE -DPICO_STDIO_USB_ENABLE_RESET_VIA_VENDOR_INTERFACE=0 -DPICO_STACK_SIZE=0x1000)
target_link_libraries(PicoMite
    pico_stdlib
    hardware_flash
    hardware_irq
    hardware_adc
    hardware_pwm
    hardware_i2c
    hardware_spi
    hardware_dma
    hardware_exception
)
