#ifndef MDOFS_HANDLE_MANAGEMENT_HPP
#define MDOFS_HANDLE_MANAGEMENT_HPP

#include "fcntl.h"

namespace simplefs {

/// Attach a filesystem file to this process
/// \param fspath path in the native filesystem (i.e. your OS filesystem) to the filesystem file
/// \param flags standard UNIX flags for the open() function
/// \return 0 if operation successful, -1 otherwise
int openFilesystem(const char* fspath, int flags);

/// Detach the currently attached filesystem file
/// \return 0 if operation successful, -1 otherwise
int closeFilesystem();

/// Check if a filesystem file is currently attached
/// \return true if filesystem is attached, false otherwise
bool isFilesystemOpen();

} // simplefs

#endif //MDOFS_HANDLE_MANAGEMENT_HPP
