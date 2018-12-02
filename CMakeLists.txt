cmake_minimum_required(VERSION 3.5)
project(mdofs)

set(CMAKE_CXX_STANDARD 11)

include_directories(
        3rdparty/include
        fsproto/generated
)

add_subdirectory(fsproto)

add_subdirectory(simplefs/app)
add_subdirectory(simplefs/test)

add_subdirectory(utils/cat/app)
add_subdirectory(utils/cat/test)
add_subdirectory(utils/ls/app)
add_subdirectory(utils/ls/test)
add_subdirectory(utils/mkdir/app)
add_subdirectory(utils/mkdir/test)
add_subdirectory(utils/mkfile/app)
add_subdirectory(utils/mkfile/test)
add_subdirectory(utils/rm/app)
add_subdirectory(utils/rm/test)
