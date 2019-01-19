add_executable(rm
        src/main.cpp)

target_link_libraries(rm
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        protocols
        simplefs
        )
