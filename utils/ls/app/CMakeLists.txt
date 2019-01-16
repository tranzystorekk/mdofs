add_executable(
        ls
        src/main.cpp
)

target_link_libraries(
        ls
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        protocols
        simplefs
)
