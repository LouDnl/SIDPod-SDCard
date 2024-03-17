# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.5)

# Include build functions from Pico SDK and others
include(bin2h.cmake)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include($ENV{PICO_EXTRAS_PATH}/external/pico_extras_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(SIDPod-SDCard C CXX ASM)

# set(TINYUSB_PATH ${PICO_SDK_PATH}/lib/tinyusb)
set(PICO_BOARD pico)
set(BOARD pico_sdk)
set(FAMILY rp2040)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
# build with debug symbols
set(CMAKE_BUILD_TYPE Debug)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

bin2h(SOURCE_FILE resources/sidpod.bmp HEADER_FILE include/sidpod_bmp.h VARIABLE_NAME SIDPOD_BMP NULL_TERMINATE)

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME}
        src/main.cpp
        src/hw_config.c
        src/io/ff_util.c
        src/audio/sid.c
        src/visualization/kiss_fftr.c
        src/visualization/kiss_fft.c
        src/display/ssd1306.c
        src/PSIDCatalog.cpp
        src/audio/SIDPlayer.cpp
        src/UI.cpp
        src/visualization/DanceFloor.cpp
        src/System.cpp
        )

pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/src/quadrature_encoder.pio)

# Tell CMake where to find other source code
add_subdirectory(include/FatFs_SPI build)

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(${PROJECT_NAME} PUBLIC
        hardware_rtc
        hardware_sync
        hardware_flash
        hardware_dma
        hardware_adc
        hardware_i2c
        hardware_clocks
        hardware_sleep
        pico_bootsel_via_double_reset
        pico_stdlib
        pico_audio_i2s
        pico_multicore
        pico_unique_id
        FatFs_SPI
        )

# Tell CMake where to find include files
target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include/
        ${CMAKE_CURRENT_LIST_DIR}/include/visualization/
        ${CMAKE_CURRENT_LIST_DIR}/include/visualization/include/
        ${CMAKE_CURRENT_LIST_DIR}/include/audio/
        ${CMAKE_CURRENT_LIST_DIR}/include/audio/include/
        ${CMAKE_CURRENT_LIST_DIR}/include/io/
        ${CMAKE_CURRENT_LIST_DIR}/include/display/include/
        )

# Tell CMake what extra compile definitions to use
target_compile_definitions(${PROJECT_NAME} PUBLIC
        CFG_TUD_MSC=0
        # PICO_RP2040_USB_DEVICE_ENUMERATION_FIX=1
        PICO_AUDIO_I2S_MONO_INPUT=1
        PICO_AUDIO_I2S_DATA_PIN=26
        PICO_AUDIO_I2S_CLOCK_PIN_BASE=27
        )

# Enable uart output and disable usb output
pico_enable_stdio_uart(${PROJECT_NAME} 1)
pico_enable_stdio_usb(${PROJECT_NAME} 0)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})
