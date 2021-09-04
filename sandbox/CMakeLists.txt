add_executable(sandbox main.cpp SandboxApplication.cpp SandboxApplication.h Quad.h)

target_link_libraries(sandbox PUBLIC ${PROJECT_NAME}_lib)

target_include_directories(sandbox PUBLIC
        ${PROJECT_SOURCE_DIR}/sandbox
        ${PROJECT_SOURCE_DIR}/src)
