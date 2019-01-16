#ifndef MDOFS_COMMON_H
#define MDOFS_COMMON_H

#include <cstdint>

#include <fcntl.h>

#include "inode.pb.h"

namespace simplefs {

constexpr unsigned int HighestBit(unsigned int v, unsigned int n = sizeof(uint32_t) * 8) {
    return ( v & (1 << (n - 1)) || n == 0 ) ? n : HighestBit(v, n - 1);
}

constexpr unsigned int MaxLengthEncodeSize(unsigned int bytes) {
    return HighestBit(bytes) / 7 + ((HighestBit(bytes) % 7) ? 1 : 0);
}

const unsigned int MAX_RECORDS_IN_DIRECTORY = 16;
const unsigned int MAX_FILENAME_LENGTH = 64;

const unsigned int MAX_RECORD_SIZE = MAX_FILENAME_LENGTH + MaxLengthEncodeSize(MAX_FILENAME_LENGTH)
        + sizeof(uint32_t) + 2;

const unsigned int MAX_DIRECTORY_SIZE = MAX_RECORDS_IN_DIRECTORY * MAX_RECORD_SIZE
        + MaxLengthEncodeSize(MAX_RECORD_SIZE) * MAX_RECORDS_IN_DIRECTORY
        + MAX_RECORDS_IN_DIRECTORY;

const unsigned int MAX_ENCODED_DIRECTORY_SIZE = MAX_DIRECTORY_SIZE + sizeof(uint32_t);

const int UNINITIALIZED_FS = -1;

enum DescriptorFlag : unsigned int {
    DESCR_READ = 0x4,
    DESCR_WRITE = 0x2,
    DESCR_RDWR = DESCR_READ | DESCR_WRITE
};

struct FileDescriptor {
    FileDescriptor() : is_free(true) {}

    int inode;
    unsigned int fpos;
    unsigned int flag;
    struct flock lock;
    bool is_free;
};

const unsigned int MAX_FILE_DESCRIPTORS = 64;

// for each process start with fs handle uninitialized
// and all descriptors closed
extern int FsHandle;
extern FileDescriptor FdTable[MAX_FILE_DESCRIPTORS];
extern unsigned int FirstFreeDescriptor;
extern unsigned int NumActiveDescriptors;

extern fsproto::InodeTable Inodes;
extern struct flock InodesLockParams;

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
