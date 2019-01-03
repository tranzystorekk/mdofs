#include "handle-management.hpp"

#include <fcntl.h>
#include <unistd.h>

#include "common.h"

using simplefs::FsHandle;
using simplefs::UNINITIALIZED_FS;

int simplefs::openFilesystem(const char* fspath, int flags) {
    if (isFilesystemOpen()) {
        return -1;
    }

    const int checkHandle = open(fspath, flags);
    if (checkHandle == -1) {
        return -1;
    }

    FsHandle = checkHandle;

    return 0;
}

int simplefs::closeFilesystem() {
    if (!isFilesystemOpen()) {
        return -1;
    }

    close(FsHandle);
    FsHandle = UNINITIALIZED_FS;

    return 0;
}

bool simplefs::isFilesystemOpen() {
    return FsHandle != UNINITIALIZED_FS;
}
