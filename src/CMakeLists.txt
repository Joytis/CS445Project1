find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

file(GLOB_RECURSE SOURCES "*.cpp" "*.h")

add_executable(project1 ${SOURCES})

include_directories( 
	${OPENGL_INCLUDE_DIRS} 
	${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_command(TARGET project1 POST_BUILD        # Adds a post-build event to project1
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${PROJECT_SOURCE_DIR}/lib/libstdc++-6.dll"      # <--this is in-file
        $<TARGET_FILE_DIR:project1>)                 # <--this is out-file path

add_custom_command(TARGET project1 PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/lib $<TARGET_FILE_DIR:project1>)

target_link_libraries(project1 ${OPENGL_LIBRARIES} freeglut.a)
