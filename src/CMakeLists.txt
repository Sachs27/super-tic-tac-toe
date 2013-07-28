aux_source_directory(. SRCS)

add_executable(super-tic-tac-toe ${SRCS})

set_target_properties(super-tic-tac-toe PROPERTIES RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_SOURCE_DIR}/bin)

target_link_libraries(super-tic-tac-toe GL GLEW glfw IL m)
