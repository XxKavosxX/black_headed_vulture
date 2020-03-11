include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/avr_libs)
add_avr_executable(MAIN main.c)
avr_target_link_libraries(MAIN LIBS)
