#include "auxilliary.h"

#include <iostream>
#include <memory>
#include <stack>
#include <string>

#include "common.h"
#include "locking.h"
#include "proto-helpers.hpp"

#include <boost/filesystem.hpp>

using fsproto::Directory;
using fsproto::Inode;
using fsproto::InodeTable;
using Path = boost::filesystem::path;

int simplefs::lookup(const char* pathname) {
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

    auto inodeTable = simplefs::getInodeTable();

    // start from root inode
    int currentInodeIndex = 0;
    std::stack<struct flock> directoryLocks;
    const Path directories = path.relative_path().remove_filename();
    for ( auto& el : directories ) {
        auto dir = getDirectory(inodeTable.second, currentInodeIndex);
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
        auto parentDir = getDirectory(inodeTable.second, currentInodeIndex);
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

    unlock(inodeTable.first);

    return result;
}

std::pair<flock, InodeTable> simplefs::getInodeTable() {
    const off_t lastPosition = lseek(FsHandle, 0, SEEK_CUR);
    lseek(FsHandle, 0, SEEK_SET);

    uint32_t inodeTableSize;
    read(FsHandle, &inodeTableSize, sizeof(uint32_t));

    // get lock on actual table
    struct flock inodeTableLockParams = lock(sizeof(uint32_t), inodeTableSize, LockType::RDLK);

    std::unique_ptr<char[]> buffer(new char[inodeTableSize]);

    read(FsHandle, buffer.get(), inodeTableSize);

    InodeTable result;
    result.ParseFromArray(buffer.get(), inodeTableSize);

    lseek(FsHandle, lastPosition, SEEK_SET);

    return std::make_pair(inodeTableLockParams, result);
}

std::pair<struct flock, Directory> simplefs::getDirectory(const InodeTable& table, int inode) {
    const Inode& node = table.inodes(inode);

    if ( node.is_free() ) {
        // TODO indicate error
    }
    if ( !(node.mode() & AccessFlag::DIR) ) {
        // TODO indicate 'not a directory' error
    }

    auto lockParams = lock(node.origin() + table.filesystem_origin(),
            MAX_DIRECTORY_SIZE + sizeof(uint32_t),
            LockType::RDLK);

    char buffer[MAX_DIRECTORY_SIZE + sizeof(uint32_t)];

    // remember current file position and go to pointed directory
    const off_t lastPosition = lseek(FsHandle, 0, SEEK_CUR);
    lseek(FsHandle, node.origin() + table.filesystem_origin(), SEEK_SET);

    read(FsHandle, buffer, MAX_DIRECTORY_SIZE + sizeof(uint32_t));

    Directory dir = protohelpers::decodeDirectory(std::string(buffer, MAX_DIRECTORY_SIZE + sizeof(uint32_t)));

    lseek(FsHandle, lastPosition, SEEK_SET);

    return std::make_pair(lockParams, dir);
}
