include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/avr_libs)
add_avr_executable(NEO neo6_test.c)
avr_target_link_libraries(NEO LIBS)

