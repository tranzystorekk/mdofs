#ifndef MDOFS_READ_H
#define MDOFS_READ_H

namespace simplefs {

/// Read data to a buffer
/// \param fd file descriptor
/// \param buf buffer for the read data
/// \param len read length in bytes
/// \return number of read bytes if successful, -1 otherwise
int read(int fd, char* buf, int len);

} // simplefs

#endif //MDOFS_READ_H
