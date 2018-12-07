set(PROTO_DEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/generated/)
set(PROTO_SRC_FILENAMES directory inode)

# helper lists
string(REGEX REPLACE "([^;]+)" "\\1.proto" PROTO_SOURCES "${PROTO_SRC_FILENAMES}")
string(REGEX REPLACE "([^;]+)" "\\1.pb.h" PROTO_GEN_HEADERS "${PROTO_SRC_FILENAMES}")
string(REGEX REPLACE "([^;]+)" "\\1.pb.cc" PROTO_GEN_SOURCES "${PROTO_SRC_FILENAMES}")
set(PROTO_GENERATED ${PROTO_GEN_HEADERS} ${PROTO_GEN_SOURCES})

string(REGEX REPLACE "([^;]+)" "${PROTO_DEST_DIR}\\1" PROTO_GEN_PATHS "${PROTO_GENERATED}")
string(REGEX REPLACE "([^;]+)" "${PROTO_DEST_DIR}\\1" PROTO_GEN_SOURCES_PATHS "${PROTO_GEN_SOURCES}")
string(REGEX REPLACE "([^;]+)" "${CMAKE_CURRENT_SOURCE_DIR}/\\1" PROTO_SOURCES_PATHS "${PROTO_SOURCES}")

add_custom_command(
        OUTPUT ${PROTO_GEN_PATHS}
        COMMAND protoc -I${CMAKE_CURRENT_SOURCE_DIR} --cpp_out=${PROTO_DEST_DIR} ${PROTO_SOURCES_PATHS}
        MAIN_DEPENDENCY ${PROTO_SOURCES}
        COMMENT "Generating protocol files"
        COMMAND_EXPAND_LISTS
)

add_library(protocols ${PROTO_GEN_SOURCES_PATHS})

target_link_libraries(protocols protobuf-lite)
