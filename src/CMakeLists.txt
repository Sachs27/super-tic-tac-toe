aux_source_directory(. SRCS)

add_executable(super-tic-tac-toe ${SRCS})

set_target_properties(super-tic-tac-toe PROPERTIES RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_SOURCE_DIR}/bin)

if (UNIX)
    set(LIBS GL GLEW glfw IL m)
elseif (WIN32)
    set(LIBS glew32 glfw3 opengl32 DevIL m)
    set(CMAKE_EXE_LINKER_FLAGS "-mwindows")
endif (UNIX)

target_link_libraries(super-tic-tac-toe ${LIBS})
