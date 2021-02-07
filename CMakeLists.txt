cmake_minimum_required(VERSION 3.17)
project(emu)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/lib/shared)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/lib/static)

add_subdirectory(deps)

add_compile_definitions(LOG_LEVEL=0)

add_library(${PROJECT_NAME}_lib STATIC
    src/Application.cpp src/Application.h src/Pch.h src/core/Event.h
    src/utils/Logging.h src/utils/Assert.h src/utils/Macros.h
    src/core/ui/GLWindowManager.h src/core/ui/GLWindowManager.cpp src/core/ui/Window.h src/core/emu/Memory.cpp src/core/emu/Memory.h src/core/emu/Cartridge.cpp src/core/emu/Cartridge.h src/core/ui/Window.cpp src/core/emu/utils.cpp src/core/emu/utils.h src/core/emu/Video.cpp src/core/emu/Video.h src/core/ui/Shader.h src/core/ui/GLShader.cpp src/core/ui/GLShader.h src/core/ui/Program.h src/core/ui/GLProgram.cpp src/core/ui/GLProgram.h src/core/ui/GLTexture.cpp src/core/ui/GLTexture.h src/utils/GLErrorMacros.h)
target_precompile_headers(${PROJECT_NAME}_lib PUBLIC src/Pch.h)
target_include_directories(${PROJECT_NAME}_lib PUBLIC
    ${PROJECT_SOURCE_DIR}/deps
    ${PROJECT_SOURCE_DIR}/deps/glfw/include
    ${PROJECT_SOURCE_DIR}/deps/glad/include
    src)
target_link_libraries(${PROJECT_NAME}_lib PUBLIC glfw glad stbi_image)
target_compile_definitions(${PROJECT_NAME}_lib PUBLIC GLFW_INCLUDE_NONE)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_lib)

add_subdirectory(test)
