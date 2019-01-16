#ifndef MDOFS_LOCKS_H
#define MDOFS_LOCKS_H

#include <fcntl.h>

namespace simplefs {

enum LockType {
    RDLK,
    WRLK
};

struct flock lock(off_t offset, off_t size, LockType type);

void lockToWrite(const struct flock& lockParams);

void unlock(struct flock& params);

} // simplefs

#endif //MDOFS_LOCKS_H
