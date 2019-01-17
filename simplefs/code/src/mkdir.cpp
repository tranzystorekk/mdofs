#include "mkdir.h"

#include <string>

#include <unistd.h>

#include "auxilliary.h"
#include "common.h"
#include "locking.h"
#include "proto-helpers.hpp"

int simplefs::mkdir(const char* name) {
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

    // change lock on inode table to WRITE type
    if ( noActiveDescriptors ) {
        simplefs::lockToWrite(tempLockParams);
    }
    else {
        simplefs::lockToWrite(simplefs::InodesLockParams);
    }

    auto parentDirAndLock = simplefs::getDirectory(simplefs::Inodes, parentDirNodeIndex, LockType::WRLK);

    // check if directory full
    if (parentDirAndLock.second.records_size() >= MAX_RECORDS_IN_DIRECTORY) {
        // unlock all created locks
        if (noActiveDescriptors) {
            unlock(tempLockParams);
            unlock(parentDirAndLock.first);
        }

        // TODO error
        return -1;
    }

    // check if requested dir name already exists
    const std::string dirBaseName = simplefs::getBaseName(name);
    if (parentDirAndLock.second.records().count(dirBaseName)) {
        // unlock all created locks
        if (noActiveDescriptors) {
            unlock(tempLockParams);
            unlock(parentDirAndLock.first);
        }

        // TODO error
        return -1;
    }

    // lock the space for the new directory
    const unsigned int freeByteIndex = simplefs::Inodes.first_free_byte();
    struct flock newDirLock = lock(freeByteIndex + simplefs::Inodes.filesystem_origin(),
            MAX_ENCODED_DIRECTORY_SIZE, LockType::WRLK);

    // obtain and set a new inode
    const unsigned int freeNodeIndex = simplefs::Inodes.first_free_node_index();

    fsproto::Inode* newInode = simplefs::Inodes.mutable_inodes(freeNodeIndex);

    protohelpers::setInodeAsDirectory(newInode, freeByteIndex, AccessFlag::RWEX);

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

    // update address of first free byte
    simplefs::Inodes.set_first_free_byte(freeByteIndex + MAX_ENCODED_DIRECTORY_SIZE);

    // add new dir record to parent directory
    auto parentRecords = parentDirAndLock.second.mutable_records();
    parentRecords->insert({dirBaseName.c_str(), freeNodeIndex});

    // make new dir
    fsproto::Directory newDirectory;

    // records is a Protobuf Map pointer
    auto records = newDirectory.mutable_records();
    records->insert({".", freeNodeIndex});
    records->insert({"..", parentDirNodeIndex});

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

    // serialize new directory
    const unsigned int newDirOrigin = freeByteIndex + simplefs::Inodes.filesystem_origin();
    const std::string encodedNewDir = protohelpers::lengthEncodeMsg(newDirectory);
    lseek(FsHandle, newDirOrigin, SEEK_SET);
    write(FsHandle, encodedNewDir.c_str(), encodedNewDir.size());

    // unlock all locks (new dir, parent dir, possibly inode table)
    unlock(newDirLock);
    unlock(parentDirAndLock.first);

    if (noActiveDescriptors) {
        unlock(tempLockParams);
    }

    return 0;
}
