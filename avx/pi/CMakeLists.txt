
add_executable(pi_serial pi_serial.c)
add_executable(pi_avx pi_avx.c)

SET(CMAKE_C_FLAGS_DEBUG "$ENV{CMAKE_C_FLAGS_DEBUG} -mavx")
SET(CMAKE_C_FLAGS_RELEASE "$ENV{CXXFLAGS} -mavx")

target_link_libraries(pi_serial time_utils)
target_link_libraries(pi_avx time_utils)