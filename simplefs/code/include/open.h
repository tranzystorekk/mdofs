#ifndef MDOFS_OPEN_H
#define MDOFS_OPEN_H

#include "common.h"

namespace simplefs {

/// Open a file descriptor for an existing file
/// \param name path to the existing file
/// \param file descriptor access mode, see simplefs::DescriptorFlag
/// \return 0 if operation successful, -1 otherwise
int open(const char* name, int flags);

/// Close a file descriptor
/// \param fd closed file descriptor
/// \return 0 if operation successful, -1 otherwise
int close(int fd);

} // simplefs

#endif //MDOFS_OPEN_H
