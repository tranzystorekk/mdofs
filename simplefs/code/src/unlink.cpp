#include "unlink.h"

#include <unistd.h>

#include "auxilliary.h"
#include "common.h"
#include "locking.h"
#include "proto-helpers.hpp"

int simplefs::unlink(const char* name) {
    const bool noActiveDescriptors = !simplefs::NumActiveDescriptors;
    struct flock tempLockParams;

    if ( noActiveDescriptors ) {
        auto inodeAndLock = simplefs::getInodeTable(LockType::WRLK);

        tempLockParams = inodeAndLock.first;
        simplefs::Inodes = inodeAndLock.second;
    }

    const std::string& parentPath = simplefs::getParentDirectory(name);
    int parentDirNodeIndex = simplefs::lookup(parentPath.c_str(), LockType::WRLK);

    if ( parentDirNodeIndex == -1 ) {
        if (noActiveDescriptors) {
            unlock(tempLockParams);
        }

        return -1;
    }

    auto parentDirAndLock = simplefs::getDirectory(simplefs::Inodes, parentDirNodeIndex, LockType::WRLK);

    const std::string& baseName = simplefs::getBaseName(name);

    // check if attempting dletion of special file
    if (baseName == "." || baseName == ".." || baseName == "/") {
        if (noActiveDescriptors) {
            unlock(tempLockParams);
            unlock(parentDirAndLock.first);
        }

        return -1;
    }

    // check if removed entity exists
    auto it = parentDirAndLock.second.mutable_records()->find(baseName);
    if (it == parentDirAndLock.second.mutable_records()->end()) {
        if (noActiveDescriptors) {
            unlock(tempLockParams);
        }

        unlock(parentDirAndLock.first);

        return -1;
    }

    fsproto::Inode& deletedFileNode = *simplefs::Inodes.mutable_inodes(it->second);

    // check if directory and not empty
    if (deletedFileNode.mode() & AccessFlag::DIR) {
        auto deletedDirAndInode = simplefs::getDirectory(simplefs::Inodes, it->second, LockType::WRLK);

        if (deletedDirAndInode.second.records_size() > 2) {
            if (noActiveDescriptors) {
                unlock(tempLockParams);
            }

            unlock(parentDirAndLock.first);
            unlock(deletedDirAndInode.first);

            return -1;
        }

        unlock(deletedDirAndInode.first);
    }

    // free inode resource
    deletedFileNode.set_is_free(true);

    // update first free inode index if needed
    if (it->second < simplefs::Inodes.first_free_node_index()) {
        simplefs::Inodes.set_first_free_node_index(it->second);
    }

    parentDirAndLock.second.mutable_records()->erase(it);

    // serialize inode table
    const std::string encodedInodeTable = protohelpers::lengthEncodeMsg(simplefs::Inodes);
    lseek(FsHandle, 0, SEEK_SET);
    write(FsHandle, encodedInodeTable.c_str(), encodedInodeTable.size());

    // serialize parent directory
    const unsigned int parentDirOrigin = simplefs::Inodes.inodes(parentDirNodeIndex).origin()
                                         + simplefs::Inodes.filesystem_origin();
    const std::string encodedParentDir = protohelpers::lengthEncodeMsg(parentDirAndLock.second);
    lseek(FsHandle, parentDirOrigin, SEEK_SET);
    write(FsHandle, encodedParentDir.c_str(), encodedParentDir.size());

    // unlock all locks
    unlock(parentDirAndLock.first);

    if (noActiveDescriptors) {
        unlock(tempLockParams);
    }

    return 0;
}
