#include "creat.h"

#include <string>

#include <unistd.h>

#include "auxilliary.h"
#include "common.h"
#include "locking.h"
#include "proto-helpers.hpp"

int simplefs::creat(const char* name, int mode) {
    if ( simplefs::NumActiveDescriptors >= simplefs::MAX_FILE_DESCRIPTORS ) {
        // TODO error
        return -1;
    }

    const bool noActiveDescriptors = !simplefs::NumActiveDescriptors;
    struct flock tempLockParams;

    if ( noActiveDescriptors ) {
        auto inodeAndLock = simplefs::getInodeTable(LockType::WRLK);

        tempLockParams = inodeAndLock.first;
        simplefs::Inodes = inodeAndLock.second;

        // check if inode table full
        if (simplefs::Inodes.first_free_node_index() >= simplefs::Inodes.max_inodes()) {
            unlock(tempLockParams);

            // TODO error
            return -1;
        }
    }

    const std::string& parentPath = simplefs::getParentDirectory(name);
    int parentDirNodeIndex = simplefs::lookup(parentPath.c_str(), LockType::WRLK);

    if ( parentDirNodeIndex == -1 ) {
        // TODO error
        return -1;
    }

    // TODO change lock on inode table to WRITE type

    auto parentDirAndLock = simplefs::getDirectory(simplefs::Inodes, parentDirNodeIndex, LockType::WRLK);

    // check if directory full
    if (parentDirAndLock.second.records_size() >= MAX_RECORDS_IN_DIRECTORY) {
        // unlock all created locks
        if ( noActiveDescriptors ) {
            unlock(tempLockParams);
            unlock(parentDirAndLock.first);
        }

        // TODO error
        return -1;
    }

    // check if requested file name already exists
    if (parentDirAndLock.second.records().count(simplefs::getBaseName(name))) {
        // unlock all created locks
        if ( noActiveDescriptors ) {
            unlock(tempLockParams);
            unlock(parentDirAndLock.first);
        }

        // TODO error
        return -1;
    }

    // lock a symbolic one byte of the new file
    const unsigned int freeByteIndex = simplefs::Inodes.first_free_byte();

    struct flock lockParams = lock(freeByteIndex + simplefs::Inodes.filesystem_origin(),
            1, LockType::WRLK);

    // obtain and set new inode
    const unsigned int freeNodeIndex = simplefs::Inodes.first_free_node_index();

    fsproto::Inode* newInode = simplefs::Inodes.mutable_inodes(freeNodeIndex);

    protohelpers::setInode(newInode, freeByteIndex, 0, mode);

    // scan inode table in search of next free node index
    const unsigned int maxInodes = simplefs::Inodes.max_inodes();
    bool freeNodeFound = false;
    for (unsigned int nodeIndex = freeNodeIndex + 1; nodeIndex < maxInodes; ++nodeIndex) {
        const fsproto::Inode& candidate = simplefs::Inodes.inodes(nodeIndex);

        if (candidate.is_free()) {
            simplefs::Inodes.set_first_free_node_index(nodeIndex);
            freeNodeFound = true;

            break;
        }
    }

    if (!freeNodeFound) {
        simplefs::Inodes.set_first_free_node_index(maxInodes);
    }

    // add new dir record to parent directory
    auto parentRecords = parentDirAndLock.second.mutable_records();
    parentRecords->insert({simplefs::getBaseName(name).c_str(), freeNodeIndex});

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

    // unlock parentDir
    unlock(parentDirAndLock.first);

    // set and get new file descriptor
    simplefs::setDescriptor(simplefs::FdTable[simplefs::FirstFreeDescriptor], freeNodeIndex, lockParams,
            DescriptorFlag::DESCR_WRITE);

    const int result = simplefs::FirstFreeDescriptor;

    simplefs::FirstFreeDescriptor = simplefs::getNextFreeDescriptor();

    ++simplefs::NumActiveDescriptors;

    return result;
}
