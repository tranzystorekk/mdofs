add_executable(mkdir
        src/main.cpp)

target_link_libraries(mkdir
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    protocols
    simplefs
)
