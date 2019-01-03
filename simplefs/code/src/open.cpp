#include "open.h"

#include "auxilliary.h"

unsigned int findNextFreeDescriptor() {
    for (unsigned int i = simplefs::FirstFreeDescriptor; i < simplefs::MAX_FILE_DESCRIPTORS; ++i) {
        if ( simplefs::FdTable[i].is_free ) {
            return i;
        }
    }

    return simplefs::MAX_FILE_DESCRIPTORS;
}

void setDescriptor(simplefs::FileDescriptor& fd, int inode, simplefs::DescriptorFlag flag) {
    fd.inode = inode;
    fd.fpos = 0;
    fd.flag = flag;
    fd.is_free = false;
}

int simplefs::open(const char* name, int flags) {
    if ( simplefs::FirstFreeDescriptor >= simplefs::MAX_FILE_DESCRIPTORS ) {
        return -1;
    }

    int node = simplefs::lookup(name);

    if (node == -1) {
        return -1;
    }

    unsigned int flag = 0;

    // TODO sort out this mess
    if ( flags & AccessFlag::READ ) {
    }

    setDescriptor(simplefs::FdTable[simplefs::FirstFreeDescriptor], node);
}