add_executable(fsize
        src/main.cpp)

target_link_libraries(fsize
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        protocols
        simplefs
        )