#ifndef MDOFS_MKDIR_H
#define MDOFS_MKDIR_H

namespace simplefs {

/// Make a new directory
/// \param name path to the new directory
/// \return 0 if operation successful, -1 otherwise
int mkdir(const char* name);

} // simplefs

#endif //MDOFS_MKDIR_H
