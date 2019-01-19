#include "write.h"

#include <memory>

#include <unistd.h>

#include "common.h"
#include "locking.h"
#include "proto-helpers.hpp"

int simplefs::write(int fd, const char* buf, int len) {
    // check if buffer is null
    if (buf == nullptr) {
        // TODO error
        return -1;
    }

    // verify requested write length
    if (len < 0) {
        //TODO error
        return -1;
    } else if (len == 0) {
        return 0;
    }

    // check if descriptor index is valid
    if (fd < 0 || fd >= simplefs::MAX_FILE_DESCRIPTORS) {
        // TODO error
        return -1;
    }

    simplefs::FileDescriptor& fileDescriptor = simplefs::FdTable[fd];

    // check if file descriptor is actually open
    if (fileDescriptor.is_free) {
        // TODO error
        return -1;
    }

    fsproto::Inode& fileNode = *simplefs::Inodes.mutable_inodes(fileDescriptor.inode);

    const unsigned int resultOffset = fileDescriptor.fpos + len;

    // check whether write would cross current file boundary
    if (resultOffset > fileNode.size()) {
        // check if last file in system
        if (fileNode.origin() + fileNode.size() >= simplefs::Inodes.first_free_byte()) {
            // lock new file fragment
            struct flock newLock = simplefs::lock(simplefs::Inodes.filesystem_origin() + fileNode.origin(),
                    resultOffset, LockType::WRLK);

            // write new portion of file
            lseek(FsHandle, simplefs::Inodes.filesystem_origin() + fileNode.origin() + fileDescriptor.fpos, SEEK_SET);
            ::write(FsHandle, buf, len);

            fileDescriptor.lock = newLock;

            // update first_free_byte and file's size
            simplefs::Inodes.set_first_free_byte(fileNode.origin() + resultOffset);
            fileNode.set_size(resultOffset);

            // serialize inode table
            const std::string encodedInodeTable = protohelpers::lengthEncodeMsg(simplefs::Inodes);
            lseek(FsHandle, 0, SEEK_SET);
            ::write(FsHandle, encodedInodeTable.c_str(), encodedInodeTable.size());
        } else {
            // lock destination segment
            struct flock movedFileLock = simplefs::lock(simplefs::Inodes.filesystem_origin()
                    + simplefs::Inodes.first_free_byte(), resultOffset, LockType::WRLK);

            // copy the unchanged portion of file to new location
            unsigned int copiedSize = fileDescriptor.fpos;
            std::unique_ptr<char[]> buffer(new char[copiedSize]);

            lseek(FsHandle, simplefs::Inodes.filesystem_origin() + fileNode.origin(), SEEK_SET);
            read(FsHandle, buffer.get(), copiedSize);

            // write new portion of file
            lseek(FsHandle, simplefs::Inodes.filesystem_origin() + simplefs::Inodes.first_free_byte(), SEEK_SET);
            ::write(FsHandle, buffer.get(), copiedSize);
            ::write(FsHandle, buf, len);

            simplefs::unlock(fileDescriptor.lock);
            fileDescriptor.lock = movedFileLock;

            // update first_free_byte and file's size
            fileNode.set_origin(simplefs::Inodes.first_free_byte());
            fileNode.set_size(resultOffset);

            simplefs::Inodes.set_first_free_byte(simplefs::Inodes.first_free_byte() + resultOffset);

            // serialize inode table
            const std::string encodedInodeTable = protohelpers::lengthEncodeMsg(simplefs::Inodes);
            lseek(FsHandle, 0, SEEK_SET);
            ::write(FsHandle, encodedInodeTable.c_str(), encodedInodeTable.size());
        }
    } else {
        const off_t writeOffset = simplefs::Inodes.filesystem_origin() + fileNode.origin() + fileDescriptor.fpos;

        lseek(FsHandle, writeOffset, SEEK_SET);
        ::write(FsHandle, buf, len);
    }

    fileDescriptor.fpos = resultOffset;

    return len;
}
