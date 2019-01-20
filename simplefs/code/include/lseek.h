#ifndef MDOFS_LSEEK_H
#define MDOFS_LSEEK_H

namespace simplefs {

/// Change current file offset
/// \param fd file descriptor
/// \param whence whence mode, see simplefs::SeekFlag
/// \param offset offset value
/// \return offset value form the file beginning if operation successful, -1 otherwise
int lseek(int fd, int whence, int offset = 0);

} // simplefs

#endif //MDOFS_LSEEK_H
