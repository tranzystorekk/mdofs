#include "common.h"

int simplefs::FsHandle = simplefs::UNINITIALIZED_FS;
simplefs::FileDescriptor simplefs::FdTable[MAX_FILE_DESCRIPTORS];
