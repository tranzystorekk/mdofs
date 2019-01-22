#include "read.h"

#include <unistd.h>

#include "common.h"

int simplefs::read(int fd, char* buf, int len) {
    if (buf == nullptr) {
        return -1;
    }

    if (len < 0) {
        return -1;
    } else if (len == 0) {
        return 0;
    }

    // check if desciptor index is valid
    if (fd < 0 || fd >= simplefs::MAX_FILE_DESCRIPTORS) {
        return -1;
    }

    simplefs::FileDescriptor& fileDescriptor = simplefs::FdTable[fd];

    // check if file descriptor is actually open
    if (fileDescriptor.is_free) {
        return -1;
    }

    fsproto::Inode& fileNode = *simplefs::Inodes.mutable_inodes(fileDescriptor.inode);

    unsigned int readableLength = len;
    const unsigned int fileSize = fileNode.size();
    if (fileDescriptor.fpos + readableLength > fileSize) {
        readableLength = fileSize - fileDescriptor.fpos;
    }

    lseek(FsHandle, simplefs::Inodes.filesystem_origin() + fileNode.origin() + fileDescriptor.fpos, SEEK_SET);
    ::read(FsHandle, buf, readableLength);

    fileDescriptor.fpos += readableLength;

    return readableLength;
}
