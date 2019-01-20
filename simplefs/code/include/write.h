#ifndef MDOFS_WRITE_H
#define MDOFS_WRITE_H

namespace simplefs {

/// Write data from buffer into file
/// \param fd file descriptor
/// \param buf buffer to write data from
/// \param len written length in bytes
/// \return number of written bytes if operation successful, -1 otherwise
int write(int fd, const char* buf, int len);

} // simplefs

#endif //MDOFS_WRITE_H
