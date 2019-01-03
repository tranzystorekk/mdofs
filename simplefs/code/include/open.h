#ifndef MDOFS_OPEN_H
#define MDOFS_OPEN_H

#include "common.h"

namespace simplefs {

int open(const char* name, int flags);

int close(int fd);

} // simplefs

#endif //MDOFS_OPEN_H
