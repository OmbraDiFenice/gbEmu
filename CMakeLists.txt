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
        src/core/graphics/opengl/GLWindowManager.h src/core/graphics/opengl/GLWindowManager.cpp src/core/graphics/Window.h src/core/emu/Memory.cpp src/core/emu/Memory.h src/core/emu/Cartridge.cpp src/core/emu/Cartridge.h src/core/graphics/Window.cpp src/core/emu/utils.cpp src/core/emu/utils.h src/core/graphics/Shader.h src/core/graphics/opengl/GLShader.cpp src/core/graphics/opengl/GLShader.h src/core/graphics/Program.h src/core/graphics/opengl/GLProgram.cpp src/core/graphics/opengl/GLProgram.h src/core/graphics/opengl/GLTexture.cpp src/core/graphics/opengl/GLTexture.h src/utils/GLErrorMacros.h src/core/graphics/Texture.h src/core/graphics/Buffer.h src/core/graphics/Buffer.cpp src/core/graphics/Renderer.h src/core/graphics/opengl/GLRenderer.cpp src/core/graphics/opengl/GLRenderer.h src/core/graphics/ColorPalette.cpp src/core/graphics/ColorPalette.h src/EmuApplication.cpp src/EmuApplication.h src/core/graphics/ShaderStorageBuffer.h src/core/graphics/opengl/GLShaderStorageBuffer.cpp src/core/graphics/opengl/GLShaderStorageBuffer.h src/core/graphics/opengl/Synchronization.cpp src/core/graphics/opengl/Synchronization.h
        src/core/graphics/opengl/GLBuffer.h src/core/graphics/opengl/GLBuffer.cpp src/core/emu/Cpu.cpp src/core/emu/Cpu.h src/core/emu/Instructions.cpp)
target_precompile_headers(${PROJECT_NAME}_lib PUBLIC src/Pch.h)
target_include_directories(${PROJECT_NAME}_lib PUBLIC
    ${PROJECT_SOURCE_DIR}/deps
    ${PROJECT_SOURCE_DIR}/deps/glfw/include
    ${PROJECT_SOURCE_DIR}/deps/glad/include
    src)
target_link_libraries(${PROJECT_NAME}_lib PUBLIC glfw glad stbi_image glm::glm)
target_compile_definitions(${PROJECT_NAME}_lib PUBLIC GLFW_INCLUDE_NONE)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_lib)

add_subdirectory(test)
add_subdirectory(sandbox)
