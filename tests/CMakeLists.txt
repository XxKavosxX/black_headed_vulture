include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/avr_libs)
add_avr_executable(BMP bmp180_test.c)
avr_target_link_libraries(BMP LIBS)

