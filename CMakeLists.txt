cmake_minimum_required(VERSION 3.8)
project(mdofs)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED COMPONENTS system filesystem)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find Boost library (system, filesystem)")
endif(NOT Boost_FOUND)

include_directories(
        3rdparty/include
        fsproto/generated
        simplefs/code/include
)

add_subdirectory(fsproto)

add_subdirectory(simplefs)

add_subdirectory(utils/init)
add_subdirectory(utils/cat)
add_subdirectory(utils/ls)
add_subdirectory(utils/mkdir)
add_subdirectory(utils/mkfile)
add_subdirectory(utils/rm)
