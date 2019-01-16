#include "locking.h"

#include <fcntl.h>

#include "common.h"

struct flock simplefs::lock(off_t offset, off_t size, simplefs::LockType type) {
    struct flock lockParams;

    lockParams.l_whence = SEEK_SET;
    lockParams.l_start = offset;
    lockParams.l_len = size;

    switch (type) {
        case LockType::RDLK:
            lockParams.l_type = F_RDLCK;
            break;

        case LockType::WRLK:
            lockParams.l_type = F_WRLCK;
            break;
    }

    fcntl(FsHandle, F_SETLKW, &lockParams);

    return lockParams;
}

void simplefs::lockToWrite(const struct flock& lockParams) {
    simplefs::lock(lockParams.l_start, lockParams.l_len, LockType::WRLK);
}

void simplefs::unlock(struct flock& params) {
    params.l_type = F_UNLCK;
    fcntl(FsHandle, F_SETLKW, &params);
}
