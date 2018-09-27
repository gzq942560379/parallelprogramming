add_executable(hello_world hello_world.c)


include_directories(${MPI_INCLUDE_PATH})
target_link_libraries(hello_world ${MPI_LIBRARIES})

#if(MPI_COMPILE_FLAGS)
#    set_target_properties(hello_world PROPERTIES
#            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
#endif()
#if(MPI_LINK_FLAGS)
#    set_target_properties(hello_world PROPERTIES LINK_FLAGS "${MPI_LINK_FLAGS}")
#endif()