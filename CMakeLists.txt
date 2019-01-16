# Project name
project(clion-arduino-demo)

# CMake version
cmake_minimum_required(VERSION 3.0)
set(CMAKE_SYSTEM_NAME Arduino)

# Adjust to your environment
set(ARDUINO_PACKAGES "$ENV{HOME}/.arduino15/packages/arduino" CACHE PATH "Path to arduino installation")

# List of files and names that should be a target
set(MAIN_FILES main.cpp tester.cpp)
set(RUN_NAMES main test)

# Options
# Adjust to your board
set(PORT "/dev/ttyACM0" CACHE STRING "USB Port")
set(MCU "atmega328p" CACHE STRING "Processor Type")
set(CPU_SPEED "16000000" CACHE STRING "Speed of the CPU")
set(PIN_VARIANT "standard" CACHE STRING "Pin Selection Variant. Either standard, mega, leonardo, eightanaloginputs")
set(PROGRAMMER "stk500v1" CACHE STRING "Programmer Type")
set(COMPILE_FLAGS "" CACHE STRING "Additional Compiler Flags")

# Update to correct version
# TODO : Make so that it auto finds
set(ARDUINO_TOOLS ${ARDUINO_PACKAGES}/tools/avr-gcc/5.4.0-atmel3.6.1-arduino2/bin)
set(ARDUINO_AVR ${ARDUINO_PACKAGES}/hardware/avr/1.6.23)
set(ARDUINO_AVRDUDE ${ARDUINO_PACKAGES}/tools/avrdude/6.3.0-arduino14)

# Set source files
file(GLOB_RECURSE SRC_FILES lib/*.cpp)

# Set Arduino source files.
file(GLOB_RECURSE ARDUINO_CORE_SRC
        ${ARDUINO_AVR}/cores/arduino/*.c
        ${ARDUINO_AVR}/cores/arduino/*.cpp)

# Include directories
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${ARDUINO_AVR}/cores/arduino
        ${ARDUINO_AVR}/variants/${PIN_VARIANT}
        ${AR_AVR_INC}
        ${AR_AVRGCC_INC}
)

# Compiler suite specification
set(CMAKE_C_COMPILER ${ARDUINO_TOOLS}/avr-gcc)
set(CMAKE_CXX_COMPILER ${ARDUINO_TOOLS}/avr-g++)
set(CMAKE_OBJCOPY ${ARDUINO_TOOLS}/avr-objcopy)
set(CMAKE_OBJDUMP ${ARDUINO_TOOLS}/avr-objdump)
set(CMAKE_RANLIB ${ARDUINO_TOOLS}/avr-ranlib)
set(CMAKE_LINKER ${ARDUINO_TOOLS}/avr-ld)

# Compiler flags
add_definitions(-mmcu=${MCU} -DF_CPU=${CPU_SPEED} -c -g -Os -Wall -fno-exceptions -ffunction-sections -fdata-sections)

# Linker flags
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")   # remove -rdynamic for C
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "") # remove -rdynamic for CXX
set(CMAKE_EXE_LINKER_FLAGS "-Os -Wl,--gc-sections -mmcu=${MCU}")

# Create a target for each item and name in the lists MAIN_FILE and RUN_NAMES
list(LENGTH MAIN_FILES LENGTH1)
math(EXPR LENGTH2 "${LENGTH1} - 1")

foreach(INDEX RANGE ${LENGTH2})
    # Get the name and file
    list(GET MAIN_FILES ${INDEX} CURRENT_FILE)
    list(GET RUN_NAMES ${INDEX} CURRENT_NAME)

    # Add executable and upload target
    add_executable(${CURRENT_NAME} ${CURRENT_FILE} ${ARDUINO_CORE_SRC} ${SRC_FILES})
    add_custom_target(upload-${CURRENT_NAME}
            COMMAND ${CMAKE_OBJCOPY} -j .text -j .data -O ihex ${CURRENT_NAME} ${PROJECT_NAME}.hex
            COMMAND ${ARDUINO_AVRDUDE}/bin/avrdude -C${ARDUINO_AVRDUDE}/etc/avrdude.conf -F -p${MCU} -c${PROGRAMMER} -P${PORT} -D -Uflash:w:${PROJECT_NAME}.hex:i
            DEPENDS ${CURRENT_NAME})
endforeach(INDEX)