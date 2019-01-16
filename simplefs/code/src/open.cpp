#include "open.h"

#include "auxilliary.h"
#include "locking.h"

unsigned int findNextFreeDescriptor() {
    for (unsigned int i = simplefs::FirstFreeDescriptor; i < simplefs::MAX_FILE_DESCRIPTORS; ++i) {
        if ( simplefs::FdTable[i].is_free ) {
            return i;
        }
    }

    return simplefs::MAX_FILE_DESCRIPTORS;
}

void setDescriptor2(simplefs::FileDescriptor& fd, int inode, const struct flock& lock, int flag) {
    fd.inode = inode;
    fd.fpos = 0;
    fd.lock = lock;
    fd.flag = flag;
    fd.is_free = false;
}

unsigned int getNextFreeDescriptor2() {
    for (unsigned int i = simplefs::FirstFreeDescriptor + 1; i < simplefs::MAX_FILE_DESCRIPTORS; ++i) {
        if ( simplefs::FdTable[i].is_free ) {
            return i;
        }
    }

    return simplefs::MAX_FILE_DESCRIPTORS;
}

int simplefs::open(const char* name, int flags) {
    if ( simplefs::NumActiveDescriptors >= simplefs::MAX_FILE_DESCRIPTORS ) {
        return -1;
    }

    LockType lockType = LockType::RDLK;
    if (flags & AccessFlag::WRITE) {
        lockType = LockType::WRLK;
    }

    if ( !simplefs::NumActiveDescriptors ) {
        auto inodeAndLock = getInodeTable(lockType);

        simplefs::InodesLockParams = inodeAndLock.first;
        simplefs::Inodes = inodeAndLock.second;
    }

    // lookup file path
    int node = simplefs::lookup(name, lockType);

    if (node == -1) {
        return -1;
    }

    const fsproto::Inode& inode = simplefs::Inodes.inodes(node);

    const unsigned int inodeMode = inode.mode();

    // TODO check if flags are valid
    if ( flags & inodeMode != flags ) {
        // TODO error
        return -1;
    }

    struct flock lockParams;

    if ( flags & DescriptorFlag::DESCR_WRITE ) {
        lockParams = simplefs::lock(inode.origin() + simplefs::Inodes.filesystem_origin(),
                inode.size(),
                LockType::WRLK);
    }
    else if ( flags & DescriptorFlag::DESCR_READ ) {
        lockParams = simplefs::lock(inode.origin() + simplefs::Inodes.filesystem_origin(),
                                    inode.size(),
                                    LockType::RDLK);
    }

    setDescriptor2(simplefs::FdTable[simplefs::FirstFreeDescriptor], node, lockParams, flags);

    const int result = simplefs::FirstFreeDescriptor;

    simplefs::FirstFreeDescriptor = getNextFreeDescriptor2();

    ++simplefs::NumActiveDescriptors;

    return result;
}

int simplefs::close(int fd) {
    if (fd < 0 || fd >= simplefs::MAX_FILE_DESCRIPTORS) {
        // TODO error
        return -1;
    }

    simplefs::FileDescriptor& closedDescriptor = simplefs::FdTable[fd];

    if ( closedDescriptor.is_free ) {
        // TODO error
        return -1;
    }

    // unlock the locked file
    simplefs::unlock(closedDescriptor.lock);

    closedDescriptor.is_free = true;

    if (fd < simplefs::FirstFreeDescriptor) {
        simplefs::FirstFreeDescriptor = fd;
    }

    if ( simplefs::NumActiveDescriptors == 1 ) {
        simplefs::unlock(simplefs::InodesLockParams);
    }

    --simplefs::NumActiveDescriptors;

    return 0;
}
