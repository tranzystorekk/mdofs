add_executable(cat
        src/main.cpp)

target_link_libraries(cat
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        protocols
        simplefs
        )