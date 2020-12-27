get_target_property(GMOCK_INCLUDE_DIRS gmock INCLUDE_DIRECTORIES)

add_executable(test main.cpp FailingTest.cpp)

target_link_libraries(test PUBLIC gmock) # gmock already includes gtest
target_link_libraries(test PUBLIC ${PROJECT_NAME}_lib)

target_include_directories(test PUBLIC
        ${GMOCK_INCLUDE_DIRS}
        ${PROJECT_SOURCE_DIR}/src)
