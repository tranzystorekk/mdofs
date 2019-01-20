#ifndef MDOFS_COMMON_H
#define MDOFS_COMMON_H

#include <cstdint>

#include <fcntl.h>

#include "inode.pb.h"

/// Contains all mdofs API components
/// \namespace simplefs
namespace simplefs {

constexpr unsigned int HighestBit(unsigned int v, unsigned int n = sizeof(uint32_t) * 8) {
    return ( v & (1 << (n - 1)) || n == 0 ) ? n : HighestBit(v, n - 1);
}

constexpr unsigned int MaxLengthEncodeSize(unsigned int bytes) {
    return HighestBit(bytes) / 7 + ((HighestBit(bytes) % 7) ? 1 : 0);
}

/// Maximum number of records in a single directory
const unsigned int MAX_RECORDS_IN_DIRECTORY = 16;

/// Maximum length of a file name string
const unsigned int MAX_FILENAME_LENGTH = 64;

const unsigned int MAX_RECORD_SIZE = MAX_FILENAME_LENGTH + MaxLengthEncodeSize(MAX_FILENAME_LENGTH)
        + sizeof(uint32_t) + 2;

const unsigned int MAX_DIRECTORY_SIZE = MAX_RECORDS_IN_DIRECTORY * MAX_RECORD_SIZE
        + MaxLengthEncodeSize(MAX_RECORD_SIZE) * MAX_RECORDS_IN_DIRECTORY
        + MAX_RECORDS_IN_DIRECTORY;

const unsigned int MAX_ENCODED_DIRECTORY_SIZE = MAX_DIRECTORY_SIZE + sizeof(uint32_t);

const int UNINITIALIZED_FS = -1;

/// File descriptor access mode
enum DescriptorFlag : unsigned int {
    /// Read access
    DESCR_READ = 0x4,
    /// Write access
    DESCR_WRITE = 0x2,
    /// Read and write access
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

/// Maximum number of open file descriptors
const unsigned int MAX_FILE_DESCRIPTORS = 64;

// for each process start with fs handle uninitialized
// and all descriptors closed
extern int FsHandle;
extern FileDescriptor FdTable[MAX_FILE_DESCRIPTORS];
extern unsigned int FirstFreeDescriptor;
extern unsigned int NumActiveDescriptors;

extern fsproto::InodeTable Inodes;
extern struct flock InodesLockParams;

/// File mode with convenience flags
enum AccessFlag : unsigned int {
    DIR = 0x8,
    /// Allows read access
    READ = 0x4,
    /// Allows write access
    WRITE = 0x2,
    /// Allows execution access
    EXEC = 0x1,
    /// Convenience symbol for READ + WRITE
    RDWR = READ | WRITE,
    /// Convenience symbol for READ + EXEC
    RDEX = READ | EXEC,
    /// Convenience symbol for WRITE + EXEC
    WREX = WRITE | EXEC,
    /// Convenience symbol for READ + WRITE + EXEC
    RWEX = READ | WRITE | EXEC
};

/// Flag for lseek operation mode
enum SeekFlag : unsigned int {
    /// Set offset as the given value
    SET,
    /// Set offset to current location plus the given value
    CUR,
    /// Set offset to the end of file
    END
};

} // simplefs

#endif //MDOFS_COMMON_H
