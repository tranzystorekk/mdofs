add_library(
        simplefs
        src/proto-helpers.cpp
        src/auxilliary.cpp
        src/handle-management.cpp
        src/locking.cpp
        src/open.cpp
        src/mkdir.cpp
        src/shared-vars.cpp
)

target_link_libraries(
        simplefs
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        protocols
)
