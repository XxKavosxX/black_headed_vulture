include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/avr_libs)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/FatFs)

add_avr_executable(USART usart_test.c)
avr_target_link_libraries(USART LIBS FATFS)

