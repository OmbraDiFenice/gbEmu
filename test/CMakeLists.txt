get_target_property(GMOCK_INCLUDE_DIRS gmock INCLUDE_DIRECTORIES)

add_executable(test main.cpp core/emu/TestMemory.cpp core/emu/TestCartridge.cpp core/emu/TestInstructions.cpp core/emu/instructions/Test8BitLoad.cpp core/emu/instructions/Test16BitLoad.cpp core/emu/instructions/TestCarry.cpp core/emu/instructions/Test8BitAlu.cpp core/emu/instructions/Test16BitAlu.cpp core/emu/instructions/TestMisc.cpp core/emu/instructions/TestRotatesAndShifts.cpp core/emu/instructions/TestBitOperations.cpp core/emu/instructions/TestJumps.cpp)

target_link_libraries(test PUBLIC gmock) # gmock already includes gtest
target_link_libraries(test PUBLIC ${PROJECT_NAME}_lib)

target_include_directories(test PUBLIC
        ${GMOCK_INCLUDE_DIRS}
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/test)
