#ifndef MDOFS_AUXILLIARY_H
#define MDOFS_AUXILLIARY_H

#include <utility>

#include <fcntl.h>

#include "directory.pb.h"
#include "inode.pb.h"

namespace simplefs {

int lookup(const char* pathname);

std::pair<struct flock, fsproto::InodeTable> getInodeTable();

std::pair<struct flock, fsproto::Directory> getDirectory(const fsproto::InodeTable& table, int inode);

} // simplefs

#endif //MDOFS_AUXILLIARY_H
