#include "opendir.h"

#include "auxilliary.h"
#include "common.h"
#include "locking.h"
#include "proto-helpers.hpp"

void setDescriptor(simplefs::FileDescriptor& fd, int inode, const struct flock& lock, int flag) {
    fd.inode = inode;
    fd.fpos = 0;
    fd.lock = lock;
    fd.flag = flag;
    fd.is_free = false;
}

unsigned int getNextFreeDescriptor() {
    for (unsigned int i = simplefs::FirstFreeDescriptor + 1; i < simplefs::MAX_FILE_DESCRIPTORS; ++i) {
        if ( simplefs::FdTable[i].is_free ) {
            return i;
        }
    }

    return simplefs::MAX_FILE_DESCRIPTORS;
}

simplefs::Dir simplefs::opendir(const char* name) {
    if ( simplefs::NumActiveDescriptors >= simplefs::MAX_FILE_DESCRIPTORS ) {
        return std::make_pair(-1, fsproto::Directory());
    }

    if (!simplefs::NumActiveDescriptors) {
        auto inodeAndLock = simplefs::getInodeTable(LockType::RDLK);

        simplefs::InodesLockParams = inodeAndLock.first;
        simplefs::Inodes = inodeAndLock.second;
    }

    // lookup dir path
    int node = simplefs::lookup(name, LockType::RDLK);

    if (node == -1) {
        return std::make_pair(-1, fsproto::Directory());
    }

    const fsproto::Inode& inode = simplefs::Inodes.inodes(node);

    const unsigned int inodeMode = inode.mode();

    if ( !((AccessFlag::READ | AccessFlag::DIR) & inodeMode) ) {
        return std::make_pair(-1, fsproto::Directory());
    }

    auto dirResult = simplefs::getDirectory(simplefs::Inodes, node, LockType::RDLK);

    setDescriptor(simplefs::FdTable[simplefs::FirstFreeDescriptor], node, dirResult.first,
                   DescriptorFlag::DESCR_READ);

    const int result = simplefs::FirstFreeDescriptor;

    simplefs::FirstFreeDescriptor = getNextFreeDescriptor();

    ++simplefs::NumActiveDescriptors;

    return std::make_pair(result, dirResult.second);
}
