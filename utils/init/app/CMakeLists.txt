add_library(initobj OBJECT src/init.cpp)

add_executable(
        init
        src/main.cpp
        $<TARGET_OBJECTS:initobj>
)

target_link_libraries(
        init
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        protocols
        simplefs
)
