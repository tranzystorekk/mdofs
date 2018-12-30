add_executable(
        init_test
        src/test-main.cpp
        src/InitTest.cpp
        $<TARGET_OBJECTS:initobj>
)

target_link_libraries(
        init_test
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        protocols
        simplefs
)
