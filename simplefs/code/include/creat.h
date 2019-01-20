#ifndef MDOFS_CREAT_H
#define MDOFS_CREAT_H

namespace simplefs {

/// Create new empty file
/// \param name path to the new file
/// \param mode access mode, see simplefs::AccessFlag
/// \return open file descriptor for the new file
int creat(const char* name, int mode);

} // simplefs

#endif //MDOFS_CREAT_H
