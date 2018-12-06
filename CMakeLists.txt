cmake_minimum_required(VERSION 3.5)
project(mdofs)

set(CMAKE_CXX_STANDARD 11)

include_directories(
        3rdparty/include
        fsproto/generated
)

add_subdirectory(fsproto)

add_subdirectory(simplefs)

add_subdirectory(utils/init)
add_subdirectory(utils/dispose)
add_subdirectory(utils/cat)
add_subdirectory(utils/ls)
add_subdirectory(utils/mkdir)
add_subdirectory(utils/mkfile)
add_subdirectory(utils/rm)
