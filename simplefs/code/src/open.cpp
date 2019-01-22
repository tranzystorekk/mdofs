#include "open.h"

#include "auxilliary.h"
#include "locking.h"

int simplefs::open(const char* name, int flags) {
    if ( simplefs::NumActiveDescriptors >= simplefs::MAX_FILE_DESCRIPTORS ) {
        return -1;
    }

    const bool noActiveDescriptors = !simplefs::NumActiveDescriptors;
    LockType lockType = LockType::RDLK;
    if (flags & AccessFlag::WRITE) {
        lockType = LockType::WRLK;
    }

    if ( noActiveDescriptors ) {
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

    // check if we are tring to open a directory
    if ( inodeMode & AccessFlag::DIR ) {
        if ( noActiveDescriptors ) {
            unlock(simplefs::InodesLockParams);
        }

        return -1;
    }

    if ( flags & inodeMode != flags ) {
        if ( noActiveDescriptors ) {
            unlock(simplefs::InodesLockParams);
        }

        return -1;
    }

    struct flock lockParams;

    // undefined behavior if none of these flags are requested
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

    simplefs::setDescriptor(simplefs::FdTable[simplefs::FirstFreeDescriptor], node, lockParams, flags);

    const int result = simplefs::FirstFreeDescriptor;

    simplefs::FirstFreeDescriptor = simplefs::getNextFreeDescriptor();

    ++simplefs::NumActiveDescriptors;

    return result;
}

int simplefs::close(int fd) {
    if (fd < 0 || fd >= simplefs::MAX_FILE_DESCRIPTORS) {
        return -1;
    }

    simplefs::FileDescriptor& closedDescriptor = simplefs::FdTable[fd];

    if ( closedDescriptor.is_free ) {
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
