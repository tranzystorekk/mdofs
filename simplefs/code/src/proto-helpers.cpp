#include "proto-helpers.hpp"

#include <memory>
#include <sstream>

#include <unistd.h>

#include "common.h"
#include "locking.h"

using namespace simplefs;

using fsproto::Directory;
using fsproto::Inode;
using google::protobuf::MessageLite;

void protohelpers::setInode(
        Inode* inode,
        unsigned int origin,
        unsigned int size,
        unsigned int mode,
        bool is_free) {
    inode->set_origin(origin);
    inode->set_size(size);
    inode->set_mode(mode);
    inode->set_is_free(is_free);
}

void protohelpers::setInodeAsDirectory(Inode* inode, unsigned int origin, unsigned int mode) {
    setInode(inode, origin, MAX_ENCODED_DIRECTORY_SIZE, mode | AccessFlag::DIR, false);
}

std::string protohelpers::lengthEncodeMsg(const MessageLite& msg) {
    std::ostringstream oss;

    const uint32_t encodedSize = msg.ByteSize();

    oss.write(reinterpret_cast<const char*>(&encodedSize), sizeof(encodedSize));
    oss.write(msg.SerializeAsString().c_str(), encodedSize);

    return oss.str();
}

Directory protohelpers::decodeDirectory(const std::string& str) {
    std::istringstream iss(str);
    uint32_t encodedSize;

    iss.read(reinterpret_cast<char*>(&encodedSize), sizeof(encodedSize));

    std::unique_ptr<char[]> decodeBuffer(new char[encodedSize]);

    iss.read(decodeBuffer.get(), encodedSize);

    Directory result;
    result.ParseFromArray(decodeBuffer.get(), encodedSize);

    return result;
}
