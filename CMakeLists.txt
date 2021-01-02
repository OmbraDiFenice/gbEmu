cmake_minimum_required(VERSION 3.17)
project(emu)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/lib/shared)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/lib/static)

add_subdirectory(deps)

add_compile_definitions(LOG_LEVEL=1)

add_library(${PROJECT_NAME}_lib STATIC src/Application.cpp src/Application.h src/Logging.h src/Assert.h src/GLWindowManager.h src/Pch.h src/GLWindowManager.cpp src/Window.h)
target_precompile_headers(${PROJECT_NAME}_lib PUBLIC src/Pch.h)
target_include_directories(${PROJECT_NAME}_lib PUBLIC
    ${PROJECT_SOURCE_DIR}/deps
    ${PROJECT_SOURCE_DIR}/deps/glfw/include
    ${PROJECT_SOURCE_DIR}/deps/glad/include
    src)
target_link_libraries(${PROJECT_NAME}_lib PUBLIC glfw glad)
target_compile_definitions(${PROJECT_NAME}_lib PUBLIC GLFW_INCLUDE_NONE)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_lib)

add_subdirectory(test)
