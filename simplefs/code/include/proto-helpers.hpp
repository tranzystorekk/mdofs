#ifndef MDOFS_PROTO_HELPERS_HPP
#define MDOFS_PROTO_HELPERS_HPP

#include <string>
#include <utility>

#include <fcntl.h>

#include "directory.pb.h"
#include "inode.pb.h"

namespace simplefs {
namespace protohelpers {

void setInode(
        fsproto::Inode* inode,
        unsigned int origin,
        unsigned int size,
        unsigned int mode,
        bool is_free = false);

void setInodeAsDirectory(fsproto::Inode* inode, unsigned int origin, unsigned int mode);

std::string lengthEncodeMsg(const google::protobuf::MessageLite& msg);

fsproto::Directory decodeDirectory(const std::string& str);

} // protohelpers
} // simplefs

#endif //MDOFS_PROTO_HELPERS_HPP
