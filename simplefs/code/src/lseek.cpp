#include "lseek.h"

#include <common.h>

int simplefs::lseek(int fd, int whence, int offset) {
    if (fd < 0 || fd >= simplefs::MAX_FILE_DESCRIPTORS) {
        return -1;
    }

    simplefs::FileDescriptor& fileDescriptor = simplefs::FdTable[fd];

    // check if file descriptor is actually open
    if (fileDescriptor.is_free) {
        return -1;
    }

    const fsproto::Inode& fileNode = simplefs::Inodes.inodes(fileDescriptor.inode);
    const unsigned int fileSize = fileNode.size();

    unsigned int& filePos = fileDescriptor.fpos;
    switch (whence) {
        case SeekFlag::SET:
            if (offset < 0 || offset > fileSize) {
                return -1;
            }

            filePos = offset;
            break;

        case SeekFlag::CUR:
            {
                const int newOffset = filePos + offset;

                if (newOffset < 0 || newOffset > fileSize) {
                    return -1;
                }

                filePos = newOffset;
            }
            break;

        case SeekFlag::END:
            filePos = fileSize;
            break;

        default:
            return -1;
    }

    return filePos;
}
