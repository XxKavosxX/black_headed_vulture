include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/avr_libs)
add_avr_executable(USART_TEST usart_test.c)
avr_target_link_libraries(USART_TEST LIBS)

