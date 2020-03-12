include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/avr_libs)
add_avr_executable(SOFT_USART_TEST soft_usart_test.c)
avr_target_link_libraries(SOFT_USART_TEST LIBS)

