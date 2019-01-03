#include "common.h"

int simplefs::FsHandle = simplefs::UNINITIALIZED_FS;
simplefs::FileDescriptor simplefs::FdTable[MAX_FILE_DESCRIPTORS];
unsigned int simplefs::FirstFreeDescriptor = 0;
unsigned int simplefs::NumActiveDescriptors = 0;

// cached inode table and its lock
fsproto::InodeTable simplefs::Inodes;
struct flock simplefs::InodesLockParams;
