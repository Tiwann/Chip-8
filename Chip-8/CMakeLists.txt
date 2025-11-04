file(GLOB_RECURSE CHIP8_SOURCES Source/*.cpp Source/*.h)

add_executable(Chip-8)
set_target_properties(Chip-8 PROPERTIES CXX_STANDARD 23)
target_sources(Chip-8 PRIVATE ${CHIP8_SOURCES})
target_link_libraries(Chip-8 PUBLIC NovaEngine)
target_compile_definitions(Chip-8 PRIVATE NOVA_APPLICATION_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

if(CMAKE_BUILD_TYPE MATCHES Release OR CMAKE_BUILD_TYPE MATCHES MinSizeRel)
    set_target_properties(Chip-8 PROPERTIES WIN32_EXECUTABLE TRUE)
endif ()