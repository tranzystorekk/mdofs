#include "proto-helpers.hpp"

#include <memory>
#include <sstream>

#include "common.h"

using namespace simplefs;

void protohelpers::setInode(
        fsproto::Inode* inode,
        unsigned int origin,
        unsigned int size,
        unsigned int mode,
        bool is_free) {
    inode->set_origin(origin);
    inode->set_size(size);
    inode->set_mode(mode);
    inode->set_is_free(is_free);
}

void protohelpers::setInodeAsDirectory(fsproto::Inode* inode, unsigned int origin, unsigned int mode) {
    //FIXME calculate dir size
    setInode(inode, origin, 0, mode | AccessFlag::DIR, false);
}

std::string protohelpers::lengthEncodeMsg(const google::protobuf::MessageLite& msg) {
    std::ostringstream oss;

    const uint32_t encodedSize = msg.ByteSizeLong();

    std::cout << encodedSize << std::endl;

    oss.write(reinterpret_cast<const char*>(&encodedSize), sizeof(encodedSize));
    oss.write(msg.SerializeAsString().c_str(), encodedSize);

    return oss.str();
}

fsproto::Directory protohelpers::decodeDirectory(const std::string& str) {
    std::istringstream iss(str);
    uint32_t encodedSize;

    iss.read(reinterpret_cast<char*>(&encodedSize), sizeof(encodedSize));

    std::unique_ptr<char> decodeBuffer(new char[encodedSize]);

    iss.read(decodeBuffer.get(), encodedSize);

    fsproto::Directory result;
    result.ParseFromArray(decodeBuffer.get(), encodedSize);

    return result;
}
