#include "auxilliary.h"

#include <iostream>
#include <memory>
#include <stack>
#include <string>

#include <cstring>

#include "common.h"
#include "locking.h"
#include "proto-helpers.hpp"

#include <boost/filesystem.hpp>

using fsproto::Directory;
using fsproto::Inode;
using fsproto::InodeTable;
using Path = boost::filesystem::path;

int simplefs::lookup(const char* pathname, LockType lockType) {
    const Path path(pathname);
    int result = -1;

    if ( !path.has_root_directory() ) {
        std::cerr << "Error: path not absolute" << std::endl;
        return -1;
    }
    if ( path.has_root_name() ) {
        std::cerr << "Error: path contains root specifier" << std::endl;
        return -1;
    }

    if (!std::strcmp("/", pathname)) {
        return 0;
    }

    // start from root inode
    int currentInodeIndex = 0;
    std::stack<struct flock> directoryLocks;
    const Path directories = path.relative_path().remove_filename();
    for ( auto& el : directories ) {
        auto dir = getDirectory(simplefs::Inodes, currentInodeIndex, WRLK);
        directoryLocks.push(dir.first);

        auto& records = dir.second.records();

        auto it = records.find(el.string());

        if (it == records.end()) {
            currentInodeIndex = -1;
            break;
        }

        currentInodeIndex = it->second;
    }

    if ( currentInodeIndex != -1 ) {
        auto parentDir = getDirectory(simplefs::Inodes, currentInodeIndex, WRLK);
        directoryLocks.push(parentDir.first);

        auto& parentRecords = parentDir.second.records();

        auto targetIt = parentRecords.find(path.filename().string());

        if ( targetIt != parentRecords.end() ) {
            result = targetIt->second;
        }
    }

    // unlock all directories in reverse order
    while ( !directoryLocks.empty() ) {
        unlock(directoryLocks.top());
        directoryLocks.pop();
    }

    return result;
}

std::pair<struct flock, fsproto::InodeTable> simplefs::getInodeTable(LockType lockType) {
    const off_t lastPosition = lseek(FsHandle, 0, SEEK_CUR);
    lseek(FsHandle, 0, SEEK_SET);

    uint32_t inodeTableSize;
    read(FsHandle, &inodeTableSize, sizeof(uint32_t));

    // get lock on actual table
    struct flock inodeTableLockParams = lock(sizeof(uint32_t), inodeTableSize, lockType);

    std::unique_ptr<char[]> buffer(new char[inodeTableSize]);

    read(FsHandle, buffer.get(), inodeTableSize);

    InodeTable result;
    result.ParseFromArray(buffer.get(), inodeTableSize);

    lseek(FsHandle, lastPosition, SEEK_SET);

    return std::make_pair(inodeTableLockParams, result);
}

std::pair<struct flock, fsproto::Directory>
simplefs::getDirectory(const fsproto::InodeTable& table, int inode, LockType lockType) {
    const Inode& node = table.inodes(inode);

    if ( node.is_free() ) {
        // TODO indicate error
    }
    if ( !(node.mode() & AccessFlag::DIR) ) {
        // TODO indicate 'not a directory' error
    }

    auto lockParams = lock(node.origin() + table.filesystem_origin(),
            MAX_DIRECTORY_SIZE + sizeof(uint32_t),
            lockType);

    char buffer[MAX_DIRECTORY_SIZE + sizeof(uint32_t)];

    // remember current file position and go to pointed directory
    const off_t lastPosition = lseek(FsHandle, 0, SEEK_CUR);
    lseek(FsHandle, node.origin() + table.filesystem_origin(), SEEK_SET);

    read(FsHandle, buffer, MAX_DIRECTORY_SIZE + sizeof(uint32_t));

    Directory dir = protohelpers::decodeDirectory(std::string(buffer, MAX_DIRECTORY_SIZE + sizeof(uint32_t)));

    lseek(FsHandle, lastPosition, SEEK_SET);

    return std::make_pair(lockParams, dir);
}

void simplefs::setDescriptor(simplefs::FileDescriptor& fd, int inode, const struct flock& lock, int flag) {
    fd.inode = inode;
    fd.fpos = 0;
    fd.lock = lock;
    fd.flag = flag;
    fd.is_free = false;
}

unsigned int simplefs::getNextFreeDescriptor() {
    for (unsigned int i = simplefs::FirstFreeDescriptor + 1; i < simplefs::MAX_FILE_DESCRIPTORS; ++i) {
        if ( simplefs::FdTable[i].is_free ) {
            return i;
        }
    }

    return simplefs::MAX_FILE_DESCRIPTORS;
}

std::string simplefs::getParentDirectory(const char* path) {
    return Path(path).parent_path().string();
}

std::string simplefs::getBaseName(const char* path) {
    return Path(path).filename().string();
}
