
add_executable(pi_serial pi_serial.c)
add_executable(pi_avx pi_avx.c)

target_link_libraries(pi_serial utils)
target_link_libraries(pi_avx utils)