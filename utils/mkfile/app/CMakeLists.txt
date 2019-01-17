add_executable(mkfile
        src/main.cpp)

target_link_libraries(mkfile
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        protocols
        simplefs
        )
