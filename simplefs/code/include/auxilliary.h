#ifndef MDOFS_AUXILLIARY_H
#define MDOFS_AUXILLIARY_H

#include "directory.pb.h"

namespace simplefs {

int lookup(const char* pathname);

fsproto::Directory getDirectory(int inode);

} // simplefs

#endif //MDOFS_AUXILLIARY_H
