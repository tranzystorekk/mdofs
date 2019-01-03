#ifndef MDOFS_HANDLE_MANAGEMENT_HPP
#define MDOFS_HANDLE_MANAGEMENT_HPP

#include "fcntl.h"

namespace simplefs {

int openFilesystem(const char* fspath, int flags);

int closeFilesystem();

bool isFilesystemOpen();

} // simplefs

#endif //MDOFS_HANDLE_MANAGEMENT_HPP
