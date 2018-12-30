add_library(
        simplefs
        src/proto-helpers.cpp
        src/auxilliary.cpp
        src/handle-management.cpp
)

target_link_libraries(simplefs protocols)
