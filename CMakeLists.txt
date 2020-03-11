# ##############################################################################
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# Copyright (c) 2016 Igor Mikolic-Torreira
#
# Inspired in part by cmake toolchains created by - Tomasz Bogdal
# (@queezythegreat on GitHub) - Matthias Kleemann (@mkleemann on GitHub)
#
# ##############################################################################

cmake_minimum_required(VERSION 3.10)

project(
  black_headed_vulture
  VERSION 1.0.0
  LANGUAGES C CXX ASM)

set(PROJECT_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
set(AVR_PATH "/usr/lib/avr")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_SYSTEM_INCLUDE_PATH "${CMAKE_FIND_ROOT_PATH}/include")
set(CMAKE_SYSTEM_LIBRARY_PATH "${CMAKE_FIND_ROOT_PATH}/lib")

set(AVR_MCU atmega328p)
set(AVR_MCU_SPEED 16000000UL)
set(AVR_UPLOADTOOL avrdude)
set(AVR_UPLOADTOOL_PORT /dev/ttyUSB0)
set(AVR_UPLOAD_SPEED 57600)
set(AVR_PROGRAMMER arduino)
set(AVR_SIZE_ARGS -C;--mcu=${AVR_MCU})
set(AVR_UPLOADTOOL_OPTIONS -b${AVR_UPLOAD_SPEED} -D -V)

set(AVR_OBJCOPY avr-objcopy)
set(AVR_SIZE_TOOL avr-size)
set(AVR_OBJDUMP avr-objdump)
set(AVR_SIZE avr-size)
set(AWK awk)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)
set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_CXX_COMPILER avr-g++)
set(CMAKE_ASM_COMPILER avr-gcc)

# ##############################################################################
# check build types: - Debug - Release - MinSizeRel - RelWithDebInfo
#
# Release is chosen, because of some optimized functions in the AVR toolchain,
# e.g. _delay_ms().
# ##############################################################################
if(NOT
   ((CMAKE_BUILD_TYPE MATCHES Release)
    OR (CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
    OR (CMAKE_BUILD_TYPE MATCHES Debug)
    OR (CMAKE_BUILD_TYPE MATCHES MinSizeRel)))
  set(CMAKE_BUILD_TYPE
      Release
      CACHE STRING
            "Choose cmake build type: Debug Release RelWithDebInfo MinSizeRel"
            FORCE)
endif(
  NOT
  ((CMAKE_BUILD_TYPE MATCHES Release)
   OR (CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
   OR (CMAKE_BUILD_TYPE MATCHES Debug)
   OR (CMAKE_BUILD_TYPE MATCHES MinSizeRel)))

# ##############################################################################
# target file name add-on
# ##############################################################################
if(WITH_MCU)
  set(MCU_TYPE_FOR_FILENAME "-${AVR_MCU}")
else(WITH_MCU)
  set(MCU_TYPE_FOR_FILENAME "")
endif(WITH_MCU)

# ##############################################################################
# status messages
# ##############################################################################
message(STATUS "Current uploadtool is: ${AVR_UPLOADTOOL}")
message(STATUS "Current programmer is: ${AVR_PROGRAMMER}")
message(STATUS "Current upload port is: ${AVR_UPLOADTOOL_PORT}")
message(STATUS "Current uploadtool options are: ${AVR_UPLOADTOOL_OPTIONS}")
message(STATUS "Current AVR MCU is set to: ${AVR_MCU}")
message(STATUS "Current AVR MCU speed is set to: ${AVR_MCU_SPEED}")

function(add_avr_executable EXECUTABLE_NAME)

  if(NOT ARGN)
    message(
      FATAL_ERROR
        "ADD_AVR_EXECUTABLE ERROR: No source files given for ${EXECUTABLE_NAME}."
    )
  endif(NOT ARGN)

  set(elf_file ${EXECUTABLE_NAME}${MCU_TYPE_FOR_FILENAME}.elf)
  set(lss_file ${EXECUTABLE_NAME}${MCU_TYPE_FOR_FILENAME}.lss)
  set(hex_file ${EXECUTABLE_NAME}${MCU_TYPE_FOR_FILENAME}.hex)
  set(map_file ${EXECUTABLE_NAME}${MCU_TYPE_FOR_FILENAME}.map)
  set(lst_file ${EXECUTABLE_NAME}${MCU_TYPE_FOR_FILENAME}.lst)
  set(eeprom_file ${EXECUTABLE_NAME}${MCU_TYPE_FOR_FILENAME}-eeprom.hex)

  # elf file
  add_executable(${elf_file} EXCLUDE_FROM_ALL ${ARGN})

  target_compile_options(
    ${elf_file}
    PUBLIC -ffunction-sections
           -fdata-sections
           -fpack-struct
           -fshort-enums
           -funsigned-char
           -funsigned-bitfields
           -mmcu=${AVR_MCU}
           -DF_CPU=${AVR_MCU_SPEED}
           -g
           -Os
           -w
           -std=gnu99
           -fno-exceptions
           -ffunction-sections
           -fdata-sections)

  target_link_libraries(
    ${elf_file}
    "-mmcu=${AVR_MCU} -Wl,--gc-sections -mrelax -Wl,-Map,${map_file}")

  # lst
  add_custom_command(
    OUTPUT ${lst_file}
    COMMAND ${AVR_OBJDUMP} -h -S ${elf_file} > ${lst_file}
    DEPENDS ${elf_file})

  # lss
  add_custom_command(
    OUTPUT ${lss_file}
    COMMAND ${AVR_OBJDUMP} -h -S --show-raw-insn ${elf_file} > ${lss_file}
    DEPENDS ${elf_file})

  # hex
  add_custom_command(
    OUTPUT ${hex_file}
    COMMAND ${AVR_OBJCOPY} -j .text -j .data -O ihex ${elf_file} ${hex_file}
    COMMAND ${AVR_SIZE_TOOL} -C;--mcu=${AVR_MCU} ${elf_file} "|" ${AWK} -f
            firmwaresize_${EXECUTABLE_NAME}.awk
    DEPENDS ${elf_file} firmwaresize_${EXECUTABLE_NAME}.awk)

  # eeprom
  add_custom_command(
    OUTPUT ${eeprom_file}
    COMMAND
      ${AVR_OBJCOPY} -j .eeprom --set-section-flags=.eeprom=alloc,load
      --change-section-lma .eeprom=0 --no-change-warnings -O ihex ${elf_file}
      ${eeprom_file}
    DEPENDS ${elf_file})

  add_custom_target(${EXECUTABLE_NAME} ALL DEPENDS ${hex_file} ${eeprom_file})

  set_target_properties(${EXECUTABLE_NAME} PROPERTIES OUTPUT_NAME "${elf_file}")

  # clean
  get_directory_property(clean_files ADDITIONAL_MAKE_CLEAN_FILES)
  set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${map_file}")

  # upload - with avrdude
  add_custom_target(
    upload_${EXECUTABLE_NAME}
    ${AVR_UPLOADTOOL}
    -p
    ${AVR_MCU}
    -c
    ${AVR_PROGRAMMER}
    ${AVR_UPLOADTOOL_OPTIONS}
    -U
    flash:w:${hex_file}
    -P
    ${AVR_UPLOADTOOL_PORT}
    DEPENDS ${hex_file}
    COMMENT "Uploading ${hex_file} to ${AVR_MCU} using ${AVR_PROGRAMMER}")

  add_custom_target(
    upload_eeprom_${EXECUTABLE_NAME}
    ${AVR_UPLOADTOOL}
    -p
    ${AVR_MCU}
    -c
    ${AVR_PROGRAMMER}
    ${AVR_UPLOADTOOL_OPTIONS}
    -U
    eeprom:w:${eeprom_file}
    -P
    ${AVR_UPLOADTOOL_PORT}
    DEPENDS ${eeprom_file}
    COMMENT "Uploading ${eeprom_file} to ${AVR_MCU} using ${AVR_PROGRAMMER}")

  # get status
  add_custom_target(
    get_status_${EXECUTABLE_NAME}
    ${AVR_UPLOADTOOL}
    -p
    ${AVR_MCU}
    -c
    ${AVR_PROGRAMMER}
    -P
    ${AVR_UPLOADTOOL_PORT}
    -n
    -v
    COMMENT "Get status from ${AVR_MCU}")

  # disassemble
  add_custom_target(
    disassemble_${EXECUTABLE_NAME}
    ${AVR_OBJDUMP} -h -S ${elf_file} > ${EXECUTABLE_NAME}.lst
    DEPENDS ${elf_file})

  # size
  add_custom_target(
    size_${EXECUTABLE_NAME}
    ${AVR_SIZE_TOOL}
    -C;--mcu=${AVR_MCU}
    ${elf_file}
    "|"
    ${AWK}
    -f
    firmwaresize_${EXECUTABLE_NAME}.awk
    DEPENDS ${elf_file} firmwaresize_${EXECUTABLE_NAME}.awk)

  add_custom_target(
    firmwaresize_${EXECUTABLE_NAME}.awk
    COMMAND echo "BEGIN {ORS=\"\";print \"\\\\n\\\\033[1;33mFirmware size (\"}"
            > firmwaresize_${EXECUTABLE_NAME}.awk
    COMMAND echo "/^Device/ {print \$2 \") is...  \"}" >>
            firmwaresize_${EXECUTABLE_NAME}.awk
    COMMAND
      echo
      "/^Program/ {print \"Flash (program): \" \$2 \" \" \$3 \" \" \$4 \")  \"}"
      >> firmwaresize_${EXECUTABLE_NAME}.awk
    COMMAND
      echo
      "/^Data/ {print \"RAM\ (globals): \" \$2 \" \" \$3 \" \" \$4 \")  \"}" >>
      firmwaresize_${EXECUTABLE_NAME}.awk
    COMMAND echo "END {print \"\\\\033[0m\\\\n\\\\n\"}" >>
            firmwaresize_${EXECUTABLE_NAME}.awk
    VERBATIM)

endfunction(add_avr_executable)

function(add_avr_library LIBRARY_NAME)

  if(NOT ARGN)
    message(FATAL_ERROR "No source files given for ${LIBRARY_NAME}.")
  endif(NOT ARGN)

  set(lib_file ${LIBRARY_NAME}${MCU_TYPE_FOR_FILENAME})

  add_library(${lib_file} STATIC ${ARGN})

  set_target_properties(${lib_file} PROPERTIES OUTPUT_NAME "${lib_file}")

  target_compile_options(
    ${lib_file}
    PUBLIC -ffunction-sections
           -fdata-sections
           -fpack-struct
           -fshort-enums
           -funsigned-char
           -funsigned-bitfields
           -mmcu=${AVR_MCU}
           -DF_CPU=${AVR_MCU_SPEED})

  if(NOT TARGET ${LIBRARY_NAME})
    add_custom_target(${LIBRARY_NAME} ALL DEPENDS ${lib_file})

    set_target_properties(${LIBRARY_NAME} PROPERTIES OUTPUT_NAME "${lib_file}")
  endif(NOT TARGET ${LIBRARY_NAME})

endfunction(add_avr_library)

function(avr_target_link_libraries EXECUTABLE_TARGET)

  if(NOT ARGN)
    message(FATAL_ERROR "Nothing to link to ${EXECUTABLE_TARGET}.")
  endif(NOT ARGN)

  get_target_property(TARGET_LIST ${EXECUTABLE_TARGET} OUTPUT_NAME)

  foreach(TGT ${ARGN})
    if(TARGET ${TGT})
      get_target_property(ARG_NAME ${TGT} OUTPUT_NAME)
      list(APPEND TARGET_LIST ${ARG_NAME})
    else(TARGET ${TGT})
      list(APPEND NON_TARGET_LIST ${TGT})
    endif(TARGET ${TGT})
  endforeach(TGT ${ARGN})

  target_link_libraries(${TARGET_LIST} ${NON_TARGET_LIST})

endfunction(avr_target_link_libraries EXECUTABLE_TARGET)

add_subdirectory(src)
add_subdirectory(app)
add_subdirectory(tests)
