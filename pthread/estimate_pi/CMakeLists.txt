
add_executable(estimate_pi estimate_pi.c estimate_pi.h)

find_package(Threads REQUIRED)
target_link_libraries(estimate_pi Threads::Threads)