
add_executable(floyd floyd.c)

target_link_libraries(floyd random_uitls)
target_link_libraries(floyd matrix)

