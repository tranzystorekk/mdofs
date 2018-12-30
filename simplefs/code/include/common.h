#ifndef MDOFS_COMMON_H
#define MDOFS_COMMON_H

#define MAX_FILE_DESCRIPTORS 64

namespace simplefs {

const int UNINITIALIZED_FS = -1;

enum DescriptorFlag {
    DESCR_READ = 0x2,
    DESCR_WRITE = 0x1
};

struct FileDescriptor {
    FileDescriptor() : is_free(true) {}

    int inode;
    DescriptorFlag flag;
    bool is_free;
};

// for each process start with fs handle uninitialized
// and all descriptors closed
static int FsHandle = UNINITIALIZED_FS;
static FileDescriptor FdTable[MAX_FILE_DESCRIPTORS];

enum AccessFlag : unsigned int {
    DIR = 0x8,
    READ = 0x4,
    WRITE = 0x2,
    EXEC = 0x1,
    RDWR = READ | WRITE,
    RDEX = READ | EXEC,
    WREX = WRITE | EXEC,
    RWEX = READ | WRITE | EXEC
};

} // simplefs

#endif //MDOFS_COMMON_H
