#ifndef MDOFS_AUXILLIARY_H
#define MDOFS_AUXILLIARY_H

#include <utility>

#include <fcntl.h>

#include "common.h"

#include "locking.h"

#include "directory.pb.h"
#include "inode.pb.h"

namespace simplefs {

int lookup(const char* pathname, LockType lockType);

std::pair<struct flock, fsproto::InodeTable> getInodeTable(LockType lockType);

std::pair<struct flock, fsproto::Directory>
getDirectory(const fsproto::InodeTable& table, int inode, LockType lockType);

void setDescriptor(FileDescriptor& fd, int inode, const struct flock& lock, int flag);

unsigned int getNextFreeDescriptor();

std::string getParentDirectory(const char* path);

std::string getBaseName(const char* path);

} // simplefs

#endif //MDOFS_AUXILLIARY_H
