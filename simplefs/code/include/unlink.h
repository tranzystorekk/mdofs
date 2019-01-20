#ifndef MDOFS_UNLINK_H
#define MDOFS_UNLINK_H

namespace simplefs {

/// Remove a file or empty directory
/// \param name path to the removed file/dir
/// \return 0 if operation successful, -1 otherwise
int unlink(const char* name);

} // simplefs

#endif //MDOFS_UNLINK_H
